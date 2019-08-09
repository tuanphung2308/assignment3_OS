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