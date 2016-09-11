# system configuration
ifeq ($(ARCHITECTURE), arm)
	CCPREFIX = arm-none-eabi-
else
	CCPREFIX = 
endif 

CC = $(CCPREFIX)gcc
CP = $(CCPREFIX)objcopy
AS = $(CCPREFIX)gcc -x assembler-with-cpp
DB = $(CCPREFIX)gdb
HEX = $(CP) -O ihex
BIN = $(CP) -O binary -S
DIS = $(CP) -S
VERILOG_HEX = $(CP) -O verilog
MKDIR_P = mkdir -p
DOXYGEN = doxygen
STLINK_FOLDER=../stlink/build
BIN_FOLDER = bin/
SRC_FOLDER = src/
HAL_FOLDER = hal/
TEST_BIN_FOLDER = test_bin/