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
        debug_line("ps<device num>  : print SPI configuration");
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
        debug_line("now : %d", hal_time_get_ms_tick());
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
