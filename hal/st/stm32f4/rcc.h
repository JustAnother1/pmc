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

#ifndef RCC_H_
#define RCC_H_

#include "hal_common.h"
#include "core_cm4.h"

/******************************************************************************/
/*                                                                            */
/*                         Reset and Clock Control                            */
/*                                                                            */
/******************************************************************************/
/********************  Bit definition for RCC_CR register  ********************/
#define  RCC_CR_HSION                        ((uint_fast32_t)0x00000001)
#define  RCC_CR_HSIRDY                       ((uint_fast32_t)0x00000002)

#define  RCC_CR_HSITRIM                      ((uint_fast32_t)0x000000F8)
#define  RCC_CR_HSITRIM_0                    ((uint_fast32_t)0x00000008)/*!<Bit 0 */
#define  RCC_CR_HSITRIM_1                    ((uint_fast32_t)0x00000010)/*!<Bit 1 */
#define  RCC_CR_HSITRIM_2                    ((uint_fast32_t)0x00000020)/*!<Bit 2 */
#define  RCC_CR_HSITRIM_3                    ((uint_fast32_t)0x00000040)/*!<Bit 3 */
#define  RCC_CR_HSITRIM_4                    ((uint_fast32_t)0x00000080)/*!<Bit 4 */

#define  RCC_CR_HSICAL                       ((uint_fast32_t)0x0000FF00)
#define  RCC_CR_HSICAL_0                     ((uint_fast32_t)0x00000100)/*!<Bit 0 */
#define  RCC_CR_HSICAL_1                     ((uint_fast32_t)0x00000200)/*!<Bit 1 */
#define  RCC_CR_HSICAL_2                     ((uint_fast32_t)0x00000400)/*!<Bit 2 */
#define  RCC_CR_HSICAL_3                     ((uint_fast32_t)0x00000800)/*!<Bit 3 */
#define  RCC_CR_HSICAL_4                     ((uint_fast32_t)0x00001000)/*!<Bit 4 */
#define  RCC_CR_HSICAL_5                     ((uint_fast32_t)0x00002000)/*!<Bit 5 */
#define  RCC_CR_HSICAL_6                     ((uint_fast32_t)0x00004000)/*!<Bit 6 */
#define  RCC_CR_HSICAL_7                     ((uint_fast32_t)0x00008000)/*!<Bit 7 */

#define  RCC_CR_HSEON                        ((uint_fast32_t)0x00010000)
#define  RCC_CR_HSERDY                       ((uint_fast32_t)0x00020000)
#define  RCC_CR_HSEBYP                       ((uint_fast32_t)0x00040000)
#define  RCC_CR_CSSON                        ((uint_fast32_t)0x00080000)
#define  RCC_CR_PLLON                        ((uint_fast32_t)0x01000000)
#define  RCC_CR_PLLRDY                       ((uint_fast32_t)0x02000000)
#define  RCC_CR_PLLI2SON                     ((uint_fast32_t)0x04000000)
#define  RCC_CR_PLLI2SRDY                    ((uint_fast32_t)0x08000000)

/********************  Bit definition for RCC_PLLCFGR register  ***************/
#define  RCC_PLLCFGR_PLLM                    ((uint_fast32_t)0x0000003F)
#define  RCC_PLLCFGR_PLLM_0                  ((uint_fast32_t)0x00000001)
#define  RCC_PLLCFGR_PLLM_1                  ((uint_fast32_t)0x00000002)
#define  RCC_PLLCFGR_PLLM_2                  ((uint_fast32_t)0x00000004)
#define  RCC_PLLCFGR_PLLM_3                  ((uint_fast32_t)0x00000008)
#define  RCC_PLLCFGR_PLLM_4                  ((uint_fast32_t)0x00000010)
#define  RCC_PLLCFGR_PLLM_5                  ((uint_fast32_t)0x00000020)

#define  RCC_PLLCFGR_PLLN                     ((uint_fast32_t)0x00007FC0)
#define  RCC_PLLCFGR_PLLN_0                   ((uint_fast32_t)0x00000040)
#define  RCC_PLLCFGR_PLLN_1                   ((uint_fast32_t)0x00000080)
#define  RCC_PLLCFGR_PLLN_2                   ((uint_fast32_t)0x00000100)
#define  RCC_PLLCFGR_PLLN_3                   ((uint_fast32_t)0x00000200)
#define  RCC_PLLCFGR_PLLN_4                   ((uint_fast32_t)0x00000400)
#define  RCC_PLLCFGR_PLLN_5                   ((uint_fast32_t)0x00000800)
#define  RCC_PLLCFGR_PLLN_6                   ((uint_fast32_t)0x00001000)
#define  RCC_PLLCFGR_PLLN_7                   ((uint_fast32_t)0x00002000)
#define  RCC_PLLCFGR_PLLN_8                   ((uint_fast32_t)0x00004000)

#define  RCC_PLLCFGR_PLLP                    ((uint_fast32_t)0x00030000)
#define  RCC_PLLCFGR_PLLP_0                  ((uint_fast32_t)0x00010000)
#define  RCC_PLLCFGR_PLLP_1                  ((uint_fast32_t)0x00020000)

#define  RCC_PLLCFGR_PLLSRC                  ((uint_fast32_t)0x00400000)
#define  RCC_PLLCFGR_PLLSRC_HSE              ((uint_fast32_t)0x00400000)
#define  RCC_PLLCFGR_PLLSRC_HSI              ((uint_fast32_t)0x00000000)

#define  RCC_PLLCFGR_PLLQ                    ((uint_fast32_t)0x0F000000)
#define  RCC_PLLCFGR_PLLQ_0                  ((uint_fast32_t)0x01000000)
#define  RCC_PLLCFGR_PLLQ_1                  ((uint_fast32_t)0x02000000)
#define  RCC_PLLCFGR_PLLQ_2                  ((uint_fast32_t)0x04000000)
#define  RCC_PLLCFGR_PLLQ_3                  ((uint_fast32_t)0x08000000)

/********************  Bit definition for RCC_CFGR register  ******************/
/*!< SW configuration */
#define  RCC_CFGR_SW                         ((uint_fast32_t)0x00000003)        /*!< SW[1:0] bits (System clock Switch) */
#define  RCC_CFGR_SW_0                       ((uint_fast32_t)0x00000001)        /*!< Bit 0 */
#define  RCC_CFGR_SW_1                       ((uint_fast32_t)0x00000002)        /*!< Bit 1 */

#define  RCC_CFGR_SW_HSI                     ((uint_fast32_t)0x00000000)        /*!< HSI selected as system clock */
#define  RCC_CFGR_SW_HSE                     ((uint_fast32_t)0x00000001)        /*!< HSE selected as system clock */
#define  RCC_CFGR_SW_PLL                     ((uint_fast32_t)0x00000002)        /*!< PLL selected as system clock */

/*!< SWS configuration */
#define  RCC_CFGR_SWS                        ((uint_fast32_t)0x0000000C)        /*!< SWS[1:0] bits (System Clock Switch Status) */
#define  RCC_CFGR_SWS_0                      ((uint_fast32_t)0x00000004)        /*!< Bit 0 */
#define  RCC_CFGR_SWS_1                      ((uint_fast32_t)0x00000008)        /*!< Bit 1 */

#define  RCC_CFGR_SWS_HSI                    ((uint_fast32_t)0x00000000)        /*!< HSI oscillator used as system clock */
#define  RCC_CFGR_SWS_HSE                    ((uint_fast32_t)0x00000004)        /*!< HSE oscillator used as system clock */
#define  RCC_CFGR_SWS_PLL                    ((uint_fast32_t)0x00000008)        /*!< PLL used as system clock */

/*!< HPRE configuration */
#define  RCC_CFGR_HPRE                       ((uint_fast32_t)0x000000F0)        /*!< HPRE[3:0] bits (AHB prescaler) */
#define  RCC_CFGR_HPRE_0                     ((uint_fast32_t)0x00000010)        /*!< Bit 0 */
#define  RCC_CFGR_HPRE_1                     ((uint_fast32_t)0x00000020)        /*!< Bit 1 */
#define  RCC_CFGR_HPRE_2                     ((uint_fast32_t)0x00000040)        /*!< Bit 2 */
#define  RCC_CFGR_HPRE_3                     ((uint_fast32_t)0x00000080)        /*!< Bit 3 */

#define  RCC_CFGR_HPRE_DIV1                  ((uint_fast32_t)0x00000000)        /*!< SYSCLK not divided */
#define  RCC_CFGR_HPRE_DIV2                  ((uint_fast32_t)0x00000080)        /*!< SYSCLK divided by 2 */
#define  RCC_CFGR_HPRE_DIV4                  ((uint_fast32_t)0x00000090)        /*!< SYSCLK divided by 4 */
#define  RCC_CFGR_HPRE_DIV8                  ((uint_fast32_t)0x000000A0)        /*!< SYSCLK divided by 8 */
#define  RCC_CFGR_HPRE_DIV16                 ((uint_fast32_t)0x000000B0)        /*!< SYSCLK divided by 16 */
#define  RCC_CFGR_HPRE_DIV64                 ((uint_fast32_t)0x000000C0)        /*!< SYSCLK divided by 64 */
#define  RCC_CFGR_HPRE_DIV128                ((uint_fast32_t)0x000000D0)        /*!< SYSCLK divided by 128 */
#define  RCC_CFGR_HPRE_DIV256                ((uint_fast32_t)0x000000E0)        /*!< SYSCLK divided by 256 */
#define  RCC_CFGR_HPRE_DIV512                ((uint_fast32_t)0x000000F0)        /*!< SYSCLK divided by 512 */

/*!< PPRE1 configuration */
#define  RCC_CFGR_PPRE1                      ((uint_fast32_t)0x00001C00)        /*!< PRE1[2:0] bits (APB1 prescaler) */
#define  RCC_CFGR_PPRE1_0                    ((uint_fast32_t)0x00000400)        /*!< Bit 0 */
#define  RCC_CFGR_PPRE1_1                    ((uint_fast32_t)0x00000800)        /*!< Bit 1 */
#define  RCC_CFGR_PPRE1_2                    ((uint_fast32_t)0x00001000)        /*!< Bit 2 */

#define  RCC_CFGR_PPRE1_DIV1                 ((uint_fast32_t)0x00000000)        /*!< HCLK not divided */
#define  RCC_CFGR_PPRE1_DIV2                 ((uint_fast32_t)0x00001000)        /*!< HCLK divided by 2 */
#define  RCC_CFGR_PPRE1_DIV4                 ((uint_fast32_t)0x00001400)        /*!< HCLK divided by 4 */
#define  RCC_CFGR_PPRE1_DIV8                 ((uint_fast32_t)0x00001800)        /*!< HCLK divided by 8 */
#define  RCC_CFGR_PPRE1_DIV16                ((uint_fast32_t)0x00001C00)        /*!< HCLK divided by 16 */

/*!< PPRE2 configuration */
#define  RCC_CFGR_PPRE2                      ((uint_fast32_t)0x0000E000)        /*!< PRE2[2:0] bits (APB2 prescaler) */
#define  RCC_CFGR_PPRE2_0                    ((uint_fast32_t)0x00002000)        /*!< Bit 0 */
#define  RCC_CFGR_PPRE2_1                    ((uint_fast32_t)0x00004000)        /*!< Bit 1 */
#define  RCC_CFGR_PPRE2_2                    ((uint_fast32_t)0x00008000)        /*!< Bit 2 */

#define  RCC_CFGR_PPRE2_DIV1                 ((uint_fast32_t)0x00000000)        /*!< HCLK not divided */
#define  RCC_CFGR_PPRE2_DIV2                 ((uint_fast32_t)0x00008000)        /*!< HCLK divided by 2 */
#define  RCC_CFGR_PPRE2_DIV4                 ((uint_fast32_t)0x0000A000)        /*!< HCLK divided by 4 */
#define  RCC_CFGR_PPRE2_DIV8                 ((uint_fast32_t)0x0000C000)        /*!< HCLK divided by 8 */
#define  RCC_CFGR_PPRE2_DIV16                ((uint_fast32_t)0x0000E000)        /*!< HCLK divided by 16 */

/*!< RTCPRE configuration */
#define  RCC_CFGR_RTCPRE                     ((uint_fast32_t)0x001F0000)
#define  RCC_CFGR_RTCPRE_0                   ((uint_fast32_t)0x00010000)
#define  RCC_CFGR_RTCPRE_1                   ((uint_fast32_t)0x00020000)
#define  RCC_CFGR_RTCPRE_2                   ((uint_fast32_t)0x00040000)
#define  RCC_CFGR_RTCPRE_3                   ((uint_fast32_t)0x00080000)
#define  RCC_CFGR_RTCPRE_4                   ((uint_fast32_t)0x00100000)

/*!< MCO1 configuration */
#define  RCC_CFGR_MCO1                       ((uint_fast32_t)0x00600000)
#define  RCC_CFGR_MCO1_0                     ((uint_fast32_t)0x00200000)
#define  RCC_CFGR_MCO1_1                     ((uint_fast32_t)0x00400000)

#define  RCC_CFGR_I2SSRC                     ((uint_fast32_t)0x00800000)

#define  RCC_CFGR_MCO1PRE                    ((uint_fast32_t)0x07000000)
#define  RCC_CFGR_MCO1PRE_0                  ((uint_fast32_t)0x01000000)
#define  RCC_CFGR_MCO1PRE_1                  ((uint_fast32_t)0x02000000)
#define  RCC_CFGR_MCO1PRE_2                  ((uint_fast32_t)0x04000000)

#define  RCC_CFGR_MCO2PRE                    ((uint_fast32_t)0x38000000)
#define  RCC_CFGR_MCO2PRE_0                  ((uint_fast32_t)0x08000000)
#define  RCC_CFGR_MCO2PRE_1                  ((uint_fast32_t)0x10000000)
#define  RCC_CFGR_MCO2PRE_2                  ((uint_fast32_t)0x20000000)

#define  RCC_CFGR_MCO2                       ((uint_fast32_t)0xC0000000)
#define  RCC_CFGR_MCO2_0                     ((uint_fast32_t)0x40000000)
#define  RCC_CFGR_MCO2_1                     ((uint_fast32_t)0x80000000)

/********************  Bit definition for RCC_CIR register  *******************/
#define  RCC_CIR_LSIRDYF                     ((uint_fast32_t)0x00000001)
#define  RCC_CIR_LSERDYF                     ((uint_fast32_t)0x00000002)
#define  RCC_CIR_HSIRDYF                     ((uint_fast32_t)0x00000004)
#define  RCC_CIR_HSERDYF                     ((uint_fast32_t)0x00000008)
#define  RCC_CIR_PLLRDYF                     ((uint_fast32_t)0x00000010)
#define  RCC_CIR_PLLI2SRDYF                  ((uint_fast32_t)0x00000020)

#define  RCC_CIR_CSSF                        ((uint_fast32_t)0x00000080)
#define  RCC_CIR_LSIRDYIE                    ((uint_fast32_t)0x00000100)
#define  RCC_CIR_LSERDYIE                    ((uint_fast32_t)0x00000200)
#define  RCC_CIR_HSIRDYIE                    ((uint_fast32_t)0x00000400)
#define  RCC_CIR_HSERDYIE                    ((uint_fast32_t)0x00000800)
#define  RCC_CIR_PLLRDYIE                    ((uint_fast32_t)0x00001000)
#define  RCC_CIR_PLLI2SRDYIE                 ((uint_fast32_t)0x00002000)

#define  RCC_CIR_LSIRDYC                     ((uint_fast32_t)0x00010000)
#define  RCC_CIR_LSERDYC                     ((uint_fast32_t)0x00020000)
#define  RCC_CIR_HSIRDYC                     ((uint_fast32_t)0x00040000)
#define  RCC_CIR_HSERDYC                     ((uint_fast32_t)0x00080000)
#define  RCC_CIR_PLLRDYC                     ((uint_fast32_t)0x00100000)
#define  RCC_CIR_PLLI2SRDYC                  ((uint_fast32_t)0x00200000)

#define  RCC_CIR_CSSC                        ((uint_fast32_t)0x00800000)

/********************  Bit definition for RCC_AHB1RSTR register  **************/
#define  RCC_AHB1RSTR_GPIOARST               ((uint_fast32_t)0x00000001)
#define  RCC_AHB1RSTR_GPIOBRST               ((uint_fast32_t)0x00000002)
#define  RCC_AHB1RSTR_GPIOCRST               ((uint_fast32_t)0x00000004)
#define  RCC_AHB1RSTR_GPIODRST               ((uint_fast32_t)0x00000008)
#define  RCC_AHB1RSTR_GPIOERST               ((uint_fast32_t)0x00000010)
#define  RCC_AHB1RSTR_GPIOFRST               ((uint_fast32_t)0x00000020)
#define  RCC_AHB1RSTR_GPIOGRST               ((uint_fast32_t)0x00000040)
#define  RCC_AHB1RSTR_GPIOHRST               ((uint_fast32_t)0x00000080)
#define  RCC_AHB1RSTR_GPIOIRST               ((uint_fast32_t)0x00000100)
#define  RCC_AHB1RSTR_CRCRST                 ((uint_fast32_t)0x00001000)
#define  RCC_AHB1RSTR_DMA1RST                ((uint_fast32_t)0x00200000)
#define  RCC_AHB1RSTR_DMA2RST                ((uint_fast32_t)0x00400000)
#define  RCC_AHB1RSTR_ETHMACRST              ((uint_fast32_t)0x02000000)
#define  RCC_AHB1RSTR_OTGHRST                ((uint_fast32_t)0x10000000)

/********************  Bit definition for RCC_AHB2RSTR register  **************/
#define  RCC_AHB2RSTR_DCMIRST                ((uint_fast32_t)0x00000001)
#define  RCC_AHB2RSTR_RNGRST                 ((uint_fast32_t)0x00000040)
#define  RCC_AHB2RSTR_OTGFSRST               ((uint_fast32_t)0x00000080)

/********************  Bit definition for RCC_AHB3RSTR register  **************/

#define  RCC_AHB3RSTR_FSMCRST                ((uint_fast32_t)0x00000001)

/********************  Bit definition for RCC_APB1RSTR register  **************/
#define  RCC_APB1RSTR_TIM2RST                ((uint_fast32_t)0x00000001)
#define  RCC_APB1RSTR_TIM3RST                ((uint_fast32_t)0x00000002)
#define  RCC_APB1RSTR_TIM4RST                ((uint_fast32_t)0x00000004)
#define  RCC_APB1RSTR_TIM5RST                ((uint_fast32_t)0x00000008)
#define  RCC_APB1RSTR_TIM6RST                ((uint_fast32_t)0x00000010)
#define  RCC_APB1RSTR_TIM7RST                ((uint_fast32_t)0x00000020)
#define  RCC_APB1RSTR_TIM12RST               ((uint_fast32_t)0x00000040)
#define  RCC_APB1RSTR_TIM13RST               ((uint_fast32_t)0x00000080)
#define  RCC_APB1RSTR_TIM14RST               ((uint_fast32_t)0x00000100)
#define  RCC_APB1RSTR_WWDGRST                ((uint_fast32_t)0x00000800)
#define  RCC_APB1RSTR_SPI2RST                ((uint_fast32_t)0x00004000)
#define  RCC_APB1RSTR_SPI3RST                ((uint_fast32_t)0x00008000)
#define  RCC_APB1RSTR_USART2RST              ((uint_fast32_t)0x00020000)
#define  RCC_APB1RSTR_USART3RST              ((uint_fast32_t)0x00040000)
#define  RCC_APB1RSTR_UART4RST               ((uint_fast32_t)0x00080000)
#define  RCC_APB1RSTR_UART5RST               ((uint_fast32_t)0x00100000)
#define  RCC_APB1RSTR_I2C1RST                ((uint_fast32_t)0x00200000)
#define  RCC_APB1RSTR_I2C2RST                ((uint_fast32_t)0x00400000)
#define  RCC_APB1RSTR_I2C3RST                ((uint_fast32_t)0x00800000)
#define  RCC_APB1RSTR_CAN1RST                ((uint_fast32_t)0x02000000)
#define  RCC_APB1RSTR_CAN2RST                ((uint_fast32_t)0x04000000)
#define  RCC_APB1RSTR_PWRRST                 ((uint_fast32_t)0x10000000)
#define  RCC_APB1RSTR_DACRST                 ((uint_fast32_t)0x20000000)

/********************  Bit definition for RCC_APB2RSTR register  **************/
#define  RCC_APB2RSTR_TIM1RST                ((uint_fast32_t)0x00000001)
#define  RCC_APB2RSTR_TIM8RST                ((uint_fast32_t)0x00000002)
#define  RCC_APB2RSTR_USART1RST              ((uint_fast32_t)0x00000010)
#define  RCC_APB2RSTR_USART6RST              ((uint_fast32_t)0x00000020)
#define  RCC_APB2RSTR_ADCRST                 ((uint_fast32_t)0x00000100)
#define  RCC_APB2RSTR_SDIORST                ((uint_fast32_t)0x00000800)
#define  RCC_APB2RSTR_SPI1RST                ((uint_fast32_t)0x00001000)
#define  RCC_APB2RSTR_SYSCFGRST              ((uint_fast32_t)0x00004000)
#define  RCC_APB2RSTR_TIM9RST                ((uint_fast32_t)0x00010000)
#define  RCC_APB2RSTR_TIM10RST               ((uint_fast32_t)0x00020000)
#define  RCC_APB2RSTR_TIM11RST               ((uint_fast32_t)0x00040000)

/* Old SPI1RST bit definition, maintained for legacy purpose */
#define  RCC_APB2RSTR_SPI1                   RCC_APB2RSTR_SPI1RST

/********************  Bit definition for RCC_AHB1ENR register  ***************/
#define  RCC_AHB1ENR_GPIOAEN                 ((uint_fast32_t)0x00000001)
#define  RCC_AHB1ENR_GPIOBEN                 ((uint_fast32_t)0x00000002)
#define  RCC_AHB1ENR_GPIOCEN                 ((uint_fast32_t)0x00000004)
#define  RCC_AHB1ENR_GPIODEN                 ((uint_fast32_t)0x00000008)
#define  RCC_AHB1ENR_GPIOEEN                 ((uint_fast32_t)0x00000010)
#define  RCC_AHB1ENR_GPIOFEN                 ((uint_fast32_t)0x00000020)
#define  RCC_AHB1ENR_GPIOGEN                 ((uint_fast32_t)0x00000040)
#define  RCC_AHB1ENR_GPIOHEN                 ((uint_fast32_t)0x00000080)
#define  RCC_AHB1ENR_GPIOIEN                 ((uint_fast32_t)0x00000100)
#define  RCC_AHB1ENR_CRCEN                   ((uint_fast32_t)0x00001000)
#define  RCC_AHB1ENR_BKPSRAMEN               ((uint_fast32_t)0x00040000)
#define  RCC_AHB1ENR_CCMDATARAMEN            ((uint_fast32_t)0x00100000)
#define  RCC_AHB1ENR_DMA1EN                  ((uint_fast32_t)0x00200000)
#define  RCC_AHB1ENR_DMA2EN                  ((uint_fast32_t)0x00400000)

#define  RCC_AHB1ENR_ETHMACEN                ((uint_fast32_t)0x02000000)
#define  RCC_AHB1ENR_ETHMACTXEN              ((uint_fast32_t)0x04000000)
#define  RCC_AHB1ENR_ETHMACRXEN              ((uint_fast32_t)0x08000000)
#define  RCC_AHB1ENR_ETHMACPTPEN             ((uint_fast32_t)0x10000000)
#define  RCC_AHB1ENR_OTGHSEN                 ((uint_fast32_t)0x20000000)
#define  RCC_AHB1ENR_OTGHSULPIEN             ((uint_fast32_t)0x40000000)

/********************  Bit definition for RCC_AHB2ENR register  ***************/
#define  RCC_AHB2ENR_DCMIEN                  ((uint_fast32_t)0x00000001)
#define  RCC_AHB2ENR_RNGEN                   ((uint_fast32_t)0x00000040)
#define  RCC_AHB2ENR_OTGFSEN                 ((uint_fast32_t)0x00000080)

/********************  Bit definition for RCC_AHB3ENR register  ***************/

#define  RCC_AHB3ENR_FSMCEN                  ((uint_fast32_t)0x00000001)

/********************  Bit definition for RCC_APB1ENR register  ***************/
#define  RCC_APB1ENR_TIM2EN                  ((uint_fast32_t)0x00000001)
#define  RCC_APB1ENR_TIM3EN                  ((uint_fast32_t)0x00000002)
#define  RCC_APB1ENR_TIM4EN                  ((uint_fast32_t)0x00000004)
#define  RCC_APB1ENR_TIM5EN                  ((uint_fast32_t)0x00000008)
#define  RCC_APB1ENR_TIM6EN                  ((uint_fast32_t)0x00000010)
#define  RCC_APB1ENR_TIM7EN                  ((uint_fast32_t)0x00000020)
#define  RCC_APB1ENR_TIM12EN                 ((uint_fast32_t)0x00000040)
#define  RCC_APB1ENR_TIM13EN                 ((uint_fast32_t)0x00000080)
#define  RCC_APB1ENR_TIM14EN                 ((uint_fast32_t)0x00000100)
#define  RCC_APB1ENR_WWDGEN                  ((uint_fast32_t)0x00000800)
#define  RCC_APB1ENR_SPI2EN                  ((uint_fast32_t)0x00004000)
#define  RCC_APB1ENR_SPI3EN                  ((uint_fast32_t)0x00008000)
#define  RCC_APB1ENR_USART2EN                ((uint_fast32_t)0x00020000)
#define  RCC_APB1ENR_USART3EN                ((uint_fast32_t)0x00040000)
#define  RCC_APB1ENR_UART4EN                 ((uint_fast32_t)0x00080000)
#define  RCC_APB1ENR_UART5EN                 ((uint_fast32_t)0x00100000)
#define  RCC_APB1ENR_I2C1EN                  ((uint_fast32_t)0x00200000)
#define  RCC_APB1ENR_I2C2EN                  ((uint_fast32_t)0x00400000)
#define  RCC_APB1ENR_I2C3EN                  ((uint_fast32_t)0x00800000)
#define  RCC_APB1ENR_CAN1EN                  ((uint_fast32_t)0x02000000)
#define  RCC_APB1ENR_CAN2EN                  ((uint_fast32_t)0x04000000)
#define  RCC_APB1ENR_PWREN                   ((uint_fast32_t)0x10000000)
#define  RCC_APB1ENR_DACEN                   ((uint_fast32_t)0x20000000)

/********************  Bit definition for RCC_APB2ENR register  ***************/
#define  RCC_APB2ENR_TIM1EN                  ((uint_fast32_t)0x00000001)
#define  RCC_APB2ENR_TIM8EN                  ((uint_fast32_t)0x00000002)
#define  RCC_APB2ENR_USART1EN                ((uint_fast32_t)0x00000010)
#define  RCC_APB2ENR_USART6EN                ((uint_fast32_t)0x00000020)
#define  RCC_APB2ENR_ADC1EN                  ((uint_fast32_t)0x00000100)
#define  RCC_APB2ENR_ADC2EN                  ((uint_fast32_t)0x00000200)
#define  RCC_APB2ENR_ADC3EN                  ((uint_fast32_t)0x00000400)
#define  RCC_APB2ENR_SDIOEN                  ((uint_fast32_t)0x00000800)
#define  RCC_APB2ENR_SPI1EN                  ((uint_fast32_t)0x00001000)
#define  RCC_APB2ENR_SYSCFGEN                ((uint_fast32_t)0x00004000)
#define  RCC_APB2ENR_TIM9EN                  ((uint_fast32_t)0x00010000)
#define  RCC_APB2ENR_TIM10EN                 ((uint_fast32_t)0x00020000)
#define  RCC_APB2ENR_TIM11EN                 ((uint_fast32_t)0x00040000)
#define  RCC_APB2ENR_SPI5EN                  ((uint_fast32_t)0x00100000)
#define  RCC_APB2ENR_SPI6EN                  ((uint_fast32_t)0x00200000)

/********************  Bit definition for RCC_AHB1LPENR register  *************/
#define  RCC_AHB1LPENR_GPIOALPEN             ((uint_fast32_t)0x00000001)
#define  RCC_AHB1LPENR_GPIOBLPEN             ((uint_fast32_t)0x00000002)
#define  RCC_AHB1LPENR_GPIOCLPEN             ((uint_fast32_t)0x00000004)
#define  RCC_AHB1LPENR_GPIODLPEN             ((uint_fast32_t)0x00000008)
#define  RCC_AHB1LPENR_GPIOELPEN             ((uint_fast32_t)0x00000010)
#define  RCC_AHB1LPENR_GPIOFLPEN             ((uint_fast32_t)0x00000020)
#define  RCC_AHB1LPENR_GPIOGLPEN             ((uint_fast32_t)0x00000040)
#define  RCC_AHB1LPENR_GPIOHLPEN             ((uint_fast32_t)0x00000080)
#define  RCC_AHB1LPENR_GPIOILPEN             ((uint_fast32_t)0x00000100)
#define  RCC_AHB1LPENR_CRCLPEN               ((uint_fast32_t)0x00001000)
#define  RCC_AHB1LPENR_FLITFLPEN             ((uint_fast32_t)0x00008000)
#define  RCC_AHB1LPENR_SRAM1LPEN             ((uint_fast32_t)0x00010000)
#define  RCC_AHB1LPENR_SRAM2LPEN             ((uint_fast32_t)0x00020000)
#define  RCC_AHB1LPENR_BKPSRAMLPEN           ((uint_fast32_t)0x00040000)
#define  RCC_AHB1LPENR_SRAM3LPEN             ((uint_fast32_t)0x00080000)
#define  RCC_AHB1LPENR_DMA1LPEN              ((uint_fast32_t)0x00200000)
#define  RCC_AHB1LPENR_DMA2LPEN              ((uint_fast32_t)0x00400000)
#define  RCC_AHB1LPENR_ETHMACLPEN            ((uint_fast32_t)0x02000000)
#define  RCC_AHB1LPENR_ETHMACTXLPEN          ((uint_fast32_t)0x04000000)
#define  RCC_AHB1LPENR_ETHMACRXLPEN          ((uint_fast32_t)0x08000000)
#define  RCC_AHB1LPENR_ETHMACPTPLPEN         ((uint_fast32_t)0x10000000)
#define  RCC_AHB1LPENR_OTGHSLPEN             ((uint_fast32_t)0x20000000)
#define  RCC_AHB1LPENR_OTGHSULPILPEN         ((uint_fast32_t)0x40000000)

/********************  Bit definition for RCC_AHB2LPENR register  *************/
#define  RCC_AHB2LPENR_DCMILPEN              ((uint_fast32_t)0x00000001)
#define  RCC_AHB2LPENR_RNGLPEN               ((uint_fast32_t)0x00000040)
#define  RCC_AHB2LPENR_OTGFSLPEN             ((uint_fast32_t)0x00000080)

/********************  Bit definition for RCC_AHB3LPENR register  *************/

#define  RCC_AHB3LPENR_FSMCLPEN              ((uint_fast32_t)0x00000001)

/********************  Bit definition for RCC_APB1LPENR register  *************/
#define  RCC_APB1LPENR_TIM2LPEN              ((uint_fast32_t)0x00000001)
#define  RCC_APB1LPENR_TIM3LPEN              ((uint_fast32_t)0x00000002)
#define  RCC_APB1LPENR_TIM4LPEN              ((uint_fast32_t)0x00000004)
#define  RCC_APB1LPENR_TIM5LPEN              ((uint_fast32_t)0x00000008)
#define  RCC_APB1LPENR_TIM6LPEN              ((uint_fast32_t)0x00000010)
#define  RCC_APB1LPENR_TIM7LPEN              ((uint_fast32_t)0x00000020)
#define  RCC_APB1LPENR_TIM12LPEN             ((uint_fast32_t)0x00000040)
#define  RCC_APB1LPENR_TIM13LPEN             ((uint_fast32_t)0x00000080)
#define  RCC_APB1LPENR_TIM14LPEN             ((uint_fast32_t)0x00000100)
#define  RCC_APB1LPENR_WWDGLPEN              ((uint_fast32_t)0x00000800)
#define  RCC_APB1LPENR_SPI2LPEN              ((uint_fast32_t)0x00004000)
#define  RCC_APB1LPENR_SPI3LPEN              ((uint_fast32_t)0x00008000)
#define  RCC_APB1LPENR_USART2LPEN            ((uint_fast32_t)0x00020000)
#define  RCC_APB1LPENR_USART3LPEN            ((uint_fast32_t)0x00040000)
#define  RCC_APB1LPENR_UART4LPEN             ((uint_fast32_t)0x00080000)
#define  RCC_APB1LPENR_UART5LPEN             ((uint_fast32_t)0x00100000)
#define  RCC_APB1LPENR_I2C1LPEN              ((uint_fast32_t)0x00200000)
#define  RCC_APB1LPENR_I2C2LPEN              ((uint_fast32_t)0x00400000)
#define  RCC_APB1LPENR_I2C3LPEN              ((uint_fast32_t)0x00800000)
#define  RCC_APB1LPENR_CAN1LPEN              ((uint_fast32_t)0x02000000)
#define  RCC_APB1LPENR_CAN2LPEN              ((uint_fast32_t)0x04000000)
#define  RCC_APB1LPENR_PWRLPEN               ((uint_fast32_t)0x10000000)
#define  RCC_APB1LPENR_DACLPEN               ((uint_fast32_t)0x20000000)

/********************  Bit definition for RCC_APB2LPENR register  *************/
#define  RCC_APB2LPENR_TIM1LPEN              ((uint_fast32_t)0x00000001)
#define  RCC_APB2LPENR_TIM8LPEN              ((uint_fast32_t)0x00000002)
#define  RCC_APB2LPENR_USART1LPEN            ((uint_fast32_t)0x00000010)
#define  RCC_APB2LPENR_USART6LPEN            ((uint_fast32_t)0x00000020)
#define  RCC_APB2LPENR_ADC1LPEN              ((uint_fast32_t)0x00000100)
#define  RCC_APB2LPENR_ADC2LPEN              ((uint_fast32_t)0x00000200)
#define  RCC_APB2LPENR_ADC3LPEN              ((uint_fast32_t)0x00000400)
#define  RCC_APB2LPENR_SDIOLPEN              ((uint_fast32_t)0x00000800)
#define  RCC_APB2LPENR_SPI1LPEN              ((uint_fast32_t)0x00001000)
#define  RCC_APB2LPENR_SYSCFGLPEN            ((uint_fast32_t)0x00004000)
#define  RCC_APB2LPENR_TIM9LPEN              ((uint_fast32_t)0x00010000)
#define  RCC_APB2LPENR_TIM10LPEN             ((uint_fast32_t)0x00020000)
#define  RCC_APB2LPENR_TIM11LPEN             ((uint_fast32_t)0x00040000)

/********************  Bit definition for RCC_BDCR register  ******************/
#define  RCC_BDCR_LSEON                      ((uint_fast32_t)0x00000001)
#define  RCC_BDCR_LSERDY                     ((uint_fast32_t)0x00000002)
#define  RCC_BDCR_LSEBYP                     ((uint_fast32_t)0x00000004)

#define  RCC_BDCR_RTCSEL                    ((uint_fast32_t)0x00000300)
#define  RCC_BDCR_RTCSEL_0                  ((uint_fast32_t)0x00000100)
#define  RCC_BDCR_RTCSEL_1                  ((uint_fast32_t)0x00000200)

#define  RCC_BDCR_RTCEN                      ((uint_fast32_t)0x00008000)
#define  RCC_BDCR_BDRST                      ((uint_fast32_t)0x00010000)

/********************  Bit definition for RCC_CSR register  *******************/
#define  RCC_CSR_LSION                       ((uint_fast32_t)0x00000001)
#define  RCC_CSR_LSIRDY                      ((uint_fast32_t)0x00000002)
#define  RCC_CSR_RMVF                        ((uint_fast32_t)0x01000000)
#define  RCC_CSR_BORRSTF                     ((uint_fast32_t)0x02000000)
#define  RCC_CSR_PADRSTF                     ((uint_fast32_t)0x04000000)
#define  RCC_CSR_PORRSTF                     ((uint_fast32_t)0x08000000)
#define  RCC_CSR_SFTRSTF                     ((uint_fast32_t)0x10000000)
#define  RCC_CSR_WDGRSTF                     ((uint_fast32_t)0x20000000)
#define  RCC_CSR_WWDGRSTF                    ((uint_fast32_t)0x40000000)
#define  RCC_CSR_LPWRRSTF                    ((uint_fast32_t)0x80000000)

/********************  Bit definition for RCC_SSCGR register  *****************/
#define  RCC_SSCGR_MODPER                    ((uint_fast32_t)0x00001FFF)
#define  RCC_SSCGR_INCSTEP                   ((uint_fast32_t)0x0FFFE000)
#define  RCC_SSCGR_SPREADSEL                 ((uint_fast32_t)0x40000000)
#define  RCC_SSCGR_SSCGEN                    ((uint_fast32_t)0x80000000)

/********************  Bit definition for RCC_PLLI2SCFGR register  ************/
#define  RCC_PLLI2SCFGR_PLLI2SN              ((uint_fast32_t)0x00007FC0)
#define  RCC_PLLI2SCFGR_PLLI2SN_0            ((uint_fast32_t)0x00000040)
#define  RCC_PLLI2SCFGR_PLLI2SN_1            ((uint_fast32_t)0x00000080)
#define  RCC_PLLI2SCFGR_PLLI2SN_2            ((uint_fast32_t)0x00000100)
#define  RCC_PLLI2SCFGR_PLLI2SN_3            ((uint_fast32_t)0x00000200)
#define  RCC_PLLI2SCFGR_PLLI2SN_4            ((uint_fast32_t)0x00000400)
#define  RCC_PLLI2SCFGR_PLLI2SN_5            ((uint_fast32_t)0x00000800)
#define  RCC_PLLI2SCFGR_PLLI2SN_6            ((uint_fast32_t)0x00001000)
#define  RCC_PLLI2SCFGR_PLLI2SN_7            ((uint_fast32_t)0x00002000)
#define  RCC_PLLI2SCFGR_PLLI2SN_8            ((uint_fast32_t)0x00004000)

#define  RCC_PLLI2SCFGR_PLLI2SR              ((uint_fast32_t)0x70000000)
#define  RCC_PLLI2SCFGR_PLLI2SR_0            ((uint_fast32_t)0x10000000)
#define  RCC_PLLI2SCFGR_PLLI2SR_1            ((uint_fast32_t)0x20000000)
#define  RCC_PLLI2SCFGR_PLLI2SR_2            ((uint_fast32_t)0x40000000)

/**
  * @brief Reset and Clock Control
  */

typedef struct
{
  __IO uint_fast32_t CR;            /*!< RCC clock control register,                                  Address offset: 0x00 */
  __IO uint_fast32_t PLLCFGR;       /*!< RCC PLL configuration register,                              Address offset: 0x04 */
  __IO uint_fast32_t CFGR;          /*!< RCC clock configuration register,                            Address offset: 0x08 */
  __IO uint_fast32_t CIR;           /*!< RCC clock interrupt register,                                Address offset: 0x0C */
  __IO uint_fast32_t AHB1RSTR;      /*!< RCC AHB1 peripheral reset register,                          Address offset: 0x10 */
  __IO uint_fast32_t AHB2RSTR;      /*!< RCC AHB2 peripheral reset register,                          Address offset: 0x14 */
  __IO uint_fast32_t AHB3RSTR;      /*!< RCC AHB3 peripheral reset register,                          Address offset: 0x18 */
  uint_fast32_t      RESERVED0;     /*!< Reserved, 0x1C                                                                    */
  __IO uint_fast32_t APB1RSTR;      /*!< RCC APB1 peripheral reset register,                          Address offset: 0x20 */
  __IO uint_fast32_t APB2RSTR;      /*!< RCC APB2 peripheral reset register,                          Address offset: 0x24 */
  uint_fast32_t      RESERVED1[2];  /*!< Reserved, 0x28-0x2C                                                               */
  __IO uint_fast32_t AHB1ENR;       /*!< RCC AHB1 peripheral clock register,                          Address offset: 0x30 */
  __IO uint_fast32_t AHB2ENR;       /*!< RCC AHB2 peripheral clock register,                          Address offset: 0x34 */
  __IO uint_fast32_t AHB3ENR;       /*!< RCC AHB3 peripheral clock register,                          Address offset: 0x38 */
  uint_fast32_t      RESERVED2;     /*!< Reserved, 0x3C                                                                    */
  __IO uint_fast32_t APB1ENR;       /*!< RCC APB1 peripheral clock enable register,                   Address offset: 0x40 */
  __IO uint_fast32_t APB2ENR;       /*!< RCC APB2 peripheral clock enable register,                   Address offset: 0x44 */
  uint_fast32_t      RESERVED3[2];  /*!< Reserved, 0x48-0x4C                                                               */
  __IO uint_fast32_t AHB1LPENR;     /*!< RCC AHB1 peripheral clock enable in low power mode register, Address offset: 0x50 */
  __IO uint_fast32_t AHB2LPENR;     /*!< RCC AHB2 peripheral clock enable in low power mode register, Address offset: 0x54 */
  __IO uint_fast32_t AHB3LPENR;     /*!< RCC AHB3 peripheral clock enable in low power mode register, Address offset: 0x58 */
  uint_fast32_t      RESERVED4;     /*!< Reserved, 0x5C                                                                    */
  __IO uint_fast32_t APB1LPENR;     /*!< RCC APB1 peripheral clock enable in low power mode register, Address offset: 0x60 */
  __IO uint_fast32_t APB2LPENR;     /*!< RCC APB2 peripheral clock enable in low power mode register, Address offset: 0x64 */
  uint_fast32_t      RESERVED5[2];  /*!< Reserved, 0x68-0x6C                                                               */
  __IO uint_fast32_t BDCR;          /*!< RCC Backup domain control register,                          Address offset: 0x70 */
  __IO uint_fast32_t CSR;           /*!< RCC clock control & status register,                         Address offset: 0x74 */
  uint_fast32_t      RESERVED6[2];  /*!< Reserved, 0x78-0x7C                                                               */
  __IO uint_fast32_t SSCGR;         /*!< RCC spread spectrum clock generation register,               Address offset: 0x80 */
  __IO uint_fast32_t PLLI2SCFGR;    /*!< RCC PLLI2S configuration register,                           Address offset: 0x84 */

} RCC_TypeDef;

#define RCC                 ((RCC_TypeDef *) RCC_BASE)

#endif /* RCC_H_ */
