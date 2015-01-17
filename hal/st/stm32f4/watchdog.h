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

#ifndef HAL_ST_STM32F4_WATCHDOG_H_
#define HAL_ST_STM32F4_WATCHDOG_H_

#include "hal_common.h"

/******************************************************************************/
/*                                                                            */
/*                           Independent WATCHDOG                             */
/*                                                                            */
/******************************************************************************/
/*******************  Bit definition for IWDG_KR register  ********************/
#define  IWDG_KR_KEY                         ((uint_fast32_t)0xFFFF)            /*!<Key value (write only, read 0000h)  */

/*******************  Bit definition for IWDG_PR register  ********************/
#define  IWDG_PR_PR                          ((uint_fast32_t)0x07)               /*!<PR[2:0] (Prescaler divider)         */
#define  IWDG_PR_PR_0                        ((uint_fast32_t)0x01)               /*!<Bit 0 */
#define  IWDG_PR_PR_1                        ((uint_fast32_t)0x02)               /*!<Bit 1 */
#define  IWDG_PR_PR_2                        ((uint_fast32_t)0x04)               /*!<Bit 2 */

/*******************  Bit definition for IWDG_RLR register  *******************/
#define  IWDG_RLR_RL                         ((uint_fast32_t)0x0FFF)            /*!<Watchdog counter reload value        */

/*******************  Bit definition for IWDG_SR register  ********************/
#define  IWDG_SR_PVU                         ((uint_fast32_t)0x01)               /*!<Watchdog prescaler value update      */
#define  IWDG_SR_RVU                         ((uint_fast32_t)0x02)               /*!<Watchdog counter reload value update */

/******************************************************************************/
/*                                                                            */
/*                            Window WATCHDOG                                 */
/*                                                                            */
/******************************************************************************/
/*******************  Bit definition for WWDG_CR register  ********************/
#define  WWDG_CR_T                           ((uint_fast32_t)0x7F)               /*!<T[6:0] bits (7-Bit counter (MSB to LSB)) */
#define  WWDG_CR_T0                          ((uint_fast32_t)0x01)               /*!<Bit 0 */
#define  WWDG_CR_T1                          ((uint_fast32_t)0x02)               /*!<Bit 1 */
#define  WWDG_CR_T2                          ((uint_fast32_t)0x04)               /*!<Bit 2 */
#define  WWDG_CR_T3                          ((uint_fast32_t)0x08)               /*!<Bit 3 */
#define  WWDG_CR_T4                          ((uint_fast32_t)0x10)               /*!<Bit 4 */
#define  WWDG_CR_T5                          ((uint_fast32_t)0x20)               /*!<Bit 5 */
#define  WWDG_CR_T6                          ((uint_fast32_t)0x40)               /*!<Bit 6 */

#define  WWDG_CR_WDGA                        ((uint_fast32_t)0x80)               /*!<Activation bit */

/*******************  Bit definition for WWDG_CFR register  *******************/
#define  WWDG_CFR_W                          ((uint_fast32_t)0x007F)            /*!<W[6:0] bits (7-bit window value) */
#define  WWDG_CFR_W0                         ((uint_fast32_t)0x0001)            /*!<Bit 0 */
#define  WWDG_CFR_W1                         ((uint_fast32_t)0x0002)            /*!<Bit 1 */
#define  WWDG_CFR_W2                         ((uint_fast32_t)0x0004)            /*!<Bit 2 */
#define  WWDG_CFR_W3                         ((uint_fast32_t)0x0008)            /*!<Bit 3 */
#define  WWDG_CFR_W4                         ((uint_fast32_t)0x0010)            /*!<Bit 4 */
#define  WWDG_CFR_W5                         ((uint_fast32_t)0x0020)            /*!<Bit 5 */
#define  WWDG_CFR_W6                         ((uint_fast32_t)0x0040)            /*!<Bit 6 */

#define  WWDG_CFR_WDGTB                      ((uint_fast32_t)0x0180)            /*!<WDGTB[1:0] bits (Timer Base) */
#define  WWDG_CFR_WDGTB0                     ((uint_fast32_t)0x0080)            /*!<Bit 0 */
#define  WWDG_CFR_WDGTB1                     ((uint_fast32_t)0x0100)            /*!<Bit 1 */

#define  WWDG_CFR_EWI                        ((uint_fast32_t)0x0200)            /*!<Early Wakeup Interrupt */

/*******************  Bit definition for WWDG_SR register  ********************/
#define  WWDG_SR_EWIF                        ((uint_fast32_t)0x01)               /*!<Early Wakeup Interrupt Flag */


/**
  * @brief Independent WATCHDOG
  */

typedef struct
{
  __IO uint_fast32_t KR;   /*!< IWDG Key register,       Address offset: 0x00 */
  __IO uint_fast32_t PR;   /*!< IWDG Prescaler register, Address offset: 0x04 */
  __IO uint_fast32_t RLR;  /*!< IWDG Reload register,    Address offset: 0x08 */
  __IO uint_fast32_t SR;   /*!< IWDG Status register,    Address offset: 0x0C */
} IWDG_TypeDef;

/**
  * @brief Window WATCHDOG
  */

typedef struct
{
  __IO uint_fast32_t CR;   /*!< WWDG Control register,       Address offset: 0x00 */
  __IO uint_fast32_t CFR;  /*!< WWDG Configuration register, Address offset: 0x04 */
  __IO uint_fast32_t SR;   /*!< WWDG Status register,        Address offset: 0x08 */
} WWDG_TypeDef;

#define WWDG_BASE             (APB1PERIPH_BASE + 0x2C00)
#define IWDG_BASE             (APB1PERIPH_BASE + 0x3000)

#define WWDG                ((WWDG_TypeDef *) WWDG_BASE)
#define IWDG                ((IWDG_TypeDef *) IWDG_BASE)

#endif /* HAL_ST_STM32F4_WATCHDOG_H_ */
