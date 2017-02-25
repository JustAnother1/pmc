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

#include "hal_led.h"

void hal_init_leds(void)
{

}

#ifdef BOARD_HAS_DEBUG_LED
void hal_toggle_debug_led(void)
{

}

void hal_set_debug_led(bool on)
{

}
#endif

#ifdef BOARD_HAS_ERROR_LED
void hal_toggle_error_led(void)
{

}

void hal_set_error_led(bool on)
{

}
#endif

#ifdef BOARD_HAS_ISR_1_LED
void hal_toggle_isr1_led(void)
{

}

void hal_set_isr1_led(bool on)
{

}
#endif

#ifdef BOARD_HAS_ISR_2_LED
void hal_toggle_isr2_led(void)
{

}

void hal_set_isr2_led(bool on)
{

}
#endif
