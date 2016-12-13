TEST_INCDIRS  = $(INCDIRS) 
TEST_INCDIRS += /usr/include/
TEST_INCDIRS += test/stub/hal/

# tests
TEST_SRCS  = test/src/debug/test_debug.c
# code that gets tested
TEST_SRCS += test/src/debug/wrap_debug.c
#stubs
TEST_SRCS += test/stub/hal/stubHal_debug.c
TEST_SRCS += test/stub/hal/stubHal_uart.c
TEST_SRCS += test/stub/hal/stubHal_time.c
TEST_SRCS += test/stub/hal/stubHal_cpu.c
TEST_SRCS += test/stub/hal/stubHal_spi.c
TEST_SRCS += test/stub/hal/stubHal_adc.c
TEST_SRCS += test/stub/hal/stubHal_i2c.c
TEST_SRCS += test/stub/hal/stubHal_din.c
TEST_SRCS += test/stub/src/device/stubDevice_buzzer.c
TEST_SRCS += test/stub/src/device/stubDevice_temperature_sensor.c
TEST_SRCS += test/stub/src/order/stubCommand_queue.c
TEST_SRCS += test/stub/src/stepper/stubTrinamic.c
TEST_SRCS += test/stub/src/stepper/stubDevice_stepper.c

TEST_OBJS = $(addprefix $(TEST_BIN_FOLDER),$(TEST_SRCS:%.c=%.o))
TEST_LDFLAGS = -g -Wall -fwhole-program -lcmocka