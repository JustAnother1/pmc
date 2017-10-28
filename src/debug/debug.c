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

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <ctype.h>
#include "error.h"
#include "debug.h"
#include "hal_adc.h"
#include "hal_cfg.h"
#include "hal_cpu.h"
#include "hal_debug.h"
#include "hal_din.h"
#include "hal_i2c.h"
#include "hal_power.h"
#include "hal_spi.h"
#include "hal_time.h"
#include "hal_uart.h"
#include "hal_usb_device_cdc.h"
#include "command_queue.h"
#include "device_buzzer.h"
#include "device_heater.h"
#include "device_stepper.h"
#include "device_temperature_sensor.h"
#include "end_stop_handling.h"
#include "protocol.h"
#include "pololu.h"
#include "trinamic.h"

#define SLOW_ORDER_HELP                1
#define SLOW_ORDER_DEBUG_INFORMATION   2
#define SLOW_ORDER_HEATER_STATUS       3
#define SLOW_ORDER_TIMER_CONFIGURATION 4

// ticks per millisecond
static uint_fast32_t tick_cnt;
static uint32_t tick_value;
static uint_fast32_t tick_max;
static uint_fast32_t tick_min;

// slow orders:
static bool order_ongoing;
static int slow_order;
static int slow_order_state;
static uint8_t slow_order_num_values;
static uint8_t slow_order_cur_value;
static uint8_t slow_order_cur_cnt;


// order parsing
static uint_fast16_t checked_bytes = 0;
static uint_fast8_t last_line_end = 'l'; // invalid value

static void count_debug_ticks_per_ms(void);
static void search_for_orders(void);
static bool parse_order(int length);
void debug_hex_buffer(uint8_t* buf, int length);
static uint_fast8_t hexstr2byte(uint8_t high, uint8_t low);
static void start_order_help(void);
static bool continue_order_help(void);
static void start_order_debug_information(void);
static bool continue_order_debug_information(void);
static void start_order_heater_status(void);
static bool continue_order_heater_status(void);
static void start_order_timer_config(int timer);
static bool continue_order_timer_config(void);
static void order_curTime(void);
static uint32_t getStartOffsetOfNextWord(uint8_t* buf, uint32_t length);
static uint32_t getNumBytesNextWord(uint8_t* buf, uint32_t length);
static uint32_t getHexNumber(uint8_t* buf, uint32_t length);
static void printMemory(uint8_t* buf, uint32_t length);
static uint_fast8_t hexChar2int(uint8_t c);
static void handle_ongoing_commands(void);

void curTest(int value);

void debug_init(void)
{
    // Tick counting
    tick_cnt = 0;
    tick_value = 0;
    tick_max = 0;
    tick_min = 42424242;
    order_ongoing = false;
    slow_order = 0;
    slow_order_state = 0;
    hal_debug_init();
    hal_cpu_check_Reset_Reason();
    debug_line(STR("Debug Console PMC"));
    debug_msg(STR("(db)"));
}

void debug_tick(void)
{
    // do some statistics / supervision tasks
    count_debug_ticks_per_ms();
    // check if we received an order
    search_for_orders();
    // handle long running orders
    handle_ongoing_commands();
}

static void handle_ongoing_commands(void)
{
    if(false == order_ongoing)
    {
        return;
    }
    if(false == hal_debug_is_send_buffer_empty())
    {
        // we send the next line only if the buffer is empty again.
        return;
    }
    // else:
    switch(slow_order)
    {
    case SLOW_ORDER_HELP:
        if(true == continue_order_help())
        {
            // finished
            order_ongoing = false;
        }
        // else try again next tick.
        break;

    case SLOW_ORDER_DEBUG_INFORMATION:
        if(true == continue_order_debug_information())
        {
            // finished
            order_ongoing = false;
        }
        // else try again next tick.
        break;

    case SLOW_ORDER_HEATER_STATUS:
        if(true == continue_order_heater_status())
        {
            // finished
            order_ongoing = false;
        }
        // else try again next tick.
        break;

    case SLOW_ORDER_TIMER_CONFIGURATION:
        if(true == continue_order_timer_config())
        {
            // finished
            order_ongoing = false;
        }
        // else try again next tick.
        break;

    default:
        order_ongoing = false;
        break;
    }
    if(false == order_ongoing)
    {
        debug_msg(STR("(db)"));
        return;
    }
}

static void count_debug_ticks_per_ms(void)
{
    uint32_t time =  hal_cpu_get_ms_tick();
    if(tick_value == time)
    {
        // another tick in this ms
        tick_cnt++;
    }
    else
    {
        if(0 != tick_value)
        {
            if(time == tick_value  + 1)
            {
                // record the statistics
                if(tick_cnt > tick_max)
                {
                    tick_max = tick_cnt;
                    // debug_line(STR("New tick max: %d\n"), tick_max);
                }
                if(tick_cnt < tick_min)
                {
                    tick_min = tick_cnt;
                    // debug_line("STR(New tick min: %d\n"), tick_min);
                }
            }
            else
            {
                // something blocked for more than one ms !
                tick_min = 0;
            }
        }
        tick_value = time;
        tick_cnt = 0;
    }
}

static void search_for_orders(void)
{
    unsigned int i;
    uint_fast16_t num_bytes_received = hal_get_available_bytes_debug_uart();
    if(checked_bytes == num_bytes_received)
    {
        // we did a check on this number of Bytes and had no order
        return;
    }
    // debug_line(STR("Received %d bytes\n"), num_bytes_received);
    //checked_bytes = num_bytes_received;
    for(i = checked_bytes -1; i < num_bytes_received; i++)
    {
        // there is a byte at offset 0,
        // but num_bytes_received = 0 mean nothing received
        uint_fast8_t c = hal_get_debug_uart_byte_at_offset(i);
        if(('\r' == c) || ('\n' == c))
        {
            if(0 == i)
            {
                // either a \r\n or an empty command
                if(c == last_line_end)
                {
                    // empty command
                    hal_forget_bytes_debug_uart(i+1);
                    checked_bytes = 0;
                    debug_msg(STR("(db)"));
                    return;
                }
                else
                {
                    // two byte line end
                }
            }
            else
            {
                last_line_end = c;
            }
            // debug_line(STR("found line feed !"));
            debug_line(STR("\r\n"));
            if(true == parse_order(i))
            {
                debug_msg(STR("(db)"));
            }
            // else command is still ongoing
            hal_forget_bytes_debug_uart(i+1);
            checked_bytes = 0;
            return;
        }
    }
    checked_bytes = num_bytes_received;
}

static int get_next_word(int start_pos, int end_pos, uint8_t *buf)
{
    int i;
    int idx = 0;
    for(i = start_pos; i < end_pos; i++)
    {
        uint8_t c = hal_get_debug_uart_byte_at_offset(i);
        switch(c)
        {
        case '\t':
        case '\r':
            // ignore
            break;

        case '\n':
            // end of command
            return i;

        default:
            buf[idx] = c;
            idx++;
            break;
        }
    }
    return i;
}

void debug_hex_buffer(uint8_t* buf, int length)
{
    int i;
    for(i = 0; i < length; i++)
    {
        debug_msg(STR("%02x "), buf[i]);
    }
}

static uint_fast8_t hexChar2int(uint8_t c)
{
    switch(c)
    {
    // case '0': return 0;
    case '1': return 1;
    case '2': return 2;
    case '3': return 3;
    case '4': return 4;
    case '5': return 5;
    case '6': return 6;
    case '7': return 7;
    case '8': return 8;
    case '9': return 9;
    case 'A':
    case 'a': return 10;
    case 'B':
    case 'b': return 11;
    case 'C':
    case 'c': return 12;
    case 'D':
    case 'd': return 13;
    case 'E':
    case 'e': return 14;
    case 'F':
    case 'f': return 15;
    default: return 0;
    }
}

static uint_fast8_t hexstr2byte(uint8_t high, uint8_t low)
{
    uint_fast8_t res = 0;
    switch(high)
    {
    case '0':break;
    case '1': res = res + 1*16; break;
    case '2': res = res + 2*16; break;
    case '3': res = res + 3*16; break;
    case '4': res = res + 4*16; break;
    case '5': res = res + 5*16; break;
    case '6': res = res + 6*16; break;
    case '7': res = res + 7*16; break;
    case '8': res = res + 8*16; break;
    case '9': res = res + 9*16; break;
    case 'A':
    case 'a': res = res + 10*16; break;
    case 'B':
    case 'b': res = res + 11*16; break;
    case 'C':
    case 'c': res = res + 12*16; break;
    case 'D':
    case 'd': res = res + 13*16; break;
    case 'E':
    case 'e': res = res + 14*16; break;
    case 'F':
    case 'f': res = res + 15*16; break;
    default: return 0;
    }
    switch(low)
    {
    case '0':break;
    case '1': res = res + 1; break;
    case '2': res = res + 2; break;
    case '3': res = res + 3; break;
    case '4': res = res + 4; break;
    case '5': res = res + 5; break;
    case '6': res = res + 6; break;
    case '7': res = res + 7; break;
    case '8': res = res + 8; break;
    case '9': res = res + 9; break;
    case 'A':
    case 'a': res = res + 10; break;
    case 'B':
    case 'b': res = res + 11; break;
    case 'C':
    case 'c': res = res + 12; break;
    case 'D':
    case 'd': res = res + 13; break;
    case 'E':
    case 'e': res = res + 14; break;
    case 'F':
    case 'f': res = res + 15; break;
    default: return 0;
    }
    return res;
}

static void start_order_timer_config(int timer)
{
    debug_line(STR("timer configuration:"));
    slow_order_cur_value = timer;
    slow_order_cur_cnt = 0;
    order_ongoing = true;
    slow_order = SLOW_ORDER_TIMER_CONFIGURATION;
    slow_order_state = 1;
}
static bool continue_order_timer_config(void)
{
    if(false == hal_time_print_Configuration(slow_order_cur_value, slow_order_cur_cnt))
    {
        return true;
    }
    else
    {
        slow_order_cur_cnt++;
        return false;
    }
}

static void start_order_heater_status(void)
{
    debug_line(STR("heater status:"));
    slow_order_num_values = dev_heater_get_count();
    slow_order_cur_value = 0;
    order_ongoing = true;
    slow_order = SLOW_ORDER_HEATER_STATUS;
    slow_order_state = 1;
}

static bool continue_order_heater_status(void)
{
    switch(slow_order_state)
    {
    case 1:
        debug_line(STR("heater %d:"), slow_order_cur_value);
        slow_order_state++;
        break;

    case 2:
    {
        uint8_t nameBuf[20];
        uint_fast8_t name_length;
        name_length = dev_heater_get_name(slow_order_cur_value, &(nameBuf[0]), 20);
        nameBuf[name_length] = 0;
        debug_line(STR("name               : %s"), &(nameBuf[0]));
        slow_order_state++;
    }
        break;

    case 3:
        switch(dev_heater_get_status(slow_order_cur_value))
        {
        case DEVICE_STATUS_ACTIVE:
            debug_line(STR("status             : Active"));
            break;

        case DEVICE_STATUS_FAULT:
            debug_line(STR("status             : Fault"));
            break;

        default:
            debug_line(STR("status             : %d"), dev_heater_get_status(slow_order_cur_value));
            break;
        }
        slow_order_state++;
        break;

    case 4:
    {
        uint_fast16_t temperature;
        temperature = dev_heater_get_temperature(slow_order_cur_value);
        debug_line(STR("cur. Temperature   : %d.%01d°C"), temperature/10, temperature%10);
        slow_order_state++;
    }
        break;

    case 5:
        dev_heater_get_debug_information(slow_order_cur_value);
        slow_order_state++;
        break;

    case 6:
        slow_order_cur_value++;
        if(slow_order_cur_value == slow_order_num_values)
        {
            slow_order_state++;
        }
        else
        {
            slow_order_state = 1;
        }
        break;

    // if we reach the default case then we are done.
    default: return true;
    }
    return false;
}

static void start_order_debug_information(void)
{
    debug_line(STR("current status:"));
    order_ongoing = true;
    slow_order = SLOW_ORDER_DEBUG_INFORMATION;
    slow_order_state = 1;
}

static bool continue_order_debug_information(void)
{
    switch(slow_order_state)
    {
    case 1: debug_line(STR("ticks per ms: max=%d, min=%d"), tick_max, tick_min); slow_order_state++; break;
    case 2: debug_line(STR("number of detected steppers: %d"), dev_stepper_get_count()); slow_order_state++; break;
    case 3: slow_order_num_values = hal_adc_get_amount();
            slow_order_cur_value = 0;
            slow_order_state++;
            break;

    case 4: if(slow_order_cur_value < slow_order_num_values)
            {
                dev_temperature_sensor_print_status(slow_order_cur_value);
                slow_order_cur_value++;
            }
            else
            {
                // printed all sensors -> done with this
                slow_order_cur_value = 0;
                slow_order_num_values = hal_din_get_amount();
                slow_order_state++;
            }
            break;

    case 5: if(slow_order_cur_value < slow_order_num_values)
            {
                if(0 == dev_input_get_switch_state(slow_order_cur_value))
                {
                    debug_line(STR("End Stop %d : not Triggered"), slow_order_cur_value );
                }
                else
                {
                    debug_line(STR("End Stop %d : Triggered"), slow_order_cur_value );
                }
                slow_order_cur_value++;
            }
            else
            {
                // printed all end stops -> done with this
                slow_order_state++;
            }
            break;

    // if we reach the default case then we are done.
    default: return true;
    }
    return false;
}

static void start_order_help(void)
{
    debug_line(STR("available commands:"));
    order_ongoing = true;
    slow_order = SLOW_ORDER_HELP;
    slow_order_state = 1;
}

static bool continue_order_help(void)
{
    switch(slow_order_state)
    {
    // a
    case 1: debug_line(STR("b<frequency>               : activate buzzer (freq=0 -> off)")); break;
    case 2: debug_line(STR("c<setting>                 : change special setting")); break;
    case 3: debug_line(STR("d                          : die - stops the processor")); break;
    case 4: debug_line(STR("e                          : show errors that have been reported.")); break;
    // f
    // g
    case 5: debug_line(STR("h                          : print this information")); break;
    case 6: debug_line(STR("ha <heat> <sens>           : associate temp.sensor to heater")); break;
    case 7: debug_line(STR("hs                         : show status of the heaters")); break;
    case 8: debug_line(STR("ht <num> <temp>            : set temperature of the heater")); break;
    // i
    // j
    // k
    case 9: debug_line(STR("l                          : list recorded debug information")); break;
    case 10: debug_line(STR("md<addressHex> <lengthHex> : print memory")); break;
    // n
    case 11: debug_line(STR("on                         : switch all power on")); break;
    case 12: debug_line(STR("off                        : switch all power off")); break;
    case 13: debug_line(STR("pa                         : print ADC configuration")); break;
#ifdef HAS_USB
    case 14: debug_line(STR("pb                         : print USB configuration")); break;
#else
    case 14: break;
#endif
    case 15: debug_line(STR("pc                         : print CPU configuration")); break;
#ifdef HAS_I2C
    case 16: debug_line(STR("pi                         : print I2C configuration")); break;
#else
    case 16: break;
#endif
    case 17: debug_line(STR("pin<Port,idx>              : print state of the pin")); break;
#ifdef HAS_TRINAMIC
    case 18: break;
#else
    case 18: debug_line(STR("pp                         : print Pololu stepper status")); break;
#endif
#ifdef EXPANSION_SPI
    case 19: debug_line(STR("pse                        : print expansion SPI configuration")); break;
#else
    case 19: break;
#endif
#ifdef STEPPER_SPI
    case 20: debug_line(STR("pss                        : print stepper SPI configuration")); break;
#else
    case 20: break;
#endif
    case 21: debug_line(STR("ptim<num>                  : print Timer Registers")); break;
#ifdef USE_STEP_DIR
#ifdef HAS_TRINAMIC
    case 22: debug_line(STR("ptri                       : print Trinamic stepper status")); break;
#else
    case 22: break;
#endif
#else
    case 22: break;
#endif
#ifdef DEBUG_UART
    case 23: debug_line(STR("pud                        : print Debug UART configuration")); break;
#else
    case 23: break;
#endif
#ifdef GCODE_UART
    case 24: debug_line(STR("pug                        : print G-Code UART configuration")); break;
#else
    case 24: break;
#endif
    case 25: debug_line(STR("pq                         : print command queue status")); break;
    // q
    case 26: debug_line(STR("r                          : reset the processor")); break;
    // s
#ifdef HAS_TRINAMIC
    case 27: debug_line(STR("sc                         : scan number of steppers")); break;
#else
    case 27:  break;
#endif
    case 28: debug_line(STR("t                          : show current time")); break;
    // u
    // v
#ifdef EXPANSION_SPI
    case 29: debug_line(STR("we<hex chars>              : write data to expansion SPI")); break;
#else
    case 29: break;
#endif
#ifdef STEPPER_SPI
    case 30: debug_line(STR("ws<hex chars>              : write data to stepper SPI")); break;
#else
    case 30: break;
#endif
    // x
    // y
    // z
    // 0..9, !, ?, ...
    // if we reach the default case then we are done.
    default: return true;
    }
    slow_order_state++;
    return false;
}

static void order_curTime(void)
{
    uint32_t now =  hal_cpu_get_ms_tick();
    debug_line(STR("now : %lu"), now);

    if(now < 1000)
    {
        debug_line(STR("%u ms"), now);
    }
    else
    {
        uint16_t millis;
        uint32_t seconds;
        millis = now % 1000;
        // debug_line(STR("millis : %u"), millis);
        seconds = now / 1000;
        // debug_line(STR("seconds : %lu"), seconds);
        if(seconds < 60)
        {
            debug_line(STR("%lu,%03u s"), seconds, millis);
        }
        else
        {
            uint32_t minutes;
            now = seconds;
            seconds = seconds % 60;
            // debug_line(STR("seconds : %lu"), seconds);
            minutes = now / 60;
            // debug_line(STR("minutes : %lu"), minutes);
            if(minutes < 60)
            {
                debug_line(STR("%lu:%02lu,%03u mm:ss"),minutes, seconds, millis);
            }
            else
            {
                uint32_t hours;
                now = minutes;
                minutes = minutes % 60;
                // debug_line(STR("minutes : %u"), minutes);
                hours = now / 60;
                debug_line(STR("%lu:%02lu:%02lu,%03u hh:mm:ss"), hours, minutes, seconds, millis);
            }
        }
    }
}

static void printMemory(uint8_t* buf, uint32_t length)
{
    if(0 < length)
    {
        // Adress(hex) : Data in Hex                                     : Data in ASCII
        //    00000000 : 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 : ....affe....beef
        debug_line(STR("Address(hex) : Data in Hex                                     : Data in ASCII"));
    }
    while(0 < length)
    {
        uint_fast8_t i;
        uint_fast8_t bytesInRow;
        uint32_t addr = (uint32_t)(uintptr_t)buf;
        if(length > 16)
        {
            bytesInRow = 16;
        }
        else
        {
            // last line
            bytesInRow = length;
        }

        // 00 00 00 00 :
        debug_msg(STR("    %02x%02x%02x%02x : "), (addr>>24) & 0xff, (addr>>16) & 0xff, (addr>>8) & 0xff, addr & 0xff);

        //  00 00 00 00  00 00 00 00  00 00 00 00  00 00 00 00  :
        for(i = 0; i < bytesInRow; i++)
        {
            debug_msg(STR("%02x "), *(buf + i));
        }
        for( ; i < 16; i++)
        {
            debug_msg(STR("   "));
        }
        //  ....affe....beef
        debug_msg(STR(": "));
        for(i = 0; i < bytesInRow; i++)
        {
            if(isalpha(*(buf + i)))
            {
                debug_msg(STR("%s"), *(buf + i));
            }
            else
            {
                debug_msg(STR("."));
            }
        }
        for( ; i < 16; i++)
        {
            debug_msg(STR(" "));
        }
        debug_msg(STR("\r\n"));
        buf = buf + 16;
        length = length - bytesInRow;
    }
}

static uint32_t getStartOffsetOfNextWord(uint8_t* buf, uint32_t length)
{
    uint32_t res = 0;
    while((length > 0) && ((' ' == *buf) || ('\t' == *buf)))
    {
        // skip whitespace before word
        buf++;
        length--;
        res ++;
    }
    if((0 != *buf) && (length > 0))
    {
        return res;
    }
    else
    {
        // only white space until end of line and no command
        return 0;
    }
}

static uint32_t getNumBytesNextWord(uint8_t* buf, uint32_t length)
{
    uint32_t res = 0;
    while((length > 0) && (' ' != *buf) && ('\t' != *buf) && (0 != *buf))
    {
        // count bytes
        res++;
        buf++;
        length--;
    }
    return res;
}

static uint32_t getHexNumber(uint8_t* buf, uint32_t length)
{
    uint32_t res = 0;
    unsigned int i;
    for(i = 0; i < length; i++)
    {
        res = res << 4;
        res = res + hexChar2int(*buf);
        buf++;
    }
    return res;
}

static bool parse_order(int length)
{
    uint8_t cmd_buf[10] = {0};
    int pos_in_buf = 0;
    pos_in_buf = get_next_word(pos_in_buf, length, &cmd_buf[0]);
    if(0 == pos_in_buf)
    {
        debug_line(STR("Invalid command ! try h for help"));
        return true;
    }
    switch(cmd_buf[0])
    {
// order = b
    case 'B':
    case 'b':
    {
        uint_fast16_t frequency = atoi((char *)&(cmd_buf[1]));
        debug_line(STR("setting frequency on buzzer to %d Hz"), frequency);
        dev_buzzer_set_frequency(0, frequency);
    }
        break;

// order = c
    case 'C':
    case 'c':
        if(1 == pos_in_buf)
        {
            debug_line(STR("Invalid command ! try h for help"));
            return true;
        }
        switch (cmd_buf[1])
        {
#ifdef HAS_TRINAMIC
// order = ct
        case 'T':
        case 't':
            // Trinamic (src/stepper/trinamic.c):
            if(false == trinamic_change_setting(&cmd_buf[2]))
            {
                debug_line(STR("Invalid command ! try h for help"));
            }
            // else -> OK
            break;
#endif

// order = cq
        case 'Q':
        case 'q':
            // Command Queue (src/order/commandqueue.c):
            if(false == cmd_queue_chnage_setting(&cmd_buf[2]))
            {
                debug_line(STR("Invalid command ! try h for help"));
            }
            // else -> OK
            break;

        default:
            debug_line(STR("Invalid command ! try h for help"));
            break;
        }
        break;

// order = d
    case 'D':
    case 'd': // die
        hal_cpu_die();
        break;

// order = e
    case 'E':
    case 'e': // report the already occurred problems
        hal_cpu_check_Reset_Reason();
        break;

// order = h
    case 'H':
    case 'h': // help - list available commands
        switch (cmd_buf[1])
        {

// order = ha
        // associate Temperature Sensor
        case 'A':
        case 'a':
        {
            uint_fast8_t heater_number;
            uint_fast8_t sensor_number;
            uint32_t numCharsNextParam;
            uint32_t startIndexOfParam;

            startIndexOfParam = 2 + getStartOffsetOfNextWord(&cmd_buf[2], length -2);
            numCharsNextParam = getNumBytesNextWord(&cmd_buf[startIndexOfParam], length - startIndexOfParam);
            heater_number = atoi((const char *)(&cmd_buf[startIndexOfParam]));

            startIndexOfParam = startIndexOfParam + numCharsNextParam;
            startIndexOfParam = startIndexOfParam + getStartOffsetOfNextWord(&cmd_buf[startIndexOfParam], length -startIndexOfParam);
            sensor_number = atoi((const char *)(&cmd_buf[startIndexOfParam]));

            dev_heater_set_temperature_sensor(heater_number, sensor_number);
            debug_line(STR("heater %d uses temperature sensor %d !"), heater_number, sensor_number);
        }
            break;

// order == hs
        // show status of heaters
        case 'S':
        case 's':
            start_order_heater_status();
            break;

//order = ht
        // set target temperature of Heater
        case 'T':
        case 't':
        {
            uint_fast8_t heater_number;
            uint_fast16_t target_temperature;
            uint32_t numCharsNextParam;
            uint32_t startIndexOfParam;

            startIndexOfParam = 2 + getStartOffsetOfNextWord(&cmd_buf[2], length -2);
            numCharsNextParam = getNumBytesNextWord(&cmd_buf[startIndexOfParam], length - startIndexOfParam);
            heater_number = atoi((const char *)(&cmd_buf[startIndexOfParam]));

            startIndexOfParam = startIndexOfParam + numCharsNextParam;
            startIndexOfParam = startIndexOfParam + getStartOffsetOfNextWord(&cmd_buf[startIndexOfParam], length -startIndexOfParam);
            target_temperature = atoi((const char *)(&cmd_buf[startIndexOfParam]));

            if(true == dev_heater_set_target_temperature(heater_number, target_temperature))
            {
                debug_line(STR("heater %d is set to temperature  %d.%01d°C !"), heater_number, target_temperature/10, target_temperature%10);
            }
            else
            {
                debug_line(STR("ERROR: could not set heater %d  to temperature  %d.%01d°C !"), heater_number, target_temperature/10, target_temperature%10);
            }
        }
            break;

// order = h
        default:
            start_order_help();
            return false;
            break;
        }
        break;

// order = l
    case 'L':
    case 'l': // list - list the available debug information
        start_order_debug_information();
        break;

// order = m
    case 'M':
    case 'm':
        if(1 == pos_in_buf)
        {
            debug_line(STR("Invalid command ! try h for help"));
            return true;
        }
        switch (cmd_buf[1])
        {
// order = md
        case 'D':
        case 'd':
        {
            uint32_t numCharsNextParam;
            uint32_t address;
            uint32_t numCharsLengthParam;
            uint32_t memoryLength;
            uint32_t startIndexOfParam;
            startIndexOfParam = 2 + getStartOffsetOfNextWord(&cmd_buf[2], length -2);
            numCharsNextParam = getNumBytesNextWord(&cmd_buf[startIndexOfParam], length - startIndexOfParam);
            address = getHexNumber(&cmd_buf[startIndexOfParam],numCharsNextParam);
            startIndexOfParam = startIndexOfParam + numCharsNextParam;
            startIndexOfParam = startIndexOfParam + getStartOffsetOfNextWord(&cmd_buf[startIndexOfParam], length -startIndexOfParam);
            numCharsLengthParam = getNumBytesNextWord(&cmd_buf[startIndexOfParam], length - startIndexOfParam);
            memoryLength = getHexNumber(&cmd_buf[startIndexOfParam], numCharsLengthParam);
            debug_line(STR("memoryLength: 0x%x address: 0x%x"), memoryLength, address);
            printMemory((uint8_t*)(uintptr_t)address, memoryLength);
            debug_line(STR(""));
        }
            break;

        default:
            debug_line(STR("Invalid command ! try h for help"));
            break;
        }
        break;

// order = o
    case 'O':
    case 'o':
        if(1 == pos_in_buf)
        {
            debug_line(STR("Invalid command ! try h for help"));
            return true;
        }
        switch (cmd_buf[1])
        {

// order = on
        case 'N':
        case 'n':
            hal_power_on_5V();
            hal_power_on_12V();
            hal_power_on_HighVoltage();
            break;

// order = of
        case 'F':
        case 'f':
            if(2 == pos_in_buf)
            {
                debug_line(STR("Invalid command ! try h for help"));
                return true;
            }
            switch(cmd_buf[2])
            {

// order = off
            case 'F':
            case 'f':
                hal_power_off_5V();
                hal_power_off_12V();
                hal_power_off_HighVoltage();
                break;

            default:
                debug_line(STR("Invalid command ! try h for help"));
                break;
            }
            break;
        default:
            debug_line(STR("Invalid command ! try h for help"));
            break;
        }
        break;

// order = p
    case 'P':
    case 'p': // print configuration
        if(1 == pos_in_buf)
        {
            debug_line(STR("Invalid command ! try h for help"));
            return true;
        }
        switch (cmd_buf[1])
        {
// order = pa
        case 'A':
        case 'a':
            hal_print_configuration_adc();

            break;
#ifdef HAS_USB
// order = pb
        case 'B':
        case 'b':
            hal_usb_print_configuration();
            break;
#endif
// order = pc
        case 'C':
        case 'c':
            hal_cpu_print_Interrupt_information();
            break;
// order = pi
        case 'I':
        case 'i':
            if(2 == pos_in_buf)
            {
#ifdef HAS_I2C
                hal_print_i2c_configuration();
#else
                debug_line(STR("Invalid command ! try h for help"));
                return true;
#endif
            }
            switch(cmd_buf[2])
            {

// order = pin
            case 'N':
            case 'n':
                hal_din_print_PinConfiguration(cmd_buf[3], atoi((char *)&(cmd_buf[4])));
                break;

            default:
                debug_line(STR("Invalid command ! try h for help"));
                break;
            }
            break;

// order = pp
#ifdef HAS_TRINAMIC
#else
        case 'P':
        case 'p':
            pololu_print_status();
            break;
#endif

// order = ps
        case 'S':
        case 's':
            if(2 == pos_in_buf)
            {
                debug_line(STR("Invalid command ! try h for help"));
                return true;
            }
            switch(cmd_buf[2])
            {
// order = pss
#ifdef STEPPER_SPI
            case 'S':
            case 's':
                hal_print_stepper_spi_configuration();
                break;
#endif

// order = pse
#ifdef EXPANSION_SPI
            case 'E':
            case 'e':
                hal_print_expansion_spi_configuration();
                break;
#endif

            default:
                debug_line(STR("Invalid command ! try h for help"));
                break;
            }
            break;

// order = pt
        case 'T':
        case 't':
            if(2 == pos_in_buf)
            {
                debug_line(STR("Invalid command ! try h for help"));
                return true;
            }
            switch(cmd_buf[2])
            {

// order = pti
            case 'I':
            case 'i':
                if(3 == pos_in_buf)
                {
                    debug_line(STR("Invalid command ! try h for help"));
                    return true;
                }
                switch(cmd_buf[3])
                {
// order = ptim
                case 'M':
                case 'm':
                    start_order_timer_config(atoi((char *)&(cmd_buf[4])));
                    return false;
                    break;

                default:
                    debug_line(STR("Invalid command ! try h for help"));
                    break;
                }
                break;
// order = ptr
            case 'R':
            case 'r':
                if(3 == pos_in_buf)
                {
                    debug_line(STR("Invalid command ! try h for help"));
                    return true;
                }
                switch(cmd_buf[3])
                {
#ifdef HAS_TRINAMIC
// order = ptri
                case 'I':
                case 'i':
                    trinamic_print_stepper_status();
                    break;
#endif
                default:
                    debug_line(STR("Invalid command ! try h for help"));
                    break;
                }
                break;

            default:
                debug_line(STR("Invalid command ! try h for help"));
                break;
            }
            break;

// order = pu
        case 'U':
        case 'u':
            if(2 == pos_in_buf)
            {
                debug_line(STR("Invalid command ! try h for help"));
                return true;
            }
            switch(cmd_buf[2])
            {

// order = pud
#ifdef DEBUG_UART
            case 'D':
            case 'd':
                hal_print_configuration_debug_uart();
                break;
#endif

// order = pug
#ifdef GCODE_UART
            case 'G':
            case 'g':
                hal_print_configuration_gcode_uart();
                break;
#endif

            default:
                debug_line(STR("Invalid command ! try h for help"));
                break;
            }
            break;

// order = pq
        case 'Q':
        case 'q':
            cmd_queue_show_status(&(cmd_buf[2]));
            break;

        default:
            debug_line(STR("Invalid command ! try h for help"));
            break;
        }
        break;

// order = w
    case 'W':
    case 'w': // write data
    {
        if(2 > (length - 2))
        {
            debug_line(STR("Invalid command ! try h for help"));
            break;
        }
        else
        {
            uint8_t receive_data[(length -2)/2];
            uint8_t send_data[(length -2)/2];
            int i;
            if(1 == pos_in_buf)
            {
                debug_line(STR("Invalid command ! try h for help"));
                return true;
            }
            switch (cmd_buf[1])
            {

// order = ws
#ifdef STEPPER_SPI
            case 'S':
            case 's':
                for (i = 0; i < (length -2)/2; i++)
                {
                    send_data[i] =  hexstr2byte(cmd_buf[2 + (i*2)], cmd_buf[2 + (i*2) + 1]);
                }
                if(false == hal_do_stepper_spi_transaction(&send_data[0], (length - 2)/2, &receive_data[0]))
                {
                    debug_line(STR("ERROR: Did not receive all bytes !"));
                }
                // else OK
                debug_msg(STR("Received: 0x"));
                debug_hex_buffer(&receive_data[0], (length -2)/2);
                debug_line(STR("Done."));
                break;
#endif

// order = we
#ifdef EXPANSION_SPI
            case 'E':
            case 'e':
                for (i = 0; i < (length -2)/2; i++)
                {
                    send_data[i] =  hexstr2byte(cmd_buf[2 + (i*2)], cmd_buf[2 + (i*2) + 1]);
                }
                if(false == hal_do_expansion_spi_transaction(&send_data[0], (length - 2)/2, &receive_data[0]))
                {
                    debug_line(STR("ERROR: Did not receive all bytes !"));
                }
                // else OK
                debug_msg(STR("Received: 0x"));
                debug_hex_buffer(&receive_data[0], (length -2)/2);
                debug_line(STR("Done."));
                break;
#endif

            default:
                debug_line(STR("Invalid command ! try h for help"));
                break;
            }
            break;
        }
    }

// order = r
    case 'R':
    case 'r': // reset the CPU
        hal_cpu_do_software_reset(RESET_REASON_DEBUG_USER_REQUEST);
        break;

// order = s
    case 'S':
    case 's': // stepper
        if(1 == pos_in_buf)
        {
            debug_line(STR("Invalid command ! try h for help"));
            return true;
        }
        switch (cmd_buf[1])
        {
#ifdef HAS_TRINAMIC
// order = sc
        case 'C':
        case 'c':
            // scan for number of Steppers
            trinamic_init(); // make sure that the SPI Interface to the Trinamic chips is initialized.
            dev_stepper_detectSteppers();  // re scan steppers
            debug_line(STR("Detected %d Steppers !"), dev_stepper_get_count());
            break;
#endif

        default:
            debug_line(STR("Invalid command ! try h for help"));
            break;
        }
        break;

// order = t
    case 'T':
    case 't': // show current time
        order_curTime();
        break;

// order = x
    case 'X':
    case 'x': // current testing
        curTest(atoi((char *)&(cmd_buf[1])));
        break;

    default: // invalid command
        debug_line(STR("Invalid command ! try h for help"));
        break;
    }
    return true;
}

void  __attribute__((weak)) curTest(int value)
{
    debug_line(STR("Found Value %d !"), value);
    debug_line(STR("Currently no Test active !!"));
}

