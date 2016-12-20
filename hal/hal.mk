# Hardware used:

# Target is the STM32F4 Discovery Board
ifeq ($(BOARD), stm407disco)
	ARCHITECTURE = arm
	BOARD_FOLDER = st/stm32f4
	LINKER_SCRIPT += -T$(HAL_FOLDER)$(BOARD_FOLDER)/mem.ld 
	LINKER_SCRIPT += -T$(HAL_FOLDER)$(BOARD_FOLDER)/sections.ld
	STARTUP += $(HAL_FOLDER)$(BOARD_FOLDER)/407discovery/startup_stm32f407xx.s
	INCDIRS += $(HAL_FOLDER)arm/
	INCDIRS += $(HAL_FOLDER)$(BOARD_FOLDER)/407discovery/
	SRC += $(HAL_FOLDER)$(BOARD_FOLDER)/st_util.c
	OPTIONS_ARCH += -mthumb -mcpu=cortex-m4
	ASFLAGS += -g -Wall -gdwarf-2 -Wa,-amhls=$(addprefix $(BIN_FOLDER),$(<:.s=.lst))
	CFLAGS += -ffunction-sections -fdata-sections -ffreestanding --specs=nano.specs 
	CFLAGS += -gdwarf-2 -fomit-frame-pointer -Wstrict-prototypes
	CFLAGS += -fverbose-asm
	ifeq ($(COMPILER),clang)
		#clang (LLVM)
		CFLAGS += --target=arm-none-eabi -fshort-enums 
	else
		# gcc
		OPTIONS_ARCH += -march=armv7e-m
		CFLAGS += -fsingle-precision-constant
		CFLAGS += -Wa,-ahlms=$(addprefix $(BIN_FOLDER),$(<:.c=.lst))
		LDFLAGS += -fsingle-precision-constant
	endif
	LDFLAGS += -flto 
	LDFLAGS += --specs=nano.specs -lgcc -lc -lnosys -gdwarf-2 $(LINKER_SCRIPT)
	LDFLAGS += -Wl,-Map=$(BIN_FOLDER)$(PROJECT).map,--gc-sections,--cref
	ifeq ($(USE_USB), yes)
		SRC += $(HAL_FOLDER)$(BOARD_FOLDER)/st_usb_device.c
		SRC += $(HAL_FOLDER)$(BOARD_FOLDER)/st_usb_descriptor.c
	endif
endif

# Target is the pipy Board
ifeq ($(BOARD), pipy)
	ARCHITECTURE = arm
	BOARD_FOLDER = st/stm32f4
	LINKER_SCRIPT += -T$(HAL_FOLDER)$(BOARD_FOLDER)/mem.ld 
	LINKER_SCRIPT += -T$(HAL_FOLDER)$(BOARD_FOLDER)/sections.ld
	STARTUP += $(HAL_FOLDER)$(BOARD_FOLDER)/pipy/startup_stm32f405xx.s
	INCDIRS += $(HAL_FOLDER)arm/
	INCDIRS += $(HAL_FOLDER)$(BOARD_FOLDER)/pipy/
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
endif

# Target is Linux 
ifeq ($(BOARD), linux)
	ARCHITECTURE = x86
	BOARD_FOLDER = linux
	INCDIRS +=$(HAL_FOLDER)$(BOARD_FOLDER)/
	INCDIRS += /usr/lib/gcc/x86_64-linux-gnu/5/include/
	INCDIRS += /usr/include
	#INCDIRS += /usr/include/c++/5/tr1/
	#INCDIRS += /usr/include/c++/5/
	#INCDIRS += $(HAL_FOLDER)arm/
	# -fhosted: stdlib is available. main returns int
	CFLAGS += -fhosted
	#lgcc for memset
	LDFLAGS += -lgcc
	LDFLAGS += -lc
	LDFLAGS += -Wl,-Map=$(BIN_FOLDER)$(PROJECT).map,--gc-sections
	LIB += -lpthread
endif
