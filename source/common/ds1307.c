#include "ds1307.h"
uint8_t get_bit(uint8_t data, int position) {
    return (data >> position) &  1;
}

uint8_t bcdToDec(uint8_t val){
    return (val / 16 * 10) + (val % 16);
}

uint8_t decToBcd(uint8_t val) {
    return (val / 10 * 16) + (val % 10);
}

void print_format(uint8_t val) {
    if (val < 10) puts("0");
    puts(itoa(bcdToDec(val)));
}

uint8_t format_hour (uint8_t value, int hour_mode){
    // return decToBcd(value) + ((hour_mode == MODE_12) ? 0 : (1 << 6));
    if (hour_mode == MODE_12) {
        //select when high
        //high = pm, low = am
        if (value >= 12) {
            // puts("Set clock: ");
            // puts(itoa(decToBcd(value - 12) + 0x60));
            // puts("\r\n");
            return decToBcd(value - 12) | 0x60; // both being high
        } else {
            return decToBcd(value) + 0x40; // hour mode = high, AM 
        }
    } else {
        //select when low
        // puts("Set clock: ");
        // puts(itoa(decToBcd(value - 12) + 0x60));
        // puts("\r\n");
        return decToBcd(value);   
    }
}

void print_time(uint8_t buf[]) {
    puts(get_week_day(buf[3]));
    puts(" ");
    print_format(buf[4]); //Date
    puts("/");
    print_format(buf[5]);
    puts("/");
    puts("20");
    print_format(buf[6]);
    puts(" ");
    print_format(buf[2]);
    puts(":");
    print_format(buf[1]);
    puts(":");
    print_format(buf[0]);
}

void set_time(uint8_t sec, uint8_t min, uint8_t hr, uint8_t hr_mode, uint8_t day, uint8_t date, uint8_t month, uint8_t year){
    //clear fifo before transaction
    bsc1_control_t control;
    bzero(&control,4);        //zero-out the control register
    mmio_write(BSC1_C, control.as_int);
    control.CLEAR_1 = 1;   
    mmio_write(BSC1_C, control.as_int);
    //register address of tiny has 1 byte
    mmio_write(BSC1_DLEN, 0x08);
    //1st byte: reg address of RTC

    mmio_write(BSC1_FIFO, 0);
    mmio_write(BSC1_FIFO, decToBcd(sec));
    mmio_write(BSC1_FIFO, decToBcd(min));
    mmio_write(BSC1_FIFO, format_hour(hr ,hr_mode));
    mmio_write(BSC1_FIFO, decToBcd(day));
    mmio_write(BSC1_FIFO, decToBcd(date));
    mmio_write(BSC1_FIFO, decToBcd(month));
    mmio_write(BSC1_FIFO, decToBcd(year));

    bzero(&control, 4);
    control.I2CEN = 1;
    control.READ = 0;
    control.ST = 1;
    mmio_write(BSC1_C, control.as_int);

    bsc1_status_t status;
    do {
        status = read_status();
    } while (!(status.done));
    
    mmio_write(BSC1_S, (1 << 1));
}