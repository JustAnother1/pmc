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
#include <ctype.h>
#include <stdarg.h>
#include "error.h"
#include "debug.h"
#include "hal_adc.h"
#include "hal_cfg.h"
#include "hal_cpu.h"
#include "hal_debug.h"
#include "hal_din.h"
#include "hal_i2c.h"
#include "hal_spi.h"
#include "hal_time.h"
#include "hal_uart.h"
#include "hal_usb_device_cdc.h"
#include "command_queue.h"
#include "device_buzzer.h"
#include "device_stepper.h"
#include "device_temperature_sensor.h"
#include "trinamic.h"



// ticks per millisecond
static uint_fast32_t tick_cnt;
static uint_fast32_t tick_value;
static uint_fast32_t tick_max;
static uint_fast32_t tick_min;

// order parsing
static uint_fast16_t checked_bytes = 0;
static uint_fast8_t last_line_end = 'l'; // invalid value

static void count_debug_ticks_per_ms(void);
static void search_for_orders(void);
static void parse_order(int length);
void debug_hex_buffer(uint8_t* buf, int length);
static uint_fast8_t hexstr2byte(uint8_t high, uint8_t low);
static void order_help(void);
static void order_curTime(void);
static uint32_t getStartOffsetOfNextWord(uint8_t* buf, uint32_t length);
static uint32_t getNumBytesNextWord(uint8_t* buf, uint32_t length);
static uint32_t getHexNumber(uint8_t* buf, uint32_t length);
static void printMemory(uint8_t* buf, uint32_t length);
static uint_fast8_t hexChar2int(uint8_t c);

void curTest(int value);

void debug_init(void)
{
    // Tick counting
    tick_cnt = 0;
    tick_value = 0;
    tick_max = 0;
    tick_min = 42424242;
    hal_debug_init();
    hal_cpu_check_Reset_Reason();
    debug_line("Debug Console PMC");
    debug_msg("(db)");
}

void debug_tick(void)
{
    // do some statistics / supervision tasks
    count_debug_ticks_per_ms();
    // check if we received an order
    search_for_orders();
}

static void count_debug_ticks_per_ms(void)
{
    uint_fast32_t time =  hal_cpu_get_ms_tick();
    if(tick_value == time)
    {
        // another tick in this ms
        tick_cnt++;
    }
    else
    {
        if(0 != tick_value)
        {
            // record the statistics
            if(tick_cnt > tick_max)
            {
                tick_max = tick_cnt;
                // debug_line("New tick max: %d\n", tick_max);
            }
            if(tick_cnt < tick_min)
            {
                tick_min = tick_cnt;
                // debug_line("New tick min: %d\n", tick_min);
            }
        }
        tick_value = time;
        tick_cnt = 0;
    }
}

static void search_for_orders(void)
{
    int i;
    uint_fast16_t num_bytes_received = hal_get_available_bytes_debug_uart();
    if(checked_bytes == num_bytes_received)
    {
        // we did a check on this number of Bytes and had no order
        return;
    }
    // debug_line("Received %d bytes\n", num_bytes_received);
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
                    debug_msg("(db)");
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
            // debug_line("found line feed !");
            debug_line("\r\n");
            parse_order(i);
            hal_forget_bytes_debug_uart(i+1);
            checked_bytes = 0;
            debug_msg("(db)");
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
        debug_msg("%02x ", buf[i]);
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

static void order_help(void)
{
    debug_line("available commands:");
    // a
    debug_line("b<frequency>               : activate buzzer (freq=0 -> off)");
    debug_line("c<setting>                 : change special setting");
    debug_line("d                          : die - stops the processor");
    // e
    // f
    // g
    debug_line("h                          : print this information");
    // i
    // j
    // k
    debug_line("l                          : list recorded debug information");
    debug_line("md<addressHex> <lengthHex> : print memory");
    // n
    // o
    debug_line("pa                         : print ADC configuration");
#ifdef HAS_USB
    debug_line("pb                         : print USB configuration");
#endif
    debug_line("pc                         : print CPU configuration");
    debug_line("pi                         : print I2C configuration");
    debug_line("pin<Port,idx>              : print state of the pin");
    debug_line("pse                        : print expansion SPI configuration");
    debug_line("pss                        : print stepper SPI configuration");
    debug_line("ptim<num>                  : print Timer Registers");
#ifdef USE_STEP_DIR
    debug_line("ptri                       : print Trinamic status");
#endif
    debug_line("pud                        : print Debug UART configuration");
    debug_line("pug                        : print G-Code UART configuration");
    // q
    debug_line("r                          : reset the processor");
    // s
    debug_line("sc                         : scan number of steppers");
    debug_line("t                          : show current time");
    // u
    // v
    debug_line("we<hex chars>              : write data to expansion SPI");
    debug_line("ws<hex chars>              : write data to stepper SPI");
    // x
    // y
    // z
    // 0..9, !, ?, ...
}

static void order_curTime(void)
{
    uint32_t millis = 0;
    uint32_t seconds = 0;
    uint32_t minutes = 0;
    uint32_t hours = 0;
    uint32_t now =  hal_cpu_get_ms_tick();
    debug_line("now : %d", now);
    if(now < 1000)
    {
        debug_line("%d ms", now);
    }
    else
    {
        millis = now %1000;
        seconds = now / 1000;
        if(seconds < 60)
        {
            debug_line("%d,%03d s", seconds, millis);
        }
        else
        {
            now = seconds;
            seconds = seconds % 60;
            minutes = now / 60;
            if(minutes < 60)
            {
                debug_line("%d:%02d,%03d mm:ss",minutes, seconds, millis);
            }
            else
            {
                now = minutes;
                minutes = minutes % 60;
                hours = now / 60;
                debug_line("%d:%02d:%02d,%03d hh:mm:ss", hours, minutes, seconds, millis);
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
        debug_line("Address(hex) : Data in Hex                                     : Data in ASCII");
    }
    while(0 < length)
    {
        uint_fast8_t i;
        uint_fast8_t bytesInRow;
        uint32_t addr = (uint32_t) buf;
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
        debug_msg("    %02x%02x%02x%02x : ", (addr>>24) & 0xff, (addr>>16) & 0xff, (addr>>8) & 0xff, addr & 0xff);

        //  00 00 00 00  00 00 00 00  00 00 00 00  00 00 00 00  :
        for(i = 0; i < bytesInRow; i++)
        {
            debug_msg("%02x ", *(buf + i));
        }
        for( ; i < 16; i++)
        {
            debug_msg("   ");
        }
        //  ....affe....beef
        debug_msg(": ");
        for(i = 0; i < bytesInRow; i++)
        {
            if(isalpha(*(buf + i)))
            {
                debug_printChar(*(buf + i));
            }
            else
            {
                debug_msg(".");
            }
        }
        for( ; i < 16; i++)
        {
            debug_msg(" ");
        }
        debug_msg("\r\n");
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
    int i;
    for(i = 0; i < length; i++)
    {
        res = res << 4;
        res = res + hexChar2int(*buf);
        buf++;
    }
    return res;
}

static void parse_order(int length)
{
    uint8_t cmd_buf[30] = {0};
    int pos_in_buf = 0;
    pos_in_buf = get_next_word(pos_in_buf, length, &cmd_buf[0]);
    if(0 == pos_in_buf)
    {
        debug_line("Invalid command ! try h for help");
        return;
    }
    switch(cmd_buf[0])
    {

    case 'B':
    case 'b':
        dev_buzzer_set_frequency(0, atoi((char *)&(cmd_buf[1])));
        break;

    case 'C':
    case 'c':
        if(1 == pos_in_buf)
        {
            debug_line("Invalid command ! try h for help");
            return;
        }
        switch (cmd_buf[1])
        {

        case 'T':
        case 't':
            // Trinamic (src/stepper/trinamic.c):
            if(false == trinamic_change_setting(&cmd_buf[2]))
            {
                debug_line("Invalid command ! try h for help");
            }
            // else -> OK
            break;

        case 'Q':
        case 'q':
            // Command Queue (src/order/commandqueue.c):
            if(false == cmd_queue_chnage_setting(&cmd_buf[2]))
            {
                debug_line("Invalid command ! try h for help");
            }
            // else -> OK
            break;

        default:
            debug_line("Invalid command ! try h for help");
            break;
        }
        break;

    case 'D':
    case 'd': // die
        hal_cpu_die();
        break;

    case 'H':
    case 'h': // help - list available commands
        order_help();
        break;

    case 'L':
    case 'l': // list - list the available debug information
        debug_line("current status:");
        debug_line("ticks per ms: max=%d, min=%d", tick_max, tick_min);
        debug_line("number of detected steppers: %d", dev_stepper_get_count());
        dev_temperature_sensor_print_status();
        break;

    case 'M':
    case 'm':
        if(1 == pos_in_buf)
        {
            debug_line("Invalid command ! try h for help");
            return;
        }
        switch (cmd_buf[1])
        {
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
            debug_line("memoryLength: 0x%x address: 0x%x", memoryLength, address);
            printMemory((uint8_t*)address, memoryLength);
            debug_line("");
        }
            break;

        default:
            debug_line("Invalid command ! try h for help");
            break;
        }
        break;

    case 'P':
    case 'p': // print configuration
        if(1 == pos_in_buf)
        {
            debug_line("Invalid command ! try h for help");
            return;
        }
        switch (cmd_buf[1])
        {
        case 'A':
        case 'a':
            hal_print_configuration_adc();

            break;
#ifdef HAS_USB
        case 'B':
        case 'b':
            hal_usb_print_configuration();
            break;
#endif

        case 'C':
        case 'c':
            hal_cpu_print_Interrupt_information();
            break;

        case 'I':
        case 'i':
            if(2 == pos_in_buf)
            {
                hal_print_i2c_configuration();
            }
            switch(cmd_buf[2])
            {
            case 'N':
            case 'n':
                hal_din_print_PinConfiguration(cmd_buf[3], atoi((char *)&(cmd_buf[4])));
                break;

            default:
                debug_line("Invalid command ! try h for help");
                break;
            }
            break;

        case 'S':
        case 's':
            if(2 == pos_in_buf)
            {
                debug_line("Invalid command ! try h for help");
                return;
            }
            switch(cmd_buf[2])
            {
            case 'S':
            case 's':
                hal_print_stepper_spi_configuration();
                break;

            case 'E':
            case 'e':
                hal_print_expansion_spi_configuration();
                break;

            default:
                debug_line("Invalid command ! try h for help");
                break;
            }
            break;
#ifdef USE_STEP_DIR
        case 'T':
        case 't':
            if(2 == pos_in_buf)
            {
                debug_line("Invalid command ! try h for help");
                return;
            }
            switch(cmd_buf[2])
            {
            case 'I':
            case 'i':
                if(3 == pos_in_buf)
                {
                    debug_line("Invalid command ! try h for help");
                    return;
                }
                switch(cmd_buf[3])
                {
                case 'M':
                case 'm':
                    hal_time_print_Configuration(atoi((char *)&(cmd_buf[4])));
                    break;

                default:
                    debug_line("Invalid command ! try h for help");
                    break;
                }
                break;

            case 'R':
            case 'r':
                if(3 == pos_in_buf)
                {
                    debug_line("Invalid command ! try h for help");
                    return;
                }
                switch(cmd_buf[3])
                {
                case 'I':
                case 'i':
                    trinamic_print_stepper_status();
                    break;

                default:
                    debug_line("Invalid command ! try h for help");
                    break;
                }
                break;

            default:
                debug_line("Invalid command ! try h for help");
                break;
            }
            break;
#endif
        case 'U':
        case 'u':
            if(2 == pos_in_buf)
            {
                debug_line("Invalid command ! try h for help");
                return;
            }
            switch(cmd_buf[2])
            {
            case 'D':
            case 'd':
                hal_print_configuration_debug_uart();
                break;

            case 'G':
            case 'g':
                hal_print_configuration_gcode_uart();
                break;

            default:
                debug_line("Invalid command ! try h for help");
                break;
            }
            break;

        default:
            debug_line("Invalid command ! try h for help");
            break;
        }
        break;

    case 'W':
    case 'w': // write data
    {
        if(2 > (length - 2))
        {
            debug_line("Invalid command ! try h for help");
            break;
        }
        else
        {
            uint8_t receive_data[(length -2)/2];
            uint8_t send_data[(length -2)/2];
            int i;
            if(1 == pos_in_buf)
            {
                debug_line("Invalid command ! try h for help");
                return;
            }
            switch (cmd_buf[1])
            {
            case 'S':
            case 's':
                for (i = 0; i < (length -2)/2; i++)
                {
                    send_data[i] =  hexstr2byte(cmd_buf[2 + (i*2)], cmd_buf[2 + (i*2) + 1]);
                }
                if(false == hal_do_stepper_spi_transaction(&send_data[0], (length - 2)/2, &receive_data[0]))
                {
                    debug_line("ERROR: Did not receive all bytes !");
                }
                // else OK
                debug_msg("Received: 0x");
                debug_hex_buffer(&receive_data[0], (length -2)/2);
                debug_line("Done.");
                break;

            case 'E':
            case 'e':
                for (i = 0; i < (length -2)/2; i++)
                {
                    send_data[i] =  hexstr2byte(cmd_buf[2 + (i*2)], cmd_buf[2 + (i*2) + 1]);
                }
                if(false == hal_do_expansion_spi_transaction(&send_data[0], (length - 2)/2, &receive_data[0]))
                {
                    debug_line("ERROR: Did not receive all bytes !");
                }
                // else OK
                debug_msg("Received: 0x");
                debug_hex_buffer(&receive_data[0], (length -2)/2);
                debug_line("Done.");
                break;

            default:
                debug_line("Invalid command ! try h for help");
                break;
            }
            break;
        }
    }

    case 'R':
    case 'r': // reset the CPU
        hal_cpu_do_software_reset(RESET_REASON_DEBUG_USER_REQUEST);
        break;

    case 'S':
    case 's': // stepper
        if(1 == pos_in_buf)
        {
            debug_line("Invalid command ! try h for help");
            return;
        }
        switch (cmd_buf[1])
        {
        case 'C':
        case 'c':
            // scan for number of Steppers
            debug_line("Detected %d Steppers !", trinamic_detect_number_of_steppers());
            break;

        default:
            debug_line("Invalid command ! try h for help");
            break;
        }
        break;

    case 'T':
    case 't': // show current time
        order_curTime();
        break;

    case 'X':
    case 'x': // current testing
        curTest(atoi((char *)&(cmd_buf[1])));
        break;

    default: // invalid command
        debug_line("Invalid command ! try h for help");
        break;
    }
}

void  __attribute__((weak)) curTest(int value)
{
    debug_line("Found Value %d !", value);
    debug_line("Currently no Test is active !!");
}

