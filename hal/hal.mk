# Hardware used:

# Target is the STM32F4 Discovery Board
ifeq ($(BOARD), stm407disco)
	ARCHITECTURE = arm
	BOARD_FOLDER = st/stm32f4
	LINKER_SCRIPT += -T$(HAL_FOLDER)$(BOARD_FOLDER)/mem.ld 
	LINKER_SCRIPT += -T$(HAL_FOLDER)$(BOARD_FOLDER)/sections.ld
	STARTUP += $(HAL_FOLDER)$(BOARD_FOLDER)/407discovery/startup_stm32f407xx.s
	INCDIRS +=$(HAL_FOLDER)arm/
	INCDIRS +=$(HAL_FOLDER)$(BOARD_FOLDER)/407discovery/
	SRC += $(HAL_FOLDER)$(BOARD_FOLDER)/st_util.c
	OPTIONS_ARCH += -mthumb -mcpu=cortex-m4 -march=armv7e-m
	ASFLAGS += -g -Wall -gdwarf-2 -Wa,-amhls=$(addprefix $(BIN_FOLDER),$(<:.s=.lst))
	CFLAGS += -ffunction-sections -fdata-sections -ffreestanding --specs=nano.specs 
	CFLAGS += -fsingle-precision-constant -gdwarf-2 -fomit-frame-pointer -Wstrict-prototypes
	CFLAGS += -fverbose-asm -Wa,-ahlms=$(addprefix $(BIN_FOLDER),$(<:.c=.lst))
	LDFLAGS += -flto -fsingle-precision-constant --specs=nano.specs -lgcc -lc -lnosys -gdwarf-2 $(LINKER_SCRIPT)
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
	INCDIRS +=$(HAL_FOLDER)arm/
	INCDIRS +=$(HAL_FOLDER)$(BOARD_FOLDER)/pipy/
	SRC += $(HAL_FOLDER)$(BOARD_FOLDER)/st_util.c
	OPTIONS_ARCH += -mthumb -mcpu=cortex-m4 -march=armv7e-m
	ASFLAGS += -g -Wall -gdwarf-2 -Wa,-amhls=$(addprefix $(BIN_FOLDER),$(<:.s=.lst))
	CFLAGS += -ffunction-sections -fdata-sections -ffreestanding --specs=nano.specs 
	CFLAGS += -fsingle-precision-constant -gdwarf-2 -fomit-frame-pointer -Wstrict-prototypes
	CFLAGS += -fverbose-asm -Wa,-ahlms=$(addprefix $(BIN_FOLDER),$(<:.c=.lst))
	LDFLAGS += -flto -fsingle-precision-constant --specs=nano.specs -lgcc -lc -lnosys -gdwarf-2 $(LINKER_SCRIPT)
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
	CFLAGS += -fhosted
	LDFLAGS += -Wl,-Map=$(BIN_FOLDER)$(PROJECT).map,--gc-sections
	LIB += -lpthread
endif
