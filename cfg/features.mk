# components used 
#  enable a component: component=yes
# disable a component: component=no
USE_UART = yes
USE_USB = no
USE_STEP_DIR = yes
# ACTIVATE_DEBUG enables / disables the debug console and debug_msg()
# if it is not defined then debug is active!
ifndef ACTIVATE_DEBUG
	ACTIVATE_DEBUG = yes
endif


# List all C defines here
DDEFS += -DPROTOCOL_VERSION_MAJOR=0
DDEFS += -DPROTOCOL_VERSION_MINOR=1
DDEFS += -DFIRMWARE_REVISION_MAJOR=0
DDEFS += -DFIRMWARE_REVISION_MINOR=1
DDEFS += -DFIRMWARE_TYPE=0
DDEFS += -DHARDWARE_TYPE=0
DDEFS += -DMAX_STEP_RATE=40000
