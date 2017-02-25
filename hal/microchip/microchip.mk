# Target is the rumba Board
ifeq ($(BOARD), rumba)
	ARCHITECTURE = avr
	BOARD_FOLDER = microchip/avr
	INCDIRS += $(HAL_FOLDER)$(BOARD_FOLDER)/rumba/
	INCDIRS += /usr/lib/avr/include/
	INCDIRS += /usr/lib/gcc/avr/4.9.2/include/
	MCU = atmega2560
	PROGRAMMER = wiring
	OPTIONS_ARCH += -mmcu=atmega2560
	# -g: produce debugging information 
	CFLAGS += -g 
	# enable warnings
	CFLAGS += -Wall
	# uses DWARF format version 2 (TODO: Why not 4? and then also change -g to -g3)
	CFLAGS += -ffunction-sections
	CFLAGS += -fdata-sections
	# warn if a float is implicitly promoted to double
	CFLAGS += -Wdouble-promotion
	# warn if more then this amount of bytes on the stack are used
	CFLAGS += -Wstack-usage=256
	# -fstack-usage: crete *.su files with the funtion based information about the stack usage 
	CFLAGS += -fstack-usage
	CFLAGS += -Wunsafe-loop-optimizations
	CFLAGS += -Wjump-misses-init
	#CFLAGS += -Wconversion
	CFLAGS +=  -Wmissing-field-initializers
	CFLAGS += -gdwarf-2
	CFLAGS += -Wstrict-prototypes
	CFLAGS += -fverbose-asm 
	CFLAGS += -fsingle-precision-constant
	CFLAGS += -Wa,-ahlms=$(addprefix $(BIN_FOLDER),$(<:.c=.lst))
	LDFLAGS += -fsingle-precision-constant
	LDFLAGS += -flto
	LDFLAGS += -gdwarf-2
	LDFLAGS += -fwhole-program
	LDFLAGS += -Wl,-Map=$(BIN_FOLDER)$(PROJECT).map,--gc-sections,--cref
endif