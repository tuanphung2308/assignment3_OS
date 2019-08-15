#include "spi.h"
#include "uart.h"

void spi_configure()
{
    spi0_cs_t control;
    bzero(&control, 4);
    // control.clock_polarity = 0;
    // control.cspol0 = 0;
    // control.cspol1 = 0;
    // control.cspol2 = 0;
    // control.cspol = 0;
    mmio_write(SPI0_CS, control.as_int);
    mmio_write(SPI0_CLK, 10000);
}

void spi_set_ce(uint8_t ce)
{
    spi0_cs_t control = read_cs();
    control.chip_select = ce;
    mmio_write(SPI0_CS, control.as_int);
}

void spi_transfer_start(void)
{
    spi0_cs_t control = read_cs();

    control.transfer_active = 1;
    mmio_write(SPI0_CS, control.as_int);
}

void spi_transfer_stop(void)
{
    spi0_cs_t control = read_cs();
    while(!(control.done)) {
        control = read_cs();
    };// wait for DONE
    control.transfer_active = 0;
    mmio_write(SPI0_CS, control.as_int);
}

uint8_t spi_transfer_byte(uint8_t data)
{
    spi0_cs_t control = read_cs();
    control.clear = 3;
    mmio_write(SPI0_CS, control.as_int);
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