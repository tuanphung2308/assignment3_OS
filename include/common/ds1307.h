#ifndef __RTCTIME__
#define __RTCTIME__
#include "i2c.h"
#include <stddef.h>
#include <stdint.h>
enum {
    MODE_24,
    MODE_12
};

enum {
    AM,
    PM
};

uint8_t get_bit(uint8_t data, int position);
uint8_t bcdToDec(uint8_t val);
uint8_t decToBcd(uint8_t val);
void print_format(uint8_t val);
uint8_t format_hour (uint8_t value, int hour_mode);
void print_time(uint8_t buf[]);
void set_time(uint8_t sec, uint8_t min, uint8_t hr, uint8_t hr_mode, uint8_t day, uint8_t date, uint8_t month, uint8_t year);
#endif