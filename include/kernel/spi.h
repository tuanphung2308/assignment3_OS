#ifndef __SPIHEADER__
#define __SPIHEADER__
#include <stddef.h>
#include <stdint.h>
#include "gpio.h"
#include "../../include/common/stdlib.h"
#include "../../include/kernel/uart.h"

enum
{
    SPI0_BASE = (0x3F204000),         
    SPI0_CS = (SPI0_BASE + 0x0),    
    SPI0_FIFO = (SPI0_BASE + 0x4),   
    SPI0_CLK = (SPI0_BASE + 0x8),   
    SPI0_DLEN = (SPI0_BASE + 0xc),   
    SPI0_LTOH = (SPI0_BASE + 0x10),    
    SPI0_DC = (SPI0_BASE + 0x14), 
};

typedef union SPI0_CS
{
    struct
    {
        uint8_t chip_select: 2; //Chip select -> select 0 here
        uint8_t clock_phase : 1; // read transfer. 0 = Write Packet Transfer. 1 = Read Packet Transfer. [0]
        uint8_t clock_polarity : 3;
        uint8_t clear : 2;
        uint8_t cspol : 1;
        uint8_t transfer_active : 1;
        uint8_t dmaen : 1; 
        uint8_t intd : 1; 
        uint8_t intr : 1; 
        uint8_t adcs : 1; 
        uint8_t ren : 1;
        uint8_t len : 1;
        uint8_t lmono : 1;
        uint8_t te_en : 1;
        uint8_t done : 1; 
        uint8_t rxd : 1; 
        uint8_t txd : 1; 
        uint8_t rxr : 1; 
        uint8_t rxf : 1; 
        uint8_t cspol0 : 1; 
        uint8_t cspol1 : 1; 
        uint8_t cspol2 : 1; 
        uint8_t dma_len : 1; 
        uint8_t len_long : 1; 
        uint8_t reserved : 6; 
    };
    uint32_t as_int;
} spi0_cs_t;

// SPI Function Prototypes
spi0_cs_t read_cs(void);
void spi0_init(void);
void spi0_start_transfer(void);
void spi0_transfer_stop(void);
void spi0_chip_select(uint8_t chip_no);
uint8_t spi0_transfer_byte(uint8_t data);

#endif