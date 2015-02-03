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

#include <inttypes.h>
#include "usb.h"
#include "hal_usb_device_cdc.h"


bool start_usb(void)
{
    return hal_usb_device_cdc_init();
}

bool usb_has_next_frame(void) // this is usb_tick()
{
    return false;
}

uint_fast8_t usb_get_parameter_byte(uint_fast8_t index)
{
    return 0;
}

uint_fast8_t usb_get_order(void)
{
    return 0;
}

uint_fast8_t usb_get_parameter_length(void)
{
    return 0;
}

uint_fast8_t usb_get_control(void)
{
    return 0;
}

void usb_send_frame(uint8_t * frame, uint_fast16_t length)
{

}

void usb_forget_frame(void)
{

}

// end of File
