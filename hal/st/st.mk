ARCHITECTURE = arm
BOARD_FOLDER = st/stm32f4

# Target is the Nucleo-64-F446
ifeq ($(BOARD), 446nucleo)
	LINKER_SCRIPT += -T$(HAL_FOLDER)$(BOARD_FOLDER)/446nucleo/STM32F446RETx_FLASH.ld 
	STARTUP += $(HAL_FOLDER)$(BOARD_FOLDER)/446nucleo/startup_stm32f446xx.s
	INCDIRS += $(HAL_FOLDER)$(BOARD_FOLDER)/446nucleo/
endif

# Target is the Nucleo-F401RE
ifeq ($(BOARD), 401nucleo)
	LINKER_SCRIPT += -T$(HAL_FOLDER)$(BOARD_FOLDER)/401nucleo/STM32F401RETx_FLASH.ld 
	STARTUP += $(HAL_FOLDER)$(BOARD_FOLDER)/401nucleo/startup_stm32f401xc.s
	INCDIRS += $(HAL_FOLDER)$(BOARD_FOLDER)/401nucleo/
endif

# Target is the STM32F4 Discovery Board
ifeq ($(BOARD), stm407disco)
	LINKER_SCRIPT += -T$(HAL_FOLDER)$(BOARD_FOLDER)/407discovery/STM32F407VGTx_FLASH.ld 
	STARTUP += $(HAL_FOLDER)$(BOARD_FOLDER)/407discovery/startup_stm32f407xx.s
	INCDIRS += $(HAL_FOLDER)$(BOARD_FOLDER)/407discovery/
endif

# Target is the pipy Board
ifeq ($(BOARD), pipy)
	LINKER_SCRIPT += -T$(HAL_FOLDER)$(BOARD_FOLDER)/pipy/STM32F407VGTx_FLASH.ld 
	STARTUP += $(HAL_FOLDER)$(BOARD_FOLDER)/pipy/startup_stm32f405xx.s
	INCDIRS += $(HAL_FOLDER)$(BOARD_FOLDER)/pipy/
endif

INCDIRS += $(HAL_FOLDER)arm/
SRC += $(HAL_FOLDER)$(BOARD_FOLDER)/st_util.c
OPTIONS_ARCH += -mthumb 
OPTIONS_ARCH += -mcpu=cortex-m4 -mfpu=vfpv4
# -g: produce debugging information 
ASFLAGS += -g 
# enable warnings
ASFLAGS += -Wall
# uses DWARF format version 2 (TODO: Why not 4? and then also change -g to -g3)
ASFLAGS += -gdwarf-2
ASFLAGS +=  -Wa,-amhls=$(addprefix $(BIN_FOLDER),$(<:.s=.lst))
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
# CFLAGS += -Wconversion cmsis code creates warnings :-(
CFLAGS +=  -Wmissing-field-initializers
# -ffreestanding: stdlib not available, start not at main.
CFLAGS += -ffreestanding
CFLAGS += -nostdlib
CFLAGS += -gdwarf-2
CFLAGS += -Wstrict-prototypes
CFLAGS += -fverbose-asm 
CFLAGS += -mfloat-abi=softfp
# TODO CFLAGS += -mfloat-abi=hard
ifeq ($(COMPILER),clang)
	#clang (LLVM)
	CFLAGS += --target=arm-none-eabi
	CFLAGS += -fshort-enums 
else
	# gcc
	OPTIONS_ARCH += -march=armv7e-m
	CFLAGS += -fsingle-precision-constant
	CFLAGS += -Wa,-ahlms=$(addprefix $(BIN_FOLDER),$(<:.c=.lst))
	LDFLAGS += -fsingle-precision-constant
endif
LDFLAGS += -flto
LDFLAGS += --specs=nosys.specs
LDFLAGS += -gdwarf-2
LDFLAGS +=  $(LINKER_SCRIPT)
LDFLAGS += -fwhole-program
LDFLAGS += -Wl,-Map=$(BIN_FOLDER)$(PROJECT).map,--gc-sections,--cref
ifeq ($(USE_USB), yes)
	SRC += $(HAL_FOLDER)$(BOARD_FOLDER)/st_usb_device.c
	SRC += $(HAL_FOLDER)$(BOARD_FOLDER)/st_usb_descriptor.c
endif
CFLAGS += -nostdinc
LDFLAGS += -nostdinc
CFLAGS += -fdiagnostics-color
