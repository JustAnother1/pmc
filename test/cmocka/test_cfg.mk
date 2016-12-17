TEST_INCDIRS  = $(INCDIRS) 
TEST_INCDIRS += /usr/include
TEST_INCDIRS += /usr/lib/gcc/x86_64-linux-gnu/5/include
TEST_INCDIRS += test/cmocka/stub/hal/


# tests
TEST_SRCS  = test/cmocka/src/debug/test_debug.c
# code that gets tested
TEST_SRCS += test/cmocka/src/debug/wrap_debug.c
#stubs
TEST_SRCS += test/cmocka/stub/hal/stubHal_debug.c
TEST_SRCS += test/cmocka/stub/hal/stubHal_uart.c
TEST_SRCS += test/cmocka/stub/hal/stubHal_time.c
TEST_SRCS += test/cmocka/stub/hal/stubHal_cpu.c
TEST_SRCS += test/cmocka/stub/hal/stubHal_spi.c
TEST_SRCS += test/cmocka/stub/hal/stubHal_adc.c
TEST_SRCS += test/cmocka/stub/hal/stubHal_i2c.c
TEST_SRCS += test/cmocka/stub/hal/stubHal_din.c
TEST_SRCS += test/cmocka/stub/src/device/stubDevice_buzzer.c
TEST_SRCS += test/cmocka/stub/src/device/stubDevice_temperature_sensor.c
TEST_SRCS += test/cmocka/stub/src/lib/stubPrintf.c
TEST_SRCS += test/cmocka/stub/src/order/stubCommand_queue.c
TEST_SRCS += test/cmocka/stub/src/stepper/stubTrinamic.c
TEST_SRCS += test/cmocka/stub/src/stepper/stubDevice_stepper.c

TEST_OBJS = $(addprefix $(TEST_BIN_FOLDER),$(TEST_SRCS:%.c=%.o))
TEST_LDFLAGS = -g -Wall -fwhole-program -lcmocka