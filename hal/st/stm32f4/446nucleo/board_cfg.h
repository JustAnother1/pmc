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

#include "st_gpio.h"
#include "st_rcc.h"

/******************************************************************************/
/*                                                                            */
/*         Clock Configuration                                                */
/*                                                                            */
/******************************************************************************/
// CPU Clock = 84 MHz
#define FREQUENCY_OF_CPU_CLK 180000000
// frequency the cpu runs with AHB / HCLK = 180MHz
#define FREQUENCY_OF_HCLK    ((uint32_t)180000000)
#define FREQUENCY_OF_APB1    45000000
#define FREQUENCY_OF_APB2    90000000

// Flash:
// Wait states (3,3V and 180MHz) RM0390_P65
#define WAIT_STATES   FLASH_ACR_LATENCY_5WS

// Power Scale 2
#define PWR_CR 0x00008100;

// 16MHz -> 180MHz (PLLCFGR)
#define RCC_PLLSRC RCC_PLLCFGR_PLLSRC_HSI
#define RCC_CR RCC_CR_PLLON + RCC_CR_HSION
#define USES_CLK_PLL 1
// PLLQ = 0
#define RCC_PLL_PLLQ  0
// PLLP = /2 -> 0 ( 00 = /2; 01 = /4; 10 = /6; 11 = /8)
#define RCC_PLL_PLLP  0
// PLLN = 360 -> 1 01 10 10 00 -> 3+5+6+8
#define RCC_PLL_PLLN  (RCC_PLLCFGR_PLLN_8 + RCC_PLLCFGR_PLLN_6 + RCC_PLLCFGR_PLLN_5 + RCC_PLLCFGR_PLLN_3)
// PLLM = /16
#define RCC_PLL_PLLM  16

// Clock Configuration (CFGR)
#define RCC_PRESC_APB2    RCC_CFGR_PPRE2_DIV2
#define RCC_PRESC_APB1    RCC_CFGR_PPRE1_DIV4
#define RCC_PRESC_AHB     RCC_CFGR_HPRE_DIV1
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
#define SPI_0_IRQ_NUMBER           SPI1_IRQn
#define SPI_0_IRQ_PRIORITY         8
#define SPI_1_IRQ_NUMBER           SPI2_IRQn
#define SPI_1_IRQ_PRIORITY         12
#define UART_1_IRQ_NUMBER          USART2_IRQn
#define UART_1_IRQ_PRIORITY        14
#define EXTI_0_IRQ_NUMBER          EXTI0_IRQn
#define EXTI_0_IRQ_PRIORITY        4
#define EXTI_1_IRQ_NUMBER          EXTI1_IRQn
#define EXTI_1_IRQ_PRIORITY        4
#define EXTI_2_IRQ_NUMBER          EXTI2_IRQn
#define EXTI_2_IRQ_PRIORITY        4
#define EXTI_3_IRQ_NUMBER          EXTI3_IRQn
#define EXTI_3_IRQ_PRIORITY        4
#define EXTI_4_IRQ_NUMBER          EXTI4_IRQn
#define EXTI_4_IRQ_PRIORITY        4
#define EXTI_5_9_IRQ_NUMBER        EXTI9_5_IRQn
#define EXTI_5_9_IRQ_PRIORITY      4
#define EXTI_10_15_IRQ_NUMBER      EXTI15_10_IRQn
#define EXTI_10_15_IRQ_PRIORITY    4

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

#define ADC_NUM_PINS               1
#define ADC_0_NAME                 "Temp_Bed"
#define ADC_1_NAME                 "Temp_CPU"
#define NUM_EXTERNAL_TEMPERATURES  1
#define ADC_EXT_0_NAME             "Temp_Thermocouple_Nozzle"
// C0
#define ADC_0_GPIO_PORT_RCC        RCC_AHB1ENR_GPIOCEN
#define ADC_0_GPIO_PORT            GPIOC
#define ADC_0_GPIO_MODER_1         GPIO_MODER_0_ANALOG_1
#define ADC_0_GPIO_MODER_0         GPIO_MODER_0_ANALOG_0


// Input Numbers for used ADC Input Pins,
// then 16 for the internal temperature sensor,
// then 0 till the end.
#define ADC_0_INPUT_NUM            10
#define ADC_1_INPUT_NUM            16
#define ADC_2_INPUT_NUM            0
#define ADC_3_INPUT_NUM            0
#define ADC_4_INPUT_NUM            0
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

#define BUZZER_NUM_PINS             1
#define BUZZER_0_NAME               "Buzzer_0"
#define BUZZER_0_TIMER_FREQUENCY    500000
#define BUZZER_0_RCC_GPIO_ENABLE    RCC_AHB1ENR_GPIOAEN
#define BUZZER_0_GPIO_PORT          GPIOA
#define BUZZER_0_MODER_0            GPIO_MODER_0_ALTFUNC_0
#define BUZZER_0_MODER_1            GPIO_MODER_0_ALTFUNC_1
#define BUZZER_0_OTYPER_0           GPIO_OTYPER_0_PUSH_PULL_0
#define BUZZER_0_OTYPER_1           GPIO_OTYPER_0_PUSH_PULL_1
#define BUZZER_0_OSPEEDR_0          GPIO_OSPEEDER_0_LOW_SPEED_0
#define BUZZER_0_OSPEEDR_1          GPIO_OSPEEDER_0_LOW_SPEED_1
#define BUZZER_0_PUPD_0             GPIO_PUPDR_0_NONE_0
#define BUZZER_0_PUPD_1             GPIO_PUPDR_0_NONE_1
#define BUZZER_0_ODR                GPIO_ODR_0
// PA0 = AF 2 = 0010
#define BUZZER_0_AFR_0_0     0x0000000d
#define BUZZER_0_AFR_0_1     0x00000002
#define BUZZER_0_AFR_1_0     0x00000000
#define BUZZER_0_AFR_1_1     0x00000000

/******************************************************************************/
/*                                                                            */
/*         P W M                                                              */
/*                                                                            */
/******************************************************************************/

#define PWM_NUM_PINS               3
// B4 - AF2
#define PWM_0_NAME                 "Nozzle"
#define PWM_0_RCC_GPIO_ENABLE      RCC_AHB1ENR_GPIOBEN
#define PWM_0_GPIO_PORT            GPIOB
#define PWM_0_MODER_0              GPIO_MODER_4_ALTFUNC_0
#define PWM_0_MODER_1              GPIO_MODER_4_ALTFUNC_1
#define PWM_0_MODER_OFF_0          GPIO_MODER_4_OUTPUT_0
#define PWM_0_MODER_OFF_1          GPIO_MODER_4_OUTPUT_1
#define PWM_0_OTYPER_0             GPIO_OTYPER_4_PUSH_PULL_0
#define PWM_0_OTYPER_1             GPIO_OTYPER_4_PUSH_PULL_1
#define PWM_0_OSPEEDR_0            GPIO_OSPEEDER_4_LOW_SPEED_0
#define PWM_0_OSPEEDR_1            GPIO_OSPEEDER_4_LOW_SPEED_1
#define PWM_0_PUPD_0               GPIO_PUPDR_4_NONE_0
#define PWM_0_PUPD_1               GPIO_PUPDR_4_NONE_1
#define PWM_0_ODR                  GPIO_ODR_4
#define PWM_0_AFR_0_0              0x000d0000
#define PWM_0_AFR_0_1              0x00020000
#define PWM_0_AFR_1_0              0x00000000
#define PWM_0_AFR_1_1              0x00000000
#define PWM_0_BSRR                 GPIO_BSRR_4
// B6 - AF2
#define PWM_1_NAME                 "Bed Heater"
#define PWM_1_RCC_GPIO_ENABLE      RCC_AHB1ENR_GPIOBEN
#define PWM_1_GPIO_PORT            GPIOB
#define PWM_1_MODER_0              GPIO_MODER_6_ALTFUNC_0
#define PWM_1_MODER_1              GPIO_MODER_6_ALTFUNC_1
#define PWM_1_MODER_OFF_0          GPIO_MODER_6_OUTPUT_0
#define PWM_1_MODER_OFF_1          GPIO_MODER_6_OUTPUT_1
#define PWM_1_OTYPER_0             GPIO_OTYPER_6_PUSH_PULL_0
#define PWM_1_OTYPER_1             GPIO_OTYPER_6_PUSH_PULL_1
#define PWM_1_OSPEEDR_0            GPIO_OSPEEDER_6_LOW_SPEED_0
#define PWM_1_OSPEEDR_1            GPIO_OSPEEDER_6_LOW_SPEED_1
#define PWM_1_PUPD_0               GPIO_PUPDR_6_NONE_0
#define PWM_1_PUPD_1               GPIO_PUPDR_6_NONE_1
#define PWM_1_ODR                  GPIO_ODR_6
#define PWM_1_AFR_0_0              0x0d000000
#define PWM_1_AFR_0_1              0x02000000
#define PWM_1_AFR_1_0              0x00000000
#define PWM_1_AFR_1_1              0x00000000
#define PWM_1_BSRR                 GPIO_BSRR_6
// A1 - AF1
#define PWM_2_NAME                 "Fan"
#define PWM_2_RCC_GPIO_ENABLE      RCC_AHB1ENR_GPIOAEN
#define PWM_2_GPIO_PORT            GPIOA
#define PWM_2_MODER_0              GPIO_MODER_1_ALTFUNC_0
#define PWM_2_MODER_1              GPIO_MODER_1_ALTFUNC_1
#define PWM_2_MODER_OFF_0          GPIO_MODER_1_OUTPUT_0
#define PWM_2_MODER_OFF_1          GPIO_MODER_1_OUTPUT_1
#define PWM_2_OTYPER_0             GPIO_OTYPER_1_PUSH_PULL_0
#define PWM_2_OTYPER_1             GPIO_OTYPER_1_PUSH_PULL_1
#define PWM_2_OSPEEDR_0            GPIO_OSPEEDER_1_LOW_SPEED_0
#define PWM_2_OSPEEDR_1            GPIO_OSPEEDER_1_LOW_SPEED_1
#define PWM_2_PUPD_0               GPIO_PUPDR_1_NONE_0
#define PWM_2_PUPD_1               GPIO_PUPDR_1_NONE_1
#define PWM_2_ODR                  GPIO_ODR_1
#define PWM_2_AFR_0_0              0x000000e0
#define PWM_2_AFR_0_1              0x00000010
#define PWM_2_AFR_1_0              0x00000000
#define PWM_2_AFR_1_1              0x00000000
#define PWM_2_BSRR                 GPIO_BSRR_1


/******************************************************************************/
/*                                                                            */
/*         L E D                                                              */
/*                                                                            */
/******************************************************************************/

#define NUMBER_OF_LED              1
// green - D2
#define LED_0_GPIO_PORT            GPIOD
#define LED_0_RCC_GPIO_ENABLE      RCC_AHB1ENR_GPIODEN
#define LED_0_MODER_0              GPIO_MODER_2_OUTPUT_0
#define LED_0_MODER_1              GPIO_MODER_2_OUTPUT_1
#define LED_0_OTYPER_0             GPIO_OTYPER_2_PUSH_PULL_0
#define LED_0_OTYPER_1             GPIO_OTYPER_2_PUSH_PULL_1
#define LED_0_OSPEEDR_0            GPIO_OSPEEDER_2_LOW_SPEED_0
#define LED_0_OSPEEDR_1            GPIO_OSPEEDER_2_LOW_SPEED_1
#define LED_0_PUPD_0               GPIO_PUPDR_2_NONE_0
#define LED_0_PUPD_1               GPIO_PUPDR_2_NONE_1
#define LED_0_ODR                  GPIO_ODR_2
#define LED_0_BSRR                 GPIO_BSRR_2

/******************************************************************************/
/*                                                                            */
/*         Universal Synchronous Asynchronous Receiver Transmitter            */
/*                                                                            */
/******************************************************************************/
// Baud Rate = PeripheralClock/(16*BRR)
// BRR = Lowest 4 bits = 1/16, Top Bits = Mantissa
// Example:
// 115200 (115226,3..) = 84MHz / (16 * 45,5625
// 45,5625 = 45 + 9/16 -> 0x2d and 0x9 -> 0x2d9
// UART0 O = Pacemaker Protocol = Pin PA10(Input) PA9(Output)
#define UART_0                     USART1  // APB2 - 90 MHz
#define UART_0_BRR                 0x30d // 115200 kbit
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

// UART1 = Debug Terminal Interface = Pin PA3(Input) PA2(Output)
#define UART_1                     USART2  // APB1 - 45 MHz
#define UART_1_BRR                 0x187 // 115200 kbit
#define UART_1_IRQ_HANDLER         USART2_IRQHandler
#define UART_1_APB1ENR             0x00020000
#define UART_1_APB2ENR             0
#define UART_1_RX_GPIO_PORT        GPIOA
#define UART_1_RX_GPIO_PORT_RCC    RCC_AHB1ENR_GPIOAEN
#define UART_1_RX_GPIO_MODER_0     GPIO_MODER_3_ALTFUNC_0
#define UART_1_RX_GPIO_MODER_1     GPIO_MODER_3_ALTFUNC_1
#define UART_1_RX_GPIO_OTYPER_0    GPIO_OTYPER_3_PUSH_PULL_0
#define UART_1_RX_GPIO_OTYPER_1    GPIO_OTYPER_3_PUSH_PULL_1
#define UART_1_RX_GPIO_OSPEEDR_0   GPIO_OSPEEDER_3_LOW_SPEED_0
#define UART_1_RX_GPIO_OSPEEDR_1   GPIO_OSPEEDER_3_LOW_SPEED_1
#define UART_1_RX_GPIO_PUPD_0      GPIO_PUPDR_3_PULL_UP_0
#define UART_1_RX_GPIO_PUPD_1      GPIO_PUPDR_3_PULL_UP_1
// PA3 = AF7
#define UART_1_RX_GPIO_AFR_0_0     0x00008000
#define UART_1_RX_GPIO_AFR_0_1     0x00007000
#define UART_1_RX_GPIO_AFR_1_0     0
#define UART_1_RX_GPIO_AFR_1_1     0


#define UART_1_TX_GPIO_PORT        GPIOA
#define UART_1_TX_GPIO_PORT_RCC    RCC_AHB1ENR_GPIOAEN
#define UART_1_TX_GPIO_MODER_0     GPIO_MODER_2_ALTFUNC_0
#define UART_1_TX_GPIO_MODER_1     GPIO_MODER_2_ALTFUNC_1
#define UART_1_TX_GPIO_OTYPER_0    GPIO_OTYPER_2_PUSH_PULL_0
#define UART_1_TX_GPIO_OTYPER_1    GPIO_OTYPER_2_PUSH_PULL_1
#define UART_1_TX_GPIO_OSPEEDR_0   GPIO_OSPEEDER_2_LOW_SPEED_0
#define UART_1_TX_GPIO_OSPEEDR_1   GPIO_OSPEEDER_2_LOW_SPEED_1
#define UART_1_TX_GPIO_PUPD_0      GPIO_PUPDR_2_PULL_UP_0
#define UART_1_TX_GPIO_PUPD_1      GPIO_PUPDR_2_PULL_UP_1
// PA2 = AF7
#define UART_1_TX_GPIO_AFR_0_0     0x00000800
#define UART_1_TX_GPIO_AFR_0_1     0x00000700
#define UART_1_TX_GPIO_AFR_1_0     0
#define UART_1_TX_GPIO_AFR_1_1     0


/******************************************************************************/
/*                                                                            */
/*         S P I                                                              */
/*                                                                            */
/******************************************************************************/
// SPI-0 = MISO(PA6), MOSI(PA7), /CS(PA4), SCLK(PA5)
#define SPI_0_MISO_GPIO_PORT_RCC  RCC_AHB1ENR_GPIOAEN
#define SPI_0_MOSI_GPIO_PORT_RCC  RCC_AHB1ENR_GPIOAEN
#define SPI_0_NSS_GPIO_PORT_RCC   RCC_AHB1ENR_GPIOAEN
#define SPI_0_SCK_GPIO_PORT_RCC   RCC_AHB1ENR_GPIOAEN
#define SPI_0                     SPI1
#define SPI_0_IRQ_HANDLER         SPI1_IRQHandler
#define SPI_0_APB1ENR             0
#define SPI_0_APB2ENR             RCC_APB2ENR_SPI1EN
#define SPI_0_MISO_GPIO_PORT      GPIOA
#define SPI_0_MOSI_GPIO_PORT      GPIOA
#define SPI_0_NSS_GPIO_PORT       GPIOA
#define SPI_0_SCK_GPIO_PORT       GPIOA

#define SPI_0_MISO_GPIO_MODER_0   GPIO_MODER_6_ALTFUNC_0
#define SPI_0_MISO_GPIO_MODER_1   GPIO_MODER_6_ALTFUNC_1
#define SPI_0_MISO_GPIO_OTYPER_0  GPIO_OTYPER_6_OPEN_DRAIN_0
#define SPI_0_MISO_GPIO_OTYPER_1  GPIO_OTYPER_6_OPEN_DRAIN_1
#define SPI_0_MISO_GPIO_OSPEEDR_0 GPIO_OSPEEDER_6_MEDIUM_SPEED_0
#define SPI_0_MISO_GPIO_OSPEEDR_1 GPIO_OSPEEDER_6_MEDIUM_SPEED_0
#define SPI_0_MISO_GPIO_PUPD_0    GPIO_PUPDR_6_PULL_UP_0
#define SPI_0_MISO_GPIO_PUPD_1    GPIO_PUPDR_6_PULL_UP_1
#define SPI_0_MISO_GPIO_AFR_0_0   0x0a0000000
#define SPI_0_MISO_GPIO_AFR_0_1   0x050000000
#define SPI_0_MISO_GPIO_AFR_1_0   0
#define SPI_0_MISO_GPIO_AFR_1_1   0

#define SPI_0_MOSI_GPIO_MODER_0   GPIO_MODER_7_ALTFUNC_0
#define SPI_0_MOSI_GPIO_MODER_1   GPIO_MODER_7_ALTFUNC_1
#define SPI_0_MOSI_GPIO_OTYPER_0  GPIO_OTYPER_7_PUSH_PULL_0
#define SPI_0_MOSI_GPIO_OTYPER_1  GPIO_OTYPER_7_PUSH_PULL_1
#define SPI_0_MOSI_GPIO_OSPEEDR_0 GPIO_OSPEEDER_7_MEDIUM_SPEED_0
#define SPI_0_MOSI_GPIO_OSPEEDR_1 GPIO_OSPEEDER_7_MEDIUM_SPEED_1
#define SPI_0_MOSI_GPIO_PUPD_0    GPIO_PUPDR_7_PULL_UP_0
#define SPI_0_MOSI_GPIO_PUPD_1    GPIO_PUPDR_7_PULL_UP_1
#define SPI_0_MOSI_GPIO_AFR_0_0   0xa0000000
#define SPI_0_MOSI_GPIO_AFR_0_1   0x50000000
#define SPI_0_MOSI_GPIO_AFR_1_0   0
#define SPI_0_MOSI_GPIO_AFR_1_1   0

#define SPI_0_NSS_GPIO_MODER_0    GPIO_MODER_4_OUTPUT_0
#define SPI_0_NSS_GPIO_MODER_1    GPIO_MODER_4_OUTPUT_1
#define SPI_0_NSS_GPIO_OTYPER_0   GPIO_OTYPER_4_PUSH_PULL_0
#define SPI_0_NSS_GPIO_OTYPER_1   GPIO_OTYPER_4_PUSH_PULL_1
#define SPI_0_NSS_GPIO_OSPEEDR_0  GPIO_OSPEEDER_4_LOW_SPEED_0
#define SPI_0_NSS_GPIO_OSPEEDR_1  GPIO_OSPEEDER_4_LOW_SPEED_0
#define SPI_0_NSS_GPIO_PUPD_0     GPIO_PUPDR_4_PULL_UP_0
#define SPI_0_NSS_GPIO_PUPD_1     GPIO_PUPDR_4_PULL_UP_1
#define SPI_0_NSS_GPIO_BSRR       GPIO_BSRR_4

#define SPI_0_SCK_GPIO_MODER_0    GPIO_MODER_5_ALTFUNC_0
#define SPI_0_SCK_GPIO_MODER_1    GPIO_MODER_5_ALTFUNC_1
#define SPI_0_SCK_GPIO_OTYPER_0   GPIO_OTYPER_5_PUSH_PULL_0
#define SPI_0_SCK_GPIO_OTYPER_1   GPIO_OTYPER_5_PUSH_PULL_1
#define SPI_0_SCK_GPIO_OSPEEDR_0  GPIO_OSPEEDER_5_MEDIUM_SPEED_0
#define SPI_0_SCK_GPIO_OSPEEDR_1  GPIO_OSPEEDER_5_MEDIUM_SPEED_1
#define SPI_0_SCK_GPIO_PUPD_0     GPIO_PUPDR_5_PULL_UP_0
#define SPI_0_SCK_GPIO_PUPD_1     GPIO_PUPDR_5_PULL_UP_0
#define SPI_0_SCK_GPIO_AFR_0_0    0x00a00000
#define SPI_0_SCK_GPIO_AFR_0_1    0x00500000
#define SPI_0_SCK_GPIO_AFR_1_0    0
#define SPI_0_SCK_GPIO_AFR_1_1    0
#define SPI_0_SCK_GPIO_BSRR       GPIO_BSRR_5

// SPI-1 MISO(PC2), MOSI(PC3), /CS(PB12), SCLK(PB10)
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

#define SPI_1_SCK_GPIO_MODER_0    GPIO_MODER_10_ALTFUNC_0
#define SPI_1_SCK_GPIO_MODER_1    GPIO_MODER_10_ALTFUNC_1
#define SPI_1_SCK_GPIO_OTYPER_0   GPIO_OTYPER_10_PUSH_PULL_0
#define SPI_1_SCK_GPIO_OTYPER_1   GPIO_OTYPER_10_PUSH_PULL_1
#define SPI_1_SCK_GPIO_OSPEEDR_0  GPIO_OSPEEDER_10_MEDIUM_SPEED_0
#define SPI_1_SCK_GPIO_OSPEEDR_1  GPIO_OSPEEDER_10_MEDIUM_SPEED_1
#define SPI_1_SCK_GPIO_PUPD_0     GPIO_PUPDR_10_PULL_UP_0
#define SPI_1_SCK_GPIO_PUPD_1     GPIO_PUPDR_10_PULL_UP_1
#define SPI_1_SCK_GPIO_AFR_0_0    0
#define SPI_1_SCK_GPIO_AFR_0_1    0
#define SPI_1_SCK_GPIO_AFR_1_0    0x00000a00
#define SPI_1_SCK_GPIO_AFR_1_1    0x00000500
#define SPI_1_SCK_GPIO_BSRR       GPIO_BSRR_10

/******************************************************************************/
/*                                                                            */
/*         Digital Output Pin                                                 */
/*                                                                            */
/******************************************************************************/

#define D_OUT_NUM_PINS             0

/******************************************************************************/
/*                                                                            */
/*         Digital Input Pin                                                  */
/*                                                                            */
/******************************************************************************/

#define EXTI_GPIO_PORT_RCC         RCC_AHB1ENR_GPIOBEN
#define EXTI_APB2ENR               RCC_APB2ENR_SYSCFGEN

#define D_IN_EXTI_FTSR             0x00000387
#define D_IN_EXTI_RTSR             0x00000387
#define D_IN_EXTI_EMR              0x00000387
#define D_IN_EXTI_IMR              0x00000387

#define D_IN_SYSCFG_EXTICR0        SYSCFG_EXTICR1_EXTI0_PB | SYSCFG_EXTICR1_EXTI1_PB | SYSCFG_EXTICR1_EXTI2_PB
#define D_IN_SYSCFG_EXTICR1        SYSCFG_EXTICR2_EXTI7_PB
#define D_IN_SYSCFG_EXTICR2        SYSCFG_EXTICR3_EXTI8_PB | SYSCFG_EXTICR3_EXTI9_PB
#define D_IN_SYSCFG_EXTICR3        0

#define D_IN_NUM_PINS              6
// B0
#define D_IN_0_NAME                "X min"
#define D_IN_0_RCC_GPIO_ENABLE     RCC_AHB1ENR_GPIOBEN
#define D_IN_0_GPIO_PORT           GPIOB
#define D_IN_0_MODER_0             GPIO_MODER_0_INPUT_0
#define D_IN_0_MODER_1             GPIO_MODER_0_INPUT_1
#define D_IN_0_OTYPER_0            GPIO_OTYPER_0_OPEN_DRAIN_0
#define D_IN_0_OTYPER_1            GPIO_OTYPER_0_OPEN_DRAIN_1
#define D_IN_0_OSPEEDR_0           GPIO_OSPEEDER_0_LOW_SPEED_0
#define D_IN_0_OSPEEDR_1           GPIO_OSPEEDER_0_LOW_SPEED_1
#define D_IN_0_PUPD_0              GPIO_PUPDR_0_PULL_DOWN_0
#define D_IN_0_PUPD_1              GPIO_PUPDR_0_PULL_DOWN_1
#define D_IN_0_IDR                 GPIO_IDR_0
// B1
#define D_IN_1_NAME                "X max"
#define D_IN_1_RCC_GPIO_ENABLE     RCC_AHB1ENR_GPIOBEN
#define D_IN_1_GPIO_PORT           GPIOB
#define D_IN_1_MODER_0             GPIO_MODER_1_INPUT_0
#define D_IN_1_MODER_1             GPIO_MODER_1_INPUT_1
#define D_IN_1_OTYPER_0            GPIO_OTYPER_1_OPEN_DRAIN_0
#define D_IN_1_OTYPER_1            GPIO_OTYPER_1_OPEN_DRAIN_1
#define D_IN_1_OSPEEDR_0           GPIO_OSPEEDER_1_LOW_SPEED_0
#define D_IN_1_OSPEEDR_1           GPIO_OSPEEDER_1_LOW_SPEED_1
#define D_IN_1_PUPD_0              GPIO_PUPDR_1_PULL_DOWN_0
#define D_IN_1_PUPD_1              GPIO_PUPDR_1_PULL_DOWN_1
#define D_IN_1_IDR                 GPIO_IDR_1
// B2
#define D_IN_2_NAME                "Y min"
#define D_IN_2_RCC_GPIO_ENABLE     RCC_AHB1ENR_GPIOBEN
#define D_IN_2_GPIO_PORT           GPIOB
#define D_IN_2_MODER_0             GPIO_MODER_2_INPUT_0
#define D_IN_2_MODER_1             GPIO_MODER_2_INPUT_1
#define D_IN_2_OTYPER_0            GPIO_OTYPER_2_OPEN_DRAIN_0
#define D_IN_2_OTYPER_1            GPIO_OTYPER_2_OPEN_DRAIN_1
#define D_IN_2_OSPEEDR_0           GPIO_OSPEEDER_2_LOW_SPEED_0
#define D_IN_2_OSPEEDR_1           GPIO_OSPEEDER_2_LOW_SPEED_1
#define D_IN_2_PUPD_0              GPIO_PUPDR_2_PULL_DOWN_0
#define D_IN_2_PUPD_1              GPIO_PUPDR_2_PULL_DOWN_1
#define D_IN_2_IDR                 GPIO_IDR_2
// B7
#define D_IN_3_NAME                "Y max"
#define D_IN_3_RCC_GPIO_ENABLE     RCC_AHB1ENR_GPIOBEN
#define D_IN_3_GPIO_PORT           GPIOB
#define D_IN_3_MODER_0             GPIO_MODER_7_INPUT_0
#define D_IN_3_MODER_1             GPIO_MODER_7_INPUT_1
#define D_IN_3_OTYPER_0            GPIO_OTYPER_7_OPEN_DRAIN_0
#define D_IN_3_OTYPER_1            GPIO_OTYPER_7_OPEN_DRAIN_1
#define D_IN_3_OSPEEDR_0           GPIO_OSPEEDER_7_LOW_SPEED_0
#define D_IN_3_OSPEEDR_1           GPIO_OSPEEDER_7_LOW_SPEED_1
#define D_IN_3_PUPD_0              GPIO_PUPDR_7_PULL_DOWN_0
#define D_IN_3_PUPD_1              GPIO_PUPDR_7_PULL_DOWN_1
#define D_IN_3_IDR                 GPIO_IDR_7
// B8
#define D_IN_4_NAME                "Z min"
#define D_IN_4_RCC_GPIO_ENABLE     RCC_AHB1ENR_GPIOBEN
#define D_IN_4_GPIO_PORT           GPIOB
#define D_IN_4_MODER_0             GPIO_MODER_8_INPUT_0
#define D_IN_4_MODER_1             GPIO_MODER_8_INPUT_1
#define D_IN_4_OTYPER_0            GPIO_OTYPER_8_OPEN_DRAIN_0
#define D_IN_4_OTYPER_1            GPIO_OTYPER_8_OPEN_DRAIN_1
#define D_IN_4_OSPEEDR_0           GPIO_OSPEEDER_8_LOW_SPEED_0
#define D_IN_4_OSPEEDR_1           GPIO_OSPEEDER_8_LOW_SPEED_1
#define D_IN_4_PUPD_0              GPIO_PUPDR_8_PULL_DOWN_0
#define D_IN_4_PUPD_1              GPIO_PUPDR_8_PULL_DOWN_1
#define D_IN_4_IDR                 GPIO_IDR_8
// B9
#define D_IN_5_NAME                "Z max"
#define D_IN_5_RCC_GPIO_ENABLE     RCC_AHB1ENR_GPIOBEN
#define D_IN_5_GPIO_PORT           GPIOB
#define D_IN_5_MODER_0             GPIO_MODER_9_INPUT_0
#define D_IN_5_MODER_1             GPIO_MODER_9_INPUT_1
#define D_IN_5_OTYPER_0            GPIO_OTYPER_9_OPEN_DRAIN_0
#define D_IN_5_OTYPER_1            GPIO_OTYPER_9_OPEN_DRAIN_1
#define D_IN_5_OSPEEDR_0           GPIO_OSPEEDER_9_LOW_SPEED_0
#define D_IN_5_OSPEEDR_1           GPIO_OSPEEDER_9_LOW_SPEED_1
#define D_IN_5_PUPD_0              GPIO_PUPDR_9_PULL_DOWN_0
#define D_IN_5_PUPD_1              GPIO_PUPDR_9_PULL_DOWN_1
#define D_IN_5_IDR                 GPIO_IDR_9

/******************************************************************************/
/*                                                                            */
/*         GPIO Port used for Stepper Step and Dir Signals                    */
/*                                                                            */
/******************************************************************************/

#define STEPPER_PORT_RCC_GPIO_ENABLE  RCC_AHB1ENR_GPIOCEN
#define STEPPER_PORT_GPIO_PORT        GPIOC

// Used Pins Mask (2 bits per pin; used = 1; unused = 0)
#define USED_PINS_MASK                0x00ffff00

// MODER : General Purpose Output
#define STEPPER_PORT_MODER            0x55555555
// OTYPER: Push Pull
#define STEPPER_PORT_OTYPER           0x00000000
// OSPEEDR: 00 = 2MHz, 01 = 25MHz, 10 = 50MHz, 11 = 100MHz
#define STEPPER_PORT_OSPEEDR          0x00000000
// Pull Up Pull Down : no
#define STEPPER_PORT_PUPD             0x00000000
// Output Data = 0
#define STEPPER_PORT_ODR              0x00000000

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
