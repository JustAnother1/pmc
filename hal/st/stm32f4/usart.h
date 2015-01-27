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

#ifndef HAL_ST_STM32F4_USART_H_
#define HAL_ST_STM32F4_USART_H_

#include "hal_common.h"


/******************************************************************************/
/*                                                                            */
/*         Universal Synchronous Asynchronous Receiver Transmitter            */
/*                                                                            */
/******************************************************************************/
/*******************  Bit definition for USART_SR register  *******************/
#define  USART_SR_PE                         ((uint32_t)0x0001)            /*!<Parity Error                 */
#define  USART_SR_FE                         ((uint32_t)0x0002)            /*!<Framing Error                */
#define  USART_SR_NE                         ((uint32_t)0x0004)            /*!<Noise Error Flag             */
#define  USART_SR_ORE                        ((uint32_t)0x0008)            /*!<OverRun Error                */
#define  USART_SR_IDLE                       ((uint32_t)0x0010)            /*!<IDLE line detected           */
#define  USART_SR_RXNE                       ((uint32_t)0x0020)            /*!<Read Data Register Not Empty */
#define  USART_SR_TC                         ((uint32_t)0x0040)            /*!<Transmission Complete        */
#define  USART_SR_TXE                        ((uint32_t)0x0080)            /*!<Transmit Data Register Empty */
#define  USART_SR_LBD                        ((uint32_t)0x0100)            /*!<LIN Break Detection Flag     */
#define  USART_SR_CTS                        ((uint32_t)0x0200)            /*!<CTS Flag                     */

/*******************  Bit definition for USART_DR register  *******************/
#define  USART_DR_DR                         ((uint32_t)0x01FF)            /*!<Data value */

/******************  Bit definition for USART_BRR register  *******************/
#define  USART_BRR_DIV_Fraction              ((uint32_t)0x000F)            /*!<Fraction of USARTDIV */
#define  USART_BRR_DIV_Mantissa              ((uint32_t)0xFFF0)            /*!<Mantissa of USARTDIV */

/******************  Bit definition for USART_CR1 register  *******************/
#define  USART_CR1_SBK                       ((uint32_t)0x0001)            /*!<Send Break                             */
#define  USART_CR1_RWU                       ((uint32_t)0x0002)            /*!<Receiver wakeup                        */
#define  USART_CR1_RE                        ((uint32_t)0x0004)            /*!<Receiver Enable                        */
#define  USART_CR1_TE                        ((uint32_t)0x0008)            /*!<Transmitter Enable                     */
#define  USART_CR1_IDLEIE                    ((uint32_t)0x0010)            /*!<IDLE Interrupt Enable                  */
#define  USART_CR1_RXNEIE                    ((uint32_t)0x0020)            /*!<RXNE Interrupt Enable                  */
#define  USART_CR1_TCIE                      ((uint32_t)0x0040)            /*!<Transmission Complete Interrupt Enable */
#define  USART_CR1_TXEIE                     ((uint32_t)0x0080)            /*!<PE Interrupt Enable                    */
#define  USART_CR1_PEIE                      ((uint32_t)0x0100)            /*!<PE Interrupt Enable                    */
#define  USART_CR1_PS                        ((uint32_t)0x0200)            /*!<Parity Selection                       */
#define  USART_CR1_PCE                       ((uint32_t)0x0400)            /*!<Parity Control Enable                  */
#define  USART_CR1_WAKE                      ((uint32_t)0x0800)            /*!<Wakeup method                          */
#define  USART_CR1_M                         ((uint32_t)0x1000)            /*!<Word length                            */
#define  USART_CR1_UE                        ((uint32_t)0x2000)            /*!<USART Enable                           */
#define  USART_CR1_OVER8                     ((uint32_t)0x8000)            /*!<USART Oversampling by 8 enable         */

/******************  Bit definition for USART_CR2 register  *******************/
#define  USART_CR2_ADD                       ((uint32_t)0x000F)            /*!<Address of the USART node            */
#define  USART_CR2_LBDL                      ((uint32_t)0x0020)            /*!<LIN Break Detection Length           */
#define  USART_CR2_LBDIE                     ((uint32_t)0x0040)            /*!<LIN Break Detection Interrupt Enable */
#define  USART_CR2_LBCL                      ((uint32_t)0x0100)            /*!<Last Bit Clock pulse                 */
#define  USART_CR2_CPHA                      ((uint32_t)0x0200)            /*!<Clock Phase                          */
#define  USART_CR2_CPOL                      ((uint32_t)0x0400)            /*!<Clock Polarity                       */
#define  USART_CR2_CLKEN                     ((uint32_t)0x0800)            /*!<Clock Enable                         */

#define  USART_CR2_STOP                      ((uint32_t)0x3000)            /*!<STOP[1:0] bits (STOP bits) */
#define  USART_CR2_STOP_0                    ((uint32_t)0x1000)            /*!<Bit 0 */
#define  USART_CR2_STOP_1                    ((uint32_t)0x2000)            /*!<Bit 1 */

#define  USART_CR2_LINEN                     ((uint32_t)0x4000)            /*!<LIN mode enable */

/******************  Bit definition for USART_CR3 register  *******************/
#define  USART_CR3_EIE                       ((uint32_t)0x0001)            /*!<Error Interrupt Enable      */
#define  USART_CR3_IREN                      ((uint32_t)0x0002)            /*!<IrDA mode Enable            */
#define  USART_CR3_IRLP                      ((uint32_t)0x0004)            /*!<IrDA Low-Power              */
#define  USART_CR3_HDSEL                     ((uint32_t)0x0008)            /*!<Half-Duplex Selection       */
#define  USART_CR3_NACK                      ((uint32_t)0x0010)            /*!<Smartcard NACK enable       */
#define  USART_CR3_SCEN                      ((uint32_t)0x0020)            /*!<Smartcard mode enable       */
#define  USART_CR3_DMAR                      ((uint32_t)0x0040)            /*!<DMA Enable Receiver         */
#define  USART_CR3_DMAT                      ((uint32_t)0x0080)            /*!<DMA Enable Transmitter      */
#define  USART_CR3_RTSE                      ((uint32_t)0x0100)            /*!<RTS Enable                  */
#define  USART_CR3_CTSE                      ((uint32_t)0x0200)            /*!<CTS Enable                  */
#define  USART_CR3_CTSIE                     ((uint32_t)0x0400)            /*!<CTS Interrupt Enable        */
#define  USART_CR3_ONEBIT                    ((uint32_t)0x0800)            /*!<USART One bit method enable */

/******************  Bit definition for USART_GTPR register  ******************/
#define  USART_GTPR_PSC                      ((uint32_t)0x00FF)            /*!<PSC[7:0] bits (Prescaler value) */
#define  USART_GTPR_PSC_0                    ((uint32_t)0x0001)            /*!<Bit 0 */
#define  USART_GTPR_PSC_1                    ((uint32_t)0x0002)            /*!<Bit 1 */
#define  USART_GTPR_PSC_2                    ((uint32_t)0x0004)            /*!<Bit 2 */
#define  USART_GTPR_PSC_3                    ((uint32_t)0x0008)            /*!<Bit 3 */
#define  USART_GTPR_PSC_4                    ((uint32_t)0x0010)            /*!<Bit 4 */
#define  USART_GTPR_PSC_5                    ((uint32_t)0x0020)            /*!<Bit 5 */
#define  USART_GTPR_PSC_6                    ((uint32_t)0x0040)            /*!<Bit 6 */
#define  USART_GTPR_PSC_7                    ((uint32_t)0x0080)            /*!<Bit 7 */

#define  USART_GTPR_GT                       ((uint32_t)0xFF00)            /*!<Guard time value */

/**
  * @brief Universal Synchronous Asynchronous Receiver Transmitter
  */

typedef struct
{
  __IO uint32_t SR;         /*!< USART Status register,                   Address offset: 0x00 */
  __IO uint32_t DR;         /*!< USART Data register,                     Address offset: 0x04 */
  __IO uint32_t BRR;        /*!< USART Baud rate register,                Address offset: 0x08 */
  __IO uint32_t CR1;        /*!< USART Control register 1,                Address offset: 0x0C */
  __IO uint32_t CR2;        /*!< USART Control register 2,                Address offset: 0x10 */
  __IO uint32_t CR3;        /*!< USART Control register 3,                Address offset: 0x14 */
  __IO uint32_t GTPR;       /*!< USART Guard time and prescaler register, Address offset: 0x18 */
} USART_TypeDef;

#define USART1_BASE         (APB2PERIPH_BASE + 0x1000)
#define USART2_BASE         (APB1PERIPH_BASE + 0x4400)
#define USART3_BASE         (APB1PERIPH_BASE + 0x4800)
#define UART4_BASE          (APB1PERIPH_BASE + 0x4C00)
#define UART5_BASE          (APB1PERIPH_BASE + 0x5000)
#define USART6_BASE         (APB2PERIPH_BASE + 0x1400)
#define USART1              ((USART_TypeDef *) USART1_BASE)
#define USART2              ((USART_TypeDef *) USART2_BASE)
#define USART3              ((USART_TypeDef *) USART3_BASE)
#define UART4               ((USART_TypeDef *) UART4_BASE)
#define UART5               ((USART_TypeDef *) UART5_BASE)
#define USART6              ((USART_TypeDef *) USART6_BASE)

#endif /* HAL_ST_STM32F4_USART_H_ */
