#include "kernel.h"
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
    set_time(1, 43, 10, MODE_12, 4, 15, 8, 19);

    uint8_t buf[]     = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    uint8_t spi_buf[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    
    int pause = 0;
    uint8_t switch_mode = 0;
    uint8_t current_sec = 0;
    
    while(1) {
        unsigned char input_char = uart_getc_without_waiting();
        if (input_char == 'p' || input_char == 'P') pause = 1;
        if (input_char == 'r' || input_char == 'R') pause = 0;
        if (input_char == 't'|| input_char == 'T') switch_mode = 1;

        if (!pause) {
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

            if (switch_mode == 1) {
                puts("Toggle Mode \r\n");
                uint8_t converted_hour = bcdToDec(buf[2]);
                if (hour_mode == MODE_12 && current_m == PM)
                    converted_hour += 12;
                
                set_time(
                    bcdToDec(buf[0]), 
                    bcdToDec(buf[1]), 
                    (converted_hour), 
                    !hour_mode, 
                    bcdToDec(buf[3]), 
                    bcdToDec(buf[4]), 
                    bcdToDec(buf[5]), 
                    bcdToDec(buf[6])
                );
                switch_mode = 0;
                continue;
            }

            for (int i = 0; i < 8; i++) {
                spi_transfer_start();           // Start a new transfer -> CS */
                spi_buf[i] = spi_transfer_byte(buf[i]); // Send don't care and recive the last byte
                spi_transfer_stop();
            }

            if (current_sec != buf[0]){
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
                current_sec = buf[0];
                // delay(5579728); // dont need this one
            }
        }
    }
}