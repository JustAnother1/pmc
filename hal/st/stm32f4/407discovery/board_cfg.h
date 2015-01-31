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
/*         I R Q                                                              */
/*                                                                            */
/******************************************************************************/
#define UART_0_IRQ_NUMBER          USART1_IRQn
#define UART_0_IRQ_PRIORITY        5
#define UART_1_IRQ_NUMBER          USART6_IRQn
#define UART_1_IRQ_PRIORITY        14
#define SPI_0_IRQ_NUMBER           SPI1_IRQn
#define SPI_0_IRQ_PRIORITY         8
#define SPI_1_IRQ_NUMBER           SPI2_IRQn
#define SPI_1_IRQ_PRIORITY         12

/******************************************************************************/
/*                                                                            */
/*         L E D                                                              */
/*                                                                            */
/******************************************************************************/

#define DEBUG_LED_GPIO_PORT        GPIOD
#define DEBUG_LED_RCC_GPIO_ENABLE  RCC_AHB1ENR_GPIODEN
#define DEBUG_LED_MODER_0          GPIO_MODER_12_OUTPUT_0
#define DEBUG_LED_MODER_1          GPIO_MODER_12_OUTPUT_1
#define DEBUG_LED_OTYPER_0         GPIO_OTYPER_12_PUSH_PULL_0
#define DEBUG_LED_OTYPER_1         GPIO_OTYPER_12_PUSH_PULL_1
#define DEBUG_LED_OSPEEDR_0        GPIO_OSPEEDER_12_LOW_SPEED_0
#define DEBUG_LED_OSPEEDR_1        GPIO_OSPEEDER_12_LOW_SPEED_1
#define DEBUG_LED_PUPD_0           GPIO_PUPDR_12_NONE_0
#define DEBUG_LED_PUPD_1           GPIO_PUPDR_12_NONE_1
#define DEBUG_LED_ODR              GPIO_ODR_12
#define DEBUG_LED_BSRR             GPIO_BSRR_12

#define ERROR_LED_GPIO_PORT        GPIOD
#define ERROR_LED_RCC_GPIO_ENABLE  RCC_AHB1ENR_GPIODEN
#define ERROR_LED_MODER_0          GPIO_MODER_14_OUTPUT_0
#define ERROR_LED_MODER_1          GPIO_MODER_14_OUTPUT_1
#define ERROR_LED_OTYPER_0         GPIO_OTYPER_14_PUSH_PULL_0
#define ERROR_LED_OTYPER_1         GPIO_OTYPER_14_PUSH_PULL_1
#define ERROR_LED_OSPEEDR_0        GPIO_OSPEEDER_12_LOW_SPEED_0
#define ERROR_LED_OSPEEDR_1        GPIO_OSPEEDER_12_LOW_SPEED_1
#define ERROR_LED_PUPD_0           GPIO_PUPDR_14_NONE_0
#define ERROR_LED_PUPD_1           GPIO_PUPDR_14_NONE_1
#define ERROR_LED_ODR              GPIO_ODR_14
#define ERROR_LED_BSRR             GPIO_BSRR_14

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
#define UART_0_RX_GPIO_MODER_0     GPIO_MODER_10_ALTFUNC_0
#define UART_0_RX_GPIO_MODER_1     GPIO_MODER_10_ALTFUNC_1
// Output Type
#define UART_0_RX_GPIO_OTYPER_0    GPIO_OTYPER_10_PUSH_PULL_0
#define UART_0_RX_GPIO_OTYPER_1    GPIO_OTYPER_10_PUSH_PULL_1
// Speed
#define UART_0_RX_GPIO_OSPEEDR_0   GPIO_OSPEEDER_10_LOW_SPEED_0
#define UART_0_RX_GPIO_OSPEEDR_1   GPIO_OSPEEDER_10_LOW_SPEED_0
// pull up
#define UART_0_RX_GPIO_PUPD_0      GPIO_PUPDR_10_PULL_UP_0
#define UART_0_RX_GPIO_PUPD_1      GPIO_PUPDR_10_PULL_UP_1
// select USART1-RX as alternate Function
// PA10 = AF7 = 0111
#define UART_0_RX_GPIO_AFR_0_0     0
#define UART_0_RX_GPIO_AFR_0_1     0
#define UART_0_RX_GPIO_AFR_1_0     0x00000800
#define UART_0_RX_GPIO_AFR_1_1     0x00000700
// GPIO Port used by the TX Pin
#define UART_0_TX_GPIO_PORT        GPIOA
// enable Clock signal for the GPIO Port used by the TX Pin
#define UART_0_TX_GPIO_PORT_RCC    RCC_AHB1ENR_GPIOAEN
// TX Pin is in Alternate Function Mode
#define UART_0_TX_GPIO_MODER_0     GPIO_MODER_9_ALTFUNC_0
#define UART_0_TX_GPIO_MODER_1     GPIO_MODER_9_ALTFUNC_1
// Output Type
#define UART_0_TX_GPIO_OTYPER_0    GPIO_OTYPER_9_PUSH_PULL_0
#define UART_0_TX_GPIO_OTYPER_1    GPIO_OTYPER_9_PUSH_PULL_1
// Speed
#define UART_0_TX_GPIO_OSPEEDR_0   GPIO_OSPEEDER_9_LOW_SPEED_0
#define UART_0_TX_GPIO_OSPEEDR_1   GPIO_OSPEEDER_9_LOW_SPEED_1
// NO pull up no pull down
#define UART_0_TX_GPIO_PUPD_0      GPIO_PUPDR_9_PULL_UP_0
#define UART_0_TX_GPIO_PUPD_1      GPIO_PUPDR_9_PULL_UP_1
// select USART1-TX as alternate Function
// PA9 = AF7 = 0111
#define UART_0_TX_GPIO_AFR_0_0     0
#define UART_0_TX_GPIO_AFR_0_1     0
#define UART_0_TX_GPIO_AFR_1_0     0x00000080
#define UART_0_TX_GPIO_AFR_1_1     0x00000070

#define UART_1                     USART6
#define UART_1_BRR                 0x2d9 // 115200 kbit
#define UART_1_IRQ_HANDLER         USART6_IRQHandler
#define UART_1_APB1ENR             0
#define UART_1_APB2ENR             0x00000020
#define UART_1_RX_GPIO_PORT        GPIOC
#define UART_1_RX_GPIO_PORT_RCC    RCC_AHB1ENR_GPIOCEN
#define UART_1_RX_GPIO_MODER_0     GPIO_MODER_7_ALTFUNC_0
#define UART_1_RX_GPIO_MODER_1     GPIO_MODER_7_ALTFUNC_1
#define UART_1_RX_GPIO_OTYPER_0    GPIO_OTYPER_7_PUSH_PULL_0
#define UART_1_RX_GPIO_OTYPER_1    GPIO_OTYPER_7_PUSH_PULL_1
#define UART_1_RX_GPIO_OSPEEDR_0   GPIO_OSPEEDER_7_LOW_SPEED_0
#define UART_1_RX_GPIO_OSPEEDR_1   GPIO_OSPEEDER_7_LOW_SPEED_1
#define UART_1_RX_GPIO_PUPD_0      GPIO_PUPDR_7_PULL_UP_0
#define UART_1_RX_GPIO_PUPD_1      GPIO_PUPDR_7_PULL_UP_1
// PC7 = AF8 = 1000
#define UART_1_RX_GPIO_AFR_0_0     0x70000000
#define UART_1_RX_GPIO_AFR_0_1     0x80000000
#define UART_1_RX_GPIO_AFR_1_0     0
#define UART_1_RX_GPIO_AFR_1_1     0


#define UART_1_TX_GPIO_PORT        GPIOC
#define UART_1_TX_GPIO_PORT_RCC    RCC_AHB1ENR_GPIOCEN
#define UART_1_TX_GPIO_MODER_0     GPIO_MODER_6_ALTFUNC_0
#define UART_1_TX_GPIO_MODER_1     GPIO_MODER_6_ALTFUNC_1
#define UART_1_TX_GPIO_OTYPER_0    GPIO_OTYPER_6_PUSH_PULL_0
#define UART_1_TX_GPIO_OTYPER_1    GPIO_OTYPER_6_PUSH_PULL_1
#define UART_1_TX_GPIO_OSPEEDR_0   GPIO_OSPEEDER_6_LOW_SPEED_0
#define UART_1_TX_GPIO_OSPEEDR_1   GPIO_OSPEEDER_6_LOW_SPEED_1
#define UART_1_TX_GPIO_PUPD_0      GPIO_PUPDR_6_PULL_UP_0
#define UART_1_TX_GPIO_PUPD_1      GPIO_PUPDR_6_PULL_UP_1
// PC6 = AF8 = 1000
#define UART_1_TX_GPIO_AFR_0_0     0x07000000
#define UART_1_TX_GPIO_AFR_0_1     0x08000000
#define UART_1_TX_GPIO_AFR_1_0     0
#define UART_1_TX_GPIO_AFR_1_1     0


/******************************************************************************/
/*                                                                            */
/*         S P I                                                              */
/*                                                                            */
/******************************************************************************/

#define SPI_0_MISO_GPIO_PORT_RCC  RCC_AHB1ENR_GPIOBEN
#define SPI_0_MOSI_GPIO_PORT_RCC  RCC_AHB1ENR_GPIOBEN
#define SPI_0_NSS_GPIO_PORT_RCC   RCC_AHB1ENR_GPIOAEN
#define SPI_0_SCK_GPIO_PORT_RCC   RCC_AHB1ENR_GPIOBEN
#define SPI_0                     SPI1
#define SPI_0_IRQ_HANDLER         SPI1_IRQHandler
#define SPI_0_APB1ENR             0
#define SPI_0_APB2ENR             RCC_APB2ENR_SPI1EN
#define SPI_0_MISO_GPIO_PORT      GPIOB
#define SPI_0_MOSI_GPIO_PORT      GPIOB
#define SPI_0_NSS_GPIO_PORT       GPIOA
#define SPI_0_SCK_GPIO_PORT       GPIOB

#define SPI_0_MISO_GPIO_MODER_0   GPIO_MODER_4_ALTFUNC_0
#define SPI_0_MISO_GPIO_MODER_1   GPIO_MODER_4_ALTFUNC_1
#define SPI_0_MISO_GPIO_OTYPER_0  GPIO_OTYPER_4_PUSH_PULL_0
#define SPI_0_MISO_GPIO_OTYPER_1  GPIO_OTYPER_4_PUSH_PULL_1
#define SPI_0_MISO_GPIO_OSPEEDR_0 GPIO_OSPEEDER_4_LOW_SPEED_0
#define SPI_0_MISO_GPIO_OSPEEDR_1 GPIO_OSPEEDER_4_LOW_SPEED_0
#define SPI_0_MISO_GPIO_PUPD_0    GPIO_PUPDR_4_PULL_UP_0
#define SPI_0_MISO_GPIO_PUPD_1    GPIO_PUPDR_4_PULL_UP_1
#define SPI_0_MISO_GPIO_AFR_0_0   0x000a0000
#define SPI_0_MISO_GPIO_AFR_0_1   0x00050000
#define SPI_0_MISO_GPIO_AFR_1_0   0
#define SPI_0_MISO_GPIO_AFR_1_1   0

#define SPI_0_MOSI_GPIO_MODER_0   GPIO_MODER_5_ALTFUNC_0
#define SPI_0_MOSI_GPIO_MODER_1   GPIO_MODER_5_ALTFUNC_1
#define SPI_0_MOSI_GPIO_OTYPER_0  GPIO_OTYPER_5_PUSH_PULL_0
#define SPI_0_MOSI_GPIO_OTYPER_1  GPIO_OTYPER_5_PUSH_PULL_1
#define SPI_0_MOSI_GPIO_OSPEEDR_0 GPIO_OSPEEDER_5_LOW_SPEED_0
#define SPI_0_MOSI_GPIO_OSPEEDR_1 GPIO_OSPEEDER_5_LOW_SPEED_1
#define SPI_0_MOSI_GPIO_PUPD_0    GPIO_PUPDR_5_PULL_UP_0
#define SPI_0_MOSI_GPIO_PUPD_1    GPIO_PUPDR_5_PULL_UP_1
#define SPI_0_MOSI_GPIO_AFR_0_0   0x00a00000
#define SPI_0_MOSI_GPIO_AFR_0_1   0x00500000
#define SPI_0_MOSI_GPIO_AFR_1_0   0
#define SPI_0_MOSI_GPIO_AFR_1_1   0

#define SPI_0_NSS_GPIO_MODER_0    GPIO_MODER_15_OUTPUT_0
#define SPI_0_NSS_GPIO_MODER_1    GPIO_MODER_15_OUTPUT_1
#define SPI_0_NSS_GPIO_OTYPER_0   GPIO_OTYPER_15_PUSH_PULL_0
#define SPI_0_NSS_GPIO_OTYPER_1   GPIO_OTYPER_15_PUSH_PULL_1
#define SPI_0_NSS_GPIO_OSPEEDR_0  GPIO_OSPEEDER_15_LOW_SPEED_0
#define SPI_0_NSS_GPIO_OSPEEDR_1  GPIO_OSPEEDER_15_LOW_SPEED_0
#define SPI_0_NSS_GPIO_PUPD_0     GPIO_PUPDR_15_PULL_UP_0
#define SPI_0_NSS_GPIO_PUPD_1     GPIO_PUPDR_15_PULL_UP_1
#define SPI_0_NSS_GPIO_BSRR       GPIO_BSRR_15

#define SPI_0_SCK_GPIO_MODER_0    GPIO_MODER_3_ALTFUNC_0
#define SPI_0_SCK_GPIO_MODER_1    GPIO_MODER_3_ALTFUNC_1
#define SPI_0_SCK_GPIO_OTYPER_0   GPIO_OTYPER_3_PUSH_PULL_0
#define SPI_0_SCK_GPIO_OTYPER_1   GPIO_OTYPER_3_PUSH_PULL_1
#define SPI_0_SCK_GPIO_OSPEEDR_0  GPIO_OSPEEDER_3_LOW_SPEED_0
#define SPI_0_SCK_GPIO_OSPEEDR_1  GPIO_OSPEEDER_3_LOW_SPEED_1
#define SPI_0_SCK_GPIO_PUPD_0     GPIO_PUPDR_3_PULL_UP_0
#define SPI_0_SCK_GPIO_PUPD_1     GPIO_PUPDR_3_PULL_UP_0
#define SPI_0_SCK_GPIO_AFR_0_0    0x0000a000
#define SPI_0_SCK_GPIO_AFR_0_1    0x00005000
#define SPI_0_SCK_GPIO_AFR_1_0    0
#define SPI_0_SCK_GPIO_AFR_1_1    0


#define SPI_1_MISO_GPIO_PORT_RCC  RCC_AHB1ENR_GPIOCEN
#define SPI_1_MOSI_GPIO_PORT_RCC  RCC_AHB1ENR_GPIOCEN
#define SPI_1_NSS_GPIO_PORT_RCC   RCC_AHB1ENR_GPIOBEN
#define SPI_1_SCK_GPIO_PORT_RCC   RCC_AHB1ENR_GPIOBEN
#define SPI_1                     SPI2
#define SPI_1_IRQ_HANDLER         SPI2_IRQHandler
#define SPI_1_APB1ENR             RCC_APB1ENR_SPI2EN
#define SPI_1_APB2ENR             0
#define SPI_1_MISO_GPIO_PORT      GPIOC
#define SPI_1_MOSI_GPIO_PORT      GPIOC
#define SPI_1_NSS_GPIO_PORT       GPIOB
#define SPI_1_SCK_GPIO_PORT       GPIOB

#define SPI_1_MISO_GPIO_MODER_0   GPIO_MODER_2_ALTFUNC_0
#define SPI_1_MISO_GPIO_MODER_1   GPIO_MODER_2_ALTFUNC_1
#define SPI_1_MISO_GPIO_OTYPER_0  GPIO_OTYPER_2_PUSH_PULL_0
#define SPI_1_MISO_GPIO_OTYPER_1  GPIO_OTYPER_2_PUSH_PULL_1
#define SPI_1_MISO_GPIO_OSPEEDR_0 GPIO_OSPEEDER_2_LOW_SPEED_0
#define SPI_1_MISO_GPIO_OSPEEDR_1 GPIO_OSPEEDER_2_LOW_SPEED_1
#define SPI_1_MISO_GPIO_PUPD_0    GPIO_PUPDR_2_PULL_UP_0
#define SPI_1_MISO_GPIO_PUPD_1    GPIO_PUPDR_2_PULL_UP_1
#define SPI_1_MISO_GPIO_AFR_0_0   0x00000a00
#define SPI_1_MISO_GPIO_AFR_0_1   0x00000500
#define SPI_1_MISO_GPIO_AFR_1_0   0
#define SPI_1_MISO_GPIO_AFR_1_1   0

#define SPI_1_MOSI_GPIO_MODER_0   GPIO_MODER_3_ALTFUNC_0
#define SPI_1_MOSI_GPIO_MODER_1   GPIO_MODER_3_ALTFUNC_1
#define SPI_1_MOSI_GPIO_OTYPER_0  GPIO_OTYPER_3_PUSH_PULL_0
#define SPI_1_MOSI_GPIO_OTYPER_1  GPIO_OTYPER_3_PUSH_PULL_1
#define SPI_1_MOSI_GPIO_OSPEEDR_0 GPIO_OSPEEDER_3_LOW_SPEED_0
#define SPI_1_MOSI_GPIO_OSPEEDR_1 GPIO_OSPEEDER_3_LOW_SPEED_1
#define SPI_1_MOSI_GPIO_PUPD_0    GPIO_PUPDR_3_PULL_UP_0
#define SPI_1_MOSI_GPIO_PUPD_1    GPIO_PUPDR_3_PULL_UP_1
#define SPI_1_MOSI_GPIO_AFR_0_0   0x0000a000
#define SPI_1_MOSI_GPIO_AFR_0_1   0x00005000
#define SPI_1_MOSI_GPIO_AFR_1_0   0
#define SPI_1_MOSI_GPIO_AFR_1_1   0

#define SPI_1_NSS_GPIO_MODER_0    GPIO_MODER_12_OUTPUT_0
#define SPI_1_NSS_GPIO_MODER_1    GPIO_MODER_12_OUTPUT_1
#define SPI_1_NSS_GPIO_OTYPER_0   GPIO_OTYPER_12_PUSH_PULL_0
#define SPI_1_NSS_GPIO_OTYPER_1   GPIO_OTYPER_12_PUSH_PULL_1
#define SPI_1_NSS_GPIO_OSPEEDR_0  GPIO_OSPEEDER_12_LOW_SPEED_0
#define SPI_1_NSS_GPIO_OSPEEDR_1  GPIO_OSPEEDER_12_LOW_SPEED_1
#define SPI_1_NSS_GPIO_PUPD_0     GPIO_PUPDR_12_PULL_UP_0
#define SPI_1_NSS_GPIO_PUPD_1     GPIO_PUPDR_12_PULL_UP_1
#define SPI_1_NSS_GPIO_BSRR       GPIO_BSRR_12

#define SPI_1_SCK_GPIO_MODER_0    GPIO_MODER_13_ALTFUNC_0
#define SPI_1_SCK_GPIO_MODER_1    GPIO_MODER_13_ALTFUNC_1
#define SPI_1_SCK_GPIO_OTYPER_0   GPIO_OTYPER_13_PUSH_PULL_0
#define SPI_1_SCK_GPIO_OTYPER_1   GPIO_OTYPER_13_PUSH_PULL_1
#define SPI_1_SCK_GPIO_OSPEEDR_0  GPIO_OSPEEDER_13_LOW_SPEED_0
#define SPI_1_SCK_GPIO_OSPEEDR_1  GPIO_OSPEEDER_13_LOW_SPEED_1
#define SPI_1_SCK_GPIO_PUPD_0     GPIO_PUPDR_13_PULL_UP_0
#define SPI_1_SCK_GPIO_PUPD_1     GPIO_PUPDR_13_PULL_UP_1
#define SPI_1_SCK_GPIO_AFR_0_0    0
#define SPI_1_SCK_GPIO_AFR_0_1    0
#define SPI_1_SCK_GPIO_AFR_1_0    0x00a00000
#define SPI_1_SCK_GPIO_AFR_1_1    0x00500000


/******************************************************************************/
/*                                                                            */
/*         Digital Output Pin                                                 */
/*                                                                            */
/******************************************************************************/

#define D_OUT_NUM_PINS             2
#define D_OUT_0_NAME               "Out 1"
#define D_OUT_0_RCC_GPIO_ENABLE    RCC_AHB1ENR_GPIODEN
#define D_OUT_0_GPIO_PORT          GPIOE
#define D_OUT_0_MODER_0            GPIO_MODER_15_INPUT_0
#define D_OUT_0_MODER_1            GPIO_MODER_15_INPUT_1
#define D_OUT_0_OTYPER_0           GPIO_OTYPER_15_OPEN_DRAIN_0
#define D_OUT_0_OTYPER_1           GPIO_OTYPER_15_OPEN_DRAIN_1
#define D_OUT_0_OSPEEDR_0          GPIO_OSPEEDER_15_LOW_SPEED_0
#define D_OUT_0_OSPEEDR_1          GPIO_OSPEEDER_15_LOW_SPEED_1
#define D_OUT_0_PUPD_0             GPIO_PUPDR_15_NONE_0
#define D_OUT_0_PUPD_1             GPIO_PUPDR_15_NONE_1
#define D_OUT_0_ODR                GPIO_ODR_15

#define D_OUT_1_NAME               "Out 2"
#define D_OUT_1_RCC_GPIO_ENABLE    RCC_AHB1ENR_GPIODEN
#define D_OUT_1_GPIO_PORT          GPIOE
#define D_OUT_1_MODER_0            GPIO_MODER_12_INPUT_0
#define D_OUT_1_MODER_1            GPIO_MODER_12_INPUT_1
#define D_OUT_1_OTYPER_0           GPIO_OTYPER_12_OPEN_DRAIN_0
#define D_OUT_1_OTYPER_1           GPIO_OTYPER_12_OPEN_DRAIN_1
#define D_OUT_1_OSPEEDR_0          GPIO_OSPEEDER_12_LOW_SPEED_0
#define D_OUT_1_OSPEEDR_1          GPIO_OSPEEDER_12_LOW_SPEED_1
#define D_OUT_1_PUPD_0             GPIO_PUPDR_12_NONE_0
#define D_OUT_1_PUPD_1             GPIO_PUPDR_12_NONE_1
#define D_OUT_1_ODR                GPIO_ODR_12

/******************************************************************************/
/*                                                                            */
/*         Digital Input Pin                                                 */
/*                                                                            */
/******************************************************************************/

#define D_IN_NUM_PINS              3
#define D_IN_0_NAME                "In 1"
#define D_IN_0_RCC_GPIO_ENABLE     RCC_AHB1ENR_GPIODEN
#define D_IN_0_GPIO_PORT           GPIOC
#define D_IN_0_MODER_0             GPIO_MODER_10_INPUT_0
#define D_IN_0_MODER_1             GPIO_MODER_10_INPUT_1
#define D_IN_0_OTYPER_0            GPIO_OTYPER_10_OPEN_DRAIN_0
#define D_IN_0_OTYPER_1            GPIO_OTYPER_10_OPEN_DRAIN_1
#define D_IN_0_OSPEEDR_0           GPIO_OSPEEDER_10_LOW_SPEED_0
#define D_IN_0_OSPEEDR_1           GPIO_OSPEEDER_10_LOW_SPEED_1
#define D_IN_0_PUPD_0              GPIO_PUPDR_10_NONE_0
#define D_IN_0_PUPD_1              GPIO_PUPDR_10_NONE_1
#define D_IN_0_IDR                 GPIO_IDR_10

#define D_IN_1_NAME                "In 2"
#define D_IN_1_RCC_GPIO_ENABLE     RCC_AHB1ENR_GPIODEN
#define D_IN_1_GPIO_PORT           GPIOC
#define D_IN_1_MODER_0             GPIO_MODER_11_INPUT_0
#define D_IN_1_MODER_1             GPIO_MODER_11_INPUT_1
#define D_IN_1_OTYPER_0            GPIO_OTYPER_11_OPEN_DRAIN_0
#define D_IN_1_OTYPER_1            GPIO_OTYPER_11_OPEN_DRAIN_1
#define D_IN_1_OSPEEDR_0           GPIO_OSPEEDER_11_LOW_SPEED_0
#define D_IN_1_OSPEEDR_1           GPIO_OSPEEDER_11_LOW_SPEED_1
#define D_IN_1_PUPD_0              GPIO_PUPDR_11_NONE_0
#define D_IN_1_PUPD_1              GPIO_PUPDR_11_NONE_1
#define D_IN_1_IDR                 GPIO_IDR_11

#define D_IN_2_NAME                "In 3"
#define D_IN_2_RCC_GPIO_ENABLE     RCC_AHB1ENR_GPIODEN
#define D_IN_2_GPIO_PORT           GPIOC
#define D_IN_2_MODER_0             GPIO_MODER_12_INPUT_0
#define D_IN_2_MODER_1             GPIO_MODER_12_INPUT_1
#define D_IN_2_OTYPER_0            GPIO_OTYPER_12_OPEN_DRAIN_0
#define D_IN_2_OTYPER_1            GPIO_OTYPER_12_OPEN_DRAIN_1
#define D_IN_2_OSPEEDR_0           GPIO_OSPEEDER_12_LOW_SPEED_0
#define D_IN_2_OSPEEDR_1           GPIO_OSPEEDER_12_LOW_SPEED_1
#define D_IN_2_PUPD_0              GPIO_PUPDR_12_NONE_0
#define D_IN_2_PUPD_1              GPIO_PUPDR_12_NONE_1
#define D_IN_2_IDR                 GPIO_IDR_12

#endif /* CPU_CFG_H_ */
