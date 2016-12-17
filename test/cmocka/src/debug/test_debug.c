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

#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <stdio.h>
#include <ctype.h>
#include <stdint.h>
#include "cmocka.h"


extern void debug_init(void);
extern void debug_tick(void);
extern void count_debug_ticks_per_ms(void);
extern void search_for_orders(void);
extern void parse_order(int length);
extern void debug_hex_buffer(uint8_t* buf, int length);
extern uint_fast8_t hexstr2byte(uint8_t high, uint8_t low);
extern void order_help(void);
extern void order_curTime(void);
extern uint32_t getStartOffsetOfNextWord(uint8_t* buf, uint32_t length);
extern uint32_t getNumBytesNextWord(uint8_t* buf, uint32_t length);
extern uint32_t getHexNumber(uint8_t* buf, uint32_t length);
extern void printMemory(uint8_t* buf, uint32_t length);
extern uint_fast8_t hexChar2int(uint8_t c);


// void debug_init(void);
static void test_debug_init(void **state)
{
    debug_init();
}

// void debug_tick(void);
// static void count_debug_ticks_per_ms(void);
// static void search_for_orders(void);
// static void parse_order(int length);
// static void debug_hex_buffer(uint8_t* buf, int length);
// static uint_fast8_t hexstr2byte(uint8_t high, uint8_t low);
// static void order_help(void);
// static void order_curTime(void);
// static uint32_t getStartOffsetOfNextWord(uint8_t* buf, uint32_t length);
static void test_getStartOffsetOfNextWord_command_and_leading_whitespace(void **state)
{
    uint8_t cmd_buf[30] = "   bla   ";
    assert_int_equal(3, getStartOffsetOfNextWord(&cmd_buf[0], 10));
}

static void test_getStartOffsetOfNextWord_command_and_one_leading_whitespace(void **state)
{
    uint8_t cmd_buf[30] = " bla   ";
    assert_int_equal(1, getStartOffsetOfNextWord(&cmd_buf[0], 10));
}

static void test_getStartOffsetOfNextWord_command_and_no_whitespace(void **state)
{
    uint8_t cmd_buf[30] = "bla   ";
    assert_int_equal(0, getStartOffsetOfNextWord(&cmd_buf[0], 10));
}

static void test_getStartOffsetOfNextWord_no_command(void **state)
{
    uint8_t cmd_buf[30] = "   ";
    assert_int_equal(0, getStartOffsetOfNextWord(&cmd_buf[0], 10));
}

static void test_getStartOffsetOfNextWord_no_whitespace(void **state)
{
    uint8_t cmd_buf[30] = "bla";
    assert_int_equal(0, getStartOffsetOfNextWord(&cmd_buf[0], 10));
}

// static uint32_t getNumBytesNextWord(uint8_t* buf, uint32_t length);
static void test_getNumBytesNextWord_lengthZero(void **state)
{
    uint8_t cmd_buf[30] = {0};
    assert_int_equal(0, getNumBytesNextWord(&cmd_buf[0], 0));
}

static void test_getNumBytesNextWord_noMoreChars(void **state)
{
    uint8_t cmd_buf[30] = {0};
    assert_int_equal(0, getNumBytesNextWord(&cmd_buf[0], 10));
}

static void test_getNumBytesNextWord_only_whitespace(void **state)
{
    uint8_t cmd_buf[30] = "   \t   ";
    assert_int_equal(0, getNumBytesNextWord(&cmd_buf[0], 10));
}

static void test_getNumBytesNextWord_command_and_leading_whitespace(void **state)
{
    uint8_t cmd_buf[30] = "   bla   ";
    assert_int_equal(3, getNumBytesNextWord(&cmd_buf[3], 10));
}

static void test_getNumBytesNextWord_command_no_leading_whitespace(void **state)
{
    uint8_t cmd_buf[30] = "bla      ";
    assert_int_equal(3, getNumBytesNextWord(&cmd_buf[0], 10));
}

// static uint32_t getHexNumber(uint8_t* buf, uint32_t length);
static void test_getHexNumber_lengthZero(void **state)
{
    uint8_t cmd_buf[30] = "bla      ";
    assert_int_equal(0, getHexNumber(&cmd_buf[0], 0));
}

static void test_getHexNumber_one_char(void **state)
{
    uint8_t cmd_buf[30] = "a      ";
    assert_int_equal(10, getHexNumber(&cmd_buf[0], 1));
}

static void test_getHexNumber_chars_withOffset(void **state)
{
    uint8_t cmd_buf[30] = " 0xdead      ";
    // 0xdead == 57005
    assert_int_equal(57005, getHexNumber(&cmd_buf[3], 4));
}

// static void printMemory(uint8_t* buf, uint32_t length);
// static uint_fast8_t hexChar2int(uint8_t c);





int main(void)
{
    const struct CMUnitTest tests[] = {
            cmocka_unit_test(test_debug_init),
            cmocka_unit_test(test_getStartOffsetOfNextWord_command_and_leading_whitespace),
            cmocka_unit_test(test_getStartOffsetOfNextWord_command_and_one_leading_whitespace),
            cmocka_unit_test(test_getStartOffsetOfNextWord_command_and_no_whitespace),
            cmocka_unit_test(test_getStartOffsetOfNextWord_no_command),
            cmocka_unit_test(test_getStartOffsetOfNextWord_no_whitespace),
            cmocka_unit_test(test_getNumBytesNextWord_lengthZero),
            cmocka_unit_test(test_getNumBytesNextWord_noMoreChars),
            cmocka_unit_test(test_getNumBytesNextWord_only_whitespace),
            cmocka_unit_test(test_getNumBytesNextWord_command_and_leading_whitespace),
            cmocka_unit_test(test_getNumBytesNextWord_command_no_leading_whitespace),
            cmocka_unit_test(test_getHexNumber_lengthZero),
            cmocka_unit_test(test_getHexNumber_one_char),
            cmocka_unit_test(test_getHexNumber_chars_withOffset),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}

