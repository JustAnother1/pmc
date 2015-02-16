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

#ifndef HAL_INCLUDE_HAL_USB_DEVICE_CDC_H_
#define HAL_INCLUDE_HAL_USB_DEVICE_CDC_H_

#include <stdbool.h>

/// cut USB connection to host - unplug USB cable
void hal_usb_device_disconnect(void);

bool hal_usb_device_cdc_init(void);


#endif /* HAL_INCLUDE_HAL_USB_DEVICE_CDC_H_ */
