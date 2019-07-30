#ifndef __I2C__
#define __I2C__
#include <stddef.h>
#include <stdint.h>

enum
{
    I2C_MASTER_BASE    = (0x3F205000),         // I2C Base address
    I2C_MASTER_C       = (I2C_MASTER_BASE + 0x00),    // I2C Control 
    I2C_MASTER_S       = (I2C_MASTER_BASE + 0x04),    // I2C Status
    I2C_MASTER_DLEN    = (I2C_MASTER_BASE + 0x08),    // I2C Data Length
    I2C_MASTER_A       = (I2C_MASTER_BASE + 0x0c),    // I2C Slave Address 
    I2C_MASTER_FIFO    = (I2C_MASTER_BASE + 0x10),    // I2C Data FIFO 
    I2C_MASTER_DIV     = (I2C_MASTER_BASE + 0x14),    // I2C Clock Divider
    I2C_MASTER_DEL     = (I2C_MASTER_BASE + 0x18),    // I2C Data Delay
    I2C_MASTER_CLKT    = (I2C_MASTER_BASE + 0x1c),    // I2C Clock Stretch Timeout 

    I2C_SLAVE_BASE = (0x7E214000), //BSC SLAVE BASE
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

typedef union I2C_flags
{
    struct
    {
        uint8_t clear_to_send : 1; // This bit is the complement of the UART clear to send
        uint8_t data_set_ready : 1; // uns
        uint8_t data_carrier_detected : 1;  //uns
        uint8_t busy : 1; //busy transmitting data 
        uint8_t receive_queue_empty : 1; // set wen rx queue empty
        uint8_t transmit_queue_full : 1; // set wen trans queue empty
        uint8_t receive_queue_full : 1; // set wen receieve queue full
        uint8_t transmit_queue_empty : 1; // set wen transmit queue full
        uint8_t ring_indicator : 1; // unsp
        uint32_t padding : 23; // reserved
    };
    uint32_t as_int;
} I2C_flags_t;

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

        uint8_t reserved: 17; //reserved
    };
    uint32_t as_int;
} I2C_slave_control_t;

#endif