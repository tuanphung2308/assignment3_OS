#ifndef __I2CHEADER__
#define __I2CHEADER__
#include <stddef.h>
#include <stdint.h>
#include "../../include/common/stdlib.h"
#include "../../include/kernel/uart.h"
enum {I2C_FIFO_SIZE = 16};

enum
{
    BSC1_BASE    = (0x3F804000),         // I2C Base address
    BSC1_C       = (BSC1_BASE + 0x0),    // I2C Control 
    BSC1_S       = (BSC1_BASE + 0x4),    // I2C Status
    BSC1_DLEN    = (BSC1_BASE + 0x8),    // I2C Data Length
    BSC1_A       = (BSC1_BASE + 0xc),    // I2C Slave Address 
    BSC1_FIFO    = (BSC1_BASE + 0x10),    // I2C Data FIFO 
    BSC1_DIV     = (BSC1_BASE + 0x14),    // I2C Clock Divider
    BSC1_DEL     = (BSC1_BASE + 0x18),    // I2C Data Delay
    BSC1_CLKT    = (BSC1_BASE + 0x1c),    // I2C Clock Stretch Timeout
};

typedef union BSC1_CONTROL
{
    struct
    {
        uint8_t READ : 1; // read transfer. 0 = Write Packet Transfer. 1 = Read Packet Transfer. [0]
        uint8_t reserved : 3; // reserved. [3:1]
        uint8_t CLEAR_1 : 1;
        uint8_t CLEAR_2 : 1; // FIFO clear. 00 = No action. x1 = Clear FIFO. This is 2bits. [5:4]
        // CLEAR should be 2 bits

        uint8_t reserved_2 : 1; // reserved. [6]
        uint8_t ST : 1; // start transfer. 0 = No action. 1 = Start a new transfer. [7]
        uint8_t INTD : 1; // 0 = Don t generate interrupts on DONE. 1 = Generate interrupt while DONE =1. [8]
        // should be 1 bit here
        uint8_t INTT : 1; // 0 = Don t generate interrupts on TXW condition. 1 = Generate interrupt while TXW = 1. [9] 
        // should be 1 bit here
        uint8_t INTR : 1; // 0 = Don t generate interrupts on RXR condition. 1 = Generate interrupt while RXR = 1. [10]
        // 1 bit length here
        uint8_t reserved_3 : 4; // reserved. [14:11]
        uint8_t I2CEN : 1; // I2C enable. 0 = BSC controller is disabled, 1 = BSC controller is enabled. [15] 
        uint16_t padding: 16; // [31:16] // reserved again
    };
    uint32_t as_int;
} bsc1_control_t;

typedef union BSC1_STATUS {
    struct {
        uint8_t transfer_active: 1;
        uint8_t done: 1;
        uint8_t txw: 1;
        uint8_t rxr: 1;
        uint8_t txd: 1; // fifo can accept data
        uint8_t rxd: 1; // fifo contains data 
        uint8_t txe: 1; // fifo empty
        uint8_t rxf: 1; // fifo full
        uint8_t err: 1; // have error
        uint8_t clkt: 1; // clock stretch timeout
        uint32_t reserved: 22; // reserved
    };
    uint32_t as_int;
} bsc1_status_t;

typedef union BSC1_DLEN {
    struct {
        uint32_t dlen: 16;
        uint32_t reserved: 16; //reserved
    };
    uint32_t as_int;
} bsc1_dlen_t;

typedef union BSC1_ADDRESS {
    struct {
        uint8_t addr: 7;
        uint32_t reserved: 25; //reserved
    };
    uint32_t as_int;
} bsc1_address_t;

typedef union BSC1_FIFO {
    struct {
        uint8_t data: 8;
        uint32_t reserved: 24; //reserved
    };
    uint32_t as_int;
} bsc1_fifo_t;

// I2C Function Prototypes

void i2c_init();
bsc1_status_t read_status();
bsc1_control_t read_control();
void i2c_start();
void i2c_write_byte(uint8_t byte);
void i2c_set_slave(uint8_t a);
void i2c_read_data(uint8_t *data, uint16_t length);
void i2c_enable();
void start_tx() ;
void i2c_stop(void);
// void set_time(uint8_t sec, uint8_t min, uint8_t hour, uint8_t day, uint8_t date, uint8_t month, uint8_t year, uint8_t mode);
#endif