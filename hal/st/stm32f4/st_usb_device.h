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

#ifndef HAL_ST_STM32F4_ST_USB_DEVICE_H_
#define HAL_ST_STM32F4_ST_USB_DEVICE_H_

#include <stdbool.h>

#include "st_usb.h"

bool usb_device_init(USBD_Class_cb_TypeDef* usb_class_cb);
void usb_device_print_configuration(void);

uint8_t get_device_status(void);
void set_device_state(uint8_t state);
void EP_Close(uint8_t  ep_addr);
USB_OTG_EP* get_out_ep_num(uint_fast8_t num);

void EP_Open(uint8_t  ep_addr,
             uint16_t ep_mps,
             uint8_t  ep_type);

void EP_PrepareRx(uint8_t  ep_addr,
                  uint8_t* pbuf,
                  uint16_t buf_len);

void usb_CtlSendData(uint8_t *pbuf, uint16_t len);
void usb_CtlError(USB_SETUP_REQ *req);
void EP_Tx(uint8_t  ep_addr,
           uint8_t* pbuf,
           uint32_t buf_len);
void usb_EPDeactivate(USB_OTG_EP *ep);

#endif /* HAL_ST_STM32F4_ST_USB_DEVICE_H_ */
