#ifndef	__STDLIB__
#define	__STDLIB__
#include <stdint.h>

enum day
{
    NaN,
    Sunday,
    Monday,
    Tuesday, 
    Wednesday, 
    Thursday, 
    Friday, 
    Saturday,
};
char* itoa(int i);
int atoi(char* cstring); // Converts a C string to an integer
char* dectohex(int i);    // Converts an integer to C string in hexadecimal form
void bzero(void* dest, int bytes); // Zeros out a segment of 'bytes' in memory
void delay(uint32_t count);
void mmio_write(uint32_t reg, uint32_t data);
uint32_t mmio_read(uint32_t reg);
const char* get_week_day(int value);
uint8_t get_bit(uint8_t data, int position);
uint8_t bcdToDec(uint8_t val);
uint8_t decToBcd(uint8_t val);
#endif