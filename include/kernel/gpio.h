#ifndef __GPIO__
#define __GPIO__
#include <stddef.h>
#include <stdint.h>
enum
{
    GPIO_BASE    = 0x3F200000,           // GPIO Base Address
    GPFSEL       = (GPIO_BASE + 0x00),   // Function Select
    GPPUD        = (GPIO_BASE + 0x94),   // GPIO Pin Pull Up/Down Enable
    GPPUDCLK0    = (GPIO_BASE + 0x98),   // GPIO Pin Pull Up/Down Enable Clock 0
};
#endif