#include "../../include/common/stdio.h"
#include "../../include/kernel/uart.h"
#include "../../include/kernel/i2c.h"
volatile uint32_t *i2c_base_ptr;
struct i2c_register_map {
    uint32_t C;
    uint32_t S;
    uint32_t DLEN;
    uint32_t A;
    uint32_t FIFO;
    uint32_t DIV;
    uint32_t DEL;
    uint32_t CLKT;
};
volatile struct i2c_register_map *I2C;
/******** useful values ********/
enum {I2C_FIFO_SIZE = 16};
enum {
    I2C_C_I2CEN = 0x8000,
    I2C_C_ST = 0x80,
    I2C_C_CLEAR = 0x10,
    I2C_C_READ = 0x01
};
enum {
    I2C_S_RXS = 0x20,
    I2C_S_TXD = 0x10,
    I2C_S_DONE = 0x02
};
void write_or(uint32_t reg, uint32_t data)
{
    *(volatile uint32_t*)reg |= data;
}

void write_and(uint32_t reg, uint32_t data)
{
    *(volatile uint32_t*)reg &= data;
}
void i2c_write_byte_2(uint8_t byte)
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
    // bsc0_control_t controls = read_control();
    // controls.ST = 1;
    // mmio_write(BSC0_C, controls.as_int);
    write_or(BSC0_C, I2C_C_ST);

    // I2C->C |= I2C_C_ST; // start transfer
    uint32_t done = mmio_read(BSC0_S) & 0x02;
    do {
        done = mmio_read(BSC0_S) & 0x02;
    } while (!done);
    // while(!(I2C->S & I2C_S_DONE)); // wait for done
    write_or(BSC0_S, I2C_S_DONE);
    // I2C->S |= I2C_S_DONE; // clear done flag
}

void i2c_read_data_2(uint8_t *data, uint16_t length)
{
    uint16_t i = 0;

    mmio_write(BSC0_DLEN, length);

    write_or(BSC0_C, I2C_C_READ | I2C_C_CLEAR);
    write_or(BSC0_C, I2C_C_ST);
    //I2C->C |= I2C_C_READ | I2C_C_CLEAR; // set read bit, clear FIFO
    //I2C->C |= I2C_C_ST; // start transfer

    while(!(mmio_read(BSC0_S) & I2C_S_DONE)) { // while not done
        while(i < length && (mmio_read(BSC0_S) & I2C_S_RXS)) { // while data left and fifo not empty
            //data[i++] = I2C->FIFO;
            //puts(dectohex(mmio_read(BSC0_S) & I2C_S_RXS));
            // puts("\r\n");
            data[i++] = mmio_read(BSC0_FIFO);
        }
    }

    while(i < length && (mmio_read(BSC0_S) & I2C_S_RXS)) { // remaining data in fifo
        data[i++] = mmio_read(BSC0_FIFO);
        // data[i++] = I2C->FIFO;
    }
    write_or(BSC0_S, I2C_S_DONE);
}

void i2c_set_address(uint8_t addr)
{
    I2C->A = addr & 0x7f; // only 7 bit
}

void i2c_write_address(uint8_t a) {
    mmio_write(BSC0_A, a);
}

void delay_ms(uint32_t ms) {
    for (uint32_t i = 0; i < ms; i ++) {
    }
}

void kernel_main(void) 
{
	uart_init();
    i2c_init();

    I2C = (volatile struct i2c_register_map *) BSC0_BASE;

    // i2c_set_address(0x68);
    i2c_write_address(0x68);
    
    bsc0_control_t controls;
    bzero(&controls, 4);
    controls.as_int = mmio_read(BSC0_C);
    controls.I2CEN = 1;
    mmio_write(BSC0_C, controls.as_int);

    // write_or(BSC0_C, I2C_C_I2CEN);
    // I2C->C |= I2C_C_I2CEN; // clear fifo, enable bsc controller
    // I2C->S = 0x50; // reset flag register
    mmio_write(BSC0_S, 0x50);

    char* print_list[] = {
        "EEET2490: Embedded Systems - Operating Systems & Interfacing\r\n",
		"------------------------------------------------------------\r\n",
		"Assessment Group: 6\r\n",
		"s3761489: Tuan Phung\r\n",
		"s3695340: Sung Jin Kim\r\n",
		"s3567923: Dang Phuc Hung\r\n"
    };
    for (int i = 0; i < 6; i ++) {
        puts(print_list[i]);
    }

    char buf[]={0x00, 0x20, 0x19, 0x18, 0x06, 0x12, 0x08, 0x16};
    char *str[]  ={"SUN", "MON", "TUES", "WED", "THUR", "FRI", "SAT"};
    int pause = 0;
    while (1) {
        unsigned char input_char = uart_getc_without_waiting();
        if (input_char == 'p') pause = 1;
        if (input_char == 'r') pause = 0;
        if (!pause) {
            i2c_write_byte_2(0x00);
    
            i2c_read_data_2((uint8_t *)buf, 8);
            buf[0] = buf[0] & 0x7F; //sec
            buf[1] = buf[1] & 0x7F; //min
            buf[2] = buf[2] & 0x3F; //hour
            buf[3] = buf[3] & 0x07; //day of the week
            buf[4] = buf[4] & 0x3F; //day of the month
            buf[5] = buf[5] & 0x1F; //month
            
            puts(str[(unsigned char)buf[3]-1]);
            puts(" ");
            puts("20");        
            puts(dectohex(buf[6]));
            puts("/");
            puts(dectohex(buf[5]));
            puts("/");
            puts(dectohex(buf[4]));
            puts(" ");
            puts(dectohex(buf[2]));
            puts(":");
            puts(dectohex(buf[1]));
            puts(":");
            puts(dectohex(buf[0]));
            puts("\r\n");
        }
        // delay_ms(50000000);
        delay(5500000);
    }
    
}