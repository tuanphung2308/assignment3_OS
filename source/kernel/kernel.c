#include "../../include/common/stdio.h"
#include "../../include/kernel/uart.h"
#include "../../include/kernel/i2c.h"

void kernel_main(void) 
{
	uart_init();
    i2c_init();
	// putc(getc());
	// gets(buf, buflen);
    // puts(buf);

    char* print_list[] = {
        "EEET2490: Embedded Systems - Operating Systems & Interfacing\r\n",
		"------------------------------------------------------------\r\n",
		"Assessment Group: 6\r\n",
		"s3761489: Tuan Phung\r\n",
		"s3695340: Sung Jin Kim\r\n",
		"s3567923: Dang Phuc Hung\r\n"
    };
    for (int i = 0; i < 6; i ++) {
        puts(print_list[i]);
    }

    // DATA LEN
    i2c_set_data_len(1);
    //FIFO
    mmio_write(BSC0_FIFO, 0x00);
    mmio_write(BSC0_A, 0x68);
    //CLEAR STATUS
    i2c_clear_status();

    //START WRITE
    bsc0_control_t control = read_control();
    control.I2CEN = 1; // Enable I2C
    control.ST = 1; // Start transfer
    control.READ = 0;
    mmio_write(BSC0_C, control.as_int);
    wait_i2c_done();
    // i2c_write(0x2, 1);

    
    char buf[]={0x00, 0x20, 0x19, 0x18, 0x06, 0x12, 0x08, 0x16};
    i2c_read(buf, 8);
    puts("20");
    puts(itoa(buf[6]));
    puts("\r\n");

    while (1)
        putc(getc());
}