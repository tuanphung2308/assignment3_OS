#ifndef __STDIO__
#define __STDIO__
#include "../../include/kernel/uart.h"
char getc(void);
void putc(char c);
void gets(char* buf, int buflen);
void puts(const char* buf);
#endif