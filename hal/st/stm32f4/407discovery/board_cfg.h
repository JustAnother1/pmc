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
// CPU Clock = 168 MHz
#define FREQUENCY_OF_CPU_CLK 168000000
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
// The Interrupt with the lowest Priority Number is the most important
// priority values can be 0..15
#define UART_0_IRQ_NUMBER          USART1_IRQn
#define UART_0_IRQ_PRIORITY        5
#define UART_1_IRQ_NUMBER          USART6_IRQn
#define UART_1_IRQ_PRIORITY        14
#define SPI_0_IRQ_NUMBER           SPI1_IRQn
#define SPI_0_IRQ_PRIORITY         8
#define SPI_1_IRQ_NUMBER           SPI2_IRQn
#define SPI_1_IRQ_PRIORITY         12
#define USB_FS_IRQ_NUMBER          OTG_FS_IRQn
#define USB_FS_IRQ_PRIORITY        6

/******************************************************************************/
/*                                                                            */
/*         T i m e r                                                          */
/*                                                                            */
/******************************************************************************/

#define TIM_1_IRQ_NUMBER           TIM1_CC_IRQn
#define TIM_1_IRQ_PRIORITY         4
#define TIM_2_IRQ_NUMBER           TIM2_IRQn
#define TIM_2_IRQ_PRIORITY         4
#define TIM_3_IRQ_NUMBER           TIM3_IRQn
#define TIM_3_IRQ_PRIORITY         4
#define TIM_4_IRQ_NUMBER           TIM4_IRQn
#define TIM_4_IRQ_PRIORITY         4
#define TIM_5_IRQ_NUMBER           TIM5_IRQn
#define TIM_5_IRQ_PRIORITY         4
#define TIM_6_IRQ_NUMBER           TIM6_DAC_IRQn
#define TIM_6_IRQ_PRIORITY         2
#define TIM_7_IRQ_NUMBER           TIM7_IRQn
#define TIM_7_IRQ_PRIORITY         10
#define TIM_8_IRQ_NUMBER           TIM8_CC_IRQn
#define TIM_8_IRQ_PRIORITY         4
#define TIM_9_IRQ_NUMBER           TIM1_BRK_TIM9_IRQn
#define TIM_9_IRQ_PRIORITY         4
#define TIM_10_IRQ_NUMBER          TIM1_UP_TIM10_IRQn
#define TIM_10_IRQ_PRIORITY        4
#define TIM_11_IRQ_NUMBER          TIM1_TRG_COM_TIM11_IRQn
#define TIM_11_IRQ_PRIORITY        4
#define TIM_12_IRQ_NUMBER          TIM8_BRK_TIM12_IRQn
#define TIM_12_IRQ_PRIORITY        4
#define TIM_13_IRQ_NUMBER          TIM8_UP_TIM13_IRQn
#define TIM_13_IRQ_PRIORITY        4
#define TIM_14_IRQ_NUMBER          TIM8_TRG_COM_TIM14_IRQn
#define TIM_14_IRQ_PRIORITY        4

/******************************************************************************/
/*                                                                            */
/*         A D C                                                              */
/*                                                                            */
/******************************************************************************/

#define ADC_NUM_PINS               4
#define ADC_0_NAME                 "Temp_0"
#define ADC_1_NAME                 "Temp_1"
#define ADC_2_NAME                 "Temp_2"
#define ADC_3_NAME                 "Temp_3"
#define ADC_4_NAME                 "Temp_4"
#define ADC_5_NAME                 "Temp_5"

#define ADC_0_GPIO_PORT_RCC        RCC_AHB1ENR_GPIOAEN
#define ADC_0_GPIO_PORT            GPIOA
#define ADC_0_GPIO_MODER_1         GPIO_MODER_5_ANALOG_1
#define ADC_0_GPIO_MODER_0         GPIO_MODER_5_ANALOG_0
#define ADC_1_GPIO_PORT_RCC        RCC_AHB1ENR_GPIOCEN
#define ADC_1_GPIO_PORT            GPIOC
#define ADC_1_GPIO_MODER_1         GPIO_MODER_0_ANALOG_1
#define ADC_1_GPIO_MODER_0         GPIO_MODER_0_ANALOG_0
#define ADC_2_GPIO_PORT_RCC        RCC_AHB1ENR_GPIOCEN
#define ADC_2_GPIO_PORT            GPIOC
#define ADC_2_GPIO_MODER_1         GPIO_MODER_1_ANALOG_1
#define ADC_2_GPIO_MODER_0         GPIO_MODER_1_ANALOG_0
#define ADC_3_GPIO_PORT_RCC        RCC_AHB1ENR_GPIOCEN
#define ADC_3_GPIO_PORT            GPIOC
#define ADC_3_GPIO_MODER_1         GPIO_MODER_4_ANALOG_1
#define ADC_3_GPIO_MODER_0         GPIO_MODER_4_ANALOG_0

// Input Numbers for used ADC Input Pins,
// then 16 for the internal temperature sensor,
// then 0 till the end.
#define ADC_0_INPUT_NUM            5
#define ADC_1_INPUT_NUM            10
#define ADC_2_INPUT_NUM            11
#define ADC_3_INPUT_NUM            14
#define ADC_4_INPUT_NUM            16
#define ADC_5_INPUT_NUM            0
#define ADC_6_INPUT_NUM            0
#define ADC_7_INPUT_NUM            0
#define ADC_8_INPUT_NUM            0
#define ADC_9_INPUT_NUM            0
#define ADC_10_INPUT_NUM           0
#define ADC_11_INPUT_NUM           0
#define ADC_12_INPUT_NUM           0
#define ADC_13_INPUT_NUM           0
#define ADC_14_INPUT_NUM           0
#define ADC_15_INPUT_NUM           0

/******************************************************************************/
/*                                                                            */
/*         Buzzer - Tone Generator                                            */
/*                                                                            */
/******************************************************************************/

#define BUZZER_NUM_PINS            1
#define BUZZER_0_NAME              "Buzzer_0"
#define BUZZER_0_RCC_GPIO_ENABLE    RCC_AHB1ENR_GPIOBEN
#define BUZZER_0_GPIO_PORT          GPIOB
#define BUZZER_0_MODER_0            GPIO_MODER_9_INPUT_0
#define BUZZER_0_MODER_1            GPIO_MODER_9_INPUT_1
#define BUZZER_0_OTYPER_0           GPIO_OTYPER_9_OPEN_DRAIN_0
#define BUZZER_0_OTYPER_1           GPIO_OTYPER_9_OPEN_DRAIN_1
#define BUZZER_0_OSPEEDR_0          GPIO_OSPEEDER_9_LOW_SPEED_0
#define BUZZER_0_OSPEEDR_1          GPIO_OSPEEDER_9_LOW_SPEED_1
#define BUZZER_0_PUPD_0             GPIO_PUPDR_9_NONE_0
#define BUZZER_0_PUPD_1             GPIO_PUPDR_9_NONE_1
#define BUZZER_0_ODR                GPIO_ODR_9
// PB9 = AF 3 = 0011
#define BUZZER_0_AFR_0_0     0
#define BUZZER_0_AFR_0_1     0
#define BUZZER_0_AFR_1_0     0x00000030
#define BUZZER_0_AFR_1_1     0x000000c0

/******************************************************************************/
/*                                                                            */
/*         P W M                                                              */
/*                                                                            */
/******************************************************************************/

#define PWM_NUM_PINS               14
#define PWM_0_NAME                 "PWM 1"
#define PWM_1_NAME                 "PWM 2"
#define PWM_2_NAME                 "PWM 3"
#define PWM_3_NAME                 "PWM 4"
#define PWM_4_NAME                 "PWM 5"
#define PWM_5_NAME                 "PWM 6"
#define PWM_6_NAME                 "PWM 7"
#define PWM_7_NAME                 "PWM 8"
#define PWM_8_NAME                 "PWM 9"
#define PWM_9_NAME                 "PWM 10"
#define PWM_10_NAME                "PWM 11"
#define PWM_11_NAME                "PWM 12"
#define PWM_12_NAME                "PWM 13"
#define PWM_13_NAME                "PWM 14"

/******************************************************************************/
/*                                                                            */
/*         L E D                                                              */
/*                                                                            */
/******************************************************************************/

#define NUMBER_OF_LED              4
// green
#define LED_0_GPIO_PORT            GPIOD
#define LED_0_RCC_GPIO_ENABLE      RCC_AHB1ENR_GPIODEN
#define LED_0_MODER_0              GPIO_MODER_12_OUTPUT_0
#define LED_0_MODER_1              GPIO_MODER_12_OUTPUT_1
#define LED_0_OTYPER_0             GPIO_OTYPER_12_PUSH_PULL_0
#define LED_0_OTYPER_1             GPIO_OTYPER_12_PUSH_PULL_1
#define LED_0_OSPEEDR_0            GPIO_OSPEEDER_12_LOW_SPEED_0
#define LED_0_OSPEEDR_1            GPIO_OSPEEDER_12_LOW_SPEED_1
#define LED_0_PUPD_0               GPIO_PUPDR_12_NONE_0
#define LED_0_PUPD_1               GPIO_PUPDR_12_NONE_1
#define LED_0_ODR                  GPIO_ODR_12
#define LED_0_BSRR                 GPIO_BSRR_12
// orange
#define LED_1_GPIO_PORT            GPIOD
#define LED_1_RCC_GPIO_ENABLE      RCC_AHB1ENR_GPIODEN
#define LED_1_MODER_0              GPIO_MODER_13_OUTPUT_0
#define LED_1_MODER_1              GPIO_MODER_13_OUTPUT_1
#define LED_1_OTYPER_0             GPIO_OTYPER_13_PUSH_PULL_0
#define LED_1_OTYPER_1             GPIO_OTYPER_13_PUSH_PULL_1
#define LED_1_OSPEEDR_0            GPIO_OSPEEDER_13_LOW_SPEED_0
#define LED_1_OSPEEDR_1            GPIO_OSPEEDER_13_LOW_SPEED_1
#define LED_1_PUPD_0               GPIO_PUPDR_13_NONE_0
#define LED_1_PUPD_1               GPIO_PUPDR_13_NONE_1
#define LED_1_ODR                  GPIO_ODR_13
#define LED_1_BSRR                 GPIO_BSRR_13
// red
#define LED_2_GPIO_PORT            GPIOD
#define LED_2_RCC_GPIO_ENABLE      RCC_AHB1ENR_GPIODEN
#define LED_2_MODER_0              GPIO_MODER_14_OUTPUT_0
#define LED_2_MODER_1              GPIO_MODER_14_OUTPUT_1
#define LED_2_OTYPER_0             GPIO_OTYPER_14_PUSH_PULL_0
#define LED_2_OTYPER_1             GPIO_OTYPER_14_PUSH_PULL_1
#define LED_2_OSPEEDR_0            GPIO_OSPEEDER_14_LOW_SPEED_0
#define LED_2_OSPEEDR_1            GPIO_OSPEEDER_14_LOW_SPEED_1
#define LED_2_PUPD_0               GPIO_PUPDR_14_NONE_0
#define LED_2_PUPD_1               GPIO_PUPDR_14_NONE_1
#define LED_2_ODR                  GPIO_ODR_14
#define LED_2_BSRR                 GPIO_BSRR_14
// blue
#define LED_3_GPIO_PORT            GPIOD
#define LED_3_RCC_GPIO_ENABLE      RCC_AHB1ENR_GPIODEN
#define LED_3_MODER_0              GPIO_MODER_15_OUTPUT_0
#define LED_3_MODER_1              GPIO_MODER_15_OUTPUT_1
#define LED_3_OTYPER_0             GPIO_OTYPER_15_PUSH_PULL_0
#define LED_3_OTYPER_1             GPIO_OTYPER_15_PUSH_PULL_1
#define LED_3_OSPEEDR_0            GPIO_OSPEEDER_15_LOW_SPEED_0
#define LED_3_OSPEEDR_1            GPIO_OSPEEDER_15_LOW_SPEED_1
#define LED_3_PUPD_0               GPIO_PUPDR_15_NONE_0
#define LED_3_PUPD_1               GPIO_PUPDR_15_NONE_1
#define LED_3_ODR                  GPIO_ODR_15
#define LED_3_BSRR                 GPIO_BSRR_15

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
#define SPI_0_MISO_GPIO_OTYPER_0  GPIO_OTYPER_4_OPEN_DRAIN_0
#define SPI_0_MISO_GPIO_OTYPER_1  GPIO_OTYPER_4_OPEN_DRAIN_1
#define SPI_0_MISO_GPIO_OSPEEDR_0 GPIO_OSPEEDER_4_MEDIUM_SPEED_0
#define SPI_0_MISO_GPIO_OSPEEDR_1 GPIO_OSPEEDER_4_MEDIUM_SPEED_0
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
#define SPI_0_MOSI_GPIO_OSPEEDR_0 GPIO_OSPEEDER_5_MEDIUM_SPEED_0
#define SPI_0_MOSI_GPIO_OSPEEDR_1 GPIO_OSPEEDER_5_MEDIUM_SPEED_1
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
#define SPI_0_SCK_GPIO_OSPEEDR_0  GPIO_OSPEEDER_3_MEDIUM_SPEED_0
#define SPI_0_SCK_GPIO_OSPEEDR_1  GPIO_OSPEEDER_3_MEDIUM_SPEED_1
#define SPI_0_SCK_GPIO_PUPD_0     GPIO_PUPDR_3_PULL_UP_0
#define SPI_0_SCK_GPIO_PUPD_1     GPIO_PUPDR_3_PULL_UP_0
#define SPI_0_SCK_GPIO_AFR_0_0    0x0000a000
#define SPI_0_SCK_GPIO_AFR_0_1    0x00005000
#define SPI_0_SCK_GPIO_AFR_1_0    0
#define SPI_0_SCK_GPIO_AFR_1_1    0
#define SPI_0_SCK_GPIO_BSRR       GPIO_BSRR_3


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
#define SPI_1_MISO_GPIO_OTYPER_0  GPIO_OTYPER_2_OPEN_DRAIN_0
#define SPI_1_MISO_GPIO_OTYPER_1  GPIO_OTYPER_2_OPEN_DRAIN_1
#define SPI_1_MISO_GPIO_OSPEEDR_0 GPIO_OSPEEDER_2_MEDIUM_SPEED_0
#define SPI_1_MISO_GPIO_OSPEEDR_1 GPIO_OSPEEDER_2_MEDIUM_SPEED_1
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
#define SPI_1_MOSI_GPIO_OSPEEDR_0 GPIO_OSPEEDER_3_MEDIUM_SPEED_0
#define SPI_1_MOSI_GPIO_OSPEEDR_1 GPIO_OSPEEDER_3_MEDIUM_SPEED_1
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
#define SPI_1_SCK_GPIO_OSPEEDR_0  GPIO_OSPEEDER_13_MEDIUM_SPEED_0
#define SPI_1_SCK_GPIO_OSPEEDR_1  GPIO_OSPEEDER_13_MEDIUM_SPEED_1
#define SPI_1_SCK_GPIO_PUPD_0     GPIO_PUPDR_13_PULL_UP_0
#define SPI_1_SCK_GPIO_PUPD_1     GPIO_PUPDR_13_PULL_UP_1
#define SPI_1_SCK_GPIO_AFR_0_0    0
#define SPI_1_SCK_GPIO_AFR_0_1    0
#define SPI_1_SCK_GPIO_AFR_1_0    0x00a00000
#define SPI_1_SCK_GPIO_AFR_1_1    0x00500000
#define SPI_1_SCK_GPIO_BSRR       GPIO_BSRR_13


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
/*         Digital Input Pin                                                  */
/*                                                                            */
/******************************************************************************/

#define D_IN_NUM_PINS              3
#define D_IN_0_NAME                "In 1"
#define D_IN_0_RCC_GPIO_ENABLE     RCC_AHB1ENR_GPIOCEN
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
#define D_IN_1_RCC_GPIO_ENABLE     RCC_AHB1ENR_GPIOCEN
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
#define D_IN_2_RCC_GPIO_ENABLE     RCC_AHB1ENR_GPIOCEN
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

/******************************************************************************/
/*                                                                            */
/*         GPIO Port used for Stepper Step and Dir Signals                    */
/*                                                                            */
/******************************************************************************/

#define STEPPER_PORT_RCC_GPIO_ENABLE  RCC_AHB1ENR_GPIODEN
#define STEPPER_PORT_GPIO_PORT        GPIOD
#define STEPPER_PORT_MODER            0x55555555
#define STEPPER_PORT_OTYPER           0x00000000
#define STEPPER_PORT_OSPEEDR          0x00000000
#define STEPPER_PORT_PUPD             0x00000000
#define STEPPER_PORT_ODR              0x00000000

/******************************************************************************/
/*                                                                            */
/*         USB FS - Universal Serial Bus Full Speed (12 MHz)                  */
/*                                                                            */
/******************************************************************************/

#define USB_FS_DP_GPIO_PORT_RCC    RCC_AHB1ENR_GPIOAEN
#define USB_FS_DM_GPIO_PORT_RCC    RCC_AHB1ENR_GPIOAEN
#define USB_FS_AHB2ENR             0x00000080
#define USB_FS_IRQ_HANDLER         OTG_FS_IRQHandler
#define USB_FS_DM_GPIO_PORT        GPIOA
#define USB_FS_DM_GPIO_MODER_0     GPIO_MODER_11_ALTFUNC_0
#define USB_FS_DM_GPIO_MODER_1     GPIO_MODER_11_ALTFUNC_1
#define USB_FS_DM_GPIO_AFR_0_0     0
#define USB_FS_DM_GPIO_AFR_0_1     0
#define USB_FS_DM_GPIO_AFR_1_0     0x00005000
#define USB_FS_DM_GPIO_AFR_1_1     0x0000a000
#define USB_FS_DM_GPIO_OTYPER_0    GPIO_OTYPER_11_PUSH_PULL_0
#define USB_FS_DM_GPIO_OTYPER_1    GPIO_OTYPER_11_PUSH_PULL_1
#define USB_FS_DM_GPIO_PUPD_0      GPIO_PUPDR_11_NONE_0
#define USB_FS_DM_GPIO_PUPD_1      GPIO_PUPDR_11_NONE_1
#define USB_FS_DM_GPIO_OSPEEDR_0   GPIO_OSPEEDER_11_HIGH_SPEED_0
#define USB_FS_DM_GPIO_OSPEEDR_1   GPIO_OSPEEDER_11_HIGH_SPEED_1
#define USB_FS_DP_GPIO_PORT        GPIOA
#define USB_FS_DP_GPIO_MODER_0     GPIO_MODER_12_ALTFUNC_0
#define USB_FS_DP_GPIO_MODER_1     GPIO_MODER_12_ALTFUNC_1
#define USB_FS_DP_GPIO_AFR_0_0     0
#define USB_FS_DP_GPIO_AFR_0_1     0
#define USB_FS_DP_GPIO_AFR_1_0     0x00050000
#define USB_FS_DP_GPIO_AFR_1_1     0x000a0000
#define USB_FS_DP_GPIO_OTYPER_0    GPIO_OTYPER_12_PUSH_PULL_0
#define USB_FS_DP_GPIO_OTYPER_1    GPIO_OTYPER_12_PUSH_PULL_1
#define USB_FS_DP_GPIO_PUPD_0      GPIO_PUPDR_12_NONE_0
#define USB_FS_DP_GPIO_PUPD_1      GPIO_PUPDR_12_NONE_1
#define USB_FS_DP_GPIO_OSPEEDR_0   GPIO_OSPEEDER_12_HIGH_SPEED_0
#define USB_FS_DP_GPIO_OSPEEDR_1   GPIO_OSPEEDER_12_HIGH_SPEED_1

/******************************************************************************/
/*                                                                            */
/*         P O W E R                                                          */
/*                                                                            */
/******************************************************************************/

// HighPower - not available
#define POWER_HAS_HIGH_SWITCH         0
// 5V - not available
#define POWER_HAS_5V_SWITCH           0
// 12V - not available
#define POWER_HAS_12V_SWITCH           0

#endif /* CPU_CFG_H_ */
