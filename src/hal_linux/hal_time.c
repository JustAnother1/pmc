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
#include "time_base.h"


static void* ms_timer_task(void * arg);

pthread_t ms_timer_thread;

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
        time_base_ISR();
    }
    return NULL;
}

