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

#ifndef HAL_ST_STM32F4_ST_TIMER_H_
#define HAL_ST_STM32F4_ST_TIMER_H_

#include "st_common.h"

/******************************************************************************/
/*                                                                            */
/*                                    TIM                                     */
/*                                                                            */
/******************************************************************************/
/*******************  Bit definition for TIM_CR1 register  ********************/
#define  TIM_CR1_CEN                         ((uint32_t)0x0001)            /*!<Counter enable        */
#define  TIM_CR1_UDIS                        ((uint32_t)0x0002)            /*!<Update disable        */
#define  TIM_CR1_URS                         ((uint32_t)0x0004)            /*!<Update request source */
#define  TIM_CR1_OPM                         ((uint32_t)0x0008)            /*!<One pulse mode        */
#define  TIM_CR1_DIR                         ((uint32_t)0x0010)            /*!<Direction             */

#define  TIM_CR1_CMS                         ((uint32_t)0x0060)            /*!<CMS[1:0] bits (Center-aligned mode selection) */
#define  TIM_CR1_CMS_0                       ((uint32_t)0x0020)            /*!<Bit 0 */
#define  TIM_CR1_CMS_1                       ((uint32_t)0x0040)            /*!<Bit 1 */

#define  TIM_CR1_ARPE                        ((uint32_t)0x0080)            /*!<Auto-reload preload enable     */

#define  TIM_CR1_CKD                         ((uint32_t)0x0300)            /*!<CKD[1:0] bits (clock division) */
#define  TIM_CR1_CKD_0                       ((uint32_t)0x0100)            /*!<Bit 0 */
#define  TIM_CR1_CKD_1                       ((uint32_t)0x0200)            /*!<Bit 1 */

/*******************  Bit definition for TIM_CR2 register  ********************/
#define  TIM_CR2_CCPC                        ((uint32_t)0x0001)            /*!<Capture/Compare Preloaded Control        */
#define  TIM_CR2_CCUS                        ((uint32_t)0x0004)            /*!<Capture/Compare Control Update Selection */
#define  TIM_CR2_CCDS                        ((uint32_t)0x0008)            /*!<Capture/Compare DMA Selection            */

#define  TIM_CR2_MMS                         ((uint32_t)0x0070)            /*!<MMS[2:0] bits (Master Mode Selection) */
#define  TIM_CR2_MMS_0                       ((uint32_t)0x0010)            /*!<Bit 0 */
#define  TIM_CR2_MMS_1                       ((uint32_t)0x0020)            /*!<Bit 1 */
#define  TIM_CR2_MMS_2                       ((uint32_t)0x0040)            /*!<Bit 2 */

#define  TIM_CR2_TI1S                        ((uint32_t)0x0080)            /*!<TI1 Selection */
#define  TIM_CR2_OIS1                        ((uint32_t)0x0100)            /*!<Output Idle state 1 (OC1 output)  */
#define  TIM_CR2_OIS1N                       ((uint32_t)0x0200)            /*!<Output Idle state 1 (OC1N output) */
#define  TIM_CR2_OIS2                        ((uint32_t)0x0400)            /*!<Output Idle state 2 (OC2 output)  */
#define  TIM_CR2_OIS2N                       ((uint32_t)0x0800)            /*!<Output Idle state 2 (OC2N output) */
#define  TIM_CR2_OIS3                        ((uint32_t)0x1000)            /*!<Output Idle state 3 (OC3 output)  */
#define  TIM_CR2_OIS3N                       ((uint32_t)0x2000)            /*!<Output Idle state 3 (OC3N output) */
#define  TIM_CR2_OIS4                        ((uint32_t)0x4000)            /*!<Output Idle state 4 (OC4 output)  */

/*******************  Bit definition for TIM_SMCR register  *******************/
#define  TIM_SMCR_SMS                        ((uint32_t)0x0007)            /*!<SMS[2:0] bits (Slave mode selection)    */
#define  TIM_SMCR_SMS_0                      ((uint32_t)0x0001)            /*!<Bit 0 */
#define  TIM_SMCR_SMS_1                      ((uint32_t)0x0002)            /*!<Bit 1 */
#define  TIM_SMCR_SMS_2                      ((uint32_t)0x0004)            /*!<Bit 2 */

#define  TIM_SMCR_TS                         ((uint32_t)0x0070)            /*!<TS[2:0] bits (Trigger selection)        */
#define  TIM_SMCR_TS_0                       ((uint32_t)0x0010)            /*!<Bit 0 */
#define  TIM_SMCR_TS_1                       ((uint32_t)0x0020)            /*!<Bit 1 */
#define  TIM_SMCR_TS_2                       ((uint32_t)0x0040)            /*!<Bit 2 */

#define  TIM_SMCR_MSM                        ((uint32_t)0x0080)            /*!<Master/slave mode                       */

#define  TIM_SMCR_ETF                        ((uint32_t)0x0F00)            /*!<ETF[3:0] bits (External trigger filter) */
#define  TIM_SMCR_ETF_0                      ((uint32_t)0x0100)            /*!<Bit 0 */
#define  TIM_SMCR_ETF_1                      ((uint32_t)0x0200)            /*!<Bit 1 */
#define  TIM_SMCR_ETF_2                      ((uint32_t)0x0400)            /*!<Bit 2 */
#define  TIM_SMCR_ETF_3                      ((uint32_t)0x0800)            /*!<Bit 3 */

#define  TIM_SMCR_ETPS                       ((uint32_t)0x3000)            /*!<ETPS[1:0] bits (External trigger prescaler) */
#define  TIM_SMCR_ETPS_0                     ((uint32_t)0x1000)            /*!<Bit 0 */
#define  TIM_SMCR_ETPS_1                     ((uint32_t)0x2000)            /*!<Bit 1 */

#define  TIM_SMCR_ECE                        ((uint32_t)0x4000)            /*!<External clock enable     */
#define  TIM_SMCR_ETP                        ((uint32_t)0x8000)            /*!<External trigger polarity */

/*******************  Bit definition for TIM_DIER register  *******************/
#define  TIM_DIER_UIE                        ((uint32_t)0x0001)            /*!<Update interrupt enable */
#define  TIM_DIER_CC1IE                      ((uint32_t)0x0002)            /*!<Capture/Compare 1 interrupt enable   */
#define  TIM_DIER_CC2IE                      ((uint32_t)0x0004)            /*!<Capture/Compare 2 interrupt enable   */
#define  TIM_DIER_CC3IE                      ((uint32_t)0x0008)            /*!<Capture/Compare 3 interrupt enable   */
#define  TIM_DIER_CC4IE                      ((uint32_t)0x0010)            /*!<Capture/Compare 4 interrupt enable   */
#define  TIM_DIER_COMIE                      ((uint32_t)0x0020)            /*!<COM interrupt enable                 */
#define  TIM_DIER_TIE                        ((uint32_t)0x0040)            /*!<Trigger interrupt enable             */
#define  TIM_DIER_BIE                        ((uint32_t)0x0080)            /*!<Break interrupt enable               */
#define  TIM_DIER_UDE                        ((uint32_t)0x0100)            /*!<Update DMA request enable            */
#define  TIM_DIER_CC1DE                      ((uint32_t)0x0200)            /*!<Capture/Compare 1 DMA request enable */
#define  TIM_DIER_CC2DE                      ((uint32_t)0x0400)            /*!<Capture/Compare 2 DMA request enable */
#define  TIM_DIER_CC3DE                      ((uint32_t)0x0800)            /*!<Capture/Compare 3 DMA request enable */
#define  TIM_DIER_CC4DE                      ((uint32_t)0x1000)            /*!<Capture/Compare 4 DMA request enable */
#define  TIM_DIER_COMDE                      ((uint32_t)0x2000)            /*!<COM DMA request enable               */
#define  TIM_DIER_TDE                        ((uint32_t)0x4000)            /*!<Trigger DMA request enable           */

/********************  Bit definition for TIM_SR register  ********************/
#define  TIM_SR_UIF                          ((uint32_t)0x0001)            /*!<Update interrupt Flag              */
#define  TIM_SR_CC1IF                        ((uint32_t)0x0002)            /*!<Capture/Compare 1 interrupt Flag   */
#define  TIM_SR_CC2IF                        ((uint32_t)0x0004)            /*!<Capture/Compare 2 interrupt Flag   */
#define  TIM_SR_CC3IF                        ((uint32_t)0x0008)            /*!<Capture/Compare 3 interrupt Flag   */
#define  TIM_SR_CC4IF                        ((uint32_t)0x0010)            /*!<Capture/Compare 4 interrupt Flag   */
#define  TIM_SR_COMIF                        ((uint32_t)0x0020)            /*!<COM interrupt Flag                 */
#define  TIM_SR_TIF                          ((uint32_t)0x0040)            /*!<Trigger interrupt Flag             */
#define  TIM_SR_BIF                          ((uint32_t)0x0080)            /*!<Break interrupt Flag               */
#define  TIM_SR_CC1OF                        ((uint32_t)0x0200)            /*!<Capture/Compare 1 Overcapture Flag */
#define  TIM_SR_CC2OF                        ((uint32_t)0x0400)            /*!<Capture/Compare 2 Overcapture Flag */
#define  TIM_SR_CC3OF                        ((uint32_t)0x0800)            /*!<Capture/Compare 3 Overcapture Flag */
#define  TIM_SR_CC4OF                        ((uint32_t)0x1000)            /*!<Capture/Compare 4 Overcapture Flag */

/*******************  Bit definition for TIM_EGR register  ********************/
#define  TIM_EGR_UG                          ((uint32_t)0x01)               /*!<Update Generation                         */
#define  TIM_EGR_CC1G                        ((uint32_t)0x02)               /*!<Capture/Compare 1 Generation              */
#define  TIM_EGR_CC2G                        ((uint32_t)0x04)               /*!<Capture/Compare 2 Generation              */
#define  TIM_EGR_CC3G                        ((uint32_t)0x08)               /*!<Capture/Compare 3 Generation              */
#define  TIM_EGR_CC4G                        ((uint32_t)0x10)               /*!<Capture/Compare 4 Generation              */
#define  TIM_EGR_COMG                        ((uint32_t)0x20)               /*!<Capture/Compare Control Update Generation */
#define  TIM_EGR_TG                          ((uint32_t)0x40)               /*!<Trigger Generation                        */
#define  TIM_EGR_BG                          ((uint32_t)0x80)               /*!<Break Generation                          */

/******************  Bit definition for TIM_CCMR1 register  *******************/
#define  TIM_CCMR1_CC1S                      ((uint32_t)0x0003)            /*!<CC1S[1:0] bits (Capture/Compare 1 Selection) */
#define  TIM_CCMR1_CC1S_0                    ((uint32_t)0x0001)            /*!<Bit 0 */
#define  TIM_CCMR1_CC1S_1                    ((uint32_t)0x0002)            /*!<Bit 1 */

#define  TIM_CCMR1_OC1FE                     ((uint32_t)0x0004)            /*!<Output Compare 1 Fast enable                 */
#define  TIM_CCMR1_OC1PE                     ((uint32_t)0x0008)            /*!<Output Compare 1 Preload enable              */

#define  TIM_CCMR1_OC1M                      ((uint32_t)0x0070)            /*!<OC1M[2:0] bits (Output Compare 1 Mode)       */
#define  TIM_CCMR1_OC1M_0                    ((uint32_t)0x0010)            /*!<Bit 0 */
#define  TIM_CCMR1_OC1M_1                    ((uint32_t)0x0020)            /*!<Bit 1 */
#define  TIM_CCMR1_OC1M_2                    ((uint32_t)0x0040)            /*!<Bit 2 */

#define  TIM_CCMR1_OC1CE                     ((uint32_t)0x0080)            /*!<Output Compare 1Clear Enable                 */

#define  TIM_CCMR1_CC2S                      ((uint32_t)0x0300)            /*!<CC2S[1:0] bits (Capture/Compare 2 Selection) */
#define  TIM_CCMR1_CC2S_0                    ((uint32_t)0x0100)            /*!<Bit 0 */
#define  TIM_CCMR1_CC2S_1                    ((uint32_t)0x0200)            /*!<Bit 1 */

#define  TIM_CCMR1_OC2FE                     ((uint32_t)0x0400)            /*!<Output Compare 2 Fast enable                 */
#define  TIM_CCMR1_OC2PE                     ((uint32_t)0x0800)            /*!<Output Compare 2 Preload enable              */

#define  TIM_CCMR1_OC2M                      ((uint32_t)0x7000)            /*!<OC2M[2:0] bits (Output Compare 2 Mode)       */
#define  TIM_CCMR1_OC2M_0                    ((uint32_t)0x1000)            /*!<Bit 0 */
#define  TIM_CCMR1_OC2M_1                    ((uint32_t)0x2000)            /*!<Bit 1 */
#define  TIM_CCMR1_OC2M_2                    ((uint32_t)0x4000)            /*!<Bit 2 */

#define  TIM_CCMR1_OC2CE                     ((uint32_t)0x8000)            /*!<Output Compare 2 Clear Enable */

/*----------------------------------------------------------------------------*/

#define  TIM_CCMR1_IC1PSC                    ((uint32_t)0x000C)            /*!<IC1PSC[1:0] bits (Input Capture 1 Prescaler) */
#define  TIM_CCMR1_IC1PSC_0                  ((uint32_t)0x0004)            /*!<Bit 0 */
#define  TIM_CCMR1_IC1PSC_1                  ((uint32_t)0x0008)            /*!<Bit 1 */

#define  TIM_CCMR1_IC1F                      ((uint32_t)0x00F0)            /*!<IC1F[3:0] bits (Input Capture 1 Filter)      */
#define  TIM_CCMR1_IC1F_0                    ((uint32_t)0x0010)            /*!<Bit 0 */
#define  TIM_CCMR1_IC1F_1                    ((uint32_t)0x0020)            /*!<Bit 1 */
#define  TIM_CCMR1_IC1F_2                    ((uint32_t)0x0040)            /*!<Bit 2 */
#define  TIM_CCMR1_IC1F_3                    ((uint32_t)0x0080)            /*!<Bit 3 */

#define  TIM_CCMR1_IC2PSC                    ((uint32_t)0x0C00)            /*!<IC2PSC[1:0] bits (Input Capture 2 Prescaler)  */
#define  TIM_CCMR1_IC2PSC_0                  ((uint32_t)0x0400)            /*!<Bit 0 */
#define  TIM_CCMR1_IC2PSC_1                  ((uint32_t)0x0800)            /*!<Bit 1 */

#define  TIM_CCMR1_IC2F                      ((uint32_t)0xF000)            /*!<IC2F[3:0] bits (Input Capture 2 Filter)       */
#define  TIM_CCMR1_IC2F_0                    ((uint32_t)0x1000)            /*!<Bit 0 */
#define  TIM_CCMR1_IC2F_1                    ((uint32_t)0x2000)            /*!<Bit 1 */
#define  TIM_CCMR1_IC2F_2                    ((uint32_t)0x4000)            /*!<Bit 2 */
#define  TIM_CCMR1_IC2F_3                    ((uint32_t)0x8000)            /*!<Bit 3 */

/******************  Bit definition for TIM_CCMR2 register  *******************/
#define  TIM_CCMR2_CC3S                      ((uint32_t)0x0003)            /*!<CC3S[1:0] bits (Capture/Compare 3 Selection)  */
#define  TIM_CCMR2_CC3S_0                    ((uint32_t)0x0001)            /*!<Bit 0 */
#define  TIM_CCMR2_CC3S_1                    ((uint32_t)0x0002)            /*!<Bit 1 */

#define  TIM_CCMR2_OC3FE                     ((uint32_t)0x0004)            /*!<Output Compare 3 Fast enable           */
#define  TIM_CCMR2_OC3PE                     ((uint32_t)0x0008)            /*!<Output Compare 3 Preload enable        */

#define  TIM_CCMR2_OC3M                      ((uint32_t)0x0070)            /*!<OC3M[2:0] bits (Output Compare 3 Mode) */
#define  TIM_CCMR2_OC3M_0                    ((uint32_t)0x0010)            /*!<Bit 0 */
#define  TIM_CCMR2_OC3M_1                    ((uint32_t)0x0020)            /*!<Bit 1 */
#define  TIM_CCMR2_OC3M_2                    ((uint32_t)0x0040)            /*!<Bit 2 */

#define  TIM_CCMR2_OC3CE                     ((uint32_t)0x0080)            /*!<Output Compare 3 Clear Enable */

#define  TIM_CCMR2_CC4S                      ((uint32_t)0x0300)            /*!<CC4S[1:0] bits (Capture/Compare 4 Selection) */
#define  TIM_CCMR2_CC4S_0                    ((uint32_t)0x0100)            /*!<Bit 0 */
#define  TIM_CCMR2_CC4S_1                    ((uint32_t)0x0200)            /*!<Bit 1 */

#define  TIM_CCMR2_OC4FE                     ((uint32_t)0x0400)            /*!<Output Compare 4 Fast enable    */
#define  TIM_CCMR2_OC4PE                     ((uint32_t)0x0800)            /*!<Output Compare 4 Preload enable */

#define  TIM_CCMR2_OC4M                      ((uint32_t)0x7000)            /*!<OC4M[2:0] bits (Output Compare 4 Mode) */
#define  TIM_CCMR2_OC4M_0                    ((uint32_t)0x1000)            /*!<Bit 0 */
#define  TIM_CCMR2_OC4M_1                    ((uint32_t)0x2000)            /*!<Bit 1 */
#define  TIM_CCMR2_OC4M_2                    ((uint32_t)0x4000)            /*!<Bit 2 */

#define  TIM_CCMR2_OC4CE                     ((uint32_t)0x8000)            /*!<Output Compare 4 Clear Enable */

/*----------------------------------------------------------------------------*/

#define  TIM_CCMR2_IC3PSC                    ((uint32_t)0x000C)            /*!<IC3PSC[1:0] bits (Input Capture 3 Prescaler) */
#define  TIM_CCMR2_IC3PSC_0                  ((uint32_t)0x0004)            /*!<Bit 0 */
#define  TIM_CCMR2_IC3PSC_1                  ((uint32_t)0x0008)            /*!<Bit 1 */

#define  TIM_CCMR2_IC3F                      ((uint32_t)0x00F0)            /*!<IC3F[3:0] bits (Input Capture 3 Filter) */
#define  TIM_CCMR2_IC3F_0                    ((uint32_t)0x0010)            /*!<Bit 0 */
#define  TIM_CCMR2_IC3F_1                    ((uint32_t)0x0020)            /*!<Bit 1 */
#define  TIM_CCMR2_IC3F_2                    ((uint32_t)0x0040)            /*!<Bit 2 */
#define  TIM_CCMR2_IC3F_3                    ((uint32_t)0x0080)            /*!<Bit 3 */

#define  TIM_CCMR2_IC4PSC                    ((uint32_t)0x0C00)            /*!<IC4PSC[1:0] bits (Input Capture 4 Prescaler) */
#define  TIM_CCMR2_IC4PSC_0                  ((uint32_t)0x0400)            /*!<Bit 0 */
#define  TIM_CCMR2_IC4PSC_1                  ((uint32_t)0x0800)            /*!<Bit 1 */

#define  TIM_CCMR2_IC4F                      ((uint32_t)0xF000)            /*!<IC4F[3:0] bits (Input Capture 4 Filter) */
#define  TIM_CCMR2_IC4F_0                    ((uint32_t)0x1000)            /*!<Bit 0 */
#define  TIM_CCMR2_IC4F_1                    ((uint32_t)0x2000)            /*!<Bit 1 */
#define  TIM_CCMR2_IC4F_2                    ((uint32_t)0x4000)            /*!<Bit 2 */
#define  TIM_CCMR2_IC4F_3                    ((uint32_t)0x8000)            /*!<Bit 3 */

/*******************  Bit definition for TIM_CCER register  *******************/
#define  TIM_CCER_CC1E                       ((uint32_t)0x0001)            /*!<Capture/Compare 1 output enable                 */
#define  TIM_CCER_CC1P                       ((uint32_t)0x0002)            /*!<Capture/Compare 1 output Polarity               */
#define  TIM_CCER_CC1NE                      ((uint32_t)0x0004)            /*!<Capture/Compare 1 Complementary output enable   */
#define  TIM_CCER_CC1NP                      ((uint32_t)0x0008)            /*!<Capture/Compare 1 Complementary output Polarity */
#define  TIM_CCER_CC2E                       ((uint32_t)0x0010)            /*!<Capture/Compare 2 output enable                 */
#define  TIM_CCER_CC2P                       ((uint32_t)0x0020)            /*!<Capture/Compare 2 output Polarity               */
#define  TIM_CCER_CC2NE                      ((uint32_t)0x0040)            /*!<Capture/Compare 2 Complementary output enable   */
#define  TIM_CCER_CC2NP                      ((uint32_t)0x0080)            /*!<Capture/Compare 2 Complementary output Polarity */
#define  TIM_CCER_CC3E                       ((uint32_t)0x0100)            /*!<Capture/Compare 3 output enable                 */
#define  TIM_CCER_CC3P                       ((uint32_t)0x0200)            /*!<Capture/Compare 3 output Polarity               */
#define  TIM_CCER_CC3NE                      ((uint32_t)0x0400)            /*!<Capture/Compare 3 Complementary output enable   */
#define  TIM_CCER_CC3NP                      ((uint32_t)0x0800)            /*!<Capture/Compare 3 Complementary output Polarity */
#define  TIM_CCER_CC4E                       ((uint32_t)0x1000)            /*!<Capture/Compare 4 output enable                 */
#define  TIM_CCER_CC4P                       ((uint32_t)0x2000)            /*!<Capture/Compare 4 output Polarity               */
#define  TIM_CCER_CC4NP                      ((uint32_t)0x8000)            /*!<Capture/Compare 4 Complementary output Polarity */

/*******************  Bit definition for TIM_CNT register  ********************/
#define  TIM_CNT_CNT                         ((uint32_t)0xFFFF)            /*!<Counter Value            */

/*******************  Bit definition for TIM_PSC register  ********************/
#define  TIM_PSC_PSC                         ((uint32_t)0xFFFF)            /*!<Prescaler Value          */

/*******************  Bit definition for TIM_ARR register  ********************/
#define  TIM_ARR_ARR                         ((uint32_t)0xFFFF)            /*!<actual auto-reload Value */

/*******************  Bit definition for TIM_RCR register  ********************/
#define  TIM_RCR_REP                         ((uint32_t)0xFF)               /*!<Repetition Counter Value */

/*******************  Bit definition for TIM_CCR1 register  *******************/
#define  TIM_CCR1_CCR1                       ((uint32_t)0xFFFF)            /*!<Capture/Compare 1 Value  */

/*******************  Bit definition for TIM_CCR2 register  *******************/
#define  TIM_CCR2_CCR2                       ((uint32_t)0xFFFF)            /*!<Capture/Compare 2 Value  */

/*******************  Bit definition for TIM_CCR3 register  *******************/
#define  TIM_CCR3_CCR3                       ((uint32_t)0xFFFF)            /*!<Capture/Compare 3 Value  */

/*******************  Bit definition for TIM_CCR4 register  *******************/
#define  TIM_CCR4_CCR4                       ((uint32_t)0xFFFF)            /*!<Capture/Compare 4 Value  */

/*******************  Bit definition for TIM_BDTR register  *******************/
#define  TIM_BDTR_DTG                        ((uint32_t)0x00FF)            /*!<DTG[0:7] bits (Dead-Time Generator set-up) */
#define  TIM_BDTR_DTG_0                      ((uint32_t)0x0001)            /*!<Bit 0 */
#define  TIM_BDTR_DTG_1                      ((uint32_t)0x0002)            /*!<Bit 1 */
#define  TIM_BDTR_DTG_2                      ((uint32_t)0x0004)            /*!<Bit 2 */
#define  TIM_BDTR_DTG_3                      ((uint32_t)0x0008)            /*!<Bit 3 */
#define  TIM_BDTR_DTG_4                      ((uint32_t)0x0010)            /*!<Bit 4 */
#define  TIM_BDTR_DTG_5                      ((uint32_t)0x0020)            /*!<Bit 5 */
#define  TIM_BDTR_DTG_6                      ((uint32_t)0x0040)            /*!<Bit 6 */
#define  TIM_BDTR_DTG_7                      ((uint32_t)0x0080)            /*!<Bit 7 */

#define  TIM_BDTR_LOCK                       ((uint32_t)0x0300)            /*!<LOCK[1:0] bits (Lock Configuration) */
#define  TIM_BDTR_LOCK_0                     ((uint32_t)0x0100)            /*!<Bit 0 */
#define  TIM_BDTR_LOCK_1                     ((uint32_t)0x0200)            /*!<Bit 1 */

#define  TIM_BDTR_OSSI                       ((uint32_t)0x0400)            /*!<Off-State Selection for Idle mode */
#define  TIM_BDTR_OSSR                       ((uint32_t)0x0800)            /*!<Off-State Selection for Run mode  */
#define  TIM_BDTR_BKE                        ((uint32_t)0x1000)            /*!<Break enable                      */
#define  TIM_BDTR_BKP                        ((uint32_t)0x2000)            /*!<Break Polarity                    */
#define  TIM_BDTR_AOE                        ((uint32_t)0x4000)            /*!<Automatic Output enable           */
#define  TIM_BDTR_MOE                        ((uint32_t)0x8000)            /*!<Main Output enable                */

/*******************  Bit definition for TIM_DCR register  ********************/
#define  TIM_DCR_DBA                         ((uint32_t)0x001F)            /*!<DBA[4:0] bits (DMA Base Address) */
#define  TIM_DCR_DBA_0                       ((uint32_t)0x0001)            /*!<Bit 0 */
#define  TIM_DCR_DBA_1                       ((uint32_t)0x0002)            /*!<Bit 1 */
#define  TIM_DCR_DBA_2                       ((uint32_t)0x0004)            /*!<Bit 2 */
#define  TIM_DCR_DBA_3                       ((uint32_t)0x0008)            /*!<Bit 3 */
#define  TIM_DCR_DBA_4                       ((uint32_t)0x0010)            /*!<Bit 4 */

#define  TIM_DCR_DBL                         ((uint32_t)0x1F00)            /*!<DBL[4:0] bits (DMA Burst Length) */
#define  TIM_DCR_DBL_0                       ((uint32_t)0x0100)            /*!<Bit 0 */
#define  TIM_DCR_DBL_1                       ((uint32_t)0x0200)            /*!<Bit 1 */
#define  TIM_DCR_DBL_2                       ((uint32_t)0x0400)            /*!<Bit 2 */
#define  TIM_DCR_DBL_3                       ((uint32_t)0x0800)            /*!<Bit 3 */
#define  TIM_DCR_DBL_4                       ((uint32_t)0x1000)            /*!<Bit 4 */

/*******************  Bit definition for TIM_DMAR register  *******************/
#define  TIM_DMAR_DMAB                       ((uint32_t)0xFFFF)            /*!<DMA register for burst accesses                    */

/*******************  Bit definition for TIM_OR register  *********************/
#define TIM_OR_TI4_RMP                       ((uint32_t)0x00C0)            /*!<TI4_RMP[1:0] bits (TIM5 Input 4 remap)             */
#define TIM_OR_TI4_RMP_0                     ((uint32_t)0x0040)            /*!<Bit 0 */
#define TIM_OR_TI4_RMP_1                     ((uint32_t)0x0080)            /*!<Bit 1 */
#define TIM_OR_ITR1_RMP                      ((uint32_t)0x0C00)            /*!<ITR1_RMP[1:0] bits (TIM2 Internal trigger 1 remap) */
#define TIM_OR_ITR1_RMP_0                    ((uint32_t)0x0400)            /*!<Bit 0 */
#define TIM_OR_ITR1_RMP_1                    ((uint32_t)0x0800)            /*!<Bit 1 */


/**
  * @brief TIM
  */

typedef struct
{
  __IO uint32_t CR1;         /*!< TIM control register 1,              Address offset: 0x00 */
  __IO uint32_t CR2;         /*!< TIM control register 2,              Address offset: 0x04 */
  __IO uint32_t SMCR;        /*!< TIM slave mode control register,     Address offset: 0x08 */
  __IO uint32_t DIER;        /*!< TIM DMA/interrupt enable register,   Address offset: 0x0C */
  __IO uint32_t SR;          /*!< TIM status register,                 Address offset: 0x10 */
  __IO uint32_t EGR;         /*!< TIM event generation register,       Address offset: 0x14 */
  __IO uint32_t CCMR1;       /*!< TIM capture/compare mode register 1, Address offset: 0x18 */
  __IO uint32_t CCMR2;       /*!< TIM capture/compare mode register 2, Address offset: 0x1C */
  __IO uint32_t CCER;        /*!< TIM capture/compare enable register, Address offset: 0x20 */
  __IO uint32_t CNT;         /*!< TIM counter register,                Address offset: 0x24 */
  __IO uint32_t PSC;         /*!< TIM prescaler,                       Address offset: 0x28 */
  __IO uint32_t ARR;         /*!< TIM auto-reload register,            Address offset: 0x2C */
  __IO uint32_t RCR;         /*!< TIM repetition counter register,     Address offset: 0x30 */
  __IO uint32_t CCR1;        /*!< TIM capture/compare register 1,      Address offset: 0x34 */
  __IO uint32_t CCR2;        /*!< TIM capture/compare register 2,      Address offset: 0x38 */
  __IO uint32_t CCR3;        /*!< TIM capture/compare register 3,      Address offset: 0x3C */
  __IO uint32_t CCR4;        /*!< TIM capture/compare register 4,      Address offset: 0x40 */
  __IO uint32_t BDTR;        /*!< TIM break and dead-time register,    Address offset: 0x44 */
  __IO uint32_t DCR;         /*!< TIM DMA control register,            Address offset: 0x48 */
  __IO uint32_t DMAR;        /*!< TIM DMA address for full transfer,   Address offset: 0x4C */
  __IO uint32_t OR;          /*!< TIM option register,                 Address offset: 0x50 */
} TIM_TypeDef;

#define TIM2_BASE             (APB1PERIPH_BASE + 0x0000)
#define TIM3_BASE             (APB1PERIPH_BASE + 0x0400)
#define TIM4_BASE             (APB1PERIPH_BASE + 0x0800)
#define TIM5_BASE             (APB1PERIPH_BASE + 0x0C00)
#define TIM6_BASE             (APB1PERIPH_BASE + 0x1000)
#define TIM7_BASE             (APB1PERIPH_BASE + 0x1400)
#define TIM12_BASE            (APB1PERIPH_BASE + 0x1800)
#define TIM13_BASE            (APB1PERIPH_BASE + 0x1C00)
#define TIM14_BASE            (APB1PERIPH_BASE + 0x2000)
#define TIM1_BASE             (APB2PERIPH_BASE + 0x0000)
#define TIM8_BASE             (APB2PERIPH_BASE + 0x0400)
#define TIM9_BASE             (APB2PERIPH_BASE + 0x4000)
#define TIM10_BASE            (APB2PERIPH_BASE + 0x4400)
#define TIM11_BASE            (APB2PERIPH_BASE + 0x4800)

#define TIM2                ((TIM_TypeDef *) TIM2_BASE)
#define TIM3                ((TIM_TypeDef *) TIM3_BASE)
#define TIM4                ((TIM_TypeDef *) TIM4_BASE)
#define TIM5                ((TIM_TypeDef *) TIM5_BASE)
#define TIM6                ((TIM_TypeDef *) TIM6_BASE)
#define TIM7                ((TIM_TypeDef *) TIM7_BASE)
#define TIM12               ((TIM_TypeDef *) TIM12_BASE)
#define TIM13               ((TIM_TypeDef *) TIM13_BASE)
#define TIM14               ((TIM_TypeDef *) TIM14_BASE)
#define TIM1                ((TIM_TypeDef *) TIM1_BASE)
#define TIM8                ((TIM_TypeDef *) TIM8_BASE)
#define TIM9                ((TIM_TypeDef *) TIM9_BASE)
#define TIM10               ((TIM_TypeDef *) TIM10_BASE)
#define TIM11               ((TIM_TypeDef *) TIM11_BASE)

#endif /* HAL_ST_STM32F4_ST_TIMER_H_ */
