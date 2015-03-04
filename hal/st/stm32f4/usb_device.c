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
#include "board_cfg.h"
#include "usb_device.h"
#include "usb.h"
#include <stddef.h>
#include "util.h"
#include "usb_descriptor.h"
#include "usb_std.h"
#include "hal_debug.h"
#include "rcc.h"

extern void set_config_descriptor(uint8_t* cfg_desc);
static void HandleInEP_ISR(void);
static void HandleOutEP_ISR(void);
static void HandleRxStatusQueueLevel_ISR(void);
static void HandleUsbReset_ISR(void);
static void HandleEnumDone_ISR(void);
static uint32_t ReadDevInEP(uint8_t epnum);
static void WriteEmptyTxFifo(uint32_t epnum);
void USB_FS_IRQ_HANDLER(void);
static uint32_t usb_ReadDevAllInEPItr(void);
static void usb_EP0_OutStart(void);
static uint32_t usb_ReadDevAllOutEp_itr(void);
static uint32_t usb_ReadDevOutEP_itr(uint8_t epnum);
static void usb_ReadPacket(uint8_t *dest, uint16_t len);
static void usb_FlushTxFifo(uint32_t num );
static void usb_EP0Activate(void);
static void usb_SetEPStatus(USB_OTG_EP *ep , uint32_t Status);
static uint32_t usb_GetEPStatus(USB_OTG_EP *ep);
static void usb_EPClearStall(USB_OTG_EP *ep);
static void usb_EPSetStall(USB_OTG_EP *ep);
static void usb_EP0StartXfer(USB_OTG_EP *ep);
static void usb_EPStartXfer(USB_OTG_EP *ep);
static void usb_EPActivate(USB_OTG_EP *ep);
static void usb_EnableDevInt(void);
static void usb_CoreInitDev(void);
static void usb_InitDevSpeed(void);
static void usb_FlushRxFifo(void);
static void usb_CoreInit(void);
static void usb_EnableCommonInt(void);
static void usb_CoreReset(void);
static void usb_CtlSendStatus(void);
static void EP_Stall(uint8_t   epnum);
static void SetupStage(void);
static void usb_ParseSetupRequest(USB_SETUP_REQ *req);
static void EP_ClrStall(uint8_t epnum);
static void usb_StdEPReq(USB_SETUP_REQ  *req);
static void usb_StdItfReq(USB_SETUP_REQ  *req);
static void usb_StdDevReq(USB_SETUP_REQ  *req);
static void usb_ClrFeature(USB_SETUP_REQ *req);
static void usb_SetFeature(USB_SETUP_REQ *req);
static void usb_GetStatus(USB_SETUP_REQ *req);
static void usb_GetConfig(USB_SETUP_REQ *req);
static void usb_SetConfig(USB_SETUP_REQ *req);
static void EP_SetAddress(uint8_t address);
static void usb_SetAddress(USB_SETUP_REQ *req);
static void DataOutStage(uint8_t epnum);
static void DataInStage(uint8_t epnum);
static void usb_WritePacket(uint8_t* src, uint8_t  ch_ep_num, uint16_t len);


#define USB_LEN_DEV_QUALIFIER_DESC                     0x0A
#define USB_LEN_DEV_DESC                               0x12
#define USB_LEN_CFG_DESC                               0x09
#define USB_LEN_IF_DESC                                0x09
#define USB_LEN_EP_DESC                                0x07
#define USB_LEN_OTG_DESC                               0x03
#define USB_LEN_LANGID_STR_DESC                        0x04
#define USB_LEN_OTHER_SPEED_DESC_SIZ                   0x09

#define USB_REQ_TYPE_STANDARD                          0x00
#define USB_REQ_TYPE_CLASS                             0x20
#define USB_REQ_TYPE_VENDOR                            0x40
#define USB_REQ_TYPE_MASK                              0x60

#define USB_REQ_RECIPIENT_DEVICE                       0x00
#define USB_REQ_RECIPIENT_INTERFACE                    0x01
#define USB_REQ_RECIPIENT_ENDPOINT                     0x02
#define USB_REQ_RECIPIENT_MASK                         0x03

#define USB_REQ_GET_STATUS                             0x00
#define USB_REQ_CLEAR_FEATURE                          0x01
#define USB_REQ_SET_FEATURE                            0x03
#define USB_REQ_SET_ADDRESS                            0x05
#define USB_REQ_GET_DESCRIPTOR                         0x06
#define USB_REQ_SET_DESCRIPTOR                         0x07
#define USB_REQ_GET_CONFIGURATION                      0x08
#define USB_REQ_SET_CONFIGURATION                      0x09
#define USB_REQ_GET_INTERFACE                          0x0A
#define USB_REQ_SET_INTERFACE                          0x0B
#define USB_REQ_SYNCH_FRAME                            0x0C


#define USB_CONFIG_REMOTE_WAKEUP                       2
#define USB_CONFIG_SELF_POWERED                        1

#define USB_FEATURE_EP_HALT                            0
#define USB_FEATURE_REMOTE_WAKEUP                      1
#define USB_FEATURE_TEST_MODE                          2


#define USB_HS_MAX_PACKET_SIZE                         512
#define USB_FS_MAX_PACKET_SIZE                         64
#define USB_MAX_EP0_SIZE                               64

/*  Device Status */
#define USBD_STATE_DEFAULT                             1
#define USBD_STATE_ADDRESSED                           2
#define USBD_STATE_CONFIGURED                          3
#define USBD_STATE_SUSPENDED                           4


/*  EP0 State */
#define USBD_EP0_IDLE                                  0
#define USBD_EP0_SETUP                                 1
#define USBD_EP0_DATA_IN                               2
#define USBD_EP0_DATA_OUT                              3
#define USBD_EP0_STATUS_IN                             4
#define USBD_EP0_STATUS_OUT                            5
#define USBD_EP0_STALL                                 6

#define USBD_EP_TYPE_CTRL                              0
#define USBD_EP_TYPE_ISOC                              1
#define USBD_EP_TYPE_BULK                              2
#define USBD_EP_TYPE_INTR                              3


#define USBD_ITF_MAX_NUM                               1

#define DSTS_ENUMSPD_HS_PHY_30MHZ_OR_60MHZ             0
#define DSTS_ENUMSPD_FS_PHY_30MHZ_OR_60MHZ             1
#define DSTS_ENUMSPD_LS_PHY_6MHZ                       2
#define DSTS_ENUMSPD_FS_PHY_48MHZ                      3

#define DCFG_FRAME_INTERVAL_80                         0
#define DCFG_FRAME_INTERVAL_85                         1
#define DCFG_FRAME_INTERVAL_90                         2
#define DCFG_FRAME_INTERVAL_95                         3

#define DEP0CTL_MPS_64                                 0
#define DEP0CTL_MPS_32                                 1
#define DEP0CTL_MPS_16                                 2
#define DEP0CTL_MPS_8                                  3

#define EP_SPEED_LOW                                   0
#define EP_SPEED_FULL                                  1
#define EP_SPEED_HIGH                                  2

#define EP_TYPE_CTRL                                   0
#define EP_TYPE_ISOC                                   1
#define EP_TYPE_BULK                                   2
#define EP_TYPE_INTR                                   3
#define EP_TYPE_MSK                                    3

#define STS_GOUT_NAK                                   1
#define STS_DATA_UPDT                                  2
#define STS_XFER_COMP                                  3
#define STS_SETUP_COMP                                 4
#define STS_SETUP_UPDT                                 6

#define USB_OTG_EP_TX_DIS                              0x0000
#define USB_OTG_EP_TX_STALL                            0x0010
#define USB_OTG_EP_TX_NAK                              0x0020
#define USB_OTG_EP_TX_VALID                            0x0030

#define USB_OTG_EP_RX_DIS                              0x0000
#define USB_OTG_EP_RX_STALL                            0x1000
#define USB_OTG_EP_RX_NAK                              0x2000
#define USB_OTG_EP_RX_VALID                            0x3000

#define USB_OTG_SPEED_HIGH                             0
#define USB_OTG_SPEED_FULL                             1
#define USB_OTG_ULPI_PHY                               1
#define USB_OTG_EMBEDDED_PHY                           2
#define USB_OTG_I2C_PHY                                3

#define RX_FIFO_FS_SIZE                                128
#define TX0_FIFO_FS_SIZE                               64
#define TX1_FIFO_FS_SIZE                               128
#define TX2_FIFO_FS_SIZE                               0
#define TX3_FIFO_FS_SIZE                               0
#define TXH_NP_HS_FIFOSIZ                              96
#define TXH_P_HS_FIFOSIZ                               96

#define USB_OTG_SPEED_PARAM_HIGH                       0
#define USB_OTG_SPEED_PARAM_HIGH_IN_FULL               1
#define USB_OTG_SPEED_PARAM_FULL                       3

typedef struct USB_OTG_core_regs //000h
{
    USB_OTG_GREGS         *GREGS;
    USB_OTG_DREGS         *DREGS;
    USB_OTG_HREGS         *HREGS;
    USB_OTG_INEPREGS      *INEP_REGS[USB_OTG_MAX_TX_FIFOS];
    USB_OTG_OUTEPREGS     *OUTEP_REGS[USB_OTG_MAX_TX_FIFOS];
    USB_OTG_HC_REGS       *HC_REGS[USB_OTG_MAX_TX_FIFOS];
    __IO uint32_t         *HPRT0;
    __IO uint32_t         *DFIFO[USB_OTG_MAX_TX_FIFOS];
    __IO uint32_t         *PCGCCTL;
} USB_OTG_CORE_REGS;

static USB_OTG_CORE_REGS USB_Regs;
static USB_OTG_CORE_REGS* USB_FS = &USB_Regs;
static uint32_t USBD_ep_status __attribute__ ((aligned (4)))  = 0;
static uint32_t  USBD_cfg_status __attribute__ ((aligned (4)))  = 0;
static uint32_t  USBD_default_cfg __attribute__ ((aligned (4)))   = 0;
static uint8_t device_status;
static USBD_Class_cb_TypeDef* class_cb;
static uint8_t device_address;
static uint32_t DevRemoteWakeup;
static uint8_t device_config;
static uint8_t device_state;
static uint8_t setup_packet[8*3];
static USB_OTG_EP in_ep[USB_OTG_MAX_TX_FIFOS];
static USB_OTG_EP out_ep[USB_OTG_MAX_TX_FIFOS];


USB_OTG_EP* get_out_ep_num(uint_fast8_t num)
{
    return &out_ep[num];
}

void set_device_state(uint8_t state)
{
    device_state = state;
}

uint8_t get_device_status(void)
{
    return device_status;
}

void usb_device_print_configuration(void)
{
    // Clock
    debug_line("RCC->AHB1ENR  = 0x%08x", RCC->AHB1ENR);
    debug_line("RCC->APB2ENR  = 0x%08x", RCC->APB2ENR);
    // pins
    debug_line("D- Pin:");
    print_gpio_configuration(USB_FS_DM_GPIO_PORT);
    debug_line("D+ Pin:");
    print_gpio_configuration(USB_FS_DP_GPIO_PORT);
    // Registers
    debug_line("Global Registers:");
    debug_line("USB_FS->GREGS->GOTGCTL             = 0x%08x", USB_FS->GREGS->GOTGCTL);
    debug_line("USB_FS->GREGS->GOTGINT             = 0x%08x", USB_FS->GREGS->GOTGINT);
    debug_line("USB_FS->GREGS->GAHBCFG             = 0x%08x", USB_FS->GREGS->GAHBCFG);
    debug_line("USB_FS->GREGS->GUSBCFG             = 0x%08x", USB_FS->GREGS->GUSBCFG);
    debug_line("USB_FS->GREGS->GRSTCTL             = 0x%08x", USB_FS->GREGS->GRSTCTL);
    debug_line("USB_FS->GREGS->GINTSTS             = 0x%08x", USB_FS->GREGS->GINTSTS);
    debug_line("USB_FS->GREGS->GINTMSK             = 0x%08x", USB_FS->GREGS->GINTMSK);
    debug_line("USB_FS->GREGS->GRXSTSR             = 0x%08x", USB_FS->GREGS->GRXSTSR);
    debug_line("USB_FS->GREGS->GRXSTSP             = 0x%08x", USB_FS->GREGS->GRXSTSP);
    debug_line("USB_FS->GREGS->GRXFSIZ             = 0x%08x", USB_FS->GREGS->GRXFSIZ);
    debug_line("USB_FS->GREGS->DIEPTXF0_HNPTXFSIZ  = 0x%08x", USB_FS->GREGS->DIEPTXF0_HNPTXFSIZ);
    debug_line("USB_FS->GREGS->HNPTXSTS            = 0x%08x", USB_FS->GREGS->HNPTXSTS);
    debug_line("USB_FS->GREGS->GCCFG               = 0x%08x", USB_FS->GREGS->GCCFG);
    debug_line("USB_FS->GREGS->CID                 = 0x%08x", USB_FS->GREGS->CID);
    debug_line("USB_FS->GREGS->HPTXFSIZ            = 0x%08x", USB_FS->GREGS->HPTXFSIZ);
    debug_line("USB_FS->GREGS->DIEPTXF[0]          = 0x%08x", USB_FS->GREGS->DIEPTXF[0]);
    debug_line("USB_FS->GREGS->DIEPTXF[1]          = 0x%08x", USB_FS->GREGS->DIEPTXF[1]);
    debug_line("USB_FS->GREGS->DIEPTXF[2]          = 0x%08x", USB_FS->GREGS->DIEPTXF[2]);
    // TODO rest of DIEPTXF[]
    debug_line("Device Mode Registers:");
    debug_line("USB_FS->DREGS->DCFG                = 0x%08x", USB_FS->DREGS->DCFG);
    debug_line("USB_FS->DREGS->DCTL                = 0x%08x", USB_FS->DREGS->DCTL);
    debug_line("USB_FS->DREGS->DSTS                = 0x%08x", USB_FS->DREGS->DSTS);
    debug_line("USB_FS->DREGS->DIEPMSK             = 0x%08x", USB_FS->DREGS->DIEPMSK);
    debug_line("USB_FS->DREGS->DOEPMSK             = 0x%08x", USB_FS->DREGS->DOEPMSK);
    debug_line("USB_FS->DREGS->DAINT               = 0x%08x", USB_FS->DREGS->DAINT);
    debug_line("USB_FS->DREGS->DAINTMSK            = 0x%08x", USB_FS->DREGS->DAINTMSK);
    debug_line("USB_FS->DREGS->DVBUSDIS            = 0x%08x", USB_FS->DREGS->DVBUSDIS);
    debug_line("USB_FS->DREGS->DVBUSPULSE          = 0x%08x", USB_FS->DREGS->DVBUSPULSE);
    debug_line("USB_FS->DREGS->DTHRCTL             = 0x%08x", USB_FS->DREGS->DTHRCTL);
    debug_line("USB_FS->DREGS->DIEPEMPMSK          = 0x%08x", USB_FS->DREGS->DIEPEMPMSK);
    // TODO
    debug_line("USB_FS->INEP_REGS[0]               = 0x%08x", USB_FS->INEP_REGS[0]);
    debug_line("USB_FS->INEP_REGS[1]               = 0x%08x", USB_FS->INEP_REGS[1]);
    debug_line("USB_FS->INEP_REGS[2]               = 0x%08x", USB_FS->INEP_REGS[2]);
    debug_line("USB_FS->INEP_REGS[3]               = 0x%08x", USB_FS->INEP_REGS[3]);
    debug_line("USB_FS->OUTEP_REGS[0]              = 0x%08x", USB_FS->OUTEP_REGS[0]);
    debug_line("USB_FS->OUTEP_REGS[1]              = 0x%08x", USB_FS->OUTEP_REGS[1]);
    debug_line("USB_FS->OUTEP_REGS[2]              = 0x%08x", USB_FS->OUTEP_REGS[2]);
    debug_line("USB_FS->OUTEP_REGS[3]              = 0x%08x", USB_FS->OUTEP_REGS[3]);
    debug_line("USB_FS->HPRT0                      = 0x%08x", USB_FS->HPRT0);
    debug_line("USB_FS->HC_REGS[0]                 = 0x%08x", USB_FS->HC_REGS[0]);
    debug_line("USB_FS->HC_REGS[1]                 = 0x%08x", USB_FS->HC_REGS[1]);
    debug_line("USB_FS->HC_REGS[2]                 = 0x%08x", USB_FS->HC_REGS[2]);
    debug_line("USB_FS->HC_REGS[3]                 = 0x%08x", USB_FS->HC_REGS[3]);
    debug_line("USB_FS->HC_REGS[4]                 = 0x%08x", USB_FS->HC_REGS[4]);
    debug_line("USB_FS->HC_REGS[5]                 = 0x%08x", USB_FS->HC_REGS[5]);
    debug_line("USB_FS->HC_REGS[6]                 = 0x%08x", USB_FS->HC_REGS[6]);
    debug_line("USB_FS->HC_REGS[7]                 = 0x%08x", USB_FS->HC_REGS[7]);
    debug_line("USB_FS->DFIFO[0]                   = 0x%08x", USB_FS->DFIFO[0]);
    debug_line("USB_FS->DFIFO[1]                   = 0x%08x", USB_FS->DFIFO[1]);
    debug_line("USB_FS->DFIFO[2]                   = 0x%08x", USB_FS->DFIFO[2]);
    debug_line("USB_FS->DFIFO[3]                   = 0x%08x", USB_FS->DFIFO[3]);
    debug_line("USB_FS->DFIFO[4]                   = 0x%08x", USB_FS->DFIFO[4]);
    debug_line("USB_FS->DFIFO[5]                   = 0x%08x", USB_FS->DFIFO[5]);
    debug_line("USB_FS->DFIFO[6]                   = 0x%08x", USB_FS->DFIFO[6]);
    debug_line("USB_FS->DFIFO[7]                   = 0x%08x", USB_FS->DFIFO[7]);
    debug_line("USB_FS->PCGCCTL                    = 0x%08x", USB_FS->PCGCCTL);
}

bool usb_device_init(USBD_Class_cb_TypeDef* usb_class_cb)
{
    uint32_t i;
    USB_OTG_EP *ep;
    uint32_t  usbcfg;
    debug_line("ub_device_init()");

    // enable clock for GPIO Port of D- and D+
    RCC->AHB1ENR |= USB_FS_DP_GPIO_PORT_RCC;
    RCC->AHB1ENR |= USB_FS_DP_GPIO_PORT_RCC;
    // enable clock for interface
    RCC->APB2ENR |= USB_FS_APB2ENR;

    // configure Pins
    // D-
    USB_FS_DM_GPIO_PORT->MODER   |=  USB_FS_DM_GPIO_MODER_1;
    USB_FS_DM_GPIO_PORT->MODER   &= ~USB_FS_DM_GPIO_MODER_0;
    USB_FS_DM_GPIO_PORT->AFR[0]  |=  USB_FS_DM_GPIO_AFR_0_1;
    USB_FS_DM_GPIO_PORT->AFR[0]  &= ~USB_FS_DM_GPIO_AFR_0_0;
    USB_FS_DM_GPIO_PORT->AFR[1]  |=  USB_FS_DM_GPIO_AFR_1_1;
    USB_FS_DM_GPIO_PORT->AFR[1]  &= ~USB_FS_DM_GPIO_AFR_1_0;
    USB_FS_DM_GPIO_PORT->OTYPER  |=  USB_FS_DM_GPIO_OTYPER_1;
    USB_FS_DM_GPIO_PORT->OTYPER  &= ~USB_FS_DM_GPIO_OTYPER_0;
    USB_FS_DM_GPIO_PORT->PUPDR   |=  USB_FS_DM_GPIO_PUPD_1;
    USB_FS_DM_GPIO_PORT->PUPDR   &= ~USB_FS_DM_GPIO_PUPD_0;
    USB_FS_DM_GPIO_PORT->OSPEEDR |=  USB_FS_DM_GPIO_OSPEEDR_1;
    USB_FS_DM_GPIO_PORT->OSPEEDR &= ~USB_FS_DM_GPIO_OSPEEDR_0;
    // D+
    USB_FS_DP_GPIO_PORT->MODER   |=  USB_FS_DP_GPIO_MODER_1;
    USB_FS_DP_GPIO_PORT->MODER   &= ~USB_FS_DP_GPIO_MODER_0;
    USB_FS_DP_GPIO_PORT->AFR[0]  |=  USB_FS_DP_GPIO_AFR_0_1;
    USB_FS_DP_GPIO_PORT->AFR[0]  &= ~USB_FS_DP_GPIO_AFR_0_0;
    USB_FS_DP_GPIO_PORT->AFR[1]  |=  USB_FS_DP_GPIO_AFR_1_1;
    USB_FS_DP_GPIO_PORT->AFR[1]  &= ~USB_FS_DP_GPIO_AFR_1_0;
    USB_FS_DP_GPIO_PORT->OTYPER  |=  USB_FS_DP_GPIO_OTYPER_1;
    USB_FS_DP_GPIO_PORT->OTYPER  &= ~USB_FS_DP_GPIO_OTYPER_0;
    USB_FS_DP_GPIO_PORT->PUPDR   |=  USB_FS_DP_GPIO_PUPD_1;
    USB_FS_DP_GPIO_PORT->PUPDR   &= ~USB_FS_DP_GPIO_PUPD_0;
    USB_FS_DP_GPIO_PORT->OSPEEDR |=  USB_FS_DP_GPIO_OSPEEDR_1;
    USB_FS_DP_GPIO_PORT->OSPEEDR &= ~USB_FS_DP_GPIO_OSPEEDR_0;

    /*Register class and user callbacks */
    class_cb = usb_class_cb;

    /* initialize the USB_FS Structure */
    USB_FS->GREGS         = (USB_OTG_GREGS *)    (USB_OTG_FS_BASE_ADDR + USB_OTG_CORE_GLOBAL_REGS_OFFSET);
    USB_FS->DREGS         = (USB_OTG_DREGS *)    (USB_OTG_FS_BASE_ADDR + USB_OTG_DEV_GLOBAL_REG_OFFSET);
    USB_FS->INEP_REGS[0]  = (USB_OTG_INEPREGS *) (USB_OTG_FS_BASE_ADDR + USB_OTG_DEV_IN_EP_REG_OFFSET);
    USB_FS->INEP_REGS[1]  = (USB_OTG_INEPREGS *) (USB_OTG_FS_BASE_ADDR + USB_OTG_DEV_IN_EP_REG_OFFSET + (USB_OTG_EP_REG_OFFSET));
    USB_FS->INEP_REGS[2]  = (USB_OTG_INEPREGS *) (USB_OTG_FS_BASE_ADDR + USB_OTG_DEV_IN_EP_REG_OFFSET + (2 * USB_OTG_EP_REG_OFFSET));
    USB_FS->INEP_REGS[3]  = (USB_OTG_INEPREGS *) (USB_OTG_FS_BASE_ADDR + USB_OTG_DEV_IN_EP_REG_OFFSET + (3 * USB_OTG_EP_REG_OFFSET));
    USB_FS->OUTEP_REGS[0] = (USB_OTG_OUTEPREGS *)(USB_OTG_FS_BASE_ADDR + USB_OTG_DEV_OUT_EP_REG_OFFSET);
    USB_FS->OUTEP_REGS[1] = (USB_OTG_OUTEPREGS *)(USB_OTG_FS_BASE_ADDR + USB_OTG_DEV_OUT_EP_REG_OFFSET + (USB_OTG_EP_REG_OFFSET));
    USB_FS->OUTEP_REGS[2] = (USB_OTG_OUTEPREGS *)(USB_OTG_FS_BASE_ADDR + USB_OTG_DEV_OUT_EP_REG_OFFSET + (2 * USB_OTG_EP_REG_OFFSET));
    USB_FS->OUTEP_REGS[3] = (USB_OTG_OUTEPREGS *)(USB_OTG_FS_BASE_ADDR + USB_OTG_DEV_OUT_EP_REG_OFFSET + (3 * USB_OTG_EP_REG_OFFSET));
    USB_FS->HREGS         = (USB_OTG_HREGS *)    (USB_OTG_FS_BASE_ADDR + USB_OTG_HOST_GLOBAL_REG_OFFSET);
    USB_FS->HPRT0         = (uint32_t *)         (USB_OTG_FS_BASE_ADDR + USB_OTG_HOST_PORT_REGS_OFFSET);
    USB_FS->HC_REGS[0]    = (USB_OTG_HC_REGS *)  (USB_OTG_FS_BASE_ADDR + USB_OTG_HOST_CHAN_REGS_OFFSET);
    USB_FS->HC_REGS[1]    = (USB_OTG_HC_REGS *)  (USB_OTG_FS_BASE_ADDR + USB_OTG_HOST_CHAN_REGS_OFFSET + (USB_OTG_CHAN_REGS_OFFSET));
    USB_FS->HC_REGS[2]    = (USB_OTG_HC_REGS *)  (USB_OTG_FS_BASE_ADDR + USB_OTG_HOST_CHAN_REGS_OFFSET + (2 * USB_OTG_CHAN_REGS_OFFSET));
    USB_FS->HC_REGS[3]    = (USB_OTG_HC_REGS *)  (USB_OTG_FS_BASE_ADDR + USB_OTG_HOST_CHAN_REGS_OFFSET + (3 * USB_OTG_CHAN_REGS_OFFSET));
    USB_FS->HC_REGS[4]    = (USB_OTG_HC_REGS *)  (USB_OTG_FS_BASE_ADDR + USB_OTG_HOST_CHAN_REGS_OFFSET + (4 * USB_OTG_CHAN_REGS_OFFSET));
    USB_FS->HC_REGS[5]    = (USB_OTG_HC_REGS *)  (USB_OTG_FS_BASE_ADDR + USB_OTG_HOST_CHAN_REGS_OFFSET + (5 * USB_OTG_CHAN_REGS_OFFSET));
    USB_FS->HC_REGS[6]    = (USB_OTG_HC_REGS *)  (USB_OTG_FS_BASE_ADDR + USB_OTG_HOST_CHAN_REGS_OFFSET + (6 * USB_OTG_CHAN_REGS_OFFSET));
    USB_FS->HC_REGS[7]    = (USB_OTG_HC_REGS *)  (USB_OTG_FS_BASE_ADDR + USB_OTG_HOST_CHAN_REGS_OFFSET + (7 * USB_OTG_CHAN_REGS_OFFSET));
    USB_FS->DFIFO[0]      = (uint32_t *)         (USB_OTG_FS_BASE_ADDR + USB_OTG_DATA_FIFO_OFFSET);
    USB_FS->DFIFO[1]      = (uint32_t *)         (USB_OTG_FS_BASE_ADDR + USB_OTG_DATA_FIFO_OFFSET + (USB_OTG_DATA_FIFO_SIZE));
    USB_FS->DFIFO[2]      = (uint32_t *)         (USB_OTG_FS_BASE_ADDR + USB_OTG_DATA_FIFO_OFFSET + (2 * USB_OTG_DATA_FIFO_SIZE));
    USB_FS->DFIFO[3]      = (uint32_t *)         (USB_OTG_FS_BASE_ADDR + USB_OTG_DATA_FIFO_OFFSET + (3 * USB_OTG_DATA_FIFO_SIZE));
    USB_FS->DFIFO[4]      = (uint32_t *)         (USB_OTG_FS_BASE_ADDR + USB_OTG_DATA_FIFO_OFFSET + (4 * USB_OTG_DATA_FIFO_SIZE));
    USB_FS->DFIFO[5]      = (uint32_t *)         (USB_OTG_FS_BASE_ADDR + USB_OTG_DATA_FIFO_OFFSET + (5 * USB_OTG_DATA_FIFO_SIZE));
    USB_FS->DFIFO[6]      = (uint32_t *)         (USB_OTG_FS_BASE_ADDR + USB_OTG_DATA_FIFO_OFFSET + (6 * USB_OTG_DATA_FIFO_SIZE));
    USB_FS->DFIFO[7]      = (uint32_t *)         (USB_OTG_FS_BASE_ADDR + USB_OTG_DATA_FIFO_OFFSET + (7 * USB_OTG_DATA_FIFO_SIZE));
    USB_FS->PCGCCTL       = (uint32_t *)         (USB_OTG_FS_BASE_ADDR + USB_OTG_PCGCCTL_OFFSET);

    /* set USB OTG core params */
    device_status = USB_OTG_DEFAULT;
    device_address = 0;
    /* Init ep structure */
    for (i = 0; i < MAX_DEVICE_ENDPOINTS ; i++)
    {
        ep = &in_ep[i];
        /* Init ep structure */
        ep->is_in = 1;
        ep->num = i;
        ep->tx_fifo_num = i;
        /* Control until ep is actvated */
        ep->type = EP_TYPE_CTRL;
        ep->maxpacket =  USB_OTG_MAX_EP0_SIZE;
        ep->xfer_buff = 0;
        ep->xfer_len = 0;
    }
    for (i = 0; i < MAX_DEVICE_ENDPOINTS; i++)
    {
        ep = &out_ep[i];
        /* Init ep structure */
        ep->is_in = 0;
        ep->num = i;
        ep->tx_fifo_num = i;
        /* Control until ep is activated */
        ep->type = EP_TYPE_CTRL;
        ep->maxpacket = USB_OTG_MAX_EP0_SIZE;
        ep->xfer_buff = 0;
        ep->xfer_len = 0;
    }
    // disable global Interrupt
    USB_FS->GREGS->GAHBCFG &= ~USB_OTG_GAHBCFG_GINT;
    /*Init the Core (common init.) */
    usb_CoreInit();
    /* Force Device Mode*/
    usbcfg = USB_FS->GREGS->GUSBCFG;
    usbcfg &= ~USB_OTG_GUSBCFG_FHMOD;
    usbcfg |= USB_OTG_GUSBCFG_FDMOD;
    USB_FS->GREGS->GUSBCFG = usbcfg;
    mDelay(50);
    /* Init Device */
    usb_CoreInitDev();
    // configure Interrupt
    NVIC_SetPriority(USB_FS_IRQ_NUMBER, USB_FS_IRQ_PRIORITY);
    NVIC_EnableIRQ(USB_FS_IRQ_NUMBER);
    /* Enable USB Global interrupt */
    USB_FS->GREGS->GAHBCFG |= USB_OTG_GAHBCFG_GINT;
    return true;
}

void USB_FS_IRQ_HANDLER(void)
{
    uint32_t irqs = USB_OTG_FS->GINTSTS & USB_OTG_FS->GINTMSK;

    if(0 == irqs) /* avoid spurious interrupt */
    {
        return;
    }

    if(0 != (irqs & USB_OTG_GINTSTS_OEPINT))
    {
        HandleOutEP_ISR();
    }

    if(0 != (irqs & USB_OTG_GINTSTS_IEPINT))
    {
        HandleInEP_ISR();
    }

    if(0 != (irqs & USB_OTG_GINTSTS_MMIS))
    {
        /* Clear interrupt */
        USB_OTG_FS->GINTSTS |= USB_OTG_GINTSTS_MMIS;
    }

    if(0 != (irqs & USB_OTG_GINTSTS_WKUINT))
    {
        uint32_t devctl;
        /* Clear the Remote Wake-up Signaling */
        devctl = USB_OTG_DCTL_RWUSIG;
        USB_FS->DREGS->DCTL &= ~devctl;
        /* Inform upper layer by the Resume Event */
        device_status = USB_OTG_CONFIGURED;
        /* Clear interrupt */
        USB_OTG_FS->GINTSTS |= USB_OTG_GINTSTS_WKUINT;
    }

    if(0 != (irqs & USB_OTG_GINTSTS_USBSUSP))
    {
        device_status  = USB_OTG_SUSPENDED;
        /* Clear interrupt */
        USB_OTG_FS->GINTSTS |= USB_OTG_GINTSTS_USBSUSP;
    }

    if(0 != (irqs & USB_OTG_GINTSTS_SOF))
    {
        if(NULL != class_cb->SOF)
        {
            class_cb->SOF();
        }
        /* Clear interrupt */
        USB_OTG_FS->GINTSTS |= USB_OTG_GINTSTS_SOF;
    }

    if(0 != (irqs & USB_OTG_GINTSTS_RXFLVL))
    {
        // RX FIFO non empty
        HandleRxStatusQueueLevel_ISR();
        /* Clear interrupt */
        USB_OTG_FS->GINTSTS |= USB_OTG_GINTSTS_RXFLVL;
    }

    if(0 != (irqs & USB_OTG_GINTSTS_USBRST))
    {
        HandleUsbReset_ISR();
        /* Clear interrupt */
        USB_OTG_FS->GINTSTS |= USB_OTG_GINTSTS_USBRST;
    }

    if(0 != (irqs & USB_OTG_GINTSTS_ENUMDNE))
    {
        HandleEnumDone_ISR();
        /* Clear interrupt */
        USB_OTG_FS->GINTSTS |= USB_OTG_GINTSTS_ENUMDNE;
    }

    if(0 != (irqs & USB_OTG_GINTSTS_IISOIXFR))
    {
        class_cb->IsoINIncomplete();
        /* Clear interrupt */
        USB_OTG_FS->GINTSTS |= USB_OTG_GINTSTS_IISOIXFR;
    }

    if(0 != (irqs & USB_OTG_GINTSTS_PXFR_INCOMPISOOUT))
    {
        class_cb->IsoOUTIncomplete();
        /* Clear interrupt */
        USB_OTG_FS->GINTSTS |= USB_OTG_GINTSTS_PXFR_INCOMPISOOUT;
    }
    /* Not used:
     *  USB_OTG_GINTSTS_OTGINT       OTG interrupt
     *  USB_OTG_GINTSTS_GINAKEFF     Global IN nonperiodic NAK effective
     *  USB_OTG_GINTSTS_GOUTNAKEFF   Global OUT NAK effective
     *  USB_OTG_GINTSTS_ESUSP        Early suspend
     *  USB_OTG_GINTSTS_ISOODRP      Isochronous OUT packet dropped interrupt
     *  USB_OTG_GINTSTS_EOPF         End of periodic frame interrupt
     *  USB_OTG_GINTSTS_DATAFSUSP    Data fetch suspended
     *  USB_OTG_GINTSTS_CIDSCHG      Connector ID status change
     *  USB_OTG_GINTSTS_SRQINT       Session request/new session detected interrupt
     */
}

/*
 * Get int status register
 * returns : int status register
 */
static  uint32_t usb_ReadDevAllInEPItr(void)
{
    uint32_t v;
    v = USB_FS->DREGS->DAINT;
    v &= USB_FS->DREGS->DAINTMSK;
    return (v & 0xffff);
}

/*
 *  configures EPO to receive SETUP packets
 */
static void usb_EP0_OutStart(void)
{
    uint32_t doeptsize0;
    doeptsize0 = (3<<DEP0XFRSIZ_SUPCNT_OFFSET)
               + (1<<DEP0XFRSIZ_PKTCNT_OFFSET)
               + 8 * 3;
    USB_FS->OUTEP_REGS[0]->DOEPTSIZ = doeptsize0;
}

/*
 * Indicates that an IN EP has a pending Interrupt
 */
static void HandleInEP_ISR(void)
{
    uint32_t  diepint;
    uint32_t ep_intr;
    uint32_t epnum = 0;
    uint32_t fifoemptymsk;
    ep_intr = usb_ReadDevAllInEPItr();
    while(ep_intr)
    {
        if(ep_intr&0x1) /* In ITR */
        {
            diepint = ReadDevInEP(epnum); /* Get In ITR status */
            if(0 != (diepint & USB_OTG_DIEPINT_XFRC))
            {
                fifoemptymsk = 0x1 << epnum;
                USB_FS->DREGS->DIEPEMPMSK &= ~fifoemptymsk;
                USB_FS->INEP_REGS[epnum]->DIEPINT = USB_OTG_DIEPINT_XFRC;
                /* TX COMPLETE */
                DataInStage(epnum);
            }
            if(0 != (diepint & USB_OTG_DIEPINT_TOC))
            {
                USB_FS->INEP_REGS[epnum]->DIEPINT = USB_OTG_DIEPINT_TOC;
            }
            if(0 != (diepint & USB_OTG_DIEPINT_ITTXFE))
            {
                USB_FS->INEP_REGS[epnum]->DIEPINT = USB_OTG_DIEPINT_ITTXFE;
            }
            if(0 != (diepint & USB_OTG_DIEPINT_INEPNE))
            {
                USB_FS->INEP_REGS[epnum]->DIEPINT = USB_OTG_DIEPINT_INEPNE;
            }
            if(0 != (diepint & USB_OTG_DIEPINT_EPDISD))
            {
                USB_FS->INEP_REGS[epnum]->DIEPINT = USB_OTG_DIEPINT_EPDISD;
            }
            if(0 != (diepint & USB_OTG_DIEPINT_TXFE))
            {
                WriteEmptyTxFifo(epnum);
                USB_FS->INEP_REGS[epnum]->DIEPINT = USB_OTG_DIEPINT_TXFE;
            }
        }
        epnum++;
        ep_intr >>= 1;
    }
}

/*
 * returns OUT endpoint interrupt bits
 */
static uint32_t usb_ReadDevAllOutEp_itr(void)
{
    uint32_t v;
    v  = USB_FS->DREGS->DAINT;
    v &= USB_FS->DREGS->DAINTMSK;
    return ((v & 0xffff0000) >> 16);
}

/*
 * returns Device OUT EP Interrupt register
 *  epnum : end point number
 * returns:  Device OUT EP Interrupt register
 */
static uint32_t usb_ReadDevOutEP_itr(uint8_t epnum)
{
    uint32_t v;
    v  = USB_FS->OUTEP_REGS[epnum]->DOEPINT;
    v &= USB_FS->DREGS->DOEPMSK;
    return v;
}

/*
 * Indicates that an OUT EP has a pending Interrupt
 */
static void HandleOutEP_ISR(void)
{
    uint32_t ep_intr;
    uint32_t doepint;
    uint32_t epnum = 0;
    /* Read in the device interrupt bits */
    ep_intr = usb_ReadDevAllOutEp_itr();
    while ( ep_intr )
    {
        if (ep_intr&0x1)
        {
            doepint = usb_ReadDevOutEP_itr(epnum);
            /* Transfer complete */
            if(0 != (doepint & USB_OTG_DOEPINT_XFRC))
            {
                /* Clear the bit in DOEPINTn for this interrupt */
                (*(volatile uint32_t *)&USB_FS->OUTEP_REGS[epnum]->DOEPINT = USB_OTG_DOEPINT_XFRC);
                /* Inform upper layer: data ready */
                /* RX COMPLETE */
                DataOutStage(epnum);
            }
            /* Endpoint disable  */
            if(0 != (doepint & USB_OTG_DOEPINT_EPDISD))
            {
                /* Clear the bit in DOEPINTn for this interrupt */
                (*(volatile uint32_t *)&USB_FS->OUTEP_REGS[epnum]->DOEPINT = USB_OTG_DOEPINT_EPDISD);
            }
            /* Setup Phase Done (control EPs) */
            if(0 != (doepint & USB_OTG_DOEPINT_STUP))
            {
                /* inform the upper layer that a setup packet is available */
                /* SETUP COMPLETE */
                SetupStage();
                (*(volatile uint32_t *)&USB_FS->OUTEP_REGS[epnum]->DOEPINT = USB_OTG_DOEPINT_STUP);
            }
        }
        epnum++;
        ep_intr >>= 1;
    }
}

/*
 * Reads a packet from the Rx FIFO
 *   dest : Destination Pointer
 *   len : No. of bytes
 */
static void usb_ReadPacket(uint8_t *dest8,
                         uint16_t len)
{
    uint32_t i=0;
    uint32_t count32b = (len + 3) / 4;
    __IO uint32_t* dest = (uint32_t *)dest8;

    for ( i = 0; i < count32b; i++, dest += 4 )
    {
        dest = USB_FS->DFIFO[0];
    }
}

/*
 * Handles the Rx Status Queue Level Interrupt
 */
static void HandleRxStatusQueueLevel_ISR(void)
{
    uint32_t int_mask;
    uint32_t status;
    USB_OTG_EP *ep;
    /* Disable the Rx Status Queue Level interrupt */
    int_mask = USB_OTG_GINTMSK_RXFLVLM;
    USB_FS->GREGS->GINTMSK &= ~int_mask;
    /* Get the Status from the top of the FIFO */
    status = USB_FS->GREGS->GRXSTSP;
    ep = &out_ep[status & USB_OTG_GRXSTSP_EPNUM];
    switch((status & USB_OTG_GRXSTSP_PKTSTS)>>USB_OTG_GRXSTSP_PKTSTS_OFFSET)
    {
    case STS_GOUT_NAK:
        break;
    case STS_DATA_UPDT:
        if(status & USB_OTG_GRXSTSP_BCNT)
        {
            uint32_t bcnt = (status & USB_OTG_GRXSTSP_BCNT)>>USB_OTG_GRXSTSP_BCNT_OFFSET;
            usb_ReadPacket(ep->xfer_buff, bcnt);
            ep->xfer_buff += bcnt;
            ep->xfer_count += bcnt;
        }
        break;
    case STS_XFER_COMP:
        break;
    case STS_SETUP_COMP:
        break;
    case STS_SETUP_UPDT:
    {
        uint32_t bcnt = (status & USB_OTG_GRXSTSP_BCNT)>>USB_OTG_GRXSTSP_BCNT_OFFSET;
        /* Copy the setup packet received in FIFO into the setup buffer in RAM */
        usb_ReadPacket(setup_packet, 8);
        ep->xfer_count += bcnt;
        break;
    }
    default:
        break;
    }
    /* Enable the Rx Status Queue Level interrupt */
    USB_FS->GREGS->GINTMSK |= int_mask;
}

/*
 * Flush a Tx FIFO
 *   num : FO num
 */
static void usb_FlushTxFifo(uint32_t num )
{
    uint32_t  greset;
    uint32_t count = 0;
    greset = USB_OTG_GRSTCTL_TXFFLSH | (num<<USB_OTG_GRSTCTL_TXFNUM_OFFSET);
    USB_FS->GREGS->GRSTCTL = greset;
    do
    {
        greset = USB_FS->GREGS->GRSTCTL;
        if (++count > 200000)
        {
            break;
        }
    }while((greset & USB_OTG_GRSTCTL_TXFFLSH) != 0);
    /* Wait for 3 PHY Clocks*/
    uDelay(3);
}

/*
 * Configure an EP
 */
void EP_Open(uint8_t ep_addr,
             uint16_t ep_mps,
             uint8_t ep_type)
{
    USB_OTG_EP *ep;
    if ((ep_addr & 0x80) == 0x80)
    {
        ep = &in_ep[ep_addr & 0x7F];
    }
    else
    {
        ep = &out_ep[ep_addr & 0x7F];
    }
    ep->num   = ep_addr & 0x7F;
    ep->is_in = (0x80 & ep_addr) != 0;
    ep->maxpacket = ep_mps;
    ep->type = ep_type;
    if (ep->is_in)
    {
        /* Assign a Tx FIFO */
        ep->tx_fifo_num = ep->num;
    }
    /* Set initial data PID. */
    if (ep_type == USB_OTG_EP_BULK )
    {
        ep->data_pid_start = 0;
    }
    usb_EPActivate(ep);
}

/*
 * This interrupt occurs when a USB Reset is detected
 */
static void HandleUsbReset_ISR(void)
{
    uint32_t daintmsk;
    uint32_t doepmsk;
    uint32_t  diepmsk;
    uint32_t dcfg;
    uint32_t dctl;
    uint32_t i;
    /* Clear the Remote Wake-up Signaling */
    dctl = USB_OTG_DCTL_RWUSIG;
    USB_FS->DREGS->DCTL &= ~dctl;
    /* Flush the Tx FIFO */
    usb_FlushTxFifo(0);
    for (i = 0; i < MAX_DEVICE_ENDPOINTS; i++)
    {
        USB_FS->INEP_REGS[i]->DIEPINT = 0xFF;
        USB_FS->OUTEP_REGS[i]->DOEPINT = 0xFF;
    }
    USB_FS->DREGS->DAINT = 0xFFFFFFFF;
    daintmsk = 0x00010001;
    USB_FS->DREGS->DAINTMSK = daintmsk;
    doepmsk =  USB_OTG_DOEPMSK_XFRCM
             | USB_OTG_DOEPMSK_EPDM
             | USB_OTG_DOEPMSK_STUPM;
    USB_FS->DREGS->DOEPMSK = doepmsk;
    diepmsk =  USB_OTG_DIEPMSK_XFRCM
             | USB_OTG_DIEPMSK_EPDM
             | USB_OTG_DIEPMSK_TOM
             | USB_OTG_DIEPMSK_INEPNMM;
    USB_FS->DREGS->DIEPMSK = diepmsk;
    /* Reset Device Address */
    dcfg = USB_FS->DREGS->DCFG;
    dcfg &= ~USB_OTG_DCFG_DAD_BITMASK; // Device Address = 0;
    USB_FS->DREGS->DCFG = dcfg;
    /* setup EP0 to receive SETUP packets */
    usb_EP0_OutStart();
    /*Reset internal state machine */
    /* Open EP0 OUT */
    EP_Open(0x00,
            USB_OTG_MAX_EP0_SIZE,
            EP_TYPE_CTRL);
    /* Open EP0 IN */
    EP_Open(0x80,
            USB_OTG_MAX_EP0_SIZE,
            EP_TYPE_CTRL);
    device_status = USB_OTG_DEFAULT;
}

/*
 * enables EP0 OUT to receive SETUP packets and configures EP0
 *   for transmitting packets
 */
static void usb_EP0Activate(void)
{
    uint32_t dsts;
    uint32_t diepctl;
    uint32_t dctl;
    /* Read the Device Status and Endpoint 0 Control registers */
    dsts = USB_FS->DREGS->DSTS;
    diepctl = USB_FS->INEP_REGS[0]->DIEPCTL;
    /* Set the MPS of the IN EP based on the enumeration speed */
    dsts = (dsts & USB_OTG_DSTS_ENUMSPD)>>USB_OTG_DSTS_ENUMSPD_OFFSET;
    switch (dsts)
    {
    case DSTS_ENUMSPD_HS_PHY_30MHZ_OR_60MHZ:
    case DSTS_ENUMSPD_FS_PHY_30MHZ_OR_60MHZ:
    case DSTS_ENUMSPD_FS_PHY_48MHZ:
        diepctl &= ~USB_OTG_DIEPCTL_MPSIZ;
        diepctl |= DEP0CTL_MPS_64;
        break;
    case DSTS_ENUMSPD_LS_PHY_6MHZ:
        diepctl &= ~USB_OTG_DIEPCTL_MPSIZ;
        diepctl |= DEP0CTL_MPS_8;
        break;
    }
    USB_FS->INEP_REGS[0]->DIEPCTL = diepctl;
    dctl = USB_OTG_DCTL_CGINAK;
    USB_FS->DREGS->DCTL |= dctl;
}

/*
 * Read the device status register and set the device speed
 */
static void HandleEnumDone_ISR(void)
{
    uint32_t gusbcfg;
    usb_EP0Activate();
    /* Set USB turn-around time based on device speed and PHY interface. */
    gusbcfg = USB_FS->GREGS->GUSBCFG;
    gusbcfg &= ~USB_OTG_GUSBCFG_TRDT_BITMASK;
    gusbcfg |= 5<<USB_OTG_GUSBCFG_TRDT_OFFSET;
    USB_FS->GREGS->GUSBCFG = gusbcfg;
}

/*
 * Set the EP Status
 *   Status : new Status
 *       ep : EP structure
 */
static void usb_SetEPStatus(USB_OTG_EP *ep , uint32_t Status)
{
    uint32_t  depctl;
    /* Process for IN endpoint */
    if (ep->is_in == 1)
    {
        depctl = USB_FS->INEP_REGS[ep->num]->DIEPCTL;
        if (Status == USB_OTG_EP_TX_STALL)
        {
            usb_EPSetStall(ep); return;
        }
        else if (Status == USB_OTG_EP_TX_NAK)
        {
            depctl |= USB_OTG_DIEPCTL_SNAK;
        }
        else if (Status == USB_OTG_EP_TX_VALID)
        {
            if(0 != (depctl & USB_OTG_DIEPCTL_STALL))
            {
                ep->even_odd_frame = 0;
                usb_EPClearStall(ep);
                return;
            }
            depctl |= USB_OTG_DIEPCTL_CNAK;
            depctl |= USB_OTG_DIEPCTL_USBAEP;
            depctl |= USB_OTG_DIEPCTL_EPENA;
        }
        else if (Status == USB_OTG_EP_TX_DIS)
        {
            depctl &= ~USB_OTG_DIEPCTL_USBAEP;
        }
        USB_FS->INEP_REGS[ep->num]->DIEPCTL = depctl;
    }
    else /* Process for OUT endpoint */
    {
        depctl = USB_FS->OUTEP_REGS[ep->num]->DOEPCTL;
        if (Status == USB_OTG_EP_RX_STALL)
        {
            depctl |= USB_OTG_DIEPCTL_STALL;
        }
        else if (Status == USB_OTG_EP_RX_NAK)
        {
            depctl |= USB_OTG_DIEPCTL_SNAK;
        }
        else if (Status == USB_OTG_EP_RX_VALID)
        {
            if(0 != (depctl & USB_OTG_DIEPCTL_STALL))
            {
                ep->even_odd_frame = 0;
                usb_EPClearStall(ep);
                return;
            }
            depctl |= USB_OTG_DIEPCTL_CNAK;
            depctl |= USB_OTG_DIEPCTL_USBAEP;
            depctl |= USB_OTG_DIEPCTL_EPENA;
        }
        else if (Status == USB_OTG_EP_RX_DIS)
        {
            depctl &= ~USB_OTG_DIEPCTL_USBAEP;
        }
        USB_FS->OUTEP_REGS[ep->num]->DOEPCTL = depctl;
    }
}

/*
 * returns the EP Status
 *         ep : endpoint structure
 * returns : EP status
 */
static uint32_t usb_GetEPStatus(USB_OTG_EP *ep)
{
    uint32_t  depctl;
    uint32_t Status = 0;
    if (ep->is_in == 1)
    {
        depctl = USB_FS->INEP_REGS[ep->num]->DIEPCTL;
        if(0 != (depctl & USB_OTG_DIEPCTL_STALL))
        {
            Status = USB_OTG_EP_TX_STALL;
        }
        else if(0 != (depctl & USB_OTG_DIEPCTL_NAKSTS))
        {
            Status = USB_OTG_EP_TX_NAK;
        }
        else
        {
            Status = USB_OTG_EP_TX_VALID;
        }
    }
    else
    {
        depctl = USB_FS->OUTEP_REGS[ep->num]->DOEPCTL;
        if(0 != (depctl & USB_OTG_DIEPCTL_STALL))
        {
            Status = USB_OTG_EP_RX_STALL;
        }
        else if(0 != (depctl & USB_OTG_DIEPCTL_NAKSTS))
        {
            Status = USB_OTG_EP_RX_NAK;
        }
        else
        {
            Status = USB_OTG_EP_RX_VALID;
        }
    }
    /* Return the current status */
    return Status;
}

/*
 * Clear the EP STALL
 */
static void usb_EPClearStall(USB_OTG_EP *ep)
{
    uint32_t  depctl;
    if(ep->is_in == 1)
    {
        depctl = USB_FS->INEP_REGS[ep->num]->DIEPCTL;
    }
    else
    {
        depctl = USB_FS->OUTEP_REGS[ep->num]->DOEPCTL;
    }
    /* clear the stall bits */
    depctl &= ~USB_OTG_DIEPCTL_STALL;
    if (ep->type == EP_TYPE_INTR || ep->type == EP_TYPE_BULK)
    {
        depctl |= USB_OTG_DIEPCTL_SD0PID_SEVNFRM;
    }
    if(ep->is_in == 1)
    {
        USB_FS->INEP_REGS[ep->num]->DIEPCTL = depctl;
    }
    else
    {
        USB_FS->OUTEP_REGS[ep->num]->DOEPCTL = depctl;
    }
}

/*
 * Set the EP STALL
 */
static void usb_EPSetStall(USB_OTG_EP *ep)
{
    uint32_t  depctl;
    if (ep->is_in == 1)
    {
        depctl = USB_FS->INEP_REGS[ep->num]->DIEPCTL;
        /* set the disable and stall bits */
        if(0 != (depctl & USB_OTG_DIEPCTL_EPENA))
        {
            depctl |= USB_OTG_DIEPCTL_EPDIS;
        }
        depctl |= USB_OTG_DIEPCTL_STALL;
        USB_FS->INEP_REGS[ep->num]->DIEPCTL = depctl;
    }
    else
    {
        depctl = USB_FS->OUTEP_REGS[ep->num]->DOEPCTL;
        /* set the stall bit */
        depctl |= USB_OTG_DIEPCTL_STALL;
        USB_FS->OUTEP_REGS[ep->num]->DOEPCTL = depctl;
    }
}

/*
 * Handle the setup for a data xfer for EP0 and starts the xfer
 */
static void usb_EP0StartXfer(USB_OTG_EP *ep)
{
    uint32_t depctl;
    uint32_t deptsiz;
    USB_OTG_INEPREGS          *in_regs;
    uint32_t fifoemptymsk = 0;
    /* IN endpoint */
    if (ep->is_in == 1)
    {
        in_regs = USB_FS->INEP_REGS[0];
        depctl  = in_regs->DIEPCTL;
        deptsiz = in_regs->DIEPTSIZ;
        /* Zero Length Packet? */
        deptsiz &= ~BITMASK_DEP0XFRSIZ_XFRSIZ;
        if (ep->xfer_len == 0)
        {
            // XFRSIZE = 0
        }
        else
        {
            if (ep->xfer_len > ep->maxpacket)
            {
                ep->xfer_len = ep->maxpacket;
                deptsiz |= ep->maxpacket;
            }
            else
            {
                deptsiz |= ep->xfer_len;
            }
        }
        deptsiz &= ~BITMASK_DEP0XFRSIZ_PKTCNT;
        deptsiz |= (1<<DEP0XFRSIZ_PKTCNT_OFFSET);
        in_regs->DIEPTSIZ = deptsiz;
        /* EP enable, IN data in FIFO */
        depctl |= USB_OTG_DIEPCTL_EPENA | USB_OTG_DIEPCTL_CNAK;
        in_regs->DIEPCTL = depctl;
        /* Enable the Tx FIFO Empty Interrupt for this EP */
        if (ep->xfer_len > 0)
        {
            fifoemptymsk |= 1 << ep->num;
            USB_FS->DREGS->DIEPEMPMSK |= fifoemptymsk;
        }
    }
    else
    {
        /* OUT endpoint */
        depctl = USB_FS->OUTEP_REGS[ep->num]->DOEPCTL;
        deptsiz = USB_FS->OUTEP_REGS[ep->num]->DOEPTSIZ;
        /* Program the transfer size and packet count as follows:
        * xfersize = N * (maxpacket + 4 - (maxpacket % 4))
        * pktcnt = N           */
        if (ep->xfer_len == 0)
        {
        }
        else
        {
            ep->xfer_len = ep->maxpacket;
        }
        deptsiz &= ~BITMASK_DEP0XFRSIZ_XFRSIZ;
        deptsiz |= ep->maxpacket;
        deptsiz &= ~BITMASK_DEP0XFRSIZ_PKTCNT;
        deptsiz |= (1<<DEP0XFRSIZ_PKTCNT_OFFSET);
        USB_FS->OUTEP_REGS[ep->num]->DOEPTSIZ = deptsiz;
        /* EP enable */
        depctl |= USB_OTG_DIEPCTL_EPENA | USB_OTG_DIEPCTL_CNAK;
        USB_FS->OUTEP_REGS[ep->num]->DOEPCTL = depctl;
    }
}

/*
 * Handle the setup for data xfer for an EP and starts the xfer
 */
static void usb_EPStartXfer(USB_OTG_EP *ep)
{
    uint32_t depctl;
    uint32_t deptsiz;
    uint32_t dsts;
    uint32_t fifoemptymsk = 0;
    /* IN endpoint */
    if (ep->is_in == 1)
    {
        depctl = USB_FS->INEP_REGS[ep->num]->DIEPCTL;
        deptsiz = USB_FS->INEP_REGS[ep->num]->DIEPTSIZ;
        /* Zero Length Packet? */
        if (ep->xfer_len == 0)
        {
            deptsiz &= ~USB_OTG_DOEPTSIZ_XFRSIZ;
            deptsiz &= ~USB_OTG_DOEPTSIZ_PKTCNT;
            deptsiz |= 1 << DOEPTSIZ_PKTCNT_OFFSET;
        }
        else
        {
            /* Program the transfer size and packet count
            * as follows: xfersize = N * maxpacket +
            * short_packet pktcnt = N + (short_packet
            * exist ? 1 : 0)
            */
            deptsiz &= ~USB_OTG_DOEPTSIZ_XFRSIZ;
            deptsiz |= ep->xfer_len;
            deptsiz &= ~USB_OTG_DOEPTSIZ_PKTCNT;
            deptsiz |= ((ep->xfer_len - 1 + ep->maxpacket) / ep->maxpacket) << DOEPTSIZ_PKTCNT_OFFSET;
            if (ep->type == EP_TYPE_ISOC)
            {
                deptsiz &= ~USB_OTG_DOEPTSIZ_STUPCNT;
                deptsiz |= (1 << DOEPTSIZ_RXDPID_STUPCNT_OFFSET);
            }
        }
        USB_FS->INEP_REGS[ep->num]->DIEPTSIZ = deptsiz;
            if (ep->type != EP_TYPE_ISOC)
            {
                /* Enable the Tx FIFO Empty Interrupt for this EP */
                if (ep->xfer_len > 0)
                {
                    fifoemptymsk = 1 << ep->num;
                    USB_FS->DREGS->DIEPEMPMSK |= fifoemptymsk;
                }
            }
        if (ep->type == EP_TYPE_ISOC)
        {
            dsts = USB_FS->DREGS->DSTS;
            if((dsts & USB_OTG_DSTS_FNSOF_0) == 0)
            {
                depctl |= USB_OTG_DIEPCTL_SODDFRM;
            }
            else
            {
                depctl |= USB_OTG_DIEPCTL_SD0PID_SEVNFRM;
            }
        }
        /* EP enable, IN data in FIFO */
        depctl |= USB_OTG_DIEPCTL_EPENA | USB_OTG_DIEPCTL_CNAK;
        USB_FS->INEP_REGS[ep->num]->DIEPCTL = depctl;
        if (ep->type == EP_TYPE_ISOC)
        {
            usb_WritePacket(ep->xfer_buff, ep->num, ep->xfer_len);
        }
    }
    else
    {
        /* OUT endpoint */
        depctl = USB_FS->OUTEP_REGS[ep->num]->DOEPCTL;
        deptsiz = USB_FS->OUTEP_REGS[ep->num]->DOEPTSIZ;
        /* Program the transfer size and packet count as follows:
        * pktcnt = N
        * xfersize = N * maxpacket
        */
        if (ep->xfer_len == 0)
        {
            deptsiz &= ~USB_OTG_DOEPTSIZ_XFRSIZ;
            deptsiz |= ep->maxpacket;
            deptsiz &= ~USB_OTG_DOEPTSIZ_PKTCNT;
            deptsiz |= 1<<DOEPTSIZ_PKTCNT_OFFSET;
        }
        else
        {
            uint32_t pktcnt = (ep->xfer_len + (ep->maxpacket - 1)) / ep->maxpacket;
            deptsiz &= ~USB_OTG_DOEPTSIZ_PKTCNT;
            deptsiz |= pktcnt<<DOEPTSIZ_PKTCNT_OFFSET;
            deptsiz &= ~USB_OTG_DOEPTSIZ_XFRSIZ;
            deptsiz |= pktcnt * ep->maxpacket;
        }
        USB_FS->OUTEP_REGS[ep->num]->DOEPTSIZ = deptsiz;

        if (ep->type == EP_TYPE_ISOC)
        {
            if (ep->even_odd_frame)
            {
                depctl |= USB_OTG_DIEPCTL_SODDFRM;
            }
            else
            {
                depctl |= USB_OTG_DIEPCTL_SD0PID_SEVNFRM;
            }
        }
        /* EP enable */
        depctl |= USB_OTG_DIEPCTL_EPENA | USB_OTG_DIEPCTL_CNAK;
        USB_FS->OUTEP_REGS[ep->num]->DOEPCTL = depctl;
    }
}

/*
 *  Activates an EP
 */
static void usb_EPActivate(USB_OTG_EP *ep)
{
    uint32_t depctl;
    uint32_t daintmsk;
    /* Read DEPCTLn register */
    if (ep->is_in == 1)
    {
        depctl = USB_FS->INEP_REGS[ep->num]->DIEPCTL;
        daintmsk = 1 << ep->num;
    }
    else
    {
        depctl = USB_FS->OUTEP_REGS[ep->num]->DOEPCTL;
        daintmsk = 1 << (ep->num + 16);
    }
    /* If the EP is already active don't change the EP Control
    * register. */
    if(0 == (depctl & USB_OTG_DIEPCTL_USBAEP))
    {
        depctl &= ~USB_OTG_DIEPCTL_MPSIZ;
        depctl |= ep->maxpacket;
        depctl &= ~USB_OTG_DIEPCTL_EPTYP;
        depctl |= ep->type<<USB_OTG_DIEPCTL_EPTYP_OFFSET;
        depctl &= ~USB_OTG_DIEPCTL_TXFNUM;
        depctl |= ep->tx_fifo_num<<USB_OTG_DIEPCTL_TXFNUM_OFFSET;
        depctl |= USB_OTG_DIEPCTL_SD0PID_SEVNFRM;
        depctl |= USB_OTG_DIEPCTL_USBAEP;
        if (ep->is_in == 1)
        {
            USB_FS->INEP_REGS[ep->num]->DIEPCTL = depctl;
        }
        else
        {
            USB_FS->OUTEP_REGS[ep->num]->DOEPCTL = depctl;
        }
    }
    /* Enable the Interrupt for this EP */
    USB_FS->DREGS->DAINTMSK |= daintmsk;
}

/*
 * Deactivates an EP
 */
static void USB_OTG_EPDeactivate(USB_OTG_EP *ep)
{
    uint32_t depctl;
    uint32_t daintmsk;
    /* Read DEPCTLn register */
    if (ep->is_in == 1)
    {
        daintmsk = 1 << ep->num;
        depctl = USB_OTG_DIEPCTL_USBAEP;
        USB_FS->INEP_REGS[ep->num]->DIEPCTL = depctl;
    }
    else
    {
        daintmsk = 1 << (ep->num + 16);
        depctl = USB_OTG_DIEPCTL_USBAEP;
        USB_FS->OUTEP_REGS[ep->num]->DOEPCTL = depctl;
    }
    /* Disable the Interrupt for this EP */
    USB_FS->DREGS->DAINTMSK &= ~daintmsk;
}

/*
 * Enables the Device mode interrupts
 */
static void usb_EnableDevInt(void)
{
    uint32_t intmsk;
    /* Disable all interrupts. */
    USB_FS->GREGS->GINTMSK = 0;
    /* Clear any pending interrupts */
    USB_FS->GREGS->GINTSTS = 0xFFFFFFFF;
    /* Enable the common interrupts */
    usb_EnableCommonInt();
    /* Enable interrupts matching to the Device mode ONLY */
    intmsk =  USB_OTG_GINTMSK_RXFLVLM
            | USB_OTG_GINTMSK_USBSUSPM
            | USB_OTG_GINTMSK_USBRST
            | USB_OTG_GINTMSK_ENUMDNEM
            | USB_OTG_GINTMSK_IEPINT
            | USB_OTG_GINTMSK_OEPINT
            | USB_OTG_GINTMSK_SOFM
            | USB_OTG_GINTMSK_IISOIXFRM
            | USB_OTG_GINTMSK_PXFRM_IISOOXFRM;
    USB_FS->GREGS->GINTMSK |= intmsk;
}

/*
 *  Initializes the USB_OTG controller registers for device mode
 */
static void usb_CoreInitDev(void)
{
    uint32_t depctl;
    uint32_t i;
    uint32_t dcfg;
    uint32_t fifo_reg = 0;
    uint32_t fifo_start = 0;
    /* Restart the Phy Clock */
    USB_FS->PCGCCTL = 0;
    /* Device configuration register */
    dcfg = USB_FS->DREGS->DCFG;
    dcfg &= ~USB_OTG_DCFG_PFIVL_BITMASK;
    dcfg |= DCFG_FRAME_INTERVAL_80<<USB_OTG_DCFG_PFIVL_OFFSET;
    USB_FS->DREGS->DCFG = dcfg;
    /* Set Full speed phy */
    usb_InitDevSpeed();
    /* set Rx FIFO size */
    USB_FS->GREGS->GRXFSIZ = RX_FIFO_FS_SIZE;
    /* EP0 TX*/
    fifo_start = RX_FIFO_FS_SIZE;
    fifo_reg = (TX0_FIFO_FS_SIZE<<16) + fifo_start;
    USB_FS->GREGS->DIEPTXF0_HNPTXFSIZ = fifo_reg;
    /* EP1 TX*/
    fifo_start = fifo_start + TX0_FIFO_FS_SIZE;
    fifo_reg = (TX1_FIFO_FS_SIZE<<16) + fifo_start;
    USB_FS->GREGS->DIEPTXF[0] = fifo_reg;
    /* EP2 TX*/
    fifo_start += TX1_FIFO_FS_SIZE;
    fifo_reg = (TX2_FIFO_FS_SIZE<<16) + fifo_start;
    USB_FS->GREGS->DIEPTXF[1] = fifo_reg;
    /* EP3 TX*/
    fifo_start += TX2_FIFO_FS_SIZE;
    fifo_reg = (TX3_FIFO_FS_SIZE<<16) + fifo_start;
    USB_FS->GREGS->DIEPTXF[2] = fifo_reg;
    /* Flush the FIFOs */
    usb_FlushTxFifo(0x10); /* all Tx FIFOs */
    usb_FlushRxFifo();
    /* Clear all pending Device Interrupts */
    USB_FS->DREGS->DIEPMSK = 0 ;
    USB_FS->DREGS->DOEPMSK = 0;
    USB_FS->DREGS->DAINT = 0xFFFFFFFF;
    USB_FS->DREGS->DAINTMSK = 0;
    for (i = 0; i < MAX_DEVICE_ENDPOINTS; i++)
    {
        depctl = USB_FS->INEP_REGS[i]->DIEPCTL;
        if(0 != (depctl & USB_OTG_DIEPCTL_EPENA))
        {
            depctl = USB_OTG_DIEPCTL_SNAK | USB_OTG_DIEPCTL_EPDIS;
        }
        else
        {
            depctl = 0;
        }
        USB_FS->INEP_REGS[i]->DIEPCTL = depctl;
        USB_FS->INEP_REGS[i]->DIEPTSIZ = 0;
        USB_FS->INEP_REGS[i]->DIEPINT = 0xFF;
    }
    for (i = 0; i <  MAX_DEVICE_ENDPOINTS; i++)
    {
        depctl = USB_FS->OUTEP_REGS[i]->DOEPCTL;
        if(0 != (depctl & USB_OTG_DIEPCTL_EPENA))
        {
            depctl = USB_OTG_DIEPCTL_SNAK | USB_OTG_DIEPCTL_EPDIS;
        }
        else
        {
            depctl = 0;
        }
        USB_FS->OUTEP_REGS[i]->DOEPCTL = depctl;
        USB_FS->OUTEP_REGS[i]->DOEPTSIZ = 0;
        USB_FS->OUTEP_REGS[i]->DOEPINT = 0xFF;
    }
    usb_EnableDevInt();
}

/*
 * Initializes the DevSpd field of DCFG register
 * depending the PHY type and the enumeration speed of the device.
 */
static void usb_InitDevSpeed(void)
{
    uint32_t dcfg;
    dcfg = USB_FS->DREGS->DCFG;
    dcfg |= USB_OTG_DCFG_DSPD;
    USB_FS->DREGS->DCFG = dcfg;
}

/*
 *  Flush a Rx FIFO
 */
static void usb_FlushRxFifo(void)
{
    uint32_t greset;
    uint32_t count = 0;
    greset = USB_OTG_GRSTCTL_RXFFLSH;
    USB_FS->GREGS->GRSTCTL = greset;
    do
    {
        greset = USB_FS->GREGS->GRSTCTL;
        if (++count > 200000)
        {
            break;
        }
    } while((greset & USB_OTG_GRSTCTL_RXFFLSH) != 0);
    /* Wait for 3 PHY Clocks*/
    uDelay(3);
}

/*
 * Initializes the USB_OTG controller registers and prepares the core
 * device mode or host mode operation.
 */
static void usb_CoreInit(void)
{
    uint32_t usbcfg;
    uint32_t gccfg;
    usbcfg = USB_FS->GREGS->GUSBCFG;
    usbcfg |= USB_OTG_GUSBCFG_PHYSEL; /* FS Interface */
    USB_FS->GREGS->GUSBCFG = usbcfg;
    /* Reset after a PHY select and set Host mode */
    usb_CoreReset();
    /* Enable the I2C interface and deactivate the power down*/
    gccfg = USB_OTG_GCCFG_PWRDWN | USB_OTG_GCCFG_VBUSASEN | USB_OTG_GCCFG_VBUSBSEN;
    USB_FS->GREGS->GCCFG = gccfg;
    mDelay(20);
    /* Program GUSBCFG.OtgUtmifsSel to I2C*/
    usbcfg = USB_FS->GREGS->GUSBCFG;
    USB_FS->GREGS->GUSBCFG = usbcfg;
}

/*
 * Initializes the common interrupts, used in both device and modes
 */
static void usb_EnableCommonInt(void)
{
    uint32_t int_mask;
    /* Clear any pending interrupts */
    USB_FS->GREGS->GINTSTS = 0xFFFFFFFF;
    /* Enable the interrupts in the INTMSK */
    int_mask = USB_OTG_GINTMSK_WUIM | USB_OTG_GINTMSK_USBSUSPM;
    USB_FS->GREGS->GINTMSK = int_mask;
}

/*
 * Soft reset of the core
 */
static void usb_CoreReset(void)
{
    uint32_t greset;
    uint32_t count = 0;
    /* Wait for AHB master IDLE state. */
    do
    {
        uDelay(3);
        greset = USB_FS->GREGS->GRSTCTL;
        if (++count > 200000)
        {
            return;
        }
    } while((greset & USB_OTG_GRSTCTL_AHBIDL) != 0);
    /* Core Soft Reset */
    count = 0;
    greset |= USB_OTG_GRSTCTL_CSRST;
    USB_FS->GREGS->GRSTCTL = greset;
    do
    {
        greset = USB_FS->GREGS->GRSTCTL;
        if (++count > 200000)
        {
            break;
        }
    } while ((greset & USB_OTG_GRSTCTL_CSRST) != 0);
    /* Wait for 3 PHY Clocks*/
    uDelay(3);
}

/*
 * Transmit data over USB
 *  ep_addr: endpoint address
 *  pbuf: pointer to Tx buffer
 *  buf_len: data length
 */
void EP_Tx(uint8_t  ep_addr,
           uint8_t* pbuf,
           uint32_t buf_len)
{
    USB_OTG_EP *ep;
    ep = &in_ep[ep_addr & 0x7F];
    /* Setup and start the Transfer */
    ep->is_in = 1;
    ep->num = ep_addr & 0x7F;
    ep->xfer_buff = pbuf;
    ep->dma_addr = (uint32_t)pbuf;
    ep->xfer_count = 0;
    ep->xfer_len  = buf_len;
    if ( ep->num == 0 )
    {
        usb_EP0StartXfer(ep);
    }
    else
    {
        usb_EPStartXfer(ep);
    }
}

/*
 * send data on the ctl pipe
 * @param  buff: pointer to data buffer
 * @param  len: length of data to be sent
 */
void  usb_CtlSendData(uint8_t *pbuf, uint16_t len)
{
    in_ep[0].total_data_len = len;
    in_ep[0].rem_data_len   = len;
    device_state = USB_OTG_EP0_DATA_IN;
    EP_Tx(0, pbuf, len);
}

/*
 * send zero length packet on the ctrl pipe
 */
static void  usb_CtlSendStatus(void)
{
    device_state = USB_OTG_EP0_STATUS_IN;
    EP_Tx(0, NULL, 0);
    usb_EP0_OutStart();
}

/*
 *  Stall an endpoint.
 *  epnum: endpoint address
 */
static void  EP_Stall(uint8_t epnum)
{
    USB_OTG_EP *ep;
    if ((0x80 & epnum) == 0x80)
    {
        ep = &in_ep[epnum & 0x7F];
    }
    else
    {
        ep = &out_ep[epnum];
    }
    ep->is_stall = 1;
    ep->num   = epnum & 0x7F;
    ep->is_in = ((epnum & 0x80) == 0x80);
    usb_EPSetStall(ep);
}

/*
 * Handle the setup stage
 */
static void SetupStage(void)
{
    USB_SETUP_REQ req;
    usb_ParseSetupRequest(&req);
    switch (req.bmRequest & 0x1F)
    {
    case USB_REQ_RECIPIENT_DEVICE:
        usb_StdDevReq(&req);
        break;

    case USB_REQ_RECIPIENT_INTERFACE:
        usb_StdItfReq(&req);
        break;

    case USB_REQ_RECIPIENT_ENDPOINT:
        usb_StdEPReq(&req);
        break;

    default:
        EP_Stall(req.bmRequest & 0x80);
        break;
    }
}

/*
 * Copy buffer into setup structure
 */
static void usb_ParseSetupRequest(USB_SETUP_REQ *req)
{
    req->bmRequest     = *(uint8_t *)  (setup_packet);
    req->bRequest      = *(uint8_t *)  (setup_packet +  1);
    req->wValue        = SWAPBYTE      (setup_packet +  2);
    req->wIndex        = SWAPBYTE      (setup_packet +  4);
    req->wLength       = SWAPBYTE      (setup_packet +  6);
    in_ep[0].ctl_data_len = req->wLength  ;
    device_state = USB_OTG_EP0_SETUP;
}

/*
 * called when an EP is disabled
 * ep_addr: endpoint address
 */
void EP_Close(uint8_t  ep_addr)
{
    USB_OTG_EP *ep;

    if ((ep_addr&0x80) == 0x80)
    {
        ep = &in_ep[ep_addr & 0x7F];
    }
    else
    {
        ep = &out_ep[ep_addr & 0x7F];
    }
    ep->num   = ep_addr & 0x7F;
    ep->is_in = (0x80 & ep_addr) != 0;
    USB_OTG_EPDeactivate(ep);
}

/*
 *  Handle USB low level Error
 */
void usb_CtlError(USB_SETUP_REQ *req)
{
    if((req->bmRequest & 0x80) == 0x80)
    {
        EP_Stall(0x80);
    }
    else
    {
        if(req->wLength == 0)
        {
            EP_Stall(0x80);
        }
        else
        {
            EP_Stall(0);
        }
    }
    usb_EP0_OutStart();
}

/*
 * Clear stall condition on endpoints.
 * @param epnum: endpoint address
 */
static void EP_ClrStall(uint8_t epnum)
{
    USB_OTG_EP *ep;
    if((0x80 & epnum) == 0x80)
    {
        ep = &in_ep[epnum & 0x7F];
    }
    else
    {
        ep = &out_ep[epnum];
    }
    ep->is_stall = 0;
    ep->num   = epnum & 0x7F;
    ep->is_in = ((epnum & 0x80) == 0x80);
    usb_EPClearStall(ep);
}

/*
 * Handle standard usb endpoint requests
 */
static void usb_StdEPReq(USB_SETUP_REQ  *req)
{
    uint8_t   ep_addr;
    ep_addr  = LOBYTE(req->wIndex);
    switch(req->bRequest)
    {
    case USB_REQ_SET_FEATURE :
        switch(device_status)
        {
        case USB_OTG_ADDRESSED:
            if((ep_addr != 0x00) && (ep_addr != 0x80))
            {
                EP_Stall(ep_addr);
            }
        break;

        case USB_OTG_CONFIGURED:
            if(req->wValue == USB_FEATURE_EP_HALT)
            {
                if((ep_addr != 0x00) && (ep_addr != 0x80))
                {
                    EP_Stall(ep_addr);
                }
            }
            class_cb->Setup(req);
            usb_CtlSendStatus();
            break;

        default:
            usb_CtlError(req);
            break;
        }
        break;

    case USB_REQ_CLEAR_FEATURE :
        switch(device_status)
        {
        case USB_OTG_ADDRESSED:
            if ((ep_addr != 0x00) && (ep_addr != 0x80))
            {
                EP_Stall(ep_addr);
            }
            break;

        case USB_OTG_CONFIGURED:
            if (req->wValue == USB_FEATURE_EP_HALT)
            {
                if ((ep_addr != 0x00) && (ep_addr != 0x80))
                {
                    EP_ClrStall(ep_addr);
                    class_cb->Setup(req);
                }
                usb_CtlSendStatus();
            }
            break;

        default:
            usb_CtlError(req);
            break;
        }
        break;

    case USB_REQ_GET_STATUS:
        switch(device_status)
        {
        case USB_OTG_ADDRESSED:
            if ((ep_addr != 0x00) && (ep_addr != 0x80))
            {
                EP_Stall(ep_addr);
            }
            break;

        case USB_OTG_CONFIGURED:
            if((ep_addr & 0x80)== 0x80)
            {
                if(in_ep[ep_addr & 0x7F].is_stall)
                {
                    USBD_ep_status = 0x0001;
                }
                else
                {
                    USBD_ep_status = 0x0000;
                }
            }
            else if((ep_addr & 0x80)== 0x00)
            {
                if(out_ep[ep_addr].is_stall)
                {
                    USBD_ep_status = 0x0001;
                }
                else
                {
                    USBD_ep_status = 0x0000;
                }
            }
            usb_CtlSendData ((uint8_t *)&USBD_ep_status, 2);
            break;

        default:
            usb_CtlError(req);
            break;
        }
        break;

    default:
        break;
    }
}

/*
 *  Handle standard usb interface requests
 */
static void usb_StdItfReq(USB_SETUP_REQ  *req)
{
    switch(device_status)
    {
    case USB_OTG_CONFIGURED:
        if (LOBYTE(req->wIndex) <= USBD_ITF_MAX_NUM)
        {
            class_cb->Setup(req);
            if(req->wLength == 0)
            {
                usb_CtlSendStatus();
            }
        }
        else
        {
            usb_CtlError(req);
        }
        break;

    default:
        usb_CtlError(req);
        break;
    }
}

/**
 * Handle standard usb device requests
 */
static void usb_StdDevReq(USB_SETUP_REQ  *req)
{
    switch (req->bRequest)
    {
    case USB_REQ_GET_DESCRIPTOR:
        if(USB_DESC_TYPE_CONFIGURATION == (req->wValue >> 8))
        {
            uint16_t len;
            uint8_t *pbuf;
            pbuf = class_cb->GetConfigDescriptor(&len);
            pbuf[1] = USB_DESC_TYPE_CONFIGURATION;
            set_config_descriptor(pbuf);
        }
        else
        {
            GetDescriptor(req);
        }
        break;

    case USB_REQ_SET_ADDRESS:
        usb_SetAddress(req);
        break;

    case USB_REQ_SET_CONFIGURATION:
        usb_SetConfig(req);
        break;

    case USB_REQ_GET_CONFIGURATION:
        usb_GetConfig(req);
        break;

    case USB_REQ_GET_STATUS:
        usb_GetStatus(req);
        break;

    case USB_REQ_SET_FEATURE:
        usb_SetFeature(req);
        break;

    case USB_REQ_CLEAR_FEATURE:
        usb_ClrFeature(req);
        break;

    default:
        usb_CtlError(req);
        break;
    }
}

/*
 * Handle clear device feature request
 */
static void usb_ClrFeature(USB_SETUP_REQ *req)
{
    switch(device_status)
    {
    case USB_OTG_ADDRESSED:
    case USB_OTG_CONFIGURED:
        if (req->wValue == USB_FEATURE_REMOTE_WAKEUP)
        {
            DevRemoteWakeup = 0;
            class_cb->Setup(req);
            usb_CtlSendStatus();
        }
        break;

    default :
        usb_CtlError(req);
        break;
    }
}

/*
 * Handle Set device feature request
 */
static void usb_SetFeature(USB_SETUP_REQ *req)
{
    uint32_t dctl;
    uint8_t test_mode = 0;
    if (req->wValue == USB_FEATURE_REMOTE_WAKEUP)
    {
        DevRemoteWakeup = 1;
        class_cb->Setup(req);
        usb_CtlSendStatus();
    }
    else if(  (req->wValue == USB_FEATURE_TEST_MODE)
            &&((req->wIndex & 0xFF) == 0) )
    {
        dctl = USB_FS->DREGS->DCTL;
        test_mode = req->wIndex >> 8;
        dctl &= ~USB_OTG_DCTL_TCTL;
        switch (test_mode)
        {
        case 1: // TEST_J
            dctl |= 1<<USB_OTG_DCTL_TCTL_OFFSET;
            break;
        case 2: // TEST_K
            dctl |= 2<<USB_OTG_DCTL_TCTL_OFFSET;
            break;
        case 3: // TEST_SE0_NAK
            dctl |= 3<<USB_OTG_DCTL_TCTL_OFFSET;
            break;
        case 4: // TEST_PACKET
            dctl |= 4<<USB_OTG_DCTL_TCTL_OFFSET;
            break;
        case 5: // TEST_FORCE_ENABLE
            dctl |= 5<<USB_OTG_DCTL_TCTL_OFFSET;
            break;
        }
        USB_FS->DREGS->DCTL = dctl;
        usb_CtlSendStatus();
    }
}

/*
 * Handle Get Status request
 */
static void usb_GetStatus(USB_SETUP_REQ *req)
{
    switch(device_status)
    {
    case USB_OTG_ADDRESSED:
    case USB_OTG_CONFIGURED:
        if(DevRemoteWakeup)
        {
            USBD_cfg_status = USB_CONFIG_SELF_POWERED | USB_CONFIG_REMOTE_WAKEUP;
        }
        else
        {
            USBD_cfg_status = USB_CONFIG_SELF_POWERED;
        }
        usb_CtlSendData((uint8_t *)&USBD_cfg_status, 1);
        break;

    default :
        usb_CtlError(req);
        break;
    }
}

/*
 * Handle Get device configuration request
 */
static void usb_GetConfig(USB_SETUP_REQ *req)
{
    if (req->wLength != 1)
    {
        usb_CtlError(req);
    }
    else
    {
        switch(device_status)
        {
        case USB_OTG_ADDRESSED:
            usb_CtlSendData((uint8_t *)&USBD_default_cfg, 1);
            break;

        case USB_OTG_CONFIGURED:
            usb_CtlSendData(&device_config, 1);
            break;

        default:
            usb_CtlError(req);
            break;
        }
    }
}

/*
 *  Handle Set device configuration request
 */
static void usb_SetConfig(USB_SETUP_REQ *req)
{
    static uint8_t  cfgidx;
    cfgidx = (uint8_t)(req->wValue);
    if(cfgidx > USB_CFG_MAX_NUM)
    {
        usb_CtlError(req);
    }
    else
    {
        switch(device_status)
        {
        case USB_OTG_ADDRESSED:
            if(cfgidx)
            {
                device_config = cfgidx;
                device_status = USB_OTG_CONFIGURED;
                class_cb->Init(cfgidx);
                usb_CtlSendStatus();
            }
            else
            {
                usb_CtlSendStatus();
            }
            break;

        case USB_OTG_CONFIGURED:
            if(cfgidx == 0)
            {
                device_status = USB_OTG_ADDRESSED;
                device_config = cfgidx;
                class_cb->DeInit(cfgidx);
                usb_CtlSendStatus();
            }
            else if(cfgidx != device_config)
            {
                /* Clear old configuration */
                class_cb->DeInit(device_config);

                /* set new configuration */
                device_config = cfgidx;
                class_cb->Init(cfgidx);
                usb_CtlSendStatus();
            }
            else
            {
                usb_CtlSendStatus();
            }
            break;

        default:
            usb_CtlError(req);
            break;
        }
    }
}

/*
 * This Function set USB device address
 * address: new device address
 */
static void  EP_SetAddress(uint8_t address)
{
    uint32_t dcfg;
    dcfg = address<<USB_OTG_DCFG_DAD_OFFSET;
    USB_FS->DREGS->DCFG |= dcfg;
}

/*
 * Set device address
 */
static void usb_SetAddress(USB_SETUP_REQ *req)
{
    uint8_t  dev_addr;
    if ((req->wIndex == 0) && (req->wLength == 0))
    {
        dev_addr = (uint8_t)(req->wValue) & 0x7F;

        if(device_status == USB_OTG_CONFIGURED)
        {
            usb_CtlError(req);
        }
        else
        {
            device_address = dev_addr;
            EP_SetAddress(dev_addr);
            usb_CtlSendStatus();
            if (dev_addr != 0)
            {
                device_status  = USB_OTG_ADDRESSED;
            }
            else
            {
                device_status  = USB_OTG_DEFAULT;
            }
        }
    }
    else
    {
        usb_CtlError(req);
    }
}

/*
 * End Point PrepareRx
 *  ep_addr: endpoint address
 *  pbuf: pointer to Rx buffer
 *  buf_len: data length
 */
void EP_PrepareRx(uint8_t   ep_addr,
                  uint8_t*  pbuf,
                  uint16_t  buf_len)
{
    USB_OTG_EP *ep;
    ep = &out_ep[ep_addr & 0x7F];
    /*setup and start the Xfer */
    ep->xfer_buff = pbuf;
    ep->xfer_len = buf_len;
    ep->xfer_count = 0;
    ep->is_in = 0;
    ep->num = ep_addr & 0x7F;
    if(ep->num == 0)
    {
        usb_EP0StartXfer(ep);
    }
    else
    {
        usb_EPStartXfer(ep);
    }
}

/*
 * Handle data out stage
 *  epnum: endpoint index
 */
static void DataOutStage(uint8_t epnum)
{
    USB_OTG_EP *ep;
    if(epnum == 0)
    {
        ep = &out_ep[0];
        if(device_state == USB_OTG_EP0_DATA_OUT)
        {
            if(ep->rem_data_len > ep->maxpacket)
            {
                ep->rem_data_len -=  ep->maxpacket;
                /*
                * continue receive data on the ctl pipe
                */
                EP_PrepareRx (0, ep->xfer_buff, MIN(ep->rem_data_len ,ep->maxpacket));
            }
            else
            {
                if(   (class_cb->EP0_RxReady != NULL)
                   && (device_status == USB_OTG_CONFIGURED) )
                {
                    class_cb->EP0_RxReady();
                }
                usb_CtlSendStatus();
            }
        }
    }
    else if(   (class_cb->DataOut != NULL)
            && (device_status == USB_OTG_CONFIGURED))
    {
        class_cb->DataOut(epnum);
    }
}

/*
 * Handle data in stage
 *  epnum: endpoint index
 */
static void DataInStage(uint8_t epnum)
{
    USB_OTG_EP *ep;
    if(epnum == 0)
    {
        ep = &in_ep[0];
        if(device_state == USB_OTG_EP0_DATA_IN)
        {
            if(ep->rem_data_len > ep->maxpacket)
            {
                ep->rem_data_len -=  ep->maxpacket;
                EP_Tx(0, ep->xfer_buff, ep->rem_data_len);
            }
            else
            { /* last packet is MPS multiple, so send ZLP packet */
                if(   (ep->total_data_len % ep->maxpacket == 0)
                   && (ep->total_data_len >= ep->maxpacket)
                   && (ep->total_data_len < ep->ctl_data_len ) )
                {
                    EP_Tx(0, NULL, 0);
                    ep->ctl_data_len = 0;
                }
                else
                {
                    if(   (class_cb->EP0_TxSent != NULL)
                       && (device_status == USB_OTG_CONFIGURED) )
                    {
                        class_cb->EP0_TxSent();
                    }
                    /*
                    * receive zero length packet on the ctl pipe
                    */
                    device_state = USB_OTG_EP0_STATUS_OUT;
                    EP_PrepareRx(0, NULL, 0);
                    usb_EP0_OutStart();
                }
            }
        }
    }
    else if(   (class_cb->DataIn != NULL)
            && (device_status == USB_OTG_CONFIGURED) )
    {
        class_cb->DataIn(epnum);
    }
}

/*
 *   Reads ep flags
 */
static uint32_t ReadDevInEP(uint8_t epnum)
{
    uint32_t v, msk, emp;
    msk = USB_FS->DREGS->DIEPMSK;
    emp = USB_FS->DREGS->DIEPEMPMSK;
    msk |= ((emp >> epnum) & 0x1) << 7;
    v = USB_FS->INEP_REGS[epnum]->DIEPINT & msk;
    return v;
}

/*
 * USB_OTG_WritePacket : Writes a packet into the Tx FIFO associated with the EP
 *   src : source pointer
 *   ch_ep_num : end point number
 *   len : No. of bytes
 */
static void usb_WritePacket(uint8_t *src, uint8_t ch_ep_num, uint16_t len)
{
    uint32_t count32b= 0 , i= 0;
    count32b =  (len + 3) / 4;
    for (i = 0; i < count32b; i++, src+=4)
    {
        USB_FS->DFIFO[ch_ep_num] = ((uint32_t *)src);
    }
}

/*
 * check FIFO for the next packet to be loaded
 */
static void WriteEmptyTxFifo(uint32_t epnum)
{
    uint32_t txstatus = 0;
    USB_OTG_EP *ep;
    uint32_t len = 0;
    uint32_t len32b;
    ep = &in_ep[epnum];
    len = ep->xfer_len - ep->xfer_count;
    if (len > ep->maxpacket)
    {
        len = ep->maxpacket;
    }
    len32b = (len + 3) / 4;
    txstatus = (0x0000ffff & USB_FS->INEP_REGS[epnum]->DTXFSTS);
    while  (txstatus > len32b && ep->xfer_count < ep->xfer_len && ep->xfer_len != 0)
    {
        /* Write the FIFO */
        len = ep->xfer_len - ep->xfer_count;
        if (len > ep->maxpacket)
        {
            len = ep->maxpacket;
        }
        len32b = (len + 3) / 4;
        usb_WritePacket(ep->xfer_buff, epnum, len);
        ep->xfer_buff  += len;
        ep->xfer_count += len;
        txstatus = (0x0000ffff & USB_FS->INEP_REGS[epnum]->DTXFSTS);
    }
}

