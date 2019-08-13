#ifndef __KHEADER__
#define __KHEADER__
#include "../../include/common/stdio.h"
#include "../../include/kernel/uart.h"
#include "../../include/kernel/spi.h"

enum day
{
    Sunday,
    Monday,
    Tuesday, 
    Wednesday, 
    Thursday, 
    Friday, 
    Saturday,
};

void kernel_main(void);
#endif