#include "../../include/kernel/i2c.h"
// #include "gpio.h"
#include "../../include/kernel/gpio.h"
#include "../../include/common/stdlib.h"
#include "../../include/kernel/uart.h"
void i2c_init() {
    //Init I2C master
    // Enable alternative function ALT0 for PIN00 and PIN01
    mmio_write(GPFSEL, (1 << 2) | (1 << 5));
}

void i2c_end() {
    //Set all pin back to input
    mmio_write(GPFSEL, (0 << 2) | (0 << 5) );
}

bsc0_status_t read_status() {
    bsc0_status_t status;
    status.as_int = mmio_read(BSC0_S);
    return status;
}

bsc0_control_t read_control() {
    bsc0_control_t controls;
    controls.as_int = mmio_read(BSC0_C);
    return controls;
}

void wait_i2c_done() {
    //Wait till i2c done
    int timeout = 50;
    bsc0_status_t status;
    do
    {
        status = read_status();
    } while (status.done && --timeout);
    if (timeout == 0) {
        //Error
    }
}

void i2c_clear_status() {
    bsc0_status_t status = read_status();
    status.clkt = 0;
    status.err = 0;
    status.done = 1;
    mmio_write(BSC0_C, status.as_int);
}

void i2c_read(char* buf, uint32_t len) {
    bsc0_status_t status = read_status();

    uint32_t remaining = len;
	uint32_t i = 0;

    //Clear FIFO
    i2c_clear_fifo();
    // Clear status
    i2c_clear_status();
    // Set data Length
    i2c_set_data_len(len);
    bsc0_control_t control = read_control();
    control.I2CEN = 1; // Enable I2C
    control.ST = 1; // Start transfer
    control.READ = 1; // Mode = read
    mmio_write(BSC0_C, control.as_int);

    do {
        while (status.rxd) {
            // If fifo contain data
            buf[i] = mmio_read(BSC0_FIFO);
            i++;
            remaining--;
        }
        status = read_status();
    } while (!(status.done));

    // Read anything left in FIFO
    do {
        buf[i] = mmio_read(BSC0_FIFO);
        i++;
        remaining--;
        status = read_status();
    } while (remaining && status.rxd);
    
    // Received a NACK
	// if (mmio_read(status) & BCM2835_BSC_S_ERR)
	// {
	// 	reason = BCM2835_I2C_REASON_ERROR_NACK;
	// }

	// Received Clock Stretch Timeout
	// else if (bcm2835_peri_read(status) & BCM2835_BSC_S_CLKT)
	// {
	// 	reason = BCM2835_I2C_REASON_ERROR_CLKT;
	// }

	// Not all data is sent
	// if (remaining)
	// {
	// 	// There is data left.
	// }
    i2c_transfer_done();
}

void i2c_write(const char * buf, uint32_t len) {
    //SLAVE ADDRESS
    uint32_t SLAVE_ADDRESS = 0x10000; //define slave address here

    bsc0_status_t status = read_status();
    i2c_init();
    mmio_write(BSC0_A, SLAVE_ADDRESS);

    //Clear FIFO
    i2c_clear_fifo();

    //Clear Status
    i2c_clear_status();

    // Set data length
    i2c_set_data_len(len);

    uint32_t remaining = len;
    uint32_t i = 0;
    // uint8_t reason = 

    // Fill FIFO with data
    while (remaining && (i < BSC_FIFO_SIZE )) {
        mmio_write(BSC0_FIFO, buf[i]);
        i++;
        remaining--;
    }

    //Enable device and start transfer
    bsc0_control_t control;
    control.ST = 1; // Start transfer
    control.I2CEN = 1; // Enable I2C
    mmio_write(BSC0_C, control.as_int);

    do {
        while (remaining && status.txd) {
            // If have data remaining and fifo can accept more data
            mmio_write(BSC0_FIFO, buf[i]);
            i++;
            remaining--;
        }
        status = read_status();
    } while (!(status.done));

    // Received a NACK
	// if (mmio_read(status) & BCM2835_BSC_S_ERR)
	// {
	// 	reason = BCM2835_I2C_REASON_ERROR_NACK;
	// }

	// Received Clock Stretch Timeout
	// else if (bcm2835_peri_read(status) & BCM2835_BSC_S_CLKT)
	// {
	// 	reason = BCM2835_I2C_REASON_ERROR_CLKT;
	// }

	// Not all data is sent
	// if (remaining)
	// {
	// 	// There is data left.
	// }
    i2c_transfer_done();
}

void i2c_set_data_len(uint32_t len) {
    bsc0_dlen dlength;
    dlength.dlen = len;
    mmio_write(BSC0_DLEN, dlength.as_int);
}

void i2c_clear_fifo() {
    //Clear FIFO
    bsc0_control_t control;
    control.CLEAR = 3; // Oneshot clear
    mmio_write(BSC0_C, control.as_int);
}

void i2c_transfer_done() {
    //Clear FIFO
    bsc0_status_t status;
    status.done = 1; // Mark as done
    mmio_write(BSC0_S, status.as_int);
}