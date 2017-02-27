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

# Define optimisation level here -O3 for best or -O0 for no optimisation
OPT += -O3
OPT += -ffunction-sections
OPT += -fdata-sections 
# coverage
ifeq ($(JENKINS),yes)
OPT += -ftest-coverage
OPT += -fprofile-arcs
CPPFLAGS += -ftest-coverage
CPPFLAGS += -fprofile-arcs
LDFLAGS += -ftest-coverage
LDFLAGS += -fprofile-arcs
LIB += -lgcov
endif
CFLAGS += $(OPT)
# -g: produce debugging information 
CFLAGS += -g
#enable warnings
CFLAGS += -Wall
#do all checks according to C standard.
CFLAGS += -pedantic
# Use C99 with GNU extensions
CFLAGS += -std=gnu99
# print warnings in color
CFLAGS += -fdiagnostics-color
CPPFLAGS += -g -Wall -pedantic -fdiagnostics-color
# to get reproduceable builds: 
CFLAGS += -save-temps=obj
ASFLAGS += -save-temps=obj
ifeq ($(COMPILER),clang)
#CFLAGS += --analyze
else
endif

CFLAGS += -nostdinc
LDFLAGS += -nostdinc

LDFLAGS += -g
LDFLAGS += -Wall

# preparing the variables
INCDIR = $(patsubst %,-I%, $(INCDIRS))
OBJS = $(addprefix $(BIN_FOLDER),$(STARTUP:.s=.o) $(SRC:.c=.o))

#targets:
#========

help:
	@echo "  "
	@echo "BUILD TARGETS FOR PMC"
	@echo "  "
	@echo "make clean                         - delete all created files"
	@echo "make doxygen                       - create documentation"
	@echo "make cmocka_test                   - run cmocka unit tests"
	@echo "make cpputest_test                 - run CppUTest unit tests"
	@echo "make list                          - readelf + objdump"
	@echo "make all BOARD=linux               - build project to run on Linux"
	@echo "make all BOARD=stm407disco         - build project to run on STM32F4 discovery board"
	@echo "make all BOARD=pipy                - build project to run on pipy board"
	@echo "make all BOARD=rumba               - build project to run on rumba board"
	@echo "make all BOARD=ultimaker_original  - build project to run on Ultimaker Original"
	@echo "make burn BOARD=stm407disco        - programm the created file to STM32F4 discovery board"
	@echo "make burn BOARD=pipy               - programm the created file to pipy board"
	@echo "make burn BOARD=rumba              - programm the created file to rumba board"
	@echo "make burn BOARD=ultimaker_original - programm the created file to Ultimaker Original"
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

ifeq ($(BOARD),$(filter $(BOARD),stm407disco pipy))
burn: 
# needs https://github.com/texane/stlink
	$(STLINK_FOLDER)/st-flash write $(BIN_FOLDER)$(PROJECT).bin 0x8000000

debug:
# needs https://github.com/texane/stlink
	$(STLINK_FOLDER)/st-util&
	$(DB) $(BIN_FOLDER)$(PROJECT).elf
	
else ifeq ($(BOARD),$(filter $(BOARD),rumba ultimaker_original))
burn: 
# needs avrdude
	avrdude -p$(MCU) -c$(PROGRAMMER) -P/dev/ttyACM0 -D -U flash:w:$(BIN_FOLDER)$(PROJECT).elf -v -v
endif

.PHONY: all clean directories list

# end of file
