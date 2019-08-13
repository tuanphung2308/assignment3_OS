#include "kernel.h"
void kernel_main(void) 
{
	uart_init();

    i2c_init();
    i2c_set_slave(0x68);
    i2c_enable();
    start_tx();

    spi0_init();
    spi0_chip_select(0);

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
    int pause = 0;
    while(1){
        unsigned char input_char = uart_getc_without_waiting();
        if (input_char == 'p' || input_char == 'P') pause = 1;
        if (input_char == 'r' || input_char == 'R') pause = 0;
        i2c_write_byte(0x00);
        
        //i2c_read_data((uint8_t *)buf, 8);

        spi0_start_transfer();
        spi_transfer_byte(i2c_read_data((uint8_t *)buf, 8));
        spi_stransfer_stop();
    }
}