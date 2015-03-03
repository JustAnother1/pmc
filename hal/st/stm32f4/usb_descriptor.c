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
#include "usb_descriptor.h"
#include "usb.h"
#include <stddef.h>
#include "usb_device.h"
#include "usb_std.h"

#define USB_MANUFACTURER_STRING                  "nomagic.de"
#define USB_PRODUCT_FS_STRING                    "Virtual ComPort"
#define USB_SERIALNUMBER_FS_STRING               "000008154711"
#define USB_CONFIGURATION_FS_STRING              "VCP Config"
#define USB_INTERFACE_FS_STRING                  "VCP Interface"

#define USB_VID                                  0x0483
#define USB_PID                                  0x7270
#define LANGID_STRING                            0x409
#define MAX_STR_DESC_SIZ                         50

static void GetString(uint8_t* desc, uint8_t* unicode, uint16_t* len);
static uint8_t GetLen(uint8_t *buf);
static uint8_t* GetDeviceDescriptor(uint16_t* length);
static uint8_t* GetLangIDStrDescriptor(uint16_t* length);
static uint8_t* GetProductStrDescriptor(uint16_t* length);
static uint8_t* GetManufacturerStrDescriptor(uint16_t* length);
static uint8_t* GetSerialStrDescriptor(uint16_t* length);
static uint8_t* GetConfigStrDescriptor(uint16_t* length);
static uint8_t* GetInterfaceStrDescriptor(uint16_t* length);


/* USB Standard Device Descriptor */
static uint8_t device_descriptor[18] __attribute__ ((aligned (4))) =
{
    0x12,                       /*bLength */
    USB_DEVICE_DESCRIPTOR_TYPE, /*bDescriptorType*/
    0x00,                       /*bcdUSB */
    0x02,
    0x00,                       /*bDeviceClass*/
    0x00,                       /*bDeviceSubClass*/
    0x00,                       /*bDeviceProtocol*/
    USB_OTG_MAX_EP0_SIZE,      /*bMaxPacketSize*/
    LOBYTE(USB_VID),           /*idVendor*/
    HIBYTE(USB_VID),           /*idVendor*/
    LOBYTE(USB_PID),           /*idVendor*/
    HIBYTE(USB_PID),           /*idVendor*/
    0x00,                       /*bcdDevice rel. 2.00*/
    0x02,
    USB_IDX_MFC_STR,           /*Index of manufacturer  string*/
    USB_IDX_PRODUCT_STR,       /*Index of product string*/
    USB_IDX_SERIAL_STR,        /*Index of serial number string*/
    USB_CFG_MAX_NUM            /*bNumConfigurations*/
}; /* USB_DeviceDescriptor */

/* USB Standard Device Descriptor */
static uint8_t language_id_descriptor[4] __attribute__ ((aligned (4))) =
{
    4,
    USB_DESC_TYPE_STRING,
    LOBYTE(LANGID_STRING),
    HIBYTE(LANGID_STRING),
};

uint8_t string_descriptor[MAX_STR_DESC_SIZ] __attribute__ ((aligned (4))) ;

/*
*         Handle Get Descriptor requests
*/
void GetDescriptor(USB_SETUP_REQ *req)
{
    uint16_t len;
    uint8_t *pbuf;

    switch (req->wValue >> 8)
    {
    case USB_DESC_TYPE_DEVICE:
        pbuf = GetDeviceDescriptor(&len);
        if((req->wLength == 64) || ( get_device_status() == USB_OTG_DEFAULT))
        {
            len = 8;
        }
        break;

    // case USB_DESC_TYPE_CONFIGURATION already handled by caller

    case USB_DESC_TYPE_STRING:
        switch ((uint8_t)(req->wValue))
        {
        case USB_IDX_LANGID_STR:
            pbuf = GetLangIDStrDescriptor(&len);
            break;

        case USB_IDX_MFC_STR:
            pbuf = GetManufacturerStrDescriptor(&len);
            break;

        case USB_IDX_PRODUCT_STR:
            pbuf = GetProductStrDescriptor(&len);
            break;

        case USB_IDX_SERIAL_STR:
            pbuf = GetSerialStrDescriptor(&len);
            break;

        case USB_IDX_CONFIG_STR:
            pbuf = GetConfigStrDescriptor(&len);
            break;

        case USB_IDX_INTERFACE_STR:
            pbuf = GetInterfaceStrDescriptor(&len);
            break;

        default:
            usb_CtlError(req);
            return;
        }
        break;
    case USB_DESC_TYPE_DEVICE_QUALIFIER:
        usb_CtlError(req);
        return;


    case USB_DESC_TYPE_OTHER_SPEED_CONFIGURATION:
        usb_CtlError(req);
        return;

    default:
        usb_CtlError(req);
        return;
    }

    if((len != 0) && (req->wLength != 0))
    {
        len = MIN(len, req->wLength);
        usb_CtlSendData(pbuf, len);
    }
}

static uint8_t* GetDeviceDescriptor(uint16_t* length)
{
    *length = sizeof(device_descriptor);
    return &device_descriptor[0];
}

static uint8_t* GetLangIDStrDescriptor(uint16_t* length)
{
    *length =  sizeof(language_id_descriptor);
    return &language_id_descriptor[0];
}

static uint8_t* GetProductStrDescriptor(uint16_t* length)
{
    GetString((uint8_t*)USB_PRODUCT_FS_STRING, string_descriptor, length);
    return &string_descriptor[0];
}

static uint8_t* GetManufacturerStrDescriptor(uint16_t* length)
{
    GetString ((uint8_t*)USB_MANUFACTURER_STRING, string_descriptor, length);
    return &string_descriptor[0];
}

static uint8_t* GetSerialStrDescriptor(uint16_t* length)
{
    GetString((uint8_t*)USB_SERIALNUMBER_FS_STRING, string_descriptor, length);
    return &string_descriptor[0];
}

static uint8_t* GetConfigStrDescriptor(uint16_t *length)
{
    GetString((uint8_t*)USB_CONFIGURATION_FS_STRING, string_descriptor, length);
    return &string_descriptor[0];
}

static uint8_t* GetInterfaceStrDescriptor(uint16_t* length)
{
    GetString((uint8_t*)USB_INTERFACE_FS_STRING, string_descriptor, length);
    return &string_descriptor[0];
}

/**
  *         Convert Ascii string into unicode one
  */
static void GetString(uint8_t *desc, uint8_t *unicode, uint16_t *len)
{
    uint8_t idx = 0;
    if (desc != NULL)
    {
        *len = GetLen(desc) * 2 + 2;
        unicode[idx++] = *len;
        unicode[idx++] = USB_DESC_TYPE_STRING;
        while(*desc != 0)
        {
            unicode[idx++] = *desc++;
            unicode[idx++] = 0x00;
        }
    }
}

/*
 * return the string length
 * buf : pointer to the ASCII string buffer
 */
static uint8_t GetLen(uint8_t *buf)
{
    uint8_t  len = 0;

    while (*buf != 0)
    {
        len++;
        buf++;
    }

    return len;
}
