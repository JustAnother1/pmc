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
#include <avr/eeprom.h>
#include "stddef.h"
#include "stdbool.h"
#include "hal_cfg.h"
#include "hal_cpu.h"
#include "hal_debug.h"
#include "hal_led.h"
#include "board_cfg.h"

#define MAX_TICK_FUNC         10

// EEPROM Address
#define EEPROM_ADDRESS_RESET_REASON (uint8_t*)(0)
#define EEPROM_ADDRESS_ISSUES_START (uint8_t*)(1)

// issues
#define NUMBER_OF_ISSUES      10
#define NO_ISSUE               0


struct tick_node {
    msTickFkt tick;
    int cycle;
    int nextCall;
};
typedef struct tick_node tick_entry;

static volatile uint32_t now;
#ifdef CPU_MS_TIMER_NEEDS_CNT
    volatile uint8_t cur_time_cnt;
#endif

static tick_entry  tick_list[MAX_TICK_FUNC];
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
#ifdef CPU_MS_TIMER_NEEDS_CNT
    cur_time_cnt = 0;
#endif
    now = 0;

    // Start time base timer
    CPU_MS_TIMER_OCRA  = CPU_MS_TIMER_RELOAD_VALUE;
    CPU_MS_TIMER_TIMSK = (CPU_MS_TIMER_TIMSK | CPU_MS_TIMER_IRQ_1) & ~CPU_MS_TIMER_IRQ_0;
    CPU_MS_TIMER_TCNT  = 0;
    CPU_MS_TIMER_TCCRA = (CPU_MS_TIMER_TCCRA | CPU_MS_TIMER_TCCRA_1) &~CPU_MS_TIMER_TCCRA_0;
    CPU_MS_TIMER_TCCRB = (CPU_MS_TIMER_TCCRB | CPU_MS_TIMER_TCCRB_1) &~CPU_MS_TIMER_TCCRB_0;
}

ISR(CPU_MS_TIMER_COMPARE_ISR, ISR_BLOCK)
{
    hal_set_isr1_led(true);
#ifdef CPU_MS_TIMER_NEEDS_CNT
    cur_time_cnt++;
    if(0 == cur_time_cnt%CPU_MS_TIMER_NEEDS_CNT)
    {
#endif
        now = now + 1ul;
#ifdef CPU_MS_TIMER_NEEDS_CNT
        cur_time_cnt = 0;
    }
#endif
    hal_set_isr1_led(false);
}

ISR(BADISR_vect, ISR_BLOCK)
{
    debug_line(STR("Interrupt occurred with no ISR !"));
    hal_set_error_led(true);
}

uint32_t hal_cpu_get_ms_tick(void)
{
    return now;
}

void hal_cpu_print_Interrupt_information(void)
{
    debug_line(STR("now  : %lu"), now);
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
    eeprom_write_byte(EEPROM_ADDRESS_RESET_REASON, (uint8_t)(0xff & reason));
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
    uint8_t reset_reason;
    int i;

    // CPU Register
    if(0 == MCUSR)
    {
        debug_line(STR("Reset: no reason"));
    }
    else
    {
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
        if(0 != (MCUSR & 0xe0))
        {
            debug_line(STR("Reset: 0x%02x"), MCUSR);
        }
    }

    // reported Reset reason
    reset_reason = eeprom_read_byte(EEPROM_ADDRESS_RESET_REASON);
    debug_msg(STR("Reset reason : "));
    switch(reset_reason)
    {
    case RESET_REASON_NO_REASON          : debug_line(STR("no reason")); break;
    case RESET_REASON_HOST_ORDER         : debug_line(STR("host order")); break;
    case RESET_REASON_DEBUG_USER_REQUEST : debug_line(STR("user request on debug interface")); break;
    case RESET_REASON_HAL                : debug_line(STR("HAL")); break;
    default                              : debug_line(STR("%d"), reset_reason); break;
    }
    eeprom_write_byte(EEPROM_ADDRESS_RESET_REASON, RESET_REASON_NO_REASON);

    // reported issues
    for(i = 0; i < NUMBER_OF_ISSUES; i++)
    {
        uint8_t issue;
        issue = eeprom_read_byte(EEPROM_ADDRESS_ISSUES_START + i);
        if(NO_ISSUE == issue)
        {
            // empty slot
        }
        else
        {
            debug_line(STR("issue %d : %d"), i, issue);
            eeprom_write_byte(EEPROM_ADDRESS_ISSUES_START + i, NO_ISSUE);
        }
    }
}

uint_fast8_t hal_cpu_get_state_byte(void)
{
    return MCUSR;
}

void hal_cpu_report_issue(uint32_t issue_number)
{
    int i;
    for(i = 0; i < NUMBER_OF_ISSUES; i++)
    {
        uint8_t issue;
        issue = eeprom_read_byte(EEPROM_ADDRESS_ISSUES_START + i);
        if(NO_ISSUE == issue)
        {
            eeprom_write_byte(EEPROM_ADDRESS_ISSUES_START + i, (uint8_t)(0xff & issue_number));
            break;
        }
        // else read next position
    }
}
