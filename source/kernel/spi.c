#include "spi.h"

void spi0_init() {
    for (int i = 8; i < 12; i++) fsel_alt0(i); // set pin from 8-11 to ALT0
    // Subject to change
}

void spi0_chip_select(uint8_t chip_no) {
    spi0_cs_t control = read_cs();
    control.chip_select = chip_no;
    mmio_write(SPI0_CS, control.as_int);
}

void spi0_start_transfer(void) {
    spi0_cs_t control = read_cs();
    control.transfer_active = 1;
    mmio_write(SPI0_CS, control.as_int);
}

void spi0_transfer_stop(void)
{
    spi0_cs_t control = read_cs();
    while(!(control.done)) {
        control = read_cs();
    };// wait for DONE
    control.transfer_active = 0;
    mmio_write(SPI0_CS, control.as_int);
}

uint8_t spi0_transfer_byte(uint8_t data)
{
    spi0_cs_t control = read_cs();
    control.clear = 3;
    mmio_write(SPI0_FIFO, data);
    do {
        control = read_cs();
    } while (!control.rxd);
    return (uint8_t) mmio_read(SPI0_FIFO);
}

spi0_cs_t read_cs() {
    spi0_cs_t control;
    control.as_int = mmio_read(SPI0_CS);
    return control;
}

void spi0_divisor(uint32_t divisor){
    mmio_write(SPI0_CLK, divisor);
}