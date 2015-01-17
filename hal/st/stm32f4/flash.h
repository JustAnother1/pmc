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

#ifndef HAL_ST_STM32F4_FLASH_H_
#define HAL_ST_STM32F4_FLASH_H_

#include "hal_common.h"

/******************************************************************************/
/*                                                                            */
/*                                    FLASH                                   */
/*                                                                            */
/******************************************************************************/
/*******************  Bits definition for FLASH_ACR register  *****************/
#define FLASH_ACR_LATENCY                    ((uint_fast32_t)0x0000000F)
#define FLASH_ACR_LATENCY_0WS                ((uint_fast32_t)0x00000000)
#define FLASH_ACR_LATENCY_1WS                ((uint_fast32_t)0x00000001)
#define FLASH_ACR_LATENCY_2WS                ((uint_fast32_t)0x00000002)
#define FLASH_ACR_LATENCY_3WS                ((uint_fast32_t)0x00000003)
#define FLASH_ACR_LATENCY_4WS                ((uint_fast32_t)0x00000004)
#define FLASH_ACR_LATENCY_5WS                ((uint_fast32_t)0x00000005)
#define FLASH_ACR_LATENCY_6WS                ((uint_fast32_t)0x00000006)
#define FLASH_ACR_LATENCY_7WS                ((uint_fast32_t)0x00000007)

#define FLASH_ACR_PRFTEN                     ((uint_fast32_t)0x00000100)
#define FLASH_ACR_ICEN                       ((uint_fast32_t)0x00000200)
#define FLASH_ACR_DCEN                       ((uint_fast32_t)0x00000400)
#define FLASH_ACR_ICRST                      ((uint_fast32_t)0x00000800)
#define FLASH_ACR_DCRST                      ((uint_fast32_t)0x00001000)
#define FLASH_ACR_BYTE0_ADDRESS              ((uint_fast32_t)0x40023C00)
#define FLASH_ACR_BYTE2_ADDRESS              ((uint_fast32_t)0x40023C03)

/*******************  Bits definition for FLASH_SR register  ******************/
#define FLASH_SR_EOP                         ((uint_fast32_t)0x00000001)
#define FLASH_SR_SOP                         ((uint_fast32_t)0x00000002)
#define FLASH_SR_WRPERR                      ((uint_fast32_t)0x00000010)
#define FLASH_SR_PGAERR                      ((uint_fast32_t)0x00000020)
#define FLASH_SR_PGPERR                      ((uint_fast32_t)0x00000040)
#define FLASH_SR_PGSERR                      ((uint_fast32_t)0x00000080)
#define FLASH_SR_BSY                         ((uint_fast32_t)0x00010000)

/*******************  Bits definition for FLASH_CR register  ******************/
#define FLASH_CR_PG                          ((uint_fast32_t)0x00000001)
#define FLASH_CR_SER                         ((uint_fast32_t)0x00000002)
#define FLASH_CR_MER                         ((uint_fast32_t)0x00000004)
#define FLASH_CR_SNB                         ((uint_fast32_t)0x000000F8)
#define FLASH_CR_SNB_0                       ((uint_fast32_t)0x00000008)
#define FLASH_CR_SNB_1                       ((uint_fast32_t)0x00000010)
#define FLASH_CR_SNB_2                       ((uint_fast32_t)0x00000020)
#define FLASH_CR_SNB_3                       ((uint_fast32_t)0x00000040)
#define FLASH_CR_SNB_4                       ((uint_fast32_t)0x00000080)
#define FLASH_CR_PSIZE                       ((uint_fast32_t)0x00000300)
#define FLASH_CR_PSIZE_0                     ((uint_fast32_t)0x00000100)
#define FLASH_CR_PSIZE_1                     ((uint_fast32_t)0x00000200)
#define FLASH_CR_STRT                        ((uint_fast32_t)0x00010000)
#define FLASH_CR_EOPIE                       ((uint_fast32_t)0x01000000)
#define FLASH_CR_LOCK                        ((uint_fast32_t)0x80000000)

/*******************  Bits definition for FLASH_OPTCR register  ***************/
#define FLASH_OPTCR_OPTLOCK                 ((uint_fast32_t)0x00000001)
#define FLASH_OPTCR_OPTSTRT                 ((uint_fast32_t)0x00000002)
#define FLASH_OPTCR_BOR_LEV_0               ((uint_fast32_t)0x00000004)
#define FLASH_OPTCR_BOR_LEV_1               ((uint_fast32_t)0x00000008)
#define FLASH_OPTCR_BOR_LEV                 ((uint_fast32_t)0x0000000C)

#define FLASH_OPTCR_WDG_SW                  ((uint_fast32_t)0x00000020)
#define FLASH_OPTCR_nRST_STOP               ((uint_fast32_t)0x00000040)
#define FLASH_OPTCR_nRST_STDBY              ((uint_fast32_t)0x00000080)
#define FLASH_OPTCR_RDP                     ((uint_fast32_t)0x0000FF00)
#define FLASH_OPTCR_RDP_0                   ((uint_fast32_t)0x00000100)
#define FLASH_OPTCR_RDP_1                   ((uint_fast32_t)0x00000200)
#define FLASH_OPTCR_RDP_2                   ((uint_fast32_t)0x00000400)
#define FLASH_OPTCR_RDP_3                   ((uint_fast32_t)0x00000800)
#define FLASH_OPTCR_RDP_4                   ((uint_fast32_t)0x00001000)
#define FLASH_OPTCR_RDP_5                   ((uint_fast32_t)0x00002000)
#define FLASH_OPTCR_RDP_6                   ((uint_fast32_t)0x00004000)
#define FLASH_OPTCR_RDP_7                   ((uint_fast32_t)0x00008000)
#define FLASH_OPTCR_nWRP                    ((uint_fast32_t)0x0FFF0000)
#define FLASH_OPTCR_nWRP_0                  ((uint_fast32_t)0x00010000)
#define FLASH_OPTCR_nWRP_1                  ((uint_fast32_t)0x00020000)
#define FLASH_OPTCR_nWRP_2                  ((uint_fast32_t)0x00040000)
#define FLASH_OPTCR_nWRP_3                  ((uint_fast32_t)0x00080000)
#define FLASH_OPTCR_nWRP_4                  ((uint_fast32_t)0x00100000)
#define FLASH_OPTCR_nWRP_5                  ((uint_fast32_t)0x00200000)
#define FLASH_OPTCR_nWRP_6                  ((uint_fast32_t)0x00400000)
#define FLASH_OPTCR_nWRP_7                  ((uint_fast32_t)0x00800000)
#define FLASH_OPTCR_nWRP_8                  ((uint_fast32_t)0x01000000)
#define FLASH_OPTCR_nWRP_9                  ((uint_fast32_t)0x02000000)
#define FLASH_OPTCR_nWRP_10                 ((uint_fast32_t)0x04000000)
#define FLASH_OPTCR_nWRP_11                 ((uint_fast32_t)0x08000000)

/******************  Bits definition for FLASH_OPTCR1 register  ***************/
#define FLASH_OPTCR1_nWRP                    ((uint_fast32_t)0x0FFF0000)
#define FLASH_OPTCR1_nWRP_0                  ((uint_fast32_t)0x00010000)
#define FLASH_OPTCR1_nWRP_1                  ((uint_fast32_t)0x00020000)
#define FLASH_OPTCR1_nWRP_2                  ((uint_fast32_t)0x00040000)
#define FLASH_OPTCR1_nWRP_3                  ((uint_fast32_t)0x00080000)
#define FLASH_OPTCR1_nWRP_4                  ((uint_fast32_t)0x00100000)
#define FLASH_OPTCR1_nWRP_5                  ((uint_fast32_t)0x00200000)
#define FLASH_OPTCR1_nWRP_6                  ((uint_fast32_t)0x00400000)
#define FLASH_OPTCR1_nWRP_7                  ((uint_fast32_t)0x00800000)
#define FLASH_OPTCR1_nWRP_8                  ((uint_fast32_t)0x01000000)
#define FLASH_OPTCR1_nWRP_9                  ((uint_fast32_t)0x02000000)
#define FLASH_OPTCR1_nWRP_10                 ((uint_fast32_t)0x04000000)
#define FLASH_OPTCR1_nWRP_11                 ((uint_fast32_t)0x08000000)

/**
  * @brief FLASH Registers
  */

typedef struct
{
  __IO uint_fast32_t ACR;      /*!< FLASH access control register,   Address offset: 0x00 */
  __IO uint_fast32_t KEYR;     /*!< FLASH key register,              Address offset: 0x04 */
  __IO uint_fast32_t OPTKEYR;  /*!< FLASH option key register,       Address offset: 0x08 */
  __IO uint_fast32_t SR;       /*!< FLASH status register,           Address offset: 0x0C */
  __IO uint_fast32_t CR;       /*!< FLASH control register,          Address offset: 0x10 */
  __IO uint_fast32_t OPTCR;    /*!< FLASH option control register ,  Address offset: 0x14 */
  __IO uint_fast32_t OPTCR1;   /*!< FLASH option control register 1, Address offset: 0x18 */
} FLASH_TypeDef;

#define FLASH               ((FLASH_TypeDef *) FLASH_R_BASE)

#endif /* HAL_ST_STM32F4_FLASH_H_ */
