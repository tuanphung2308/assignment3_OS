#include "../../include/common/stdio.h"
#include "../../include/kernel/uart.h"

void kernel_main(void) 
{
	uart_init();
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

    puts("------------------------------------------------------------\r\n");
    char* buf = ""; 
    int buflen = 10;
    puts("Enter an integer: \n\r");
    gets(buf, buflen);
    puts("The hex result is: ");
    puts(dectohex(atoi(buf)));
    puts("\r\n");
    puts("------------------------------------------------------------\r\n");

	while (1)
		putc(getc());
}