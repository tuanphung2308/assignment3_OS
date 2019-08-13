#include "kernel.h"
void kernel_main(void) 
{
	uart_init();

    //SPI Initialize
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
    while (1) {
        spi0_start_transfer();
        uint8_t ret = spi_transfer_byte(0x8);
        puts("Value is: ");
        puts(dectohex(ret));
        puts("\r\n");
        delay(500000);
        spi_transfer_stop();
    }
}