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
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/wdt.h>
#include "stddef.h"
#include "stdbool.h"
#include "hal_cfg.h"
#include "hal_cpu.h"
#include "hal_debug.h"
#include "hal_led.h"
#include "board_cfg.h"

#define MAX_TICK_FUNC         10


struct tick_node {
    msTickFkt tick;
    int cycle;
    int nextCall;
};
typedef struct tick_node tick_entry;

static tick_entry  tick_list[MAX_TICK_FUNC];
static volatile uint32_t now = 0;
static uint32_t lastTickAt = 0;

void hal_cpu_init_hal(void)
{
    int i;
    // Initialize the variables
    for(i=0; i < MAX_TICK_FUNC; i++)
    {
        tick_list[i].tick = NULL;
    }

    // power on everything
    PRR0 = 0;
    PRR1 = 0;

    // start time
    now = 0;
    // Start time base timer
    CPU_MS_TIMER_OCRA  = CPU_MS_TIMER_RELOAD_VALUE;
    CPU_MS_TIMER_TIMSK = 0x02;
    CPU_MS_TIMER_TCCRA = 0;
    CPU_MS_TIMER_TCCRC = 0;
    CPU_MS_TIMER_TCNT  = 0;
    CPU_MS_TIMER_TCCRB = 0x09; // No prescaler - CTC
}

ISR(TIMER1_COMPA_vect, ISR_BLOCK)
{
    hal_set_isr1_led(true);
    now++;
    hal_set_isr1_led(false);
}

ISR(TIMER1_OVF_vect, ISR_BLOCK)
{
    // Should not happen, right?
}

ISR(TIMER1_CAPT_vect, ISR_BLOCK)
{
    // Should not happen, right?
}

ISR(BADISR_vect, ISR_BLOCK)
{
    debug_line(STR("Interrupt occurred with no ISR !"));
}

uint32_t hal_cpu_get_ms_tick(void)
{
    return now;
}

void hal_cpu_tick(void)
{
    // this gets called from the main loop.
    // the main loop should spin multiple times each millisecond.
    uint32_t curTick = hal_cpu_get_ms_tick();
    if(curTick != lastTickAt)
    {
        int i;
        for(i = 0; i < MAX_TICK_FUNC; i++)
        {
            tick_list[i].nextCall--;
            if(1 > tick_list[i].nextCall)
            {
                if(NULL != tick_list[i].tick)
                {
                    // Execute that function
                    (*tick_list[i].tick)();
                }
                tick_list[i].nextCall = tick_list[i].cycle;
            }
        }
        // for safety here:
        // If the tick functions take more than ms then we rather skip a ms than
        // to never stop ticking anymore.
        lastTickAt = curTick;
    }
}

void hal_cpu_die(void)
{
    debug_line(STR("not implemented!"));
}

void hal_cpu_do_software_reset(uint32_t reason)
{
    wdt_enable(WDTO_15MS);
    for(;;)
    {
        // wait for watchdog to expire
    }
}

void hal_cpu_remove_ms_tick_function(msTickFkt function_to_remove)
{
    if(NULL != function_to_remove)
    {
        int i;
        // Check if this function is new,...
        for(i = 0; i < MAX_TICK_FUNC; i++)
        {
            if(function_to_remove == tick_list[i].tick)
            {
                // Function already in list
                tick_list[i].tick = NULL;
                return;
            }
            // else check next slot
        }
    }
}

void hal_cpu_add_ms_tick_function(msTickFkt additional_function)
{
    hal_cpu_add_ms_tick_function_cycle(additional_function, 1);
}

void hal_cpu_add_ms_tick_function_cycle(msTickFkt additional_function, int everyMs)
{
    if(NULL != additional_function)
    {
        int i;
        // Check if this function is new,...
        for(i = 0; i < MAX_TICK_FUNC; i++)
        {
            if(additional_function == tick_list[i].tick)
            {
                // Function already in list
                 debug_line(STR("INFO: Tried to double add a Tick Function !"));
                return;
            }
            // else check next slot
        }
        // a new function so add to list
        for(i = 0; i < MAX_TICK_FUNC; i++)
        {
            if(NULL == tick_list[i].tick)
            {
                // No tick in this Entry
                tick_list[i].tick = additional_function;
                tick_list[i].cycle = everyMs;
                tick_list[i].nextCall = 1;
                return;
            }
            // else check next slot
        }
        // no more free slots
        debug_line(STR("ERROR: Could not add ms Tick Function!"));
        hal_cpu_report_issue(7);
    }
}

void hal_cpu_check_Reset_Reason(void)
{
    if(0 == MCUSR)
    {
        debug_line(STR("Reset: no reason"));
        return;
    }
    if(0 != (MCUSR & 0x01))
    {
        debug_line(STR("Reset: Power on"));
    }
    if(0 != (MCUSR & 0x02))
    {
        debug_line(STR("Reset: External"));
    }
    if(0 != (MCUSR & 0x04))
    {
        debug_line(STR("Reset: Brown out"));
    }
    if(0 != (MCUSR & 0x08))
    {
        debug_line(STR("Reset: Watchdog"));
    }
    if(0 != (MCUSR & 0x10))
    {
        debug_line(STR("Reset: JTAG"));
    }
}

void hal_cpu_print_Interrupt_information(void)
{

}

void hal_cpu_report_issue(uint32_t issue_number)
{

}
