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

#include "hal_usb_device_cdc.h"

void hal_usb_device_cdc_disconnect(void)
{

}

bool hal_usb_device_cdc_init(cdc_call_back_api_typ* client)
{
	// TODO
	(void) client;
    return false;
}

void hal_usb_device_cdc_send_data(uint8_t * data, uint_fast16_t length)
{
	// TODO
	(void) data;
	(void) length;
}

void hal_usb_print_configuration(void)
{

}

