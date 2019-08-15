#include "ds1307.h"

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

void set_time(uint8_t sec, uint8_t min, uint8_t hr, uint8_t rtc_mode, uint8_t day, uint8_t date, uint8_t month, uint8_t year){
    // CLEAR FIFO x.x
    bsc1_control_t control = read_control();
    control.CLEAR_1 = 1;   
    mmio_write(BSC1_C, control.as_int);
    // DLEN = 8 Bit
    mmio_write(BSC1_DLEN, 8);

    mmio_write(BSC1_FIFO, 0x00);
    mmio_write(BSC1_FIFO, decToBcd(sec));
    mmio_write(BSC1_FIFO, decToBcd(min));
    mmio_write(BSC1_FIFO, format_hour(hr ,rtc_mode));
    mmio_write(BSC1_FIFO, decToBcd(day));
    mmio_write(BSC1_FIFO, decToBcd(date));
    mmio_write(BSC1_FIFO, decToBcd(month));
    mmio_write(BSC1_FIFO, decToBcd(year));

    //Start Writing and send everythign within 1 packet
    control = read_control();
    control.I2CEN = 1;
    control.READ = 0;
    control.ST = 1;
    mmio_write(BSC1_C, control.as_int);


    bsc1_status_t status;
    do {
        status = read_status();
    } while (!(status.done));
    
    //Clear status
    status = read_status();
    status.done = 1;
    mmio_write(BSC1_S, status.as_int);
}