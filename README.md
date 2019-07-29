"# assignment1-EEET2490" 

```
arm-none-eabi-gcc --version
qemu-system-arm --version
make --version
make clean
make all
qemu-system-arm -m 256 -M raspi2 -serial stdio -kernel myos.elf
```

A Raspberry Pi has two UARTs: Mini UART and PL011 UART.
In first assignment, we are going to work with first one
Specification of the Mini UART registers, please go to page 8 of the BCM2837 ARM Peripherals manual.
Official Rasp's Doc
https://www.raspberrypi.org/documentation/configuration/uart.md
GPIO
To be able to use the Mini UART, we need to activate pins 14 and 15
ref: page 102 of BCM2837 ARM Peripherals manual
Pins 14 and 15 have the TXD1 and RXD1 alternative functions available. This means that if we select alternative function number 5 for pins 14 and 15, they will be used as a Mini UART Transmit Data pin and Mini UART Receive Data pin, respectively. The GPFSEL1 register is used to control alternative functions for pins 10-19. 
ref: page 92 of BCM2837 ARM Peripherals manual
