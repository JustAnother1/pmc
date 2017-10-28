# Hardware used:
ifeq ($(BOARD),$(filter $(BOARD),stm407disco pipy 401nucleo 446nucleo))
include hal/st/st.mk
endif

ifeq ($(BOARD),$(filter $(BOARD),rumba ultimaker_original))
include hal/microchip/microchip.mk
endif

# Target is Linux 
ifeq ($(BOARD), linux)
	ARCHITECTURE = x86
	BOARD_FOLDER = linux
	INCDIRS +=$(HAL_FOLDER)$(BOARD_FOLDER)/
	INCDIRS += /usr/lib/gcc/x86_64-linux-gnu/5/include/
	INCDIRS += /usr/include
	# -fhosted: stdlib is available. main returns int
	CFLAGS += -fhosted
	#lgcc for memset
	LDFLAGS += -lgcc
	LDFLAGS += -lc
	LDFLAGS += -Wl,-Map=$(BIN_FOLDER)$(PROJECT).map,--gc-sections
	LIB += -lpthread
endif
