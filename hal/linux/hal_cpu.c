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
#include <unistd.h>
#include "stddef.h"
#include "pthread.h"
#include "hal_cpu.h"
#include "hal_debug.h"

pthread_t ms_timer_thread;
static volatile uint32_t now = 0;

static void* ms_timer_task(void * arg);

void hal_cpu_init_hal(void)
{
    int ret = pthread_create( &ms_timer_thread, NULL, ms_timer_task, NULL);
    if(ret)
    {
        fprintf(stderr,"Error - pthread_create() return code: %d\n",ret);
        exit(1);
    }
}

void hal_cpu_die(void)
{
    exit(1);
}

void hal_cpu_do_software_reset(uint32_t reason)
{
    debug_line("not implemented!");
}

void hal_cpu_add_ms_tick_function(msTickFkt additional_function)
{

}

void hal_cpu_add_ms_tick_function_cycle(msTickFkt additional_function, int everyMs)
{

}

void hal_cpu_tick(void)
{
    // do not use the whole CPU
    usleep(100);
}

static void* ms_timer_task(void * arg)
{
    for(;;)
    {
        usleep(1000);
        now++;
        // TODO
    }
    return NULL;
}

uint32_t hal_cpu_get_ms_tick(void)
{
    return now;
}

void hal_cpu_check_Reset_Reason(void)
{

}

void hal_cpu_print_Interrupt_information(void)
{

}
