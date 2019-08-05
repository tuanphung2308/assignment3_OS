#include "../../include/common/stdio.h"
#include "../../include/kernel/uart.h"
#include "../../include/kernel/i2c.h"

void kernel_main(void) 
{
	uart_init();
    i2c_init();

    i2c_write_address(0x68);
    
    i2c_enable();

    mmio_write(BSC0_S, 0x50);

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

    char buf[]={0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    char *str[]  ={
        "Sunday",
        "Monday", 
        "Tuesday", 
        "Wednesday", 
        "Thursday", 
        "Friday", 
        "Saturday"
    };
    int pause = 0;
    char current_sec = 0x00;
    while (1) {
        unsigned char input_char = uart_getc_without_waiting();
        if (input_char == 'p') pause = 1;
        if (input_char == 'r') pause = 0;
        if (!pause) {
            i2c_write_byte(0x00);
    
            i2c_read_data((uint8_t *)buf, 8);
            buf[0] = buf[0] & 0x7F; //sec
            if (current_sec != buf[0]){
                buf[1] = buf[1] & 0x7F; //min
                buf[2] = buf[2] & 0x3F; //hour
                buf[3] = buf[3] & 0x07; //day of the week
                buf[4] = buf[4] & 0x3F; //day of the month
                buf[5] = buf[5] & 0x1F; //month
                
                char temp = buf[4];
                buf[4] = buf[6];
                buf[6] = temp;
                
                puts(str[(unsigned char)buf[3]-1]);
                puts(" ");
                for (int i = 6; i >= 0; i--) {
                    if (i == 3) continue;
                    if (buf[i] < 10) puts("0");
                    puts(dectohex(buf[i]));
                    if (i > 4) {
                        puts("/");
                    } else if (i == 4) {
                        puts(" ");
                    } else if (i > 0){
                        puts(":");
                    }
                }
                puts("\r\n");
                current_sec = buf[0];
            }           
        }
        // delay(5500000);
    }
}