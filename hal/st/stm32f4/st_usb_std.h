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

#ifndef HAL_ST_STM32F4_ST_USB_STD_H_
#define HAL_ST_STM32F4_ST_USB_STD_H_

// see http://usb.org

#define USB_DESC_TYPE_DEVICE                     1
#define USB_DESC_TYPE_CONFIGURATION              2
#define USB_DESC_TYPE_STRING                     3
#define USB_DESC_TYPE_INTERFACE                  4
#define USB_DESC_TYPE_ENDPOINT                   5
#define USB_DESC_TYPE_DEVICE_QUALIFIER           6
#define USB_DESC_TYPE_OTHER_SPEED_CONFIGURATION  7
#define USB_IDX_LANGID_STR                       0x00
#define USB_IDX_MFC_STR                          0x01
#define USB_IDX_PRODUCT_STR                      0x02
#define USB_IDX_SERIAL_STR                       0x03
#define USB_IDX_CONFIG_STR                       0x04
#define USB_IDX_INTERFACE_STR                    0x05
#define USB_DEVICE_DESCRIPTOR_TYPE               0x01
#define USB_CONFIGURATION_DESCRIPTOR_TYPE        0x02
#define USB_STRING_DESCRIPTOR_TYPE               0x03
#define USB_INTERFACE_DESCRIPTOR_TYPE            0x04
#define USB_ENDPOINT_DESCRIPTOR_TYPE             0x05
#define USB_DEVICE_DESCRIPTOR_TYPE               0x01
#define USB_CONFIGURATION_DESCRIPTOR_TYPE        0x02
#define USB_STRING_DESCRIPTOR_TYPE               0x03
#define USB_INTERFACE_DESCRIPTOR_TYPE            0x04
#define USB_ENDPOINT_DESCRIPTOR_TYPE             0x05
#define USB_DEVICE_DESCRIPTOR_TYPE               0x01
#define USB_CONFIGURATION_DESCRIPTOR_TYPE        0x02
#define USB_STRING_DESCRIPTOR_TYPE               0x03
#define USB_INTERFACE_DESCRIPTOR_TYPE            0x04
#define USB_ENDPOINT_DESCRIPTOR_TYPE             0x05
#define STANDARD_ENDPOINT_DESC_SIZE              0x09
#define CDC_DESCRIPTOR_TYPE                      0x21
#define DEVICE_CLASS_CDC                         0x02
#define DEVICE_SUBCLASS_CDC                      0x00

#endif /* HAL_ST_STM32F4_ST_USB_STD_H_ */
