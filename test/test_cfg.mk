TEST_INCDIRS = $(INCDIRS) /usr/include/

TEST_SRCS = test/debug/test_debug.c test/debug/wrap_debug.c \
test/stub/hal/stubHal_debug.c test/stub/hal/stubHal_uart.c \
test/stub/hal/stubHal_time.c test/stub/hal/stubHal_cpu.c \
test/stub/hal/stubHal_spi.c test/stub/src/stepper/stubTrinamic.c \
test/stub/src/order/stubCommand_queue.c test/stub/src/stepper/stubDevice_stepper.c

TEST_OBJS = $(addprefix $(TEST_BIN_FOLDER),$(TEST_SRCS:%.c=%.o))
TEST_LDFLAGS = -g -Wall -fwhole-program -lcmocka