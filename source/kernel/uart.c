#include "../../include/kernel/uart.h"
void mmio_write(uint32_t reg, uint32_t data)
{
    *(volatile uint32_t*)reg = data;
}

uint32_t mmio_read(uint32_t reg)
{
    return *(volatile uint32_t*)reg;
}

uart_flags_t read_flags(void)
{
    uart_flags_t flags;
    flags.as_int = mmio_read(UART0_FR);
    return flags;
}

unsigned char uart_getc(void)
{
    uart_flags_t flags;
    do
    {
        flags = read_flags();
    } while (flags.receive_queue_empty);
    // What does the condition in the while loop mean?
    // Rx Fifo Empty Flag when the receive FIFO is full.
    return mmio_read(UART0_DR);
}

void uart_putc(unsigned char c)
{
    uart_flags_t flags;
    do
    {
        flags = read_flags();
    } while (flags.transmit_queue_full);
    // What does the condition in the while loop mean?
    // Tx Fifo Full flag the TXFE bit is set when the transmit FIFO is empty.
    // This bit does not indicate if there is data in the
    // transmit shift register

    mmio_write(UART0_DR, c);
}

void uart_init(void)
{
    uart_control_t control;
    bzero(&control, 4);
    //Zero out 4 consecutive bytes

    mmio_write(UART0_CR, control.as_int);
    //Disable UART0
    // What is the purpose of the three previous lines?

    // Disable pull up/down for all GPIO pins & delay for 150 cycles.
    mmio_write(GPPUD, 0x00000000);
    delay(150);    // Why is the delay required here? // 150 cycles

    // Disable pull up/down for pin 14,15 & delay for 150 cycles.
    mmio_write(GPPUDCLK0, (1 << 14) | (1 << 15));
    delay(150);    // Why is the delay required here? // 150 cycles

    // Write 0 to GPPUDCLK0 to make it take effect.
    mmio_write(GPPUDCLK0, 0x00000000);
    // What is the purpose of the five previous lines?

    // Clear pending interrupts.
    mmio_write(UART0_ICR, 0x7FF);

    // Enable FIFO & 8 bit data transmission (1 stop bit, no parity).
    mmio_write(UART0_LCRH, (1 << 4) | (1 << 5) | (1 << 6));
    // Mask all interrupts.
    mmio_write(UART0_IMSC, (1 << 1) | (1 << 4) | (1 << 5) | (1 << 6) |
                           (1 << 7) | (1 << 8) | (1 << 9) | (1 << 10));
    control.uart_enabled = 1;
    control.transmit_enabled = 1;
    control.receive_enabled = 1;
    // Enable UART0, receive & transfer part of UART.
    mmio_write(UART0_CR, control.as_int);
}