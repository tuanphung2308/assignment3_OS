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

    char buf[]={0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    char buf_2[]={0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
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
    
    while(1) {
        unsigned char input_char = uart_getc_without_waiting();
        if (input_char == 'p' || input_char == 'P') pause = 1;
        if (input_char == 'r' || input_char == 'R') pause = 0;
        if (!pause) {

            i2c_write_byte(0x00);
            i2c_read_data((uint8_t *)buf, 8);
            if (current_sec != buf[0]){
                //swapping day and year here to follow dd/mm/yy format
                char temp = buf[4]; 
                buf[4] = buf[6];
                buf[6] = temp;
                for (int i = 0; i < 8; i++) {
                    spi_transfer_start();           // Start a new transfer -> CS */
                    buf_2[i] = spi_transfer_byte(buf[i]); // Send don't care and recive the last byte
                    spi_transfer_stop();
                }
                
                puts(str[(unsigned char)buf_2[3]-1]);
                puts(" ");
                for (int i = 6; i >= 0; i--) {
                    if (i == 3) continue;
                    if (buf_2[i] < 10) puts("0");
                    puts(dectohex(buf_2[i]));
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
    }
}