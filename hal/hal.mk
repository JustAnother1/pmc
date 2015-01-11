# Hardware used:

# Target is the STM32F4 Discovery Board
ifeq ($(BOARD), stm407disco)
	ARCHITECTURE = arm
	BOARD_FOLDER = st/stm32f4
	ASFLAGS += -g -Wall -gdwarf-2 -Wa,-amhls=$(addprefix $(BIN_FOLDER),$(<:.s=.lst))
	CFLAGS +=  -ffunction-sections -fdata-sections -ffreestanding --specs=nano.specs -fsingle-precision-constant -gdwarf-2 -fomit-frame-pointer -Wstrict-prototypes -fverbose-asm -Wa,-ahlms=$(addprefix $(BIN_FOLDER),$(<:.c=.lst))
	LDFLAGS += -flto -fsingle-precision-constant --specs=nano.specs -lgcc -lc -lnosys -gdwarf-2 $(LINKER_SCRIPT) -Wl,-Map=$(BIN_FOLDER)$(PROJECT).map,--gc-sections,--cref
	INCDIRS +=$(HAL_FOLDER)arm/	
	OPTIONS_ARCH += -mthumb -mcpu=cortex-m4 -march=armv7e-m
	LINKER_SCRIPT = -T$(HAL_FOLDER)$(BOARD_FOLDER)/mem.ld -T$(HAL_FOLDER)$(BOARD_FOLDER)/sections.ld
	STARTUP += $(HAL_FOLDER)$(BOARD_FOLDER)/startup_stm32f407xx.s
endif

# Target is Linux 
ifeq ($(BOARD), linux)
	ARCHITECTURE = x86
	BOARD_FOLDER = linux
	CFLAGS += -fhosted
	LDFLAGS += -Wl,-Map=$(BIN_FOLDER)$(PROJECT).map,--gc-sections
	LIB += -lpthread
endif
