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

# Define project name
PROJECT = pmc
# Hardware used
BOARD = linux
#components used
USE_UART = yes
USE_USB = no

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
CFLAGS = $(OPT) -g -Wall  -fhosted -pedantic -std=gnu99
LDFLAGS = -g -Wall -fwhole-program -Wl,-Map=$(BIN_FOLDER)$(PROJECT).map,--gc-sections

# system configuration
ifeq ($(BOARD), linux)
	CCPREFIX = 
endif
CC = $(CCPREFIX)gcc
CP = $(CCPREFIX)objcopy
AS = $(CCPREFIX)gcc -x assembler-with-cpp
HEX = $(CP) -O ihex
BIN = $(CP) -O binary -S
DIS = $(CP) -S
VERILOG_HEX = $(CP) -O verilog
MKDIR_P = mkdir -p
DOXYGEN = doxygen
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

SRC += $(SRC_FOLDER)main.c
SRC += $(SRC_FOLDER)error.c
SRC += $(SRC_FOLDER)events.c
SRC += $(SRC_FOLDER)fw_cfg.c
SRC += $(SRC_FOLDER)led.c
SRC += $(SRC_FOLDER)time_base.c
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

ifeq ($(USE_UART), yes)
	SRC += $(SRC_FOLDER)com/uart.c
	SRC += $(SRC_FOLDER)hal_$(BOARD)/hal_uart.c
	DDEFS += -DHAS_UART
endif
ifeq ($(USE_USB), yes)
	SRC += $(SRC_FOLDER)com/usb.c
	DDEFS += -DHAS_USB
endif

INCDIR = $(patsubst %,-I%, $(INCDIRS))
OBJS = $(addprefix $(BIN_FOLDER),$(SRC:.c=.o))

#targets:

all: directories $(OBJS) $(BIN_FOLDER)$(PROJECT).elf
	$(TRGT)size $(BIN_FOLDER)$(PROJECT).elf

$(BIN_FOLDER)%o: %c
	@$(MKDIR_P) $(@D)
	$(CC) -c $(CFLAGS) $(DDEFS) $(INCDIR) $< -o $@

%elf: $(OBJS)
	$(CC) $(OBJS) $(LDFLAGS) $(LIB) -o $@

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

.PHONY: all clean directories
