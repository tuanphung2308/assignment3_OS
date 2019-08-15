#include "gpio.h"
void fsel_alt0(int pin_no) {
    int shift = (pin_no % 10) * 3;
    int page_number = pin_no / 10;
    if (page_number == 0) {
        *(volatile uint32_t*) GPFSEL0 |= (0b100 << shift);
    } else if (page_number== 1) {
        *(volatile uint32_t*) GPFSEL1 |= (0b100 << shift);
    }
}

void i2c_init() {
    //Init I2C master
    // Enable alternative function ALT0 for PIN00 and PIN01
    *(volatile uint32_t*) GPFSEL0 |= (0b100 << 6);
    *(volatile uint32_t*) GPFSEL0 |= (0b100 << 9);
}

void spi0_init() {
    for (int i = 8; i < 12; i++) fsel_alt0(i); // set pin from 8-11 to ALT0
}