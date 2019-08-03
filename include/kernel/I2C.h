#ifndef __I2CHEADER__
#define __I2CHEADER__
#include <stddef.h>
#include <stdint.h>

// #define START_READ      BSC_C_I2CEN|BSC_C_ST|BSC_C_CLEAR|BSC_C_READ
// #define START_WRITE     BSC_C_I2CEN|BSC_C_ST
// #define CLEAR_STATUS    BSC_S_CLKT|BSC_S_ERR|BSC_S_DONE

#define BSC_FIFO_SIZE   				16

enum
{
    BSC0_BASE    = (0x3F804000),         // I2C Base address
    BSC0_C       = (BSC0_BASE + 0x0),    // I2C Control 
    BSC0_S       = (BSC0_BASE + 0x4),    // I2C Status
    BSC0_DLEN    = (BSC0_BASE + 0x8),    // I2C Data Length
    BSC0_A       = (BSC0_BASE + 0xc),    // I2C Slave Address 
    BSC0_FIFO    = (BSC0_BASE + 0x10),    // I2C Data FIFO 
    BSC0_DIV     = (BSC0_BASE + 0x14),    // I2C Clock Divider
    BSC0_DEL     = (BSC0_BASE + 0x18),    // I2C Data Delay
    BSC0_CLKT    = (BSC0_BASE + 0x1c),    // I2C Clock Stretch Timeout

    I2C_SLAVE_BASE = (0x3F214000), //BSC SLAVE BASE
    I2C_SLAVE_DR = (I2C_SLAVE_BASE + 0x00), //DATA REGISTER
    I2C_SLAVE_RSR = (I2C_SLAVE_BASE + 0x04), //STATUS REGISTER AND ERROR CLEAR REGISTER
    I2C_SLAVE_SLV = (I2C_SLAVE_BASE + 0x08), // I2C slave address value 
    I2C_SLAVE_CR = (I2C_SLAVE_BASE + 0x0C), // Control register 
    I2C_SLAVE_FR = (I2C_SLAVE_BASE + 0x10), //Flag register
    I2C_SLAVE_IFLS = (I2C_SLAVE_BASE + 0x14), //Interrupt fifo level select register
    I2C_SLAVE_IMSC = (I2C_SLAVE_BASE + 0x18), //Interupt Mask Set Clear Register 
    I2C_SLAVE_RIS = (I2C_SLAVE_BASE + 0x1c), //Raw Interupt Status Register 
    I2C_SLAVE_MIS = (I2C_SLAVE_BASE + 0x20), //Masked Interupt Status Register 
    I2C_SLAVE_ICR = (I2C_SLAVE_BASE + 0x24), //Interupt Clear Register 
    I2C_SLAVE_DMACR = (I2C_SLAVE_BASE + 0x28), //DMA Control Register 
    I2C_SLAVE_TDR = (I2C_SLAVE_BASE + 0x2C), //FIFO Test Data 
    I2C_SLAVE_GPUSTAT = (I2C_SLAVE_BASE + 0x30), //GPU Status Register 
    I2C_SLAVE_HCTRL = (I2C_SLAVE_BASE + 0x34), //Host Control Register 
    I2C_SLAVE_DEBUG1 = (I2C_SLAVE_BASE + 0x38), //I2C Debug Register
    I2C_SLAVE_DEBUG2 = (I2C_SLAVE_BASE + 0x3C), //SPI Debug Register 
};

typedef union BSC0_CONTROL
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
} bsc0_control_t;

typedef union BSC0_STATUS {
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
} bsc0_status_t;

typedef union BSC0_DLEN {
    struct {
        uint32_t dlen: 16;
        uint32_t reserved: 16; //reserved
    };
    uint32_t as_int;
} bsc0_dlen_t;

typedef union BSC0_ADDRESS {
    struct {
        uint8_t addr: 7;
        uint32_t reserved: 25; //reserved
    };
    uint32_t as_int;
} bsc0_address_t;

typedef union BSC0_FIFO {
    struct {
        uint8_t data: 8;
        uint32_t reserved: 24; //reserved
    };
    uint32_t as_int;
} bsc0_fifo_t;

// I2C Function Prototypes

typedef union I2C_slave_control
{
    struct
    {
        uint8_t slave_enabled : 1; //ENABLE I2C SLAVE = 1
        uint8_t spi_enabled : 1; //enable SPI = 1
        uint8_t i2c_enabled : 1; //enable I2C = 1
        uint8_t cpha : 1; //disable: 0
        uint8_t cpol : 1; //disable: 0
        uint8_t enstat : 1; //ENSTAT ENABLE STATUS 8bit register 
        // 0 = Status register disabled. Implies ordinary I2C
        // protocol.
        // 1 = Status register enabled. When enabled the
        // status register is transferred as a first data
        // character on the I2C bus. Status register is
        // transferred to the host.
        // NOTE: The same behaviour is achieved from the
        // Host side by using bit SLVADDR[6] of the slave
        // address. 

        uint8_t enctrl : 1; // ENCTRL ENABLE CONTROL 8bit register
        // 0 = Control register disabled. Implies ordinary I2C protocol.
        // 1 = Control register enabled. When enabled the
        // control register is received as a first data
        // character on the I2C bus.
        // NOTE: The same behaviour is achieved from the
        // Host side by using bit SLVADDR[6] of the slave address

        uint8_t brk : 1; //BRK Break current operation:  
        // Stop operation and clear the FIFOs. 

        uint8_t txe : 1; //TXE Transmit Enable
        //1 = Transmit mode enabled 

        uint8_t rxe: 1; //RXE Receive Enable
        //1 = Receive mode enabled 

        uint8_t INV_RXF: 1; //INV-RX Inverse RX status flags
        //set to 0

        uint8_t TESTFIFO: 1; //default to 0

        uint8_t HOSTCTRLEN: 1; // default to 0

        uint8_t INV_TXF: 1; //default to 0

        uint32_t reserved: 17; //reserved
    };
    uint32_t as_int;
} I2C_slave_control_t;
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
void i2c_init();
bsc0_status_t read_status();
bsc0_control_t read_control();
void i2c_start();
void i2c_write_byte(uint8_t byte);
void i2c_write_address(uint8_t a);
void i2c_read_data(uint8_t *data, uint16_t length);
void i2c_enable();
#endif