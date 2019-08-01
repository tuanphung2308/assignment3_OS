#include <stdint.h>
#include <stdio.h>
int main(void) {
    int i = 0;
    #define MAX_VALUE 2147483647 // Max value of 32-bit
    static char hexvalue[11] = "";  // HEX_SIZE 32 Bit -> 32/4 + 3 (0x + '\0')
    uint16_t j = 0;
    uint16_t temp = 0;
    int sign = 0;
    if (i < 0) {
        i = MAX_VALUE + i + 1;
        sign = 1;
    }
    if (i == 0)
    {
        hexvalue[0] = '0';    // cstring is char array with 12 elements
        hexvalue[1] = '\0';
        printf("0");
    } else {
while (i != 0)    // Where does i come from?
    {
        if ((i % 16) < 10)    // Why does this condition exist?
                            // Hint: Look at the ASCII table
                            // How should hexvalue be declared?
            hexvalue[j++] = 48 + (i % 16);
        else                  // What is the purpose of decimal values 48 and 55?
            hexvalue[j++] = 55 + (i % 16);
        i /= 16;
    }
    if (sign == 1) {
        hexvalue[j-1] = 'F';
    }
    // Input x0
    // hexvalue[j++] = 'x';
    // hexvalue[j++] = '0';
    hexvalue[j] = '\0';
    // Reverse
    j--;
    while (i < j)    // What does this while loop do?
    {
        temp = hexvalue[i];
        hexvalue[i] = hexvalue[j];
        hexvalue[j] = temp;
        j--;
        i++;
    }
    printf("%s\n", hexvalue);
    }
}