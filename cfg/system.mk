# system configuration
ifeq ($(ARCHITECTURE), arm)
	CCPREFIX = arm-none-eabi-
else ifeq ($(ARCHITECTURE), avr)
	CCPREFIX = avr-
else 
	CCPREFIX = 
endif 

ifeq ($(COMPILER),clang)
	CC = clang
	CPP = clang
	LD = $(CCPREFIX)ld
	SIZE = llvm-size
	# INCDIRS += 
else
	CC = $(CCPREFIX)gcc
	CPP =$(CCPREFIX)g++
	LD = $(CCPREFIX)gcc
	SIZE = size
endif

AS = $(CCPREFIX)gcc -x assembler-with-cpp
CP = $(CCPREFIX)objcopy
DB = $(CCPREFIX)gdb
OBJDUMP = $(CCPREFIX)objdump
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