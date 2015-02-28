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
#include "rcc.h"
#include "gpio.h"
#include "board_cfg.h"
#include <stddef.h>
#include "usb.h"
#include "usb_device.h"
#include "usb_std.h"

static void usb_cdc_Init (uint8_t cfgidx);
static void usb_cdc_DeInit (uint8_t cfgidx);
static void usb_cdc_Setup (USB_SETUP_REQ *req);
static void usb_cdc_EP0_RxReady(void);
static void usb_cdc_DataIn(uint8_t epnum);
static void usb_cdc_DataOut(uint8_t epnum);
static void usb_cdc_SOF(void);
static uint8_t* usb_cdc_GetCfgDesc(uint16_t *length);
void set_config_descriptor(uint8_t* cfg_desc); // used in usb_device.c

#define USB_SIZ_STRING_LANGID                   4
#define USB_CDC_CONFIG_DESC_SIZ                 (67)
#define USB_CDC_DESC_SIZ                        (67-9)
#define CDC_DATA_IN_PACKET_SIZE                 *(uint16_t *)(pConfig_descriptor + 57)
#define CDC_DATA_OUT_PACKET_SIZE                *(uint16_t *)(pConfig_descriptor + 64)
/*
 * CDC Requests
 */
#define SEND_ENCAPSULATED_COMMAND               0x00
#define GET_ENCAPSULATED_RESPONSE               0x01
#define SET_COMM_FEATURE                        0x02
#define GET_COMM_FEATURE                        0x03
#define CLEAR_COMM_FEATURE                      0x04
#define SET_LINE_CODING                         0x20
#define GET_LINE_CODING                         0x21
#define SET_CONTROL_LINE_STATE                  0x22
#define SEND_BREAK                              0x23
#define NO_CMD                                  0xFF

#define CDC_DATA_MAX_PACKET_SIZE                64   /* Endpoint IN & OUT Packet size */
#define CDC_CMD_PACKET_SZE                      8    /* Control Endpoint Packet size */
#define CDC_IN_FRAME_INTERVAL                   5    /* Number of frames between IN transfers */
#define APP_RX_DATA_SIZE                        2048 /* Total size of IN buffer:
                                                        APP_RX_DATA_SIZE*8/MAX_BAUDARATE*1000 should be > CDC_IN_FRAME_INTERVAL */
#define USBD_ITF_MAX_NUM                        1


static uint8_t usbd_cdc_CfgDesc  [USB_CDC_CONFIG_DESC_SIZ] __attribute__ ((aligned (4))) ;
static __IO uint32_t  usbd_cdc_AltSet  __attribute__ ((aligned (4))) = 0;
static uint8_t USB_Rx_Buffer   [CDC_DATA_MAX_PACKET_SIZE] __attribute__ ((aligned (4))) ;
static uint8_t APP_Rx_Buffer   [APP_RX_DATA_SIZE] __attribute__ ((aligned (4))) ;
static uint8_t CmdBuff[CDC_CMD_PACKET_SZE] __attribute__ ((aligned (4))) ;
static uint32_t APP_Rx_ptr_in  = 0;
static uint32_t APP_Rx_ptr_out = 0;
static uint32_t APP_Rx_length  = 0;
static uint8_t  USB_Tx_State = 0;
static uint32_t cdcCmd = 0xFF;
static uint32_t cdcLen = 0;
/* CDC interface class callbacks structure */
static USBD_Class_cb_TypeDef  USBD_CDC_cb =
{
  usb_cdc_Init,
  usb_cdc_DeInit,
  usb_cdc_Setup,
  NULL,                 /* EP0_TxSent, */
  usb_cdc_EP0_RxReady,
  usb_cdc_DataIn,
  usb_cdc_DataOut,
  usb_cdc_SOF,
  NULL,
  NULL,
  usb_cdc_GetCfgDesc,
};
/* USB CDC device Configuration Descriptor */
static uint8_t usbd_cdc_CfgDesc[USB_CDC_CONFIG_DESC_SIZ]  __attribute__ ((aligned (4))) =
{
  /*Configuration Descriptor*/
  0x09,   /* bLength: Configuration Descriptor size */
  USB_CONFIGURATION_DESCRIPTOR_TYPE,      /* bDescriptorType: Configuration */
  USB_CDC_CONFIG_DESC_SIZ,                /* wTotalLength:no of returned bytes */
  0x00,
  0x02,   /* bNumInterfaces: 2 interface */
  0x01,   /* bConfigurationValue: Configuration value */
  0x00,   /* iConfiguration: Index of string descriptor describing the configuration */
  0xC0,   /* bmAttributes: self powered */
  0x32,   /* MaxPower 0 mA */

  /*---------------------------------------------------------------------------*/

  /*Interface Descriptor */
  0x09,   /* bLength: Interface Descriptor size */
  USB_INTERFACE_DESCRIPTOR_TYPE,  /* bDescriptorType: Interface */
  /* Interface descriptor type */
  0x00,   /* bInterfaceNumber: Number of Interface */
  0x00,   /* bAlternateSetting: Alternate setting */
  0x01,   /* bNumEndpoints: One endpoints used */
  0x02,   /* bInterfaceClass: Communication Interface Class */
  0x02,   /* bInterfaceSubClass: Abstract Control Model */
  0x01,   /* bInterfaceProtocol: Common AT commands */
  0x00,   /* iInterface: */

  /*Header Functional Descriptor*/
  0x05,   /* bLength: Endpoint Descriptor size */
  0x24,   /* bDescriptorType: CS_INTERFACE */
  0x00,   /* bDescriptorSubtype: Header Func Desc */
  0x10,   /* bcdCDC: spec release number */
  0x01,

  /*Call Management Functional Descriptor*/
  0x05,   /* bFunctionLength */
  0x24,   /* bDescriptorType: CS_INTERFACE */
  0x01,   /* bDescriptorSubtype: Call Management Func Desc */
  0x00,   /* bmCapabilities: D0+D1 */
  0x01,   /* bDataInterface: 1 */

  /*ACM Functional Descriptor*/
  0x04,   /* bFunctionLength */
  0x24,   /* bDescriptorType: CS_INTERFACE */
  0x02,   /* bDescriptorSubtype: Abstract Control Management desc */
  0x02,   /* bmCapabilities */

  /*Union Functional Descriptor*/
  0x05,   /* bFunctionLength */
  0x24,   /* bDescriptorType: CS_INTERFACE */
  0x06,   /* bDescriptorSubtype: Union func desc */
  0x00,   /* bMasterInterface: Communication class interface */
  0x01,   /* bSlaveInterface0: Data Class Interface */

  /*Endpoint 2 Descriptor*/
  0x07,                           /* bLength: Endpoint Descriptor size */
  USB_ENDPOINT_DESCRIPTOR_TYPE,   /* bDescriptorType: Endpoint */
  CDC_CMD_EP,                     /* bEndpointAddress */
  0x03,                           /* bmAttributes: Interrupt */
  LOBYTE(CDC_CMD_PACKET_SZE),     /* wMaxPacketSize: */
  HIBYTE(CDC_CMD_PACKET_SZE),
  0xFF,                           /* bInterval: */

  /*---------------------------------------------------------------------------*/

  /*Data class interface descriptor*/
  0x09,   /* bLength: Endpoint Descriptor size */
  USB_INTERFACE_DESCRIPTOR_TYPE,  /* bDescriptorType: */
  0x01,   /* bInterfaceNumber: Number of Interface */
  0x00,   /* bAlternateSetting: Alternate setting */
  0x02,   /* bNumEndpoints: Two endpoints used */
  0x0A,   /* bInterfaceClass: CDC */
  0x00,   /* bInterfaceSubClass: */
  0x00,   /* bInterfaceProtocol: */
  0x00,   /* iInterface: */

  /*Endpoint OUT Descriptor*/
  0x07,   /* bLength: Endpoint Descriptor size */
  USB_ENDPOINT_DESCRIPTOR_TYPE,      /* bDescriptorType: Endpoint */
  CDC_OUT_EP,                        /* bEndpointAddress */
  0x02,                              /* bmAttributes: Bulk */
  LOBYTE(CDC_DATA_MAX_PACKET_SIZE),  /* wMaxPacketSize: */
  HIBYTE(CDC_DATA_MAX_PACKET_SIZE),
  0x00,                              /* bInterval: ignore for Bulk transfer */

  /*Endpoint IN Descriptor*/
  0x07,   /* bLength: Endpoint Descriptor size */
  USB_ENDPOINT_DESCRIPTOR_TYPE,      /* bDescriptorType: Endpoint */
  CDC_IN_EP,                         /* bEndpointAddress */
  0x02,                              /* bmAttributes: Bulk */
  LOBYTE(CDC_DATA_MAX_PACKET_SIZE),  /* wMaxPacketSize: */
  HIBYTE(CDC_DATA_MAX_PACKET_SIZE),
  0x00                               /* bInterval: ignore for Bulk transfer */
} ;
static cdc_call_back_api_typ* client_cb;
static uint8_t* pConfig_descriptor;


void set_config_descriptor(uint8_t* cfg_desc)
{
    pConfig_descriptor = cfg_desc;
}

void hal_usb_device_cdc_disconnect(void)
{
    //TODO do soft disconnect
}

void hal_usb_device_cdc_send_data(uint8_t * data, uint_fast16_t length)
{
    uint_fast16_t i = 0;
    while (i < length)
    {
        APP_Rx_Buffer[APP_Rx_ptr_in] = data[i];
        APP_Rx_ptr_in++;
        i++;
        if (APP_Rx_ptr_in == APP_RX_DATA_SIZE)
        {
            APP_Rx_ptr_in = 0;
        }
    }
}

bool hal_usb_device_cdc_init(cdc_call_back_api_typ* client)
{
    if(NULL == client)
    {
        return false;
    }
    client_cb = client;
    // enable clock for GPIO Port of D- and D+
    RCC->AHB1ENR |= USB_FS_DP_GPIO_PORT_RCC;
    RCC->AHB1ENR |= USB_FS_DP_GPIO_PORT_RCC;
    // enable clock for interface
    RCC->APB2ENR |= USB_FS_APB2ENR;

    // configure Pins
    // D-
    USB_HS_DM_GPIO_PORT->MODER   |=  USB_HS_DM_GPIO_MODER_1;
    USB_HS_DM_GPIO_PORT->MODER   &= ~USB_HS_DM_GPIO_MODER_0;
    USB_HS_DM_GPIO_PORT->AFR[0]  |=  USB_HS_DM_GPIO_AFR_0_1;
    USB_HS_DM_GPIO_PORT->AFR[0]  &= ~USB_HS_DM_GPIO_AFR_0_0;
    USB_HS_DM_GPIO_PORT->AFR[1]  |=  USB_HS_DM_GPIO_AFR_1_1;
    USB_HS_DM_GPIO_PORT->AFR[1]  &= ~USB_HS_DM_GPIO_AFR_1_0;
    USB_HS_DM_GPIO_PORT->OTYPER  |=  USB_HS_DM_GPIO_OTYPER_1;
    USB_HS_DM_GPIO_PORT->OTYPER  &= ~USB_HS_DM_GPIO_OTYPER_0;
    USB_HS_DM_GPIO_PORT->PUPDR   |=  USB_HS_DM_GPIO_PUPD_1;
    USB_HS_DM_GPIO_PORT->PUPDR   &= ~USB_HS_DM_GPIO_PUPD_0;
    USB_HS_DM_GPIO_PORT->OSPEEDR |=  USB_HS_DM_GPIO_OSPEEDR_1;
    USB_HS_DM_GPIO_PORT->OSPEEDR &= ~USB_HS_DM_GPIO_OSPEEDR_0;
    // D+
    USB_HS_DP_GPIO_PORT->MODER   |=  USB_HS_DP_GPIO_MODER_1;
    USB_HS_DP_GPIO_PORT->MODER   &= ~USB_HS_DP_GPIO_MODER_0;
    USB_HS_DP_GPIO_PORT->AFR[0]  |=  USB_HS_DP_GPIO_AFR_0_1;
    USB_HS_DP_GPIO_PORT->AFR[0]  &= ~USB_HS_DP_GPIO_AFR_0_0;
    USB_HS_DP_GPIO_PORT->AFR[1]  |=  USB_HS_DP_GPIO_AFR_1_1;
    USB_HS_DP_GPIO_PORT->AFR[1]  &= ~USB_HS_DP_GPIO_AFR_1_0;
    USB_HS_DP_GPIO_PORT->OTYPER  |=  USB_HS_DP_GPIO_OTYPER_1;
    USB_HS_DP_GPIO_PORT->OTYPER  &= ~USB_HS_DP_GPIO_OTYPER_0;
    USB_HS_DP_GPIO_PORT->PUPDR   |=  USB_HS_DP_GPIO_PUPD_1;
    USB_HS_DP_GPIO_PORT->PUPDR   &= ~USB_HS_DP_GPIO_PUPD_0;
    USB_HS_DP_GPIO_PORT->OSPEEDR |=  USB_HS_DP_GPIO_OSPEEDR_1;
    USB_HS_DP_GPIO_PORT->OSPEEDR &= ~USB_HS_DP_GPIO_OSPEEDR_0;

    return usb_device_init(&USBD_CDC_cb);
}

/*
 * Initialize the CDC interface
 *  cfgidx: Configuration index
 */
static void usb_cdc_Init(uint8_t cfgidx)
{
    /* Open EP IN */
    EP_Open(CDC_IN_EP,
            CDC_DATA_IN_PACKET_SIZE,
            USB_OTG_EP_BULK);
    /* Open EP OUT */
    EP_Open(CDC_OUT_EP,
            CDC_DATA_OUT_PACKET_SIZE,
            USB_OTG_EP_BULK);
    /* Open Command IN EP */
    EP_Open(CDC_CMD_EP,
            CDC_CMD_PACKET_SZE,
            USB_OTG_EP_INT);
    /* Initialize the Interface physical components */
    client_cb->Init();
    /* Prepare Out endpoint to receive next packet */
    EP_PrepareRx(CDC_OUT_EP,
                 (uint8_t*)(USB_Rx_Buffer),
                 CDC_DATA_OUT_PACKET_SIZE);
}

/*
 * DeInitialize the CDC layer
 *  cfgidx: Configuration index
 */
static void usb_cdc_DeInit(uint8_t cfgidx)
{
    /* Open EP IN */
    EP_Close(CDC_IN_EP);
    /* Open EP OUT */
    EP_Close(CDC_OUT_EP);
    /* Open Command IN EP */
    EP_Close(CDC_CMD_EP);
    /* Restore default state of the Interface physical components */
    client_cb->DeInit();
}

/*
 * Handle the CDC specific requests
 *  req: usb requests
 */
static void usb_cdc_Setup(USB_SETUP_REQ *req)
{
    uint16_t len;
    uint8_t  *pbuf;

    switch (req->bmRequest & USB_REQ_TYPE_MASK)
    {
    /* CDC Class Requests -------------------------------*/
    case USB_REQ_TYPE_CLASS :
        /* Check if the request is a data setup packet */
        if (req->wLength)
        {
            /* Check if the request is Device-to-Host */
            if (req->bmRequest & 0x80)
            {
                /* Get the data to be sent to Host from interface layer */
                //TODO API : APP_FOPS.pIf_Ctrl(req->bRequest, CmdBuff, req->wLength);

                /* Send the data to the host */
                usb_CtlSendData(CmdBuff, req->wLength);
            }
            else /* Host-to-Device requeset */
            {
                USB_OTG_EP* out_ep = get_out_ep_num(0);
                /* Set the value of the current command to be processed */
                cdcCmd = req->bRequest;
                cdcLen = req->wLength;
                /* Prepare the reception of the buffer over EP0
                Next step: the received data will be managed in usbd_cdc_EP0_TxSent()
                function. */
                out_ep->total_data_len = req->wLength;
                out_ep->rem_data_len   = req->wLength;
                set_device_state(USB_OTG_EP0_DATA_OUT);
                EP_PrepareRx (0, CmdBuff, req->wLength);
            }
        }
        else /* No Data request */
        {
            /* Transfer the command to the interface layer */
            //TODO API : APP_FOPS.pIf_Ctrl(req->bRequest, NULL, 0);
        }
        return;

    default:
        usb_CtlError(req);
        return;

    /* Standard Requests -------------------------------*/
    case USB_REQ_TYPE_STANDARD:
        switch (req->bRequest)
        {
        case USB_REQ_GET_DESCRIPTOR:
            if( (req->wValue >> 8) == CDC_DESCRIPTOR_TYPE)
            {
                pbuf = usbd_cdc_CfgDesc + 9 + (9 * USBD_ITF_MAX_NUM);
                len = MIN(USB_CDC_DESC_SIZ , req->wLength);
                usb_CtlSendData(pbuf, len);
            }
            // TODO else ?
            break;

        case USB_REQ_GET_INTERFACE :
            usb_CtlSendData ((uint8_t *)&usbd_cdc_AltSet, 1);
            break;

        case USB_REQ_SET_INTERFACE :
            if ((uint8_t)(req->wValue) < USBD_ITF_MAX_NUM)
            {
                usbd_cdc_AltSet = (uint8_t)(req->wValue);
            }
            else
            {
                /* Call the error management function (command will be nacked */
                usb_CtlError(req);
            }
            break;
        }
    }
}

/*
 * Data received on control endpoint
 */
static void usb_cdc_EP0_RxReady(void)
{
    if (cdcCmd != NO_CMD)
    {
        /* Process the data */
        //TODO API : APP_FOPS.pIf_Ctrl(cdcCmd, CmdBuff, cdcLen);
        /* Reset the command variable to default value */
        cdcCmd = NO_CMD;
    }
}

/*
 * Data sent on non-control IN endpoint
 * epnum: endpoint number
 */
static void usb_cdc_DataIn(uint8_t epnum)
{
    uint16_t USB_Tx_ptr;
    uint16_t USB_Tx_length;

    if (USB_Tx_State == 1)
    {
        if (APP_Rx_length == 0)
        {
            USB_Tx_State = 0;
        }
        else
        {
            if (APP_Rx_length > CDC_DATA_IN_PACKET_SIZE)
            {
                USB_Tx_ptr = APP_Rx_ptr_out;
                USB_Tx_length = CDC_DATA_IN_PACKET_SIZE;
                APP_Rx_ptr_out += CDC_DATA_IN_PACKET_SIZE;
                APP_Rx_length -= CDC_DATA_IN_PACKET_SIZE;
            }
            else
            {
                USB_Tx_ptr = APP_Rx_ptr_out;
                USB_Tx_length = APP_Rx_length;
                APP_Rx_ptr_out += APP_Rx_length;
                APP_Rx_length = 0;
            }
            /* Prepare the available data buffer to be sent on IN endpoint */
            EP_Tx(CDC_IN_EP,
                  (uint8_t*)&APP_Rx_Buffer[USB_Tx_ptr],
                  USB_Tx_length);
        }
    }
}

/*
 *  Data received on non-control Out endpoint
 * epnum: endpoint number
 */
static void usb_cdc_DataOut(uint8_t epnum)
{
    uint16_t USB_Rx_Cnt;
    /* Get the received data buffer and update the counter */
    USB_OTG_EP* out_ep = get_out_ep_num(epnum);
    USB_Rx_Cnt = out_ep->xfer_count;
    /* USB data will be immediately processed, this allow next USB traffic being
     NAKed till the end of the application Xfer */
    client_cb->receive(&USB_Rx_Buffer[0], USB_Rx_Cnt);
    /* Prepare Out endpoint to receive next packet */
    EP_PrepareRx(CDC_OUT_EP,
                 (uint8_t*)(USB_Rx_Buffer),
                 CDC_DATA_OUT_PACKET_SIZE);
}

/*
 * Start Of Frame event management
 */
static void usb_cdc_SOF(void)
{
    uint16_t USB_Tx_ptr;
    uint16_t USB_Tx_length;

    if(USB_Tx_State != 1)
    {
        if (APP_Rx_ptr_out == APP_RX_DATA_SIZE)
        {
            APP_Rx_ptr_out = 0;
        }
        if(APP_Rx_ptr_out == APP_Rx_ptr_in)
        {
            USB_Tx_State = 0;
            return;
        }
        if(APP_Rx_ptr_out > APP_Rx_ptr_in) /* rollback */
        {
            APP_Rx_length = APP_RX_DATA_SIZE - APP_Rx_ptr_out;
        }
        else
        {
            APP_Rx_length = APP_Rx_ptr_in - APP_Rx_ptr_out;
        }
        if (APP_Rx_length > CDC_DATA_IN_PACKET_SIZE)
        {
            USB_Tx_ptr = APP_Rx_ptr_out;
            USB_Tx_length = CDC_DATA_IN_PACKET_SIZE;
            APP_Rx_ptr_out += CDC_DATA_IN_PACKET_SIZE;
            APP_Rx_length -= CDC_DATA_IN_PACKET_SIZE;
        }
        else
        {
            USB_Tx_ptr = APP_Rx_ptr_out;
            USB_Tx_length = APP_Rx_length;
            APP_Rx_ptr_out += APP_Rx_length;
            APP_Rx_length = 0;
        }
        USB_Tx_State = 1;
        EP_Tx(CDC_IN_EP,
              (uint8_t*)&APP_Rx_Buffer[USB_Tx_ptr],
              USB_Tx_length);
    }
}

/*
 * Return configuration descriptor
 * length : pointer data length
 * returns: pointer to descriptor buffer
 */
static uint8_t *usb_cdc_GetCfgDesc(uint16_t *length)
{
    *length = sizeof (usbd_cdc_CfgDesc);
    return usbd_cdc_CfgDesc;
}
