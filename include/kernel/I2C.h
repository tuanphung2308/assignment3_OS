#ifndef __I2C__
#define __I2C__
#include <stddef.h>
#include <stdint.h>

enum
{
    I2C_BASE    = (0x3F205000),         // I2C Base address
    I2C_C       = (I2C_BASE + 0x00),    // I2C Control 
    I2C_s       = (I2C_BASE + 0x04),    // I2C Status
    I2C_DLEN    = (I2C_BASE + 0x08),    // I2C Data Length
    I2C_A       = (I2C_BASE + 0x0c),    // I2C Slave Address 
    I2C_FIFO    = (I2C_BASE + 0x10),    // I2C Data FIFO 
    I2C_DIV     = (I2C_BASE + 0x14),    // I2C Clock Divider
    I2C_DEL     = (I2C_BASE + 0x18),    // I2C Data Delay
    I2C_CLKT    = (I2C_BASE + 0x1c),    // I2C Clock Stretch Timeout 
};

typedef union I2C_flags
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
} I2C_flags_t;

typedef union I2C_control
{
    struct
    {
        uint8_t I2C_enabled : 1; //ENABLE I2C
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
} I2C_control_t;

#endif