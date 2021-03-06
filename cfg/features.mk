# components used 
#  enable a component: component=yes
# disable a component: component=no
USE_UART = yes
USE_USB = no
USE_STEP_DIR = yes
ifeq ($(BOARD),$(filter $(BOARD),401nucleo 446nucleo))
	USE_I2C = no
else
	USE_I2C = yes
endif
# ACTIVATE_DEBUG enables / disables the debug console and debug_msg()
# if it is not defined then debug is active!
ifndef ACTIVATE_DEBUG
	ACTIVATE_DEBUG = yes
endif
# Watchdog
USE_WATCHDOG = no


#Watchdog
ifeq ($(USE_WATCHDOG), yes)
	DDEFS += -DWATCHDOG_ACTIVE
endif

# printf
DDEFS += -DPRINTF_LONG_SUPPORT
DDEFS += -DPRINTF_FLOAT_SUPPORT

# List all C defines here
DDEFS += -DPROTOCOL_VERSION_MAJOR=0
DDEFS += -DPROTOCOL_VERSION_MINOR=1
DDEFS += -DFIRMWARE_REVISION_MAJOR=0
DDEFS += -DFIRMWARE_REVISION_MINOR=1
DDEFS += -DFIRMWARE_TYPE=0

DDEFS += -DMAX_STEP_RATE=40000

ifeq ($(BOARD),$(filter $(BOARD),rumba ultimaker_original))
# AVR
DDEFS += -DHARDWARE_TYPE=1
DDEFS += -DSTEP_TIMER_FREQ_16MHZ
DDEFS += -DPOLL_STEP_BUFFER_REFILL
USE_TRINAMIC = no
else
# default Pacemeaker
DDEFS += -DHARDWARE_TYPE=0
DDEFS += -DSTEP_TIMER_FREQ_12MHZ
ifeq ($(BOARD),$(filter $(BOARD),401nucleo 446nucleo))
	USE_TRINAMIC = no
else
    USE_TRINAMIC = yes
endif
endif
