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
#include "hal_time.h"


static void* ms_timer_task(void * arg);

pthread_t ms_timer_thread;
static volatile uint32_t now = 0;

void hal_time_init(void)
{
    int ret = pthread_create( &ms_timer_thread, NULL, ms_timer_task, NULL);
    if(ret)
    {
        fprintf(stderr,"Error - pthread_create() return code: %d\n",ret);
        exit(1);
    }
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

void hal_time_ms_sleep(uint_fast32_t ms)
{
    uint32_t i;
    for(i = 0; i < ms; i++)
    {
        usleep(1000);
    }
}
uint32_t hal_time_get_ms_tick(void)
{
    return now;
}

bool hal_time_start_timer(uint_fast8_t device, uint32_t clock, uint_fast16_t reload_value, TimerFkt function)
{
    // TODO
    return false;
}
void hal_time_stop_timer(uint_fast8_t device)
{

}

bool hal_time_set_timer_reload(uint_fast8_t device, uint16_t reload_value)
{
    return false;
}

void hal_time_print_Configuration(int timerNumber)
{

}

