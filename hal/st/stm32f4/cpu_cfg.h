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

#include "flash.h"
#include "rcc.h"

// frequency of the External oscillator in Hz
#define HSE_VALUE                         ((uint_fast32_t)8000000)
// frequency the cpu runs with
#define FREQUENCY_OF_SYSTEM_CORE_CLOCK    ((uint32_t)168000000)

// Flash:
// Wait states (3,3V and 168MHz) RM0090_P80
#define WAIT_STATES  FLASH_ACR_LATENCY_5WS

// 8MHz -> 168MHz (PLLCFGR)
// PLLQ = 4
#define RCC_PLL_PLLQ  RCC_PLLCFGR_PLLQ_2
// PLLP = /2
#define RCC_PLL_PLLP  0
// PLLN = 336
#define RCC_PLL_PLLN (RCC_PLLCFGR_PLLN_8 +  RCC_PLLCFGR_PLLN_6 + RCC_PLLCFGR_PLLN_4)
// PLLM = 8
#define RCC_PLL_PLLM 8

// Clock Configuration (CFGR)
#define RCC_MC02          RCC_CFGR_MCO2_1
#define RCC_MC01          RCC_CFGR_MCO1_1
#define RCC_PRESC_APB2    RCC_CFGR_PPRE2_DIV2
#define RCC_PRESC_APB1    RCC_CFGR_PPRE1_DIV4
#define RCC_PRESC_AHB     RCC_CFGR_HPRE_DIV1
#define RCC_SYS_CLK_SW    RCC_CFGR_SWS_PLL

#endif /* CPU_CFG_H_ */
