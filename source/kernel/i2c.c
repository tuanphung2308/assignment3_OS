#include "../../include/kernel/i2c.h"
// #include "gpio.h"
#include "../../include/kernel/gpio.h"
#include "../../include/common/stdlib.h"
#include "../../include/kernel/uart.h"
void i2c_init() {
    //Init I2C master
    // Enable alternative function ALT0 for PIN00 and PIN01
    // mmio_write(GPFSEL, (1 << 8) | (1 << 11));
    *(volatile uint32_t*) GPFSEL |= (0b100 << ((2 % 10) * 3));
    *(volatile uint32_t*) GPFSEL |= (0b100 << ((3 % 10) * 3));
}

void i2c_start() {
    bsc0_status_t status = read_status();
    status.txd = 1;
    status.txe = 1;
    mmio_write(BSC0_C, status.as_int);
}

bsc0_status_t read_status() {
    bsc0_status_t status;
    // bzero(&status, 4);
    status.as_int = mmio_read(BSC0_S);
    return status;
}

bsc0_control_t read_control() {
    bsc0_control_t controls;
    // bzero(&controls, 4);
    controls.as_int = mmio_read(BSC0_C);
    return controls;
}

void i2c_write_address(uint8_t a) {
    mmio_write(BSC0_A, a);
}

void i2c_write_byte(uint8_t byte)
{                                   
    mmio_write(BSC0_DLEN, 0x1);
    // I2C->DLEN = 1; // one byte transfer
    // I2C->C |= I2C_C_CLEAR; // clear fifo
    bsc0_control_t controls = read_control();
    // controls.CLEAR = 3;
    mmio_write(BSC0_C, controls.as_int); //OK
    // write_or(BSC0_C, I2C_C_CLEAR);

    // I2C->C &= ~I2C_C_READ; // clear read bit -> write
    controls.READ = 0; //OK 
    mmio_write(BSC0_C, controls.as_int); //OK
    // write_and(BSC0_C, ~I2C_C_READ);

    mmio_write(BSC0_FIFO, byte);
    // I2C->FIFO = byte;
    controls = read_control();
    controls.ST = 1;
    mmio_write(BSC0_C, controls.as_int);
    // write_or(BSC0_C, I2C_C_ST);

    // I2C->C |= I2C_C_ST; // start transfer
    bsc0_status_t status = read_status();
    //     done = mmio_read(BSC0_S) & 0x02;
    // } while (!done);
    do {
        status = read_status();
    } while(!(status.done & 0x01));
    // while(!(I2C->S & I2C_S_DONE)); // wait for done
    status = read_status();
    status.done = 1;
    mmio_write(BSC0_S, status.as_int);
    // write_or(BSC0_S, I2C_S_DONE);
    // I2C->S |= I2C_S_DONE; // clear done flag
}