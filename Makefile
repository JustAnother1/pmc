# This program is free software; you can redistribute it and/or
# modify it under the terms of the GNU General Public License version 2
# as published by the Free Software Foundation.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License along
# with this program; if not, see <http://www.gnu.org/licenses/>

################################################################################
# User Configurations - start                                                 #
################################################################################

# Define project name
PROJECT = pmc
# Hardware used
# supported are : 
# "linux" - compiles with gcc to run on a Linux System. Most hrdware functions just do nothing. 
# "stm407disco" - ST Development Board STM32F4 Discovery with a STM32F407 processor.
BOARD = linux
#BOARD = stm407disco
# components used 
#  enable a component: component=yes
# disable a component: component=no
USE_UART = yes
USE_USB = no
USE_SPI = yes
USE_STEP_DIR = yes

# List all C defines here

DDEFS += -DPROTOCOL_VERSION_MAJOR=0
DDEFS += -DPROTOCOL_VERSION_MINOR=1
DDEFS += -DFIRMWARE_REVISION_MAJOR=0
DDEFS += -DFIRMWARE_REVISION_MINOR=1
DDEFS += -DFIRMWARE_TYPE=0
DDEFS += -DHARDWARE_TYPE=0
DDEFS += -DMAX_STEP_RATE=40000

# List all user libraries here
LIB +=
ifeq ($(BOARD), linux)
	LIB += -lpthread
endif

# Define optimisation level here
OPT = -O3 -ffunction-sections -fdata-sections
CFLAGS = $(OPT) -g -Wall -pedantic -std=gnu99
LDFLAGS =  -g -Wall -fwhole-program $(LIB)

ifeq ($(BOARD), stm407disco)
	ASFLAGS = -g -Wall -gdwarf-2 -Wa,-amhls=$(addprefix $(BIN_FOLDER),$(<:.s=.lst))
	CFLAGS +=  -ffunction-sections -fdata-sections -ffreestanding --specs=nano.specs -fsingle-precision-constant -gdwarf-2 -fomit-frame-pointer -Wstrict-prototypes -fverbose-asm -Wa,-ahlms=$(addprefix $(BIN_FOLDER),$(<:.c=.lst))
	LDFLAGS += -flto -fsingle-precision-constant --specs=nano.specs -lgcc -lc -lnosys -gdwarf-2 $(LINKER_SCRIPT) -Wl,-Map=$(BIN_FOLDER)$(PROJECT).map,--gc-sections,--cref
endif
ifeq ($(BOARD), linux)
	CFLAGS += -fhosted
	LDFLAGS += -Wl,-Map=$(BIN_FOLDER)$(PROJECT).map,--gc-sections
endif

# system configuration
ifeq ($(BOARD), linux)
	CCPREFIX = 
endif
ifeq ($(BOARD), stm407disco)
	CCPREFIX = arm-none-eabi-
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
STLINK_FOLDER=../stlink
BIN_FOLDER = bin/
SRC_FOLDER = src/

################################################################################
# end of configuration
################################################################################

# List all directories here
INCDIRS +=$(SRC_FOLDER)
INCDIRS +=$(SRC_FOLDER)com/
INCDIRS +=$(SRC_FOLDER)device/
INCDIRS +=$(SRC_FOLDER)hal_include/
INCDIRS +=$(SRC_FOLDER)order/

# general source files - always compiled
SRC += $(SRC_FOLDER)main.c
SRC += $(SRC_FOLDER)error.c
SRC += $(SRC_FOLDER)events.c
SRC += $(SRC_FOLDER)fw_cfg.c
SRC += $(SRC_FOLDER)led.c
SRC += $(SRC_FOLDER)com/com.c
SRC += $(SRC_FOLDER)device/device_buzzer.c
SRC += $(SRC_FOLDER)device/device_heater.c
SRC += $(SRC_FOLDER)device/device_input.c
SRC += $(SRC_FOLDER)device/device_output.c
SRC += $(SRC_FOLDER)device/device_pwm.c
SRC += $(SRC_FOLDER)device/device_stepper.c
SRC += $(SRC_FOLDER)device/device_temperature_sensor.c
SRC += $(SRC_FOLDER)device/step.c
SRC += $(SRC_FOLDER)hal_$(BOARD)/hal_cpu.c
SRC += $(SRC_FOLDER)hal_$(BOARD)/hal_led.c
SRC += $(SRC_FOLDER)hal_$(BOARD)/hal_time.c
SRC += $(SRC_FOLDER)hal_$(BOARD)/hal_watchdog.c
SRC += $(SRC_FOLDER)order/command_queue.c
SRC += $(SRC_FOLDER)order/orderhandler.c

# source files for specific features - only compiled if feature is activated
ifeq ($(USE_UART), yes)
	SRC += $(SRC_FOLDER)com/uart.c
	SRC += $(SRC_FOLDER)hal_$(BOARD)/hal_uart.c
	DDEFS += -DHAS_UART
endif
ifeq ($(USE_USB), yes)
	SRC += $(SRC_FOLDER)com/usb.c
	DDEFS += -DHAS_USB
endif

ifeq ($(USE_SPI), yes)
	DDEFS += -DHAS_SPI
	SRC += $(SRC_FOLDER)hal_$(BOARD)/hal_spi.c
endif

ifeq ($(USE_STEP_DIR), yes)
	DDEFS += -DUSE_STEP_DIR
endif

# source files for specific boards - only compiled if we build for that board
ifeq ($(BOARD),stm407disco)
	INCDIRS +=$(SRC_FOLDER)hal_$(BOARD)/
	OPTIONS_ARCH += -mthumb -mcpu=cortex-m4 -march=armv7e-m
	LINKER_SCRIPT = -T$(SRC_FOLDER)hal_$(BOARD)/mem.ld -T$(SRC_FOLDER)hal_$(BOARD)/sections.ld
	STARTUP += $(SRC_FOLDER)hal_$(BOARD)/startup_stm32f407xx.s
endif

# preparing the variables
INCDIR = $(patsubst %,-I%, $(INCDIRS))
OBJS = $(addprefix $(BIN_FOLDER),$(STARTUP:.s=.o) $(SRC:.c=.o))

#targets:

all: directories $(OBJS) $(BIN_FOLDER)$(PROJECT).elf $(OBJS) $(BIN_FOLDER)$(PROJECT).bin
	$(TRGT)size $(BIN_FOLDER)$(PROJECT).elf

$(BIN_FOLDER)%o: %c
	@$(MKDIR_P) $(@D)
	$(CC) -c $(CFLAGS) $(DDEFS) $(OPTIONS_ARCH) $(INCDIR) $< -o $@

$(BIN_FOLDER)%o: %s
	@$(MKDIR_P) $(@D)
	$(AS) -c $(ASFLAGS) $(OPTIONS_ARCH) $< -o $@

%elf: $(OBJS)
	$(CC) $(OBJS) $(LDFLAGS) $(OPTIONS_ARCH) $(LIB) -o $@

%hex: %elf
	$(HEX) $< $@

%bin: %elf
	$(BIN) $< $@

%txt: %elf
	$(DIS) $< $@ > $@

%vhx: %elf
	$(VERILOG_HEX) $< $@

doxygen:
	-rm -rf doc/doxygen
	$(DOXYGEN) Doxyfile

directories:
	@$(MKDIR_P) $(BIN_FOLDER)

clean:
	-rm -rf $(BIN_FOLDER)

burn: 
	$(STLINK_FOLDER)/st-flash write $(BIN_FOLDER)/$(PROJECT).bin 0x8000000

debug:
	$(STLINK_FOLDER)/st-util&
	$(DB) $(BIN_FOLDER)$(PROJECT).elf

.PHONY: all clean directories

# end of file
