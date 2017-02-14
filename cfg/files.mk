
# List all directories here
INCDIRS +=$(SRC_FOLDER)
INCDIRS +=$(SRC_FOLDER)com/
INCDIRS +=$(SRC_FOLDER)debug/
INCDIRS +=$(SRC_FOLDER)device/
INCDIRS +=$(SRC_FOLDER)lib/
INCDIRS +=$(SRC_FOLDER)order/
INCDIRS +=$(SRC_FOLDER)stepper/
INCDIRS +=$(HAL_FOLDER)include/
INCDIRS +=$(HAL_FOLDER)
ifdef BOARD_FOLDER
	INCDIRS +=$(HAL_FOLDER)$(BOARD_FOLDER)/
endif

# general source files - always compiled
SRC += $(SRC_FOLDER)main.c
SRC += $(SRC_FOLDER)error.c
SRC += $(SRC_FOLDER)events.c
SRC += $(SRC_FOLDER)fw_cfg.c
SRC += $(SRC_FOLDER)com/com.c
SRC += $(SRC_FOLDER)device/device_buzzer.c
SRC += $(SRC_FOLDER)device/device_heater.c
SRC += $(SRC_FOLDER)device/device_input.c
SRC += $(SRC_FOLDER)device/device_output.c
SRC += $(SRC_FOLDER)device/device_pwm.c
SRC += $(SRC_FOLDER)device/device_temperature_sensor.c
SRC += $(SRC_FOLDER)lib/printf.c
SRC += $(SRC_FOLDER)lib/atoi.c
SRC += $(SRC_FOLDER)lib/isalpha.c
SRC += $(SRC_FOLDER)lib/log.c
SRC += $(SRC_FOLDER)lib/copy_string.c
SRC += $(SRC_FOLDER)stepper/device_stepper.c
SRC += $(SRC_FOLDER)stepper/step.c
SRC += $(SRC_FOLDER)stepper/trinamic.c
SRC += $(SRC_FOLDER)order/command_queue.c
SRC += $(SRC_FOLDER)order/orderhandler.c
SRC += $(SRC_FOLDER)endStopHandling.c
# Hardware abstraction layer - used peripherals
SRC += $(HAL_FOLDER)$(BOARD_FOLDER)/hal_adc.c
SRC += $(HAL_FOLDER)$(BOARD_FOLDER)/hal_buzzer.c
SRC += $(HAL_FOLDER)$(BOARD_FOLDER)/hal_cpu.c
SRC += $(HAL_FOLDER)$(BOARD_FOLDER)/hal_din.c
SRC += $(HAL_FOLDER)$(BOARD_FOLDER)/hal_dout.c
SRC += $(HAL_FOLDER)$(BOARD_FOLDER)/hal_i2c.c
SRC += $(HAL_FOLDER)$(BOARD_FOLDER)/hal_led.c
SRC += $(HAL_FOLDER)$(BOARD_FOLDER)/hal_nvram.c
SRC += $(HAL_FOLDER)$(BOARD_FOLDER)/hal_power.c
SRC += $(HAL_FOLDER)$(BOARD_FOLDER)/hal_pwm.c
SRC += $(HAL_FOLDER)$(BOARD_FOLDER)/hal_spi.c
SRC += $(HAL_FOLDER)$(BOARD_FOLDER)/hal_time.c
SRC += $(HAL_FOLDER)$(BOARD_FOLDER)/hal_watchdog.c


# source files for specific features - only compiled if feature is activated
ifeq ($(USE_UART), yes)
	SRC += $(SRC_FOLDER)com/uart.c
	SRC += $(HAL_FOLDER)$(BOARD_FOLDER)/hal_uart.c
	DDEFS += -DHAS_UART
else
# TODO this can probably be done in a better way,...
	ifeq ($(ACTIVATE_DEBUG), yes)
		SRC += $(SRC_FOLDER)com/uart.c
		SRC += $(HAL_FOLDER)$(BOARD_FOLDER)/hal_uart.c
	endif
endif

ifeq ($(USE_USB), yes)
	SRC += $(SRC_FOLDER)com/usb.c
	SRC += $(HAL_FOLDER)$(BOARD_FOLDER)/hal_usb_device_cdc.c
	DDEFS += -DHAS_USB
endif

ifeq ($(USE_STEP_DIR), yes)
	DDEFS += -DUSE_STEP_DIR
	SRC +=  $(HAL_FOLDER)$(BOARD_FOLDER)/hal_stepper_port.c
endif

ifeq ($(ACTIVATE_DEBUG), yes)
	SRC += $(SRC_FOLDER)debug/debug.c
	SRC += $(HAL_FOLDER)$(BOARD_FOLDER)/hal_debug.c
	DDEFS += -DDEBUG_ACTIVE
endif

