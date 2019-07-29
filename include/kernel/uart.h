#ifndef __UARTHEADER__
#define __UARTHEADER__
#include <stddef.h>
#include <stdint.h>
#include "../../include/common/stdlib.h"
#include "../../include/common/stdio.h"
enum
{
    GPIO_BASE    = 0x3F200000,           // GPIO Base Address
    GPPUD        = (GPIO_BASE + 0x94),   // GPIO Pin Pull Up/Down Enable
    GPPUDCLK0    = (GPIO_BASE + 0x98),   // GPIO Pin Pull Up/Down Enable Clock 0
    UART0_BASE   = (GPIO_BASE + 0x1000), // UART Base Address
    UART0_DR     = (UART0_BASE + 0x00),  // Data Register
    UART0_RSRECR = (UART0_BASE + 0x04),  // Receive Status Register / Error Clear Register
    UART0_FR     = (UART0_BASE + 0x18),  // Flag Register
    UART0_ILPR   = (UART0_BASE + 0x20),  // Disable IrDA Register
    UART0_IBRD   = (UART0_BASE + 0x24),  // Integer Baud Rate Division Register
    UART0_FBRD   = (UART0_BASE + 0x28),  // Fractional Baud Rate Division Register
    UART0_LCRH   = (UART0_BASE + 0x2C),  // Line Control Register
    UART0_CR     = (UART0_BASE + 0x30),  // Control Register
    UART0_IFLS   = (UART0_BASE + 0x34),  // Interrupt FIFO Level Select Register
    UART0_IMSC   = (UART0_BASE + 0x38),  // Interrupt Mask Set / Clear Register
    UART0_RIS    = (UART0_BASE + 0x3C),  // Raw Interrupt Status Register
    UART0_MIS    = (UART0_BASE + 0x40),  // Masked Interrupt Status Register
    UART0_ICR    = (UART0_BASE + 0x44),  // Interrupt Clear Register
    UART0_DMACR  = (UART0_BASE + 0x48),  // Disable DMA Control Register
    UART0_ITCR   = (UART0_BASE + 0x80),  // Test Control Register
    UART0_ITIP   = (UART0_BASE + 0x84),  // Test Control Register
    UART0_ITOP   = (UART0_BASE + 0x88),  // Test Control Register
    UART0_TDR    = (UART0_BASE + 0x8C),  // Test Data Register
};

typedef union uart_flags
{
    struct
    {
        uint8_t clear_to_send : 1; // This bit is the complement of the UART clear to send
        uint8_t data_set_ready : 1; // uns
        uint8_t data_carrier_detected : 1;  //uns
        uint8_t busy : 1; //busy transmitting data 
        uint8_t receive_queue_empty : 1; // set wen rx queue empty
        uint8_t transmit_queue_full : 1; // set wen trans queue empty
        uint8_t receive_queue_full : 1; // set wen receieve queue full
        uint8_t transmit_queue_empty : 1; // set wen transmit queue full
        uint8_t ring_indicator : 1; // unsp
        uint32_t padding : 23; // reserved
    };
    uint32_t as_int;
} uart_flags_t;

typedef union uart_control
{
    struct
    {
        uint8_t uart_enabled : 1; //ENABLE UART
        uint8_t sir_enabled : 1; //unsupported
        uint8_t sir_low_power_mode : 1; //unsupported
        uint8_t reserved : 4; // reserved
        uint8_t loop_back_enabled : 1; // the modem outputs are also fed through to the modem inputs
        uint8_t transmit_enabled : 1; //enable transmit. the UART is disabled in the middle of 
        // transmission, it completes the current
        // character before stopping
        uint8_t receive_enabled : 1; // RECEIEVE enable.
        uint8_t data_transmit_ready : 1; //unsupported
        uint8_t request_to_send : 1; //Request to send. This bit is the 
        //complement of the UART request to send
        uint8_t out1 : 1; // unsupported
        uint8_t out2 : 1; // unsupported
        uint8_t rts_hardware_flow_control_enabled : 1; 
        //Data is only requested when there is space in the receive FIFO for it to be received
        uint8_t cts_hardware_flow_control_enabled : 1; //Data is only transmitted when the
        //nUARTCTS signal is asserted
        uint16_t padding;
    };
    uint32_t as_int;
} uart_control_t;

void mmio_write(uint32_t reg, uint32_t data);
uint32_t mmio_read(uint32_t reg);
uart_flags_t read_flags(void);
unsigned char uart_getc(void);
void uart_putc(unsigned char c);
void uart_init(void);
#endif