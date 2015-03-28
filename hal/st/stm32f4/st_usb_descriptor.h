/*
 * usb_descriptor.h
 *
 *  Created on: 21.02.2015
 *      Author: lars
 */

#ifndef HAL_ST_STM32F4_ST_USB_DESCRIPTOR_H_
#define HAL_ST_STM32F4_ST_USB_DESCRIPTOR_H_

#include <inttypes.h>

#include "st_usb.h"

void GetDescriptor(USB_SETUP_REQ *req);

#endif /* HAL_ST_STM32F4_ST_USB_DESCRIPTOR_H_ */
