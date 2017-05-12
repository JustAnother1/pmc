CPPUTEST_t_TEST_INCDIRS  = $(INCDIRS) 
CPPUTEST_t_TEST_INCDIRS += /usr/include
CPPUTEST_t_TEST_INCDIRS += test/cpputest/stub/

CPPUTEST_TEST_INCDIRS = $(patsubst %,-I%, $(CPPUTEST_t_TEST_INCDIRS))

CPPFLAGS += -std=c++11

# tests
CPPUTEST_TEST_SRCS  += cpputest/test_hal_st_uart.cpp
CPPUTEST_TEST_SRCS  += cpputest/wrap_hal_st_uart.cpp
CPPUTEST_TEST_SRCS  += cpputest/test_hal_st_spi.cpp
CPPUTEST_TEST_SRCS  += cpputest/wrap_hal_st_spi.cpp
CPPUTEST_TEST_SRCS  += cpputest/test_src_lib_log.cpp
CPPUTEST_TEST_SRCS  += cpputest/wrap_src_lib_log.cpp
CPPUTEST_TEST_SRCS  += cpputest/test_src_stepper_step.cpp
CPPUTEST_TEST_SRCS  += cpputest/wrap_src_stepper_step.cpp
# stubs
CPPUTEST_TEST_SRCS  += cpputest/stub/stub_st_util.cpp
CPPUTEST_TEST_SRCS  += cpputest/stub/stub_arm_c4.cpp
CPPUTEST_TEST_SRCS  += cpputest/stub/stub_hal_cpu.cpp
CPPUTEST_TEST_SRCS  += cpputest/stub/stub_hw_registers.cpp
CPPUTEST_TEST_SRCS  += cpputest/stub/stub_hal_led.cpp
CPPUTEST_TEST_SRCS  += cpputest/stub/stub_hal_time.cpp
CPPUTEST_TEST_SRCS  += cpputest/stub/stub_hal_stepper_port.cpp
CPPUTEST_TEST_SRCS  += cpputest/stub/stub_src_error.cpp
CPPUTEST_TEST_SRCS  += cpputest/stub/stub_src_stepper_device_stepper.cpp
CPPUTEST_TEST_SRCS  += cpputest/stub/stub_src_stepper_pololu.cpp
CPPUTEST_TEST_SRCS  += cpputest/stub/stub_src_lib_printf.cpp
# test suite
CPPUTEST_TEST_SRCS  += cpputest/AllTests.cpp


CPPUTEST_TEST_OBJS = $(addprefix $(TEST_BIN_FOLDER),$(CPPUTEST_TEST_SRCS:%.cpp=%.o))
CPPUTEST_TEST_LDFLAGS = -g -Wall -fwhole-program -L$(CPPUTEST_HOME)/lib -lCppUTest -lCppUTestExt