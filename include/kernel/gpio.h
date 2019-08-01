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

struct gpio_register_map {
	uint32_t FSEL[6];
	uint32_t: 32;
	uint32_t SET[2];
	uint32_t: 32;
	uint32_t CLR[2];
	uint32_t: 32;
	uint32_t LEV[2];
	uint32_t: 32;
	uint32_t EDS[2];
	uint32_t: 32;
	uint32_t REN[2];
	uint32_t: 32;
	uint32_t FEN[2];
	uint32_t: 32;
	uint32_t HEN[2];
	uint32_t: 32;
	uint32_t LEN[2];
	uint32_t: 32;
	uint32_t AREN[2];
	uint32_t: 32;
	uint32_t AFEN[2];
	uint32_t: 32; // 0x90
	uint32_t PUD;
	uint32_t PUDCLK[2];
// BCM2711 only
	uint32_t: 32; // 0xa0
	uint32_t: 32;
	uint32_t: 32;
	uint32_t: 32;
	uint32_t: 32; // 0xb0
	uint32_t: 32;
	uint32_t: 32;
	uint32_t: 32;
	uint32_t: 32; // 0xc0
	uint32_t: 32;
	uint32_t: 32;
	uint32_t: 32;
	uint32_t: 32; // 0xd0
	uint32_t: 32;
	uint32_t: 32;
	uint32_t: 32;
	uint32_t: 32; // 0xe0
	uint32_t PUPPDN[4];
};
#endif