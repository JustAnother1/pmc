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

#include <stdbool.h>
#include "error.h"
#include "com.h"
#include "hal_cpu.h"
#include "hal_led.h"
#include "hal_cfg.h"
#include "hal_usb_device_cdc.h"

void error_fatal_error(char* msg)
{
    uint_fast8_t i = com_copy_string_to_parameter(msg, com_get_start_parameter());
    com_send_debug_frame_with_filled_parameter(i);
    error_signal_error_and_die();
}

void error_signal_error_and_die(void)
{
    hal_led_set_led(DEBUG_LED, true);
#ifdef HAS_USB
    hal_usb_device_disconnect();
#endif
    hal_cpu_die();
}

