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

#include "hal_watchdog.h"
#include "st_watchdog.h"

#define INDEPENDENT_WATCHDOG_USED
#define WINDOW_WATCHDOG_USED

void watchdog_init(void)
{
#ifdef INDEPENDENT_WATCHDOG_USED
    IWDG->KR = 0x5555; // unlock Registers PR and PLP
    IWDG->PR = 7;      // Prescaler = 1/256
    IWDG->RLR = 0xfff; // Reload Value
    IWDG->KR = 0xCCCC; // Start watch dog
#endif
#ifdef WINDOW_WATCHDOG_USED
    WWDG->CFR = 0x1ff; // Early wakeup interrupt, prescaler 1/8, window full open ;-)
    WWDG->CR = 0xff; // enable Window watch dog
#endif
}

void watchdog_tick(void)
{
#ifdef INDEPENDENT_WATCHDOG_USED
    IWDG->KR = 0xAAAA; // Reset timer to programmed reload value
#endif
#ifdef WINDOW_WATCHDOG_USED
    WWDG->CR = 0xff; // reset down counter
#endif
}

void WWDG_IRQHandler(void)
{
    // todo
}


