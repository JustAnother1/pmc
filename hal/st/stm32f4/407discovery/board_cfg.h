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

#ifndef CPU_CFG_H_
#define CPU_CFG_H_

/******************************************************************************/
/*                                                                            */
/*         Clock Configuration                                                */
/*                                                                            */
/******************************************************************************/

// frequency the cpu runs with AHB / HCLK = 84MHz
#define FREQUENCY_OF_HCLK    ((uint32_t)84000000)

// Flash:
// Wait states (3,3V and 168MHz) RM0090_P80
#define WAIT_STATES   FLASH_ACR_LATENCY_5WS

// 8MHz -> 168MHz (PLLCFGR)
// PLLQ = 7
#define RCC_PLL_PLLQ  (RCC_PLLCFGR_PLLQ_0 + RCC_PLLCFGR_PLLQ_1 + RCC_PLLCFGR_PLLQ_2)
// PLLP = /2
#define RCC_PLL_PLLP  0
// PLLN = 336 -> 101010000 -> 4+6+8
#define RCC_PLL_PLLN  (RCC_PLLCFGR_PLLN_8 +  RCC_PLLCFGR_PLLN_6 + RCC_PLLCFGR_PLLN_4)
// PLLM = 8
#define RCC_PLL_PLLM  8

// Clock Configuration (CFGR)
#define RCC_MC02          RCC_CFGR_MCO2_1
#define RCC_MC01          RCC_CFGR_MCO1_1
#define RCC_PRESC_APB2    RCC_CFGR_PPRE2_DIV1
#define RCC_PRESC_APB1    RCC_CFGR_PPRE1_DIV2
#define RCC_PRESC_AHB     RCC_CFGR_HPRE_DIV2
#define RCC_SYS_CLK_SW    RCC_CFGR_SW_PLL

/******************************************************************************/
/*                                                                            */
/*         L E D                                                              */
/*                                                                            */
/******************************************************************************/

#define DEBUG_LED_GPIO_PORT        GPIOD
#define DEBUG_LED_RCC_GPIO_ENABLE  RCC_AHB1ENR_GPIODEN
#define DEBUG_LED_MODER            GPIO_MODER_MODER12_0
#define DEBUG_LED_OTYPER           GPIO_OTYPER_OT_12
#define DEBUG_LED_OSPEEDR          GPIO_OSPEEDER_OSPEEDR12
#define DEBUG_LED_PUPD             GPIO_PUPDR_PUPDR12
#define DEBUG_LED_ODR              GPIO_ODR_ODR_12

#define ERROR_LED_GPIO_PORT        GPIOD
#define ERROR_LED_RCC_GPIO_ENABLE  RCC_AHB1ENR_GPIODEN
#define ERROR_LED_MODER            GPIO_MODER_MODER14_0
#define ERROR_LED_OTYPER           GPIO_OTYPER_OT_14
#define ERROR_LED_OSPEEDR          GPIO_OSPEEDER_OSPEEDR14
#define ERROR_LED_PUPD             GPIO_PUPDR_PUPDR14
#define ERROR_LED_ODR              GPIO_ODR_ODR_14

/******************************************************************************/
/*                                                                            */
/*         Universal Synchronous Asynchronous Receiver Transmitter            */
/*                                                                            */
/******************************************************************************/

#define UART_0                     USART1
#define UART_0_BRR                 0x2d9 // 115200 kbit
#define UART_0_IRQ_HANDLER         USART1_IRQHandler
#define UART_0_APB1ENR             0
#define UART_0_APB2ENR             0x00000010
// GPIO Port used by the RX Pin
#define UART_0_RX_GPIO_PORT        GPIOA
// enable Clock signal for the GPIO Port used by the RX Pin
#define UART_0_RX_GPIO_PORT_RCC    RCC_AHB1ENR_GPIOAEN
// RX Pin is in Alternate Function Mode
#define UART_0_RX_GPIO_MODER_1     GPIO_MODER_MODER10_1
#define UART_0_RX_GPIO_MODER_0     GPIO_MODER_MODER10_0
// Output Type
#define UART_0_RX_GPIO_OTYPER_1    0
#define UART_0_RX_GPIO_OTYPER_0    GPIO_OTYPER_OT_10
// Speed
#define UART_0_RX_GPIO_OSPEEDR_1   0
#define UART_0_RX_GPIO_OSPEEDR_0   GPIO_OSPEEDER_OSPEEDR10
// NO pull up no pull down
#define UART_0_RX_GPIO_PUPD_1      0
#define UART_0_RX_GPIO_PUPD_0      GPIO_PUPDR_PUPDR10
// select USART1-RX as alternate Function
// PA10 = AF7 = 0111
#define UART_0_RX_GPIO_AFR_0_1     0
#define UART_0_RX_GPIO_AFR_0_0     0
#define UART_0_RX_GPIO_AFR_1_1     0x00000700
#define UART_0_RX_GPIO_AFR_1_0     0x00000800
// GPIO Port used by the TX Pin
#define UART_0_TX_GPIO_PORT        GPIOA
// enable Clock signal for the GPIO Port used by the TX Pin
#define UART_0_TX_GPIO_PORT_RCC    RCC_AHB1ENR_GPIOAEN
// TX Pin is in Alternate Function Mode
#define UART_0_TX_GPIO_MODER_1     GPIO_MODER_MODER9_1
#define UART_0_TX_GPIO_MODER_0     GPIO_MODER_MODER9_0
// Output Type
#define UART_0_TX_GPIO_OTYPER_1    0
#define UART_0_TX_GPIO_OTYPER_0    GPIO_OTYPER_OT_9
// Speed
#define UART_0_TX_GPIO_OSPEEDR_1   0
#define UART_0_TX_GPIO_OSPEEDR_0   GPIO_OSPEEDER_OSPEEDR9
// NO pull up no pull down
#define UART_0_TX_GPIO_PUPD_1      0
#define UART_0_TX_GPIO_PUPD_0      GPIO_PUPDR_PUPDR9
// select USART1-TX as alternate Function
// PA9 = AF7 = 0111
#define UART_0_TX_GPIO_AFR_0_1     0
#define UART_0_TX_GPIO_AFR_0_0     0
#define UART_0_TX_GPIO_AFR_1_1     0x00000070
#define UART_0_TX_GPIO_AFR_1_0     0x00000080


#define UART_1                     USART6
#define UART_1_BRR                 0x2d9 // 115200 kbit
#define UART_1_IRQ_HANDLER         USART6_IRQHandler
#define UART_1_APB1ENR             0
#define UART_1_APB2ENR             0x00000020
#define UART_1_RX_GPIO_PORT        GPIOC
#define UART_1_RX_GPIO_PORT_RCC    RCC_AHB1ENR_GPIOCEN
#define UART_1_RX_GPIO_MODER_1     GPIO_MODER_MODER7_1
#define UART_1_RX_GPIO_MODER_0     GPIO_MODER_MODER7_0
#define UART_1_RX_GPIO_OTYPER_1    0
#define UART_1_RX_GPIO_OTYPER_0    GPIO_OTYPER_OT_7
#define UART_1_RX_GPIO_OSPEEDR_1   0
#define UART_1_RX_GPIO_OSPEEDR_0   GPIO_OSPEEDER_OSPEEDR7
#define UART_1_RX_GPIO_PUPD_1      0
#define UART_1_RX_GPIO_PUPD_0      GPIO_PUPDR_PUPDR7
// PC7 = AF8 = 1000
#define UART_1_RX_GPIO_AFR_0_1     0x80000000
#define UART_1_RX_GPIO_AFR_0_0     0x70000000
#define UART_1_RX_GPIO_AFR_1_1     0
#define UART_1_RX_GPIO_AFR_1_0     0

#define UART_1_TX_GPIO_PORT        GPIOC
#define UART_1_TX_GPIO_PORT_RCC    RCC_AHB1ENR_GPIOCEN
#define UART_1_TX_GPIO_MODER_1     GPIO_MODER_MODER6_1
#define UART_1_TX_GPIO_MODER_0     GPIO_MODER_MODER6_0
#define UART_1_TX_GPIO_OTYPER_1    0
#define UART_1_TX_GPIO_OTYPER_0    GPIO_OTYPER_OT_6
#define UART_1_TX_GPIO_OSPEEDR_1   0
#define UART_1_TX_GPIO_OSPEEDR_0   GPIO_OSPEEDER_OSPEEDR6
#define UART_2_TX_GPIO_PUPD_1      0
#define UART_1_TX_GPIO_PUPD_0      GPIO_PUPDR_PUPDR6
// PC6 = AF8 = 1000
#define UART_1_TX_GPIO_AFR_0_1     0x08000000
#define UART_1_TX_GPIO_AFR_0_0     0x07000000
#define UART_1_TX_GPIO_AFR_1_1     0
#define UART_1_TX_GPIO_AFR_1_0     0

#endif /* CPU_CFG_H_ */
