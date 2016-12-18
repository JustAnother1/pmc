/*
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License version 2
 * as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, see <http://www.gnu.org/licenses/>
 *
 */

#include "CppUTest/TestHarness.h"
#include "CppUTest/TestOutput.h"
#include "CppUTest/TestTestingFixture.h"
#include "CppUTest/PlatformSpecificFunctions.h"

#include "hal_uart.h"
#include "hal_cfg.h"
#include "st_usart.h"

typedef struct {
    // receive
    uint8_t         receive_buffer[UART_RECEIVE_BUFFER_SIZE];
    uint_fast16_t   read_pos;
    uint_fast16_t   write_pos;
    // send
    uint8_t         send_buffer[UART_SEND_BUFFER_SIZE];
    bool            is_sending;
    uint_fast16_t   send_pos; // the byte that will be send next
    uint_fast16_t   send_end_pos; // free slot after data to send
    // the port
    USART_TypeDef * port;
}uart_device_typ;

extern volatile uart_device_typ devices[MAX_UART];
extern bool gcode_initialized;
extern bool debug_initialized;

TEST_GROUP(HalStUartTestGroup)
{

};

TEST(HalStUartTestGroup, hal_print_configuration_gcode_uart)
{
    hal_print_configuration_gcode_uart();
}

TEST(HalStUartTestGroup, hal_print_configuration_debug_uart)
{
    hal_print_configuration_debug_uart();
}


TEST(HalStUartTestGroup, hal_init_gcode_uart)
{
    gcode_initialized = false;
    CHECK(hal_init_gcode_uart());
}

TEST(HalStUartTestGroup, hal_init_gcode_uart_alreadyInitialized)
{
    gcode_initialized = true;
    CHECK_FALSE(hal_init_gcode_uart());
}

TEST(HalStUartTestGroup, hal_init_debug_uart)
{
    debug_initialized = false;
    CHECK(hal_init_debug_uart());
}

TEST(HalStUartTestGroup, hal_init_debug_uart_alreadyInitialized)
{
    debug_initialized = true;
    CHECK_FALSE(hal_init_debug_uart());
}


TEST(HalStUartTestGroup, hal_get_gcode_uart_byte_at_offset)
{
    devices[GCODE_UART].read_pos = 0;
    devices[GCODE_UART].receive_buffer[0] = 'a';
    devices[GCODE_UART].receive_buffer[1] = 'b';
    CHECK_EQUAL('a', hal_get_gcode_uart_byte_at_offset(0));
    devices[GCODE_UART].read_pos = 1;
    CHECK_EQUAL('b', hal_get_gcode_uart_byte_at_offset(0));
    devices[GCODE_UART].read_pos = 0;
    CHECK_EQUAL('b', hal_get_gcode_uart_byte_at_offset(1));
    CHECK_EQUAL('a', hal_get_gcode_uart_byte_at_offset(UART_RECEIVE_BUFFER_SIZE));
}

TEST(HalStUartTestGroup, hal_get_debug_uart_byte_at_offset)
{
    devices[DEBUG_UART].read_pos = 0;
    devices[DEBUG_UART].receive_buffer[0] = 'a';
    devices[DEBUG_UART].receive_buffer[1] = 'b';
    CHECK_EQUAL('a', hal_get_debug_uart_byte_at_offset(0));
    devices[DEBUG_UART].read_pos = 1;
    CHECK_EQUAL('b', hal_get_debug_uart_byte_at_offset(0));
    devices[DEBUG_UART].read_pos = 0;
    CHECK_EQUAL('b', hal_get_debug_uart_byte_at_offset(1));
    CHECK_EQUAL('a', hal_get_debug_uart_byte_at_offset(UART_RECEIVE_BUFFER_SIZE));
}


TEST(HalStUartTestGroup, hal_get_available_bytes_gcode_uart)
{
    devices[GCODE_UART].read_pos = 0;
    devices[GCODE_UART].write_pos = 0;
    CHECK_EQUAL(0, hal_get_available_bytes_gcode_uart());
    devices[GCODE_UART].read_pos = 5;
    devices[GCODE_UART].write_pos = 5;
    CHECK_EQUAL(0, hal_get_available_bytes_gcode_uart());
    devices[GCODE_UART].read_pos = 0;
    devices[GCODE_UART].write_pos = 5;
    CHECK_EQUAL(5, hal_get_available_bytes_gcode_uart());
    devices[GCODE_UART].read_pos = UART_RECEIVE_BUFFER_SIZE -6;
    devices[GCODE_UART].write_pos = UART_RECEIVE_BUFFER_SIZE-1;
    CHECK_EQUAL(5, hal_get_available_bytes_gcode_uart());
    devices[GCODE_UART].read_pos = UART_RECEIVE_BUFFER_SIZE-3;
    devices[GCODE_UART].write_pos = 2;
    CHECK_EQUAL(5, hal_get_available_bytes_gcode_uart());
}

TEST(HalStUartTestGroup, hal_get_available_bytes_debug_uart)
{
    devices[DEBUG_UART].read_pos = 0;
    devices[DEBUG_UART].write_pos = 0;
    CHECK_EQUAL(0, hal_get_available_bytes_debug_uart());
    devices[DEBUG_UART].read_pos = 5;
    devices[DEBUG_UART].write_pos = 5;
    CHECK_EQUAL(0, hal_get_available_bytes_debug_uart());
    devices[DEBUG_UART].read_pos = 0;
    devices[DEBUG_UART].write_pos = 5;
    CHECK_EQUAL(5, hal_get_available_bytes_debug_uart());
    devices[DEBUG_UART].read_pos = UART_RECEIVE_BUFFER_SIZE -6;
    devices[DEBUG_UART].write_pos = UART_RECEIVE_BUFFER_SIZE-1;
    CHECK_EQUAL(5, hal_get_available_bytes_debug_uart());
    devices[DEBUG_UART].read_pos = UART_RECEIVE_BUFFER_SIZE-3;
    devices[DEBUG_UART].write_pos = 2;
    CHECK_EQUAL(5, hal_get_available_bytes_debug_uart());
}


TEST(HalStUartTestGroup, hal_forget_bytes_gcode_uart)
{
    devices[GCODE_UART].read_pos = 0;
    hal_forget_bytes_gcode_uart(0);
    CHECK_EQUAL(0, devices[GCODE_UART].read_pos);
    hal_forget_bytes_gcode_uart(5);
    CHECK_EQUAL(5, devices[GCODE_UART].read_pos);
    hal_forget_bytes_gcode_uart(5);
    CHECK_EQUAL(10, devices[GCODE_UART].read_pos);
    hal_forget_bytes_gcode_uart(UART_RECEIVE_BUFFER_SIZE);
    CHECK_EQUAL(10, devices[GCODE_UART].read_pos);
    devices[GCODE_UART].read_pos = UART_RECEIVE_BUFFER_SIZE -2;
    hal_forget_bytes_gcode_uart(3);
    CHECK_EQUAL(1, devices[GCODE_UART].read_pos);
}

TEST(HalStUartTestGroup, hal_forget_bytes_debug_uart)
{
    devices[DEBUG_UART].read_pos = 0;
    hal_forget_bytes_debug_uart(0);
    CHECK_EQUAL(0, devices[DEBUG_UART].read_pos);
    hal_forget_bytes_debug_uart(5);
    CHECK_EQUAL(5, devices[DEBUG_UART].read_pos);
    hal_forget_bytes_debug_uart(5);
    CHECK_EQUAL(10, devices[DEBUG_UART].read_pos);
    hal_forget_bytes_debug_uart(UART_RECEIVE_BUFFER_SIZE);
    CHECK_EQUAL(10, devices[DEBUG_UART].read_pos);
    devices[DEBUG_UART].read_pos = UART_RECEIVE_BUFFER_SIZE -2;
    hal_forget_bytes_debug_uart(3);
    CHECK_EQUAL(1, devices[DEBUG_UART].read_pos);
}


TEST(HalStUartTestGroup, hal_send_frame_gcode_uart)
{
    devices[GCODE_UART].is_sending = false;
    hal_send_frame_gcode_uart(NULL, 0);
    hal_send_frame_gcode_uart(NULL, 5);
}

TEST(HalStUartTestGroup, hal_send_frame_debug_uart)
{
    devices[DEBUG_UART].is_sending = false;
    hal_send_frame_debug_uart(NULL, 0);
    hal_send_frame_debug_uart(NULL, 5);
}


TEST(HalStUartTestGroup, hal_send_frame_non_blocking_gcode_uart)
{
    devices[GCODE_UART].is_sending = false;
    CHECK(hal_send_frame_non_blocking_gcode_uart(NULL, 0));
    CHECK_FALSE(hal_send_frame_non_blocking_gcode_uart(NULL, 5));
}

/*

void hal_send_frame_gcode_uart(uint8_t * frame, uint_fast16_t length);
void hal_send_frame_debug_uart(uint8_t * frame, uint_fast16_t length);

bool hal_send_frame_non_blocking_gcode_uart(uint8_t * frame, uint_fast16_t length);
bool hal_send_frame_non_blocking_debug_uart(uint8_t * frame, uint_fast16_t length);
*/
