#include "../../include/kernel/i2c.h"
// #include "gpio.h"
#include "../../include/kernel/gpio.h"
#include "../../include/common/stdlib.h"
#include "../../include/kernel/uart.h"
void i2c_init() {
    //Init I2C master
    // Enable alternative function ALT0 for PIN00 and PIN01
    *(volatile uint32_t*) GPFSEL |= (0b100 << 6);
    *(volatile uint32_t*) GPFSEL |= (0b100 << 9);
}

void i2c_enable() {
    bsc0_control_t controls;
    bzero(&controls, 4);
    controls.as_int = mmio_read(BSC0_C);
    controls.I2CEN = 1;
    mmio_write(BSC0_C, controls.as_int);
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
    bsc0_control_t controls = read_control();
    mmio_write(BSC0_C, controls.as_int); //OK

    controls.READ = 0; //OK 
    mmio_write(BSC0_C, controls.as_int); //OK

    mmio_write(BSC0_FIFO, byte);
    controls = read_control();
    controls.ST = 1;
    mmio_write(BSC0_C, controls.as_int);

    bsc0_status_t status = read_status();
    do {
        status = read_status();
    } while(!(status.done & 0x01));

    // status = read_status();
    status.done = 1;
    mmio_write(BSC0_S, status.as_int);
}

void i2c_read_data(uint8_t *data, uint16_t length)
{
    uint16_t i = 0;

    mmio_write(BSC0_DLEN, length);

    bsc0_control_t controls = read_control();
    bsc0_status_t status = read_status();
    controls.CLEAR_1 = 1;
    controls.READ = 1;
    mmio_write(BSC0_C, controls.as_int);
    controls.ST = 1;
    mmio_write(BSC0_C, controls.as_int);

    
    status = read_status();
    while(!status.done) { // while not done
        while(i < length && status.rxd) { // while data left and fifo not empty
            data[i++] = mmio_read(BSC0_FIFO);
            status = read_status();
        }
        status = read_status();
    }

    while(i < length && status.rxd) { // remaining data in fifo
        data[i++] = mmio_read(BSC0_FIFO);
        status = read_status();
    }

    status = read_status();
    status.done = 1;
    mmio_write(BSC0_S, status.as_int);
}

void start_tx() {
    bsc0_status_t status;
    status = read_status();
    status.txd = 1; //fifo can accpet data
    status.txe = 1; // fifo empty
    mmio_write(BSC0_S, status.as_int);
}