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

include hal/hal.mk
include cfg/features.mk
include cfg/system.mk
include cfg/files.mk

# Define optimisation level here
OPT += -O3 -ffunction-sections -fdata-sections
CFLAGS += $(OPT) -g -Wall -pedantic -std=gnu99
LDFLAGS +=  -g -Wall -fwhole-program $(LIB)


# preparing the variables
INCDIR = $(patsubst %,-I%, $(INCDIRS))
OBJS = $(addprefix $(BIN_FOLDER),$(STARTUP:.s=.o) $(SRC:.c=.o))


#targets:
#========

help:
	@echo "  "
	@echo "BUILD TARGETS FOR PMC"
	@echo "  "
	@echo "make clean                  - delete all created files"
	@echo "make doxygen                - create Documentation"
	@echo "make all BOARD=linux        - build project to run on Linux"
	@echo "make all BOARD=stm407disco  - build project to run on STM32F4 Discovery Board"
	@echo "make burn BOARD=stm407disco - programm the created file to STM32F4 Discovery Board"
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
	@echo "Board = $(BOARD)"
	@echo "Architecture = $(ARCHITECTURE)"
	@echo "Board Folder = $(BOARD_FOLDER)" 
	@$(MKDIR_P) $(BIN_FOLDER)

clean:
	-rm -rf $(BIN_FOLDER)
ifeq ($(BOARD),stm407disco)
burn: 
	$(STLINK_FOLDER)/st-flash write $(BIN_FOLDER)$(PROJECT).bin 0x8000000

debug:
	$(STLINK_FOLDER)/st-util&
	$(DB) $(BIN_FOLDER)$(PROJECT).elf
endif

.PHONY: all clean directories

# end of file
