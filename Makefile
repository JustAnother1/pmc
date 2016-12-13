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
include cfg/features.mk

include hal/hal.mk
include cfg/system.mk
include cfg/files.mk
include test/test_cfg.mk

# lib math used for log() in ADC to temperature calculation.
LIB += -lm 
LIB +=  -u _printf_float

# Define optimisation level here -O3 or -O0 for no optimisation
OPT += -O0 -ffunction-sections -fdata-sections 
# coverage
# -ftest-coverage -fprofile-arcs
CFLAGS += $(OPT) -g -Wall -pedantic -std=gnu99
# to get reproduceable builds: 
CFLAGS += -save-temps
ASFLAGS += -save-temps
# TODO CFLAGS += -mfloat-abi=hard
ifeq ($(COMPILER),clang)
#CFLAGS += --analyze
else
endif


LDFLAGS +=  -g -Wall -fwhole-program $(LIB)

# preparing the variables
INCDIR = $(patsubst %,-I%, $(INCDIRS))
TEST_INCDIR = $(patsubst %,-I%, $(TEST_INCDIRS))
OBJS = $(addprefix $(BIN_FOLDER),$(STARTUP:.s=.o) $(SRC:.c=.o))


#targets:
#========

help:
	@echo "  "
	@echo "BUILD TARGETS FOR PMC"
	@echo "  "
	@echo "make clean                  - delete all created files"
	@echo "make doxygen                - create documentation"
	@echo "make test                   - run unit tests"
	@echo "make list                   - readelf + objdump"
	@echo "make all BOARD=linux        - build project to run on Linux"
	@echo "make all BOARD=stm407disco  - build project to run on STM32F4 discovery board"
	@echo "make all BOARD=pipy         - build project to run on pipy board"
	@echo "make burn BOARD=stm407disco - programm the created file to STM32F4 discovery board"
	@echo "make burn BOARD=pipy        - programm the created file to pipy board"
	@echo "  "

all: directories $(OBJS) $(BIN_FOLDER)$(PROJECT).elf $(OBJS) $(BIN_FOLDER)$(PROJECT).bin
	$(TRGT)size $(BIN_FOLDER)$(PROJECT).elf

$(BIN_FOLDER)%o: %c
	@$(MKDIR_P) $(@D)
	$(CC) -c $(CFLAGS) $(DDEFS) $(OPTIONS_ARCH) $(INCDIR) $< -o $@

$(BIN_FOLDER)%o: %s
	@$(MKDIR_P) $(@D)
	$(AS) -c $(ASFLAGS) $(OPTIONS_ARCH) $< -o $@

%elf: $(OBJS)
	$(LD) $(OBJS) $(LDFLAGS) $(OPTIONS_ARCH) $(LIB) -o $@
	$(SIZE) $(BIN_FOLDER)$(PROJECT).elf

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

include test/test_target.mk

directories:
	@echo "Board = $(BOARD)"
	@echo "Architecture = $(ARCHITECTURE)"
	@echo "Board Folder = $(BOARD_FOLDER)" 
	@$(MKDIR_P) $(BIN_FOLDER)
	@$(MKDIR_P) $(TEST_BIN_FOLDER)

clean:
	-rm -rf $(TEST_BIN_FOLDER)
	-rm -rf $(BIN_FOLDER)
	
list:
	@echo " READ -> $(BIN_FOLDER)$(PROJECT).rd"
	@arm-none-eabi-readelf -Wall $(BIN_FOLDER)$(PROJECT).elf > $(BIN_FOLDER)$(PROJECT).rd
	@echo " LIST -> $(BIN_FOLDER)$(PROJECT).lst"
	@arm-none-eabi-objdump -axdDSstr $(BIN_FOLDER)$(PROJECT).elf > $(BIN_FOLDER)$(PROJECT).lst

ifeq ($(BOARD),stm407disco)
burn: 
# needs https://github.com/texane/stlink
	$(STLINK_FOLDER)/st-flash write $(BIN_FOLDER)$(PROJECT).bin 0x8000000

debug:
# needs https://github.com/texane/stlink
	$(STLINK_FOLDER)/st-util&
	$(DB) $(BIN_FOLDER)$(PROJECT).elf
endif

ifeq ($(BOARD),pipy)
burn: 
# needs https://github.com/texane/stlink
	$(STLINK_FOLDER)/st-flash write $(BIN_FOLDER)$(PROJECT).bin 0x8000000
debug:
# needs https://github.com/texane/stlink
	$(STLINK_FOLDER)/st-util&
	$(DB) $(BIN_FOLDER)$(PROJECT).elf
endif
.PHONY: all clean directories list

# end of file
