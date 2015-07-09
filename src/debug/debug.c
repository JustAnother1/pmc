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
#include <stdarg.h>
#include "hal_cfg.h"
#include "hal_uart.h"
#include "hal_spi.h"
#include "hal_time.h"
#include "error.h"
#include "debug.h"
#include "hal_cpu.h"
#include "hal_debug.h"
#include "device_stepper.h"
#include "hal_usb_device_cdc.h"
#include "command_queue.h"

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
static void debug_hex_buffer(uint8_t* buf, int length);
static uint_fast8_t hexstr2byte(uint8_t high, uint8_t low);

void debug_init(void)
{
    // Tick counting
    tick_cnt = 0;
    tick_value = 0;
    tick_max = 0;
    tick_min = 42424242;
    hal_debug_init();
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
    uint_fast32_t time =  hal_time_get_ms_tick();
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
    uint_fast16_t num_bytes_received = hal_uart_get_available_bytes(DEBUG_UART);
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
        uint_fast8_t c = hal_uart_get_byte_at_offset(DEBUG_UART, i);
        if(('\r' == c) || ('\n' == c))
        {
            if(0 == i)
            {
                // either a \r\n or an empty command
                if(c == last_line_end)
                {
                    // empty command
                    hal_uart_forget_bytes(DEBUG_UART, i+1);
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
            parse_order(i);
            hal_uart_forget_bytes(DEBUG_UART, i+1);
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
        uint8_t c = hal_uart_get_byte_at_offset(DEBUG_UART, i);
        switch(c)
        {
        case '\t':
        case '\r':
            // ignore
            break;

        case ' ':
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

static void debug_hex_buffer(uint8_t* buf, int length)
{
    int i;
    for(i = 0; i < length; i++)
    {
        debug_msg("%02x ", buf[i]);
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

static void parse_order(int length)
{
    uint8_t cmd_buf[10] = {0};
    int pos_in_buf = 0;
    pos_in_buf = get_next_word(pos_in_buf, length, &cmd_buf[0]);
    switch(cmd_buf[0])
    {
    case 'H':
    case 'h': // help - list available commands
        debug_line("available commands:");
        debug_line("h               : print this information");
        debug_line("l               : list recorded debug information");
        debug_line("d               : die - stops the processor");
        debug_line("r               : reset the processor");
        debug_line("t               : show current time");
        debug_line("pu<device num>  : print UART configuration");
#ifdef HAS_USB
        debug_line("pb              : print USB configuration");
#endif
        debug_line("ps<device num>  : print SPI configuration");
        debug_line("ws<hex chars>   : write data to SPI");
        debug_line("c<setting>      : change special setting");
        break;

    case 'D':
    case 'd': // die
        hal_cpu_die();
        break;

    case 'R':
    case 'r': // reset the CPU
        hal_cpu_do_software_reset();
        break;

    case 'L':
    case 'l': // list - list the available debug information
        debug_line("current status:");
        debug_line("ticks per ms: max=%d, min=%d", tick_max, tick_min);
        debug_line("number of detected steppers: %d", dev_stepper_get_count());
        break;

    case 'T':
    case 't': // show current time
    {
        uint32_t millis = 0;
        uint32_t seconds = 0;
        uint32_t minutes = 0;
        uint32_t hours = 0;
        uint32_t now =  hal_time_get_ms_tick();
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
        break;

    case 'P':
    case 'p': // print configuration
        switch (cmd_buf[1])
        {
        case 'U':
        case 'u':
            hal_uart_print_configuration(cmd_buf[2] - '0'); // quick and dirty a2i()
            break;

        case 'S':
        case 's':
            hal_spi_print_configuration(cmd_buf[2] - '0'); // quick and dirty a2i()
            break;
#ifdef HAS_USB
        case 'B':
        case 'b':
            hal_usb_print_configuration();
            break;
#endif

        default:
            debug_line("Invalid command ! try h for help");
            break;
        }
        break;

        case 'W':
        case 'w': // write data
        {
            uint8_t receive_data[(length -2)/2];
            uint8_t send_data[(length -2)/2];
            int i;
            switch (cmd_buf[1])
            {
            case 'S':
            case 's':
                for (i = 0; i < (length -2)/2; i++)
                {
                    send_data[i] =  hexstr2byte(cmd_buf[2 + (i*2)], cmd_buf[2 + (i*2) + 1]);
                }
                if(false == hal_spi_do_transaction(STEPPER_SPI, &send_data[0], (length - 2)/2, &receive_data[0]))
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

        case 'C':
        case 'c':
            switch (cmd_buf[1])
            {
            case 'Q':
            case 'q':
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

    default: // invalid command
        debug_line("Invalid command ! try h for help");
        break;
    }
}
