#include "../../include/common/stdlib.h"
#include <stddef.h>
#include <stdint.h>
char* itoa(int i) {
    uint16_t isneg = 0;
    static char cstring[11] = "";
    uint16_t j = 0;
    if (i == 0)
    {
        cstring[0] = '0';    // cstring is char array with 12 elements
        cstring[1] = '\0';
        return cstring;
    }
    if (i < 0)
    {
        isneg = 1;    // isneg can be declared as an integer variable
        i = -i;
    }
    while (i != 0)
    {
        cstring[j++] = '0' + (i % 10); // What is '0' for? 
                                    // Hint: Have a look at the ASCII table
        i /= 10;
    }

    if (isneg)
        cstring[j++] = '-';
    cstring[j] = '\0';
    j--;    // Why is j decremented here? 
            // Hint: Look at the purpose of the next step

    i = 0;
    while (i < j)    // What does this while loop do?
    {
        isneg = cstring[i];
        cstring[i] = cstring[j];
        cstring[j] = isneg;
        j--;
        i++;
    }
    return cstring;
}

int atoi(char* cstring) {
    int res = 0; // Initialize result 
    // Iterate through all characters of input string and 
    // update result 
    for (int i = 0; cstring[i] != '\0'; ++i) 
        res = res * 10 + cstring[i] - '0'; 
  
    // return result. 
    return res;  // res will need to be declared in this function?
}

char*       dectohex(int i)  {
    #define MAX_VALUE 2147483647 // Max value of 32-bit
    static char hexvalue[11] = "";  // HEX_SIZE 32 Bit -> 32/4 + 3 (0x + '\0')
    uint16_t j = 0;
    uint16_t temp = 0;
    int sign = 0;
    if (i == 0)
    {
        hexvalue[0] = '0';    // cstring is char array with 12 elements
        hexvalue[1] = '\0';
        return hexvalue;
    } else {
        if (i < 0) {
            i = MAX_VALUE + i + 1;
            sign = 1;
        }
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
        // if (j == 1)
        //     hexvalue[j++] = '0';
        // }
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
    }
    return hexvalue;  
}

void bzero(void* dest, int bytes) {
    char* d = dest;
    while (bytes--)
        *d++ = 0; // What does this line of code do?
}// Zeros out a segment of 'bytes' in memory 

void delay(uint32_t count) {
    asm volatile("__delay_%=: subs %[count], %[count], #1; bne __delay_%=\n"
              : "=r"(count) : [count] "0" (count) : "cc");
}

void mmio_write(uint32_t reg, uint32_t data)
{
    *(volatile uint32_t*)reg = data;
}

uint32_t mmio_read(uint32_t reg)
{
    return *(volatile uint32_t*)reg;
}

const char* get_week_day(int day) {
    switch (day){
        case Sunday:
            return "Sunday";
        case Monday:
            return "Monday";
        case Tuesday:
            return "Tuesday";
        case Wednesday:
            return "Wednesday";
        case Thursday:
            return "Thursday";
        case Friday:
            return "Friday";
        case Saturday:
            return "Saturday";
        default:
            return "ERR";
    }
}