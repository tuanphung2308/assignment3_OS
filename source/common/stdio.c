#include "../../include/common/stdio.h"
char getc(void) {
    return uart_getc();
}
void putc(char c) {
    uart_putc(c);
}
void gets(char* buf, int buflen) {
    int i = 0;
    char c;
    for (i = 0; (c = getc()) != '\r' && (buflen > 1); i++, buflen--)
    {   // How do the parameters in the for loop work?
        // What is '\r' and how is it different from '\n'?
        // The '\n' is the "Line Feed" and '\r' is the carriage return. 
        //\r commands the carriage to go back leftwards until it hits the leftmost stop (a slow operation)
        //\n commands the roller to roll up one line (a much faster operation) -- that's the reason you always have 
        // \r before \n, so that the roller can move while the carriage is still going leftwards!
        putc(c);
        buf[i] = c;
    }
    putc('\n');
    if (c == '\n')    // Why does this condition exist?
        buf[i] = '\0';
    else
        buf[buflen-1] = '\0';
}
void puts(const char* buf) {
    for (int i = 0; buf[i] != '\0'; i++)
        putc(buf[i]);
}