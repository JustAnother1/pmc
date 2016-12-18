CMOCKA_t_TEST_INCDIRS  = $(INCDIRS) 
CMOCKA_t_TEST_INCDIRS += /usr/include
CMOCKA_t_TEST_INCDIRS += /usr/lib/gcc/x86_64-linux-gnu/5/include
CMOCKA_t_TEST_INCDIRS += test/cmocka/stub/hal/

CMOCKA_TEST_INCDIRS = $(patsubst %,-I%, $(CMOCKA_t_TEST_INCDIRS))

# tests
CMOCKA_TEST_SRCS  = cmocka/src/debug/test_debug.c
# code that gets tested
CMOCKA_TEST_SRCS += cmocka/src/debug/wrap_debug.c
#stubs
CMOCKA_TEST_SRCS += cmocka/stub/hal/stubHal_debug.c
CMOCKA_TEST_SRCS += cmocka/stub/hal/stubHal_uart.c
CMOCKA_TEST_SRCS += cmocka/stub/hal/stubHal_time.c
CMOCKA_TEST_SRCS += cmocka/stub/hal/stubHal_cpu.c
CMOCKA_TEST_SRCS += cmocka/stub/hal/stubHal_spi.c
CMOCKA_TEST_SRCS += cmocka/stub/hal/stubHal_adc.c
CMOCKA_TEST_SRCS += cmocka/stub/hal/stubHal_i2c.c
CMOCKA_TEST_SRCS += cmocka/stub/hal/stubHal_din.c
CMOCKA_TEST_SRCS += cmocka/stub/src/device/stubDevice_buzzer.c
CMOCKA_TEST_SRCS += cmocka/stub/src/device/stubDevice_temperature_sensor.c
CMOCKA_TEST_SRCS += cmocka/stub/src/lib/stubPrintf.c
CMOCKA_TEST_SRCS += cmocka/stub/src/order/stubCommand_queue.c
CMOCKA_TEST_SRCS += cmocka/stub/src/stepper/stubTrinamic.c
CMOCKA_TEST_SRCS += cmocka/stub/src/stepper/stubDevice_stepper.c

CMOCKA_TEST_OBJS = $(addprefix $(TEST_BIN_FOLDER),$(CMOCKA_TEST_SRCS:%.c=%.o))
CMOCKA_TEST_LDFLAGS = -g -Wall -fwhole-program -lcmocka