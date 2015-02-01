
# List all directories here
INCDIRS +=$(SRC_FOLDER)
INCDIRS +=$(SRC_FOLDER)com/
INCDIRS +=$(SRC_FOLDER)device/
INCDIRS +=$(SRC_FOLDER)order/
INCDIRS +=$(SRC_FOLDER)debug/
INCDIRS +=$(HAL_FOLDER)include/
INCDIRS +=$(HAL_FOLDER)$(BOARD_FOLDER)/

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
SRC += $(SRC_FOLDER)device/device_stepper.c
SRC += $(SRC_FOLDER)device/device_temperature_sensor.c
SRC += $(SRC_FOLDER)device/step.c
SRC += $(SRC_FOLDER)order/command_queue.c
SRC += $(SRC_FOLDER)order/orderhandler.c
# Hardware abstraction layer - used peripherals
SRC += $(HAL_FOLDER)$(BOARD_FOLDER)/hal_adc.c
SRC += $(HAL_FOLDER)$(BOARD_FOLDER)/hal_buzzer.c
SRC += $(HAL_FOLDER)$(BOARD_FOLDER)/hal_cpu.c
SRC += $(HAL_FOLDER)$(BOARD_FOLDER)/hal_din.c
SRC += $(HAL_FOLDER)$(BOARD_FOLDER)/hal_dout.c
SRC += $(HAL_FOLDER)$(BOARD_FOLDER)/hal_led.c
SRC += $(HAL_FOLDER)$(BOARD_FOLDER)/hal_pwm.c
SRC += $(HAL_FOLDER)$(BOARD_FOLDER)/hal_time.c
SRC += $(HAL_FOLDER)$(BOARD_FOLDER)/hal_watchdog.c

# source files for specific features - only compiled if feature is activated
ifeq ($(USE_UART), yes)
	SRC += $(SRC_FOLDER)com/uart.c
	SRC += $(HAL_FOLDER)$(BOARD_FOLDER)/hal_uart.c
	DDEFS += -DHAS_UART
endif
ifeq ($(USE_USB), yes)
	SRC += $(SRC_FOLDER)com/usb.c
	DDEFS += -DHAS_USB
endif

ifeq ($(USE_SPI), yes)
	DDEFS += -DHAS_SPI
	SRC += $(HAL_FOLDER)$(BOARD_FOLDER)/hal_spi.c
endif

ifeq ($(USE_STEP_DIR), yes)
	DDEFS += -DUSE_STEP_DIR
endif

ifeq ($(ACTIVATE_DEBUG), yes)
	SRC += $(SRC_FOLDER)debug/debug.c
	SRC += $(HAL_FOLDER)$(BOARD_FOLDER)/hal_debug.c
	DDEFS += -DDEBUG_ACTIVE
endif

