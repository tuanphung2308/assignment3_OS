#include "kernel.h"
enum {
    MODE_24,
    MODE_12
};

enum {
    AM,
    PM
};

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
            puts("Set clock: ");
            puts(itoa(decToBcd(value - 12) + 0x60));
            puts("\r\n");
            return decToBcd(value - 12) + 0x60; // both being high
        } else {
            return decToBcd(value) + 0x40; // hour mode = high, AM 
        }
    } else {
        //select when low
        return decToBcd(value);   
    }
}

void set_time(uint8_t sec, uint8_t min, uint8_t hour, uint8_t day, uint8_t date, uint8_t month, uint8_t year, uint8_t mode) {
    uint8_t time_stop[]={
        0x00, 
        0x80
    };
    i2c_write_data(time_stop, 2);
    uint8_t time[]={
        0x00, 
        decToBcd(sec), 
        decToBcd(min), 
        format_hour(hour, mode), 
        decToBcd(day), 
        decToBcd(date), 
        decToBcd(month), 
        decToBcd(year)
    };
    i2c_write_data(time, 8);
}

void change_time_mode(uint8_t hour, uint8_t mode) {
    uint8_t time[]={
        0x02, 
        0x00 
    };
    puts("Hours is: ");
    // puts(itoa(hour));
    // buf[3] = format_hour(bcdToDec(buf[3]), mode);
    time[1] = format_hour(16, MODE_12);
    i2c_write_data(time, 2);
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

void kernel_main(void) 
{
	uart_init();

    spi0_init();
    spi_configure();
    spi_set_ce(SPI_CS_CE0);

    i2c_init();
    i2c_set_slave(0x68);
    i2c_enable();
    start_tx();

    char* print_list[] = {
        "EEET2490: Embedded Systems - Operating Systems & Interfacing\r\n",
		"------------------------------------------------------------\r\n",
		"Assessment Group: 6\r\n",
		"s3761489: Tuan Phung\r\n",
		"s3695340: Sung Jin Kim\r\n",
		"s3567923: Dang Phuc Hung\r\n",
        "\r\n",
        "DS1307 Real Time Clock Data\r\n",
        "---------------------------\r\n"
    };
    for (int i = 0; i < 9; i ++) {
        puts(print_list[i]);
    }
    set_time(3, 1, 16, 4, 15, 8, 19, MODE_24);

    uint8_t buf[]    ={0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    uint8_t spi_buf[]={0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    // char buf_2[]={0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    
    int pause = 0;
    uint8_t switch_mode = 0;
    // char current_sec = 0x00;
    // set_time_to_RTC(3,4,10, 1, 3, 14, 8, 19);
    
    while(1) {
        unsigned char input_char = uart_getc_without_waiting();
        if (input_char == 'p' || input_char == 'P') pause = 1;
        if (input_char == 'r' || input_char == 'R') pause = 0;
        if (input_char == 't'|| input_char == 'T') switch_mode = 1;
        if (switch_mode == 1) {
            puts("Toggle Mode \r\n");
            // set_time(3, 1, 16, 4, 15, 8, 19, MODE_24);
            // change_time_mode(1, MODE_12);
            set_time(3, 1, 16, 4, 15, 8, 19, MODE_12);
            switch_mode = 0;
        }
        if (!pause && !switch_mode) {
            i2c_write_byte(0x00);
            i2c_read_data((uint8_t *)buf, 8);

            uint8_t hour_mode = get_bit(buf[2], 6);
            uint8_t current_m = get_bit(buf[2], 5);

            buf[0] &= 0x7F; //second
            buf[1] &= 0x7F;
            if (hour_mode == MODE_24) {
                buf[2] &= 0x3F; // 6 consecutive
            } else {
                buf[2] &= 0x1F; // 5 consecutive 1
            }
            buf[4] &= 0x07;
            buf[5] &= 0x3F;
            buf[6] &= 0x1F;

            for (int i = 0; i < 8; i++) {
                spi_transfer_start();           // Start a new transfer -> CS */
                spi_buf[i] = spi_transfer_byte(buf[i]); // Send don't care and recive the last byte
                spi_transfer_stop();
            }

            // if (current_sec != buf[0]){
                // print_time(buf);
            print_time(spi_buf);
            if (hour_mode == MODE_12) {
                if (current_m == AM) {
                    puts(" AM");
                } else {
                    puts(" PM");
                }
            }
            puts("\r\n");
            // current_sec = buf[0];
            delay(5500000);
            // }
        }
    }
}