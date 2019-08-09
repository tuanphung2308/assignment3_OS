#ifndef __SPIHEADER__
#define __SPIHEADER__
#include <stddef.h>
#include <stdint.h>
#include "gpio.h"
#include "../../include/common/stdlib.h"
#include "../../include/kernel/uart.h"

enum
{
    SPI0_BASE    = (0x3F204000),         // I2C Base address
    SPI0_CS       = (SPI0_BASE + 0x0),    // I2C Control 
    SPI0_FIFO       = (SPI0_BASE + 0x4),    // I2C Status
    SPI0_CLK    = (SPI0_BASE + 0x8),    // I2C Data Length
    SPI0_DLEN       = (SPI0_BASE + 0xc),    // I2C Slave Address 
    SPI0_LTOH    = (SPI0_BASE + 0x10),    // I2C Data FIFO 
    SPI0_DC     = (SPI0_BASE + 0x14),    // I2C Clock Divider
};

typedef union SPI0_CS
{
    struct
    {
        uint8_t chip_select: 2; //Chip select -> select 0 here
        uint8_t clock_phase : 1; // read transfer. 0 = Write Packet Transfer. 1 = Read Packet Transfer. [0]
        uint8_t clock_polarity : 3; // reserved. [3:1]
        uint8_t clear : 2;
        // CLEAR should be 2 bits

        uint8_t cspol : 1; // reserved. [6]
        uint8_t transfer_active : 1; // start transfer. 0 = No action. 1 = Start a new transfer. [7]
        uint8_t dmaen : 1; // 0 = Don t generate interrupts on DONE. 1 = Generate interrupt while DONE =1. [8]
        // should be 1 bit here
        uint8_t intd : 1; // 0 = Don t generate interrupts on TXW condition. 1 = Generate interrupt while TXW = 1. [9] 
        // should be 1 bit here
        uint8_t intr : 1; // 0 = Don t generate interrupts on RXR condition. 1 = Generate interrupt while RXR = 1. [10]
        // 1 bit length here
        uint8_t adcs : 1; // reserved. [14:11]
        uint8_t ren : 1; // I2C enable. 0 = BSC controller is disabled, 1 = BSC controller is enabled. [15] 
        uint8_t len : 1; // I2C enable. 0 = BSC controller is disabled, 1 = BSC controller is enabled. [15] 
        uint8_t lmono : 1; // I2C enable. 0 = BSC controller is disabled, 1 = BSC controller is enabled. [15] 
        uint8_t te_en : 1; // I2C enable. 0 = BSC controller is disabled, 1 = BSC controller is enabled. [15] 
        uint8_t done : 1; // I2C enable. 0 = BSC controller is disabled, 1 = BSC controller is enabled. [15] 
        uint8_t rxd : 1; // I2C enable. 0 = BSC controller is disabled, 1 = BSC controller is enabled. [15] 
        uint8_t txd : 1; // I2C enable. 0 = BSC controller is disabled, 1 = BSC controller is enabled. [15] 
        uint8_t rxr : 1; // I2C enable. 0 = BSC controller is disabled, 1 = BSC controller is enabled. [15] 
        uint8_t rxf : 1; // I2C enable. 0 = BSC controller is disabled, 1 = BSC controller is enabled. [15] 
        uint8_t cspol0 : 1; // I2C enable. 0 = BSC controller is disabled, 1 = BSC controller is enabled. [15] 
        uint8_t cspol1 : 1; // I2C enable. 0 = BSC controller is disabled, 1 = BSC controller is enabled. [15] 
        uint8_t cspol2 : 1; // I2C enable. 0 = BSC controller is disabled, 1 = BSC controller is enabled. [15] 
        uint8_t dma_len : 1; // I2C enable. 0 = BSC controller is disabled, 1 = BSC controller is enabled. [15] 
        uint8_t len_long : 1; // I2C enable. 0 = BSC controller is disabled, 1 = BSC controller is enabled. [15] 
        uint8_t reserved : 6; // I2C enable. 0 = BSC controller is disabled, 1 = BSC controller is enabled. [15] 
    };
    uint32_t as_int;
} spi0_cs_t;

// SPI Function Prototypes
spi0_cs_t read_cs(void);
void spi0_init(void);
void spi0_start_transfer(void);
void spi_transfer_stop(void);
void spi0_chip_select(uint8_t chip_no);
uint8_t spi_transfer_byte(uint8_t data);
#endif