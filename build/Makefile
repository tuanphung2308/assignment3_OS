TARGET = kernel7.img
ELFILE = myos.elf 
CC = arm-none-eabi-gcc
OC = arm-none-eabi-objcopy
EM = qemu-system-arm.exe
CPU = cortex-a7
MEM = 1024
CFLAGS = -mcpu=$(CPU) -fpic -ffreestanding -Wall
EMFLAGS = -m $(MEM) -M raspi2 -serial stdio -kernel $(OBJS_DIR)/$(ELFILE)
CSRCFLAGS = -g -O2 -Wall -Wextra
LFLAGS = -ffreestanding -O2 -nostdlib

SRC_CMN_DIR = ../source/common
SRC_KER_DIR = ../source/kernel
INC_CMN_DIR = ../include/common
INC_KER_DIR = ../include/kernel
OBJS_DIR = objects
EM_DIR = C:/Program Files/qemu
CC_DIR = C:/Program Files (x86)/GNU Tools ARM Embedded/8 2019-q3-update/bin
# /usr/local/Cellar/arm-none-eabi-gcc/8-2018-q4-major
# EM_DIR = no qemu yet
# CC_DIR = /usr/local/Cellar/arm-none-eabi-gcc/8-2018-q4-major/bin
all: $(TARGET)

clean: 
	# @echo Removing Object Files $(wildcard $(OBJS_DIR)/*.o)
	# @rm $(OBJS_DIR)/*.o 
	# @rm $(OBJS_DIR)/*.img
	# @rm $(OBJS_DIR)/*.elf
	# @echo Removing $(ELFILE)

	@echo Removing Object Files $(wildcard $(OBJS_DIR)/*.o)
	@del $(OBJS_DIR)\*.o
	@echo Removing $(OBJS_DIR)/$(ELFILE)
	@del $(OBJS_DIR)\$(ELFILE)
	@echo Removing $(OBJS_DIR)/$(TARGET)
	@del $(OBJS_DIR)\$(TARGET)

# run:
# 	@echo Mounting image to QEMU  
# 	qemu-system-arm -m 256 -M raspi2 -serial stdio -kernel $(OBJS_DIR)/myos.elf

run: $(OBJS_DIR)/$(ELFILE)
	@$(EM_DIR)/$(EM) $(EMFLAGS)

.PHONY: clean all run

$(OBJS_DIR)/%.o : $(SRC_KER_DIR)/%.s 
	@echo Compiling Assembler Source File $< 
	@$(CC_DIR)/$(CC) $(CFLAGS) -c $< -o $@ 

$(OBJS_DIR)/%.o : $(SRC_CMN_DIR)/%.c $(INC_CMN_DIR)/%.h 
	@echo Compiling C Source File $< 
	@$(CC_DIR)/$(CC) $(CFLAGS) -I$(INC_CMN_DIR) -I$(INC_KER_DIR) -c $< -o $@ $(CSRCFLAGS)

$(OBJS_DIR)/%.o : $(SRC_KER_DIR)/%.c $(INC_KER_DIR)/%.h 
	@echo Compiling C Source File $< 
	@$(CC_DIR)/$(CC) $(CFLAGS) -I$(INC_CMN_DIR) -I$(INC_KER_DIR) -c $< -o $@ $(CSRCFLAGS)

$(OBJS_DIR)/%.o : $(SRC_KER_DIR)/%.c
	@echo Compiling C Source File $< 
	@$(CC_DIR)/$(CC) $(CFLAGS) -I$(INC_CMN_DIR) -I$(INC_KER_DIR) -c $< -o $@ $(CSRCFLAGS) 

$(OBJS_DIR)/$(ELFILE) : $(OBJS_DIR)/boot.o $(OBJS_DIR)/kernel.o $(OBJS_DIR)/stdlib.o $(OBJS_DIR)/stdio.o $(OBJS_DIR)/uart.o $(OBJS_DIR)/I2C.o
	@echo Linking Objects Files: $^
	@$(CC_DIR)/$(CC) -T linker.ld -o $(OBJS_DIR)/$(ELFILE) $(LFLAGS) $^ 
	@echo Extensible Linkable Format File: $@ created 

$(TARGET) : $(OBJS_DIR)/$(ELFILE) 
	@$(CC_DIR)/$(OC) $(OBJS_DIR)/$(ELFILE) -O binary $(OBJS_DIR)/$(TARGET) 
	@echo Kernel Image File: $(TARGET) created
