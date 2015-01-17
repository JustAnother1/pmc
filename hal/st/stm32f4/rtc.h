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

#ifndef HAL_ST_STM32F4_RTC_H_
#define HAL_ST_STM32F4_RTC_H_

#include "hal_common.h"

/******************************************************************************/
/*                                                                            */
/*                           Real-Time Clock (RTC)                            */
/*                                                                            */
/******************************************************************************/
/********************  Bits definition for RTC_TR register  *******************/
#define RTC_TR_PM                            ((uint_fast32_t)0x00400000)
#define RTC_TR_HT                            ((uint_fast32_t)0x00300000)
#define RTC_TR_HT_0                          ((uint_fast32_t)0x00100000)
#define RTC_TR_HT_1                          ((uint_fast32_t)0x00200000)
#define RTC_TR_HU                            ((uint_fast32_t)0x000F0000)
#define RTC_TR_HU_0                          ((uint_fast32_t)0x00010000)
#define RTC_TR_HU_1                          ((uint_fast32_t)0x00020000)
#define RTC_TR_HU_2                          ((uint_fast32_t)0x00040000)
#define RTC_TR_HU_3                          ((uint_fast32_t)0x00080000)
#define RTC_TR_MNT                           ((uint_fast32_t)0x00007000)
#define RTC_TR_MNT_0                         ((uint_fast32_t)0x00001000)
#define RTC_TR_MNT_1                         ((uint_fast32_t)0x00002000)
#define RTC_TR_MNT_2                         ((uint_fast32_t)0x00004000)
#define RTC_TR_MNU                           ((uint_fast32_t)0x00000F00)
#define RTC_TR_MNU_0                         ((uint_fast32_t)0x00000100)
#define RTC_TR_MNU_1                         ((uint_fast32_t)0x00000200)
#define RTC_TR_MNU_2                         ((uint_fast32_t)0x00000400)
#define RTC_TR_MNU_3                         ((uint_fast32_t)0x00000800)
#define RTC_TR_ST                            ((uint_fast32_t)0x00000070)
#define RTC_TR_ST_0                          ((uint_fast32_t)0x00000010)
#define RTC_TR_ST_1                          ((uint_fast32_t)0x00000020)
#define RTC_TR_ST_2                          ((uint_fast32_t)0x00000040)
#define RTC_TR_SU                            ((uint_fast32_t)0x0000000F)
#define RTC_TR_SU_0                          ((uint_fast32_t)0x00000001)
#define RTC_TR_SU_1                          ((uint_fast32_t)0x00000002)
#define RTC_TR_SU_2                          ((uint_fast32_t)0x00000004)
#define RTC_TR_SU_3                          ((uint_fast32_t)0x00000008)

/********************  Bits definition for RTC_DR register  *******************/
#define RTC_DR_YT                            ((uint_fast32_t)0x00F00000)
#define RTC_DR_YT_0                          ((uint_fast32_t)0x00100000)
#define RTC_DR_YT_1                          ((uint_fast32_t)0x00200000)
#define RTC_DR_YT_2                          ((uint_fast32_t)0x00400000)
#define RTC_DR_YT_3                          ((uint_fast32_t)0x00800000)
#define RTC_DR_YU                            ((uint_fast32_t)0x000F0000)
#define RTC_DR_YU_0                          ((uint_fast32_t)0x00010000)
#define RTC_DR_YU_1                          ((uint_fast32_t)0x00020000)
#define RTC_DR_YU_2                          ((uint_fast32_t)0x00040000)
#define RTC_DR_YU_3                          ((uint_fast32_t)0x00080000)
#define RTC_DR_WDU                           ((uint_fast32_t)0x0000E000)
#define RTC_DR_WDU_0                         ((uint_fast32_t)0x00002000)
#define RTC_DR_WDU_1                         ((uint_fast32_t)0x00004000)
#define RTC_DR_WDU_2                         ((uint_fast32_t)0x00008000)
#define RTC_DR_MT                            ((uint_fast32_t)0x00001000)
#define RTC_DR_MU                            ((uint_fast32_t)0x00000F00)
#define RTC_DR_MU_0                          ((uint_fast32_t)0x00000100)
#define RTC_DR_MU_1                          ((uint_fast32_t)0x00000200)
#define RTC_DR_MU_2                          ((uint_fast32_t)0x00000400)
#define RTC_DR_MU_3                          ((uint_fast32_t)0x00000800)
#define RTC_DR_DT                            ((uint_fast32_t)0x00000030)
#define RTC_DR_DT_0                          ((uint_fast32_t)0x00000010)
#define RTC_DR_DT_1                          ((uint_fast32_t)0x00000020)
#define RTC_DR_DU                            ((uint_fast32_t)0x0000000F)
#define RTC_DR_DU_0                          ((uint_fast32_t)0x00000001)
#define RTC_DR_DU_1                          ((uint_fast32_t)0x00000002)
#define RTC_DR_DU_2                          ((uint_fast32_t)0x00000004)
#define RTC_DR_DU_3                          ((uint_fast32_t)0x00000008)

/********************  Bits definition for RTC_CR register  *******************/
#define RTC_CR_COE                           ((uint_fast32_t)0x00800000)
#define RTC_CR_OSEL                          ((uint_fast32_t)0x00600000)
#define RTC_CR_OSEL_0                        ((uint_fast32_t)0x00200000)
#define RTC_CR_OSEL_1                        ((uint_fast32_t)0x00400000)
#define RTC_CR_POL                           ((uint_fast32_t)0x00100000)
#define RTC_CR_COSEL                         ((uint_fast32_t)0x00080000)
#define RTC_CR_BCK                           ((uint_fast32_t)0x00040000)
#define RTC_CR_SUB1H                         ((uint_fast32_t)0x00020000)
#define RTC_CR_ADD1H                         ((uint_fast32_t)0x00010000)
#define RTC_CR_TSIE                          ((uint_fast32_t)0x00008000)
#define RTC_CR_WUTIE                         ((uint_fast32_t)0x00004000)
#define RTC_CR_ALRBIE                        ((uint_fast32_t)0x00002000)
#define RTC_CR_ALRAIE                        ((uint_fast32_t)0x00001000)
#define RTC_CR_TSE                           ((uint_fast32_t)0x00000800)
#define RTC_CR_WUTE                          ((uint_fast32_t)0x00000400)
#define RTC_CR_ALRBE                         ((uint_fast32_t)0x00000200)
#define RTC_CR_ALRAE                         ((uint_fast32_t)0x00000100)
#define RTC_CR_DCE                           ((uint_fast32_t)0x00000080)
#define RTC_CR_FMT                           ((uint_fast32_t)0x00000040)
#define RTC_CR_BYPSHAD                       ((uint_fast32_t)0x00000020)
#define RTC_CR_REFCKON                       ((uint_fast32_t)0x00000010)
#define RTC_CR_TSEDGE                        ((uint_fast32_t)0x00000008)
#define RTC_CR_WUCKSEL                       ((uint_fast32_t)0x00000007)
#define RTC_CR_WUCKSEL_0                     ((uint_fast32_t)0x00000001)
#define RTC_CR_WUCKSEL_1                     ((uint_fast32_t)0x00000002)
#define RTC_CR_WUCKSEL_2                     ((uint_fast32_t)0x00000004)

/********************  Bits definition for RTC_ISR register  ******************/
#define RTC_ISR_RECALPF                      ((uint_fast32_t)0x00010000)
#define RTC_ISR_TAMP1F                       ((uint_fast32_t)0x00002000)
#define RTC_ISR_TAMP2F                       ((uint_fast32_t)0x00004000)
#define RTC_ISR_TSOVF                        ((uint_fast32_t)0x00001000)
#define RTC_ISR_TSF                          ((uint_fast32_t)0x00000800)
#define RTC_ISR_WUTF                         ((uint_fast32_t)0x00000400)
#define RTC_ISR_ALRBF                        ((uint_fast32_t)0x00000200)
#define RTC_ISR_ALRAF                        ((uint_fast32_t)0x00000100)
#define RTC_ISR_INIT                         ((uint_fast32_t)0x00000080)
#define RTC_ISR_INITF                        ((uint_fast32_t)0x00000040)
#define RTC_ISR_RSF                          ((uint_fast32_t)0x00000020)
#define RTC_ISR_INITS                        ((uint_fast32_t)0x00000010)
#define RTC_ISR_SHPF                         ((uint_fast32_t)0x00000008)
#define RTC_ISR_WUTWF                        ((uint_fast32_t)0x00000004)
#define RTC_ISR_ALRBWF                       ((uint_fast32_t)0x00000002)
#define RTC_ISR_ALRAWF                       ((uint_fast32_t)0x00000001)

/********************  Bits definition for RTC_PRER register  *****************/
#define RTC_PRER_PREDIV_A                    ((uint_fast32_t)0x007F0000)
#define RTC_PRER_PREDIV_S                    ((uint_fast32_t)0x00001FFF)

/********************  Bits definition for RTC_WUTR register  *****************/
#define RTC_WUTR_WUT                         ((uint_fast32_t)0x0000FFFF)

/********************  Bits definition for RTC_CALIBR register  ***************/
#define RTC_CALIBR_DCS                       ((uint_fast32_t)0x00000080)
#define RTC_CALIBR_DC                        ((uint_fast32_t)0x0000001F)

/********************  Bits definition for RTC_ALRMAR register  ***************/
#define RTC_ALRMAR_MSK4                      ((uint_fast32_t)0x80000000)
#define RTC_ALRMAR_WDSEL                     ((uint_fast32_t)0x40000000)
#define RTC_ALRMAR_DT                        ((uint_fast32_t)0x30000000)
#define RTC_ALRMAR_DT_0                      ((uint_fast32_t)0x10000000)
#define RTC_ALRMAR_DT_1                      ((uint_fast32_t)0x20000000)
#define RTC_ALRMAR_DU                        ((uint_fast32_t)0x0F000000)
#define RTC_ALRMAR_DU_0                      ((uint_fast32_t)0x01000000)
#define RTC_ALRMAR_DU_1                      ((uint_fast32_t)0x02000000)
#define RTC_ALRMAR_DU_2                      ((uint_fast32_t)0x04000000)
#define RTC_ALRMAR_DU_3                      ((uint_fast32_t)0x08000000)
#define RTC_ALRMAR_MSK3                      ((uint_fast32_t)0x00800000)
#define RTC_ALRMAR_PM                        ((uint_fast32_t)0x00400000)
#define RTC_ALRMAR_HT                        ((uint_fast32_t)0x00300000)
#define RTC_ALRMAR_HT_0                      ((uint_fast32_t)0x00100000)
#define RTC_ALRMAR_HT_1                      ((uint_fast32_t)0x00200000)
#define RTC_ALRMAR_HU                        ((uint_fast32_t)0x000F0000)
#define RTC_ALRMAR_HU_0                      ((uint_fast32_t)0x00010000)
#define RTC_ALRMAR_HU_1                      ((uint_fast32_t)0x00020000)
#define RTC_ALRMAR_HU_2                      ((uint_fast32_t)0x00040000)
#define RTC_ALRMAR_HU_3                      ((uint_fast32_t)0x00080000)
#define RTC_ALRMAR_MSK2                      ((uint_fast32_t)0x00008000)
#define RTC_ALRMAR_MNT                       ((uint_fast32_t)0x00007000)
#define RTC_ALRMAR_MNT_0                     ((uint_fast32_t)0x00001000)
#define RTC_ALRMAR_MNT_1                     ((uint_fast32_t)0x00002000)
#define RTC_ALRMAR_MNT_2                     ((uint_fast32_t)0x00004000)
#define RTC_ALRMAR_MNU                       ((uint_fast32_t)0x00000F00)
#define RTC_ALRMAR_MNU_0                     ((uint_fast32_t)0x00000100)
#define RTC_ALRMAR_MNU_1                     ((uint_fast32_t)0x00000200)
#define RTC_ALRMAR_MNU_2                     ((uint_fast32_t)0x00000400)
#define RTC_ALRMAR_MNU_3                     ((uint_fast32_t)0x00000800)
#define RTC_ALRMAR_MSK1                      ((uint_fast32_t)0x00000080)
#define RTC_ALRMAR_ST                        ((uint_fast32_t)0x00000070)
#define RTC_ALRMAR_ST_0                      ((uint_fast32_t)0x00000010)
#define RTC_ALRMAR_ST_1                      ((uint_fast32_t)0x00000020)
#define RTC_ALRMAR_ST_2                      ((uint_fast32_t)0x00000040)
#define RTC_ALRMAR_SU                        ((uint_fast32_t)0x0000000F)
#define RTC_ALRMAR_SU_0                      ((uint_fast32_t)0x00000001)
#define RTC_ALRMAR_SU_1                      ((uint_fast32_t)0x00000002)
#define RTC_ALRMAR_SU_2                      ((uint_fast32_t)0x00000004)
#define RTC_ALRMAR_SU_3                      ((uint_fast32_t)0x00000008)

/********************  Bits definition for RTC_ALRMBR register  ***************/
#define RTC_ALRMBR_MSK4                      ((uint_fast32_t)0x80000000)
#define RTC_ALRMBR_WDSEL                     ((uint_fast32_t)0x40000000)
#define RTC_ALRMBR_DT                        ((uint_fast32_t)0x30000000)
#define RTC_ALRMBR_DT_0                      ((uint_fast32_t)0x10000000)
#define RTC_ALRMBR_DT_1                      ((uint_fast32_t)0x20000000)
#define RTC_ALRMBR_DU                        ((uint_fast32_t)0x0F000000)
#define RTC_ALRMBR_DU_0                      ((uint_fast32_t)0x01000000)
#define RTC_ALRMBR_DU_1                      ((uint_fast32_t)0x02000000)
#define RTC_ALRMBR_DU_2                      ((uint_fast32_t)0x04000000)
#define RTC_ALRMBR_DU_3                      ((uint_fast32_t)0x08000000)
#define RTC_ALRMBR_MSK3                      ((uint_fast32_t)0x00800000)
#define RTC_ALRMBR_PM                        ((uint_fast32_t)0x00400000)
#define RTC_ALRMBR_HT                        ((uint_fast32_t)0x00300000)
#define RTC_ALRMBR_HT_0                      ((uint_fast32_t)0x00100000)
#define RTC_ALRMBR_HT_1                      ((uint_fast32_t)0x00200000)
#define RTC_ALRMBR_HU                        ((uint_fast32_t)0x000F0000)
#define RTC_ALRMBR_HU_0                      ((uint_fast32_t)0x00010000)
#define RTC_ALRMBR_HU_1                      ((uint_fast32_t)0x00020000)
#define RTC_ALRMBR_HU_2                      ((uint_fast32_t)0x00040000)
#define RTC_ALRMBR_HU_3                      ((uint_fast32_t)0x00080000)
#define RTC_ALRMBR_MSK2                      ((uint_fast32_t)0x00008000)
#define RTC_ALRMBR_MNT                       ((uint_fast32_t)0x00007000)
#define RTC_ALRMBR_MNT_0                     ((uint_fast32_t)0x00001000)
#define RTC_ALRMBR_MNT_1                     ((uint_fast32_t)0x00002000)
#define RTC_ALRMBR_MNT_2                     ((uint_fast32_t)0x00004000)
#define RTC_ALRMBR_MNU                       ((uint_fast32_t)0x00000F00)
#define RTC_ALRMBR_MNU_0                     ((uint_fast32_t)0x00000100)
#define RTC_ALRMBR_MNU_1                     ((uint_fast32_t)0x00000200)
#define RTC_ALRMBR_MNU_2                     ((uint_fast32_t)0x00000400)
#define RTC_ALRMBR_MNU_3                     ((uint_fast32_t)0x00000800)
#define RTC_ALRMBR_MSK1                      ((uint_fast32_t)0x00000080)
#define RTC_ALRMBR_ST                        ((uint_fast32_t)0x00000070)
#define RTC_ALRMBR_ST_0                      ((uint_fast32_t)0x00000010)
#define RTC_ALRMBR_ST_1                      ((uint_fast32_t)0x00000020)
#define RTC_ALRMBR_ST_2                      ((uint_fast32_t)0x00000040)
#define RTC_ALRMBR_SU                        ((uint_fast32_t)0x0000000F)
#define RTC_ALRMBR_SU_0                      ((uint_fast32_t)0x00000001)
#define RTC_ALRMBR_SU_1                      ((uint_fast32_t)0x00000002)
#define RTC_ALRMBR_SU_2                      ((uint_fast32_t)0x00000004)
#define RTC_ALRMBR_SU_3                      ((uint_fast32_t)0x00000008)

/********************  Bits definition for RTC_WPR register  ******************/
#define RTC_WPR_KEY                          ((uint_fast32_t)0x000000FF)

/********************  Bits definition for RTC_SSR register  ******************/
#define RTC_SSR_SS                           ((uint_fast32_t)0x0000FFFF)

/********************  Bits definition for RTC_SHIFTR register  ***************/
#define RTC_SHIFTR_SUBFS                     ((uint_fast32_t)0x00007FFF)
#define RTC_SHIFTR_ADD1S                     ((uint_fast32_t)0x80000000)

/********************  Bits definition for RTC_TSTR register  *****************/
#define RTC_TSTR_PM                          ((uint_fast32_t)0x00400000)
#define RTC_TSTR_HT                          ((uint_fast32_t)0x00300000)
#define RTC_TSTR_HT_0                        ((uint_fast32_t)0x00100000)
#define RTC_TSTR_HT_1                        ((uint_fast32_t)0x00200000)
#define RTC_TSTR_HU                          ((uint_fast32_t)0x000F0000)
#define RTC_TSTR_HU_0                        ((uint_fast32_t)0x00010000)
#define RTC_TSTR_HU_1                        ((uint_fast32_t)0x00020000)
#define RTC_TSTR_HU_2                        ((uint_fast32_t)0x00040000)
#define RTC_TSTR_HU_3                        ((uint_fast32_t)0x00080000)
#define RTC_TSTR_MNT                         ((uint_fast32_t)0x00007000)
#define RTC_TSTR_MNT_0                       ((uint_fast32_t)0x00001000)
#define RTC_TSTR_MNT_1                       ((uint_fast32_t)0x00002000)
#define RTC_TSTR_MNT_2                       ((uint_fast32_t)0x00004000)
#define RTC_TSTR_MNU                         ((uint_fast32_t)0x00000F00)
#define RTC_TSTR_MNU_0                       ((uint_fast32_t)0x00000100)
#define RTC_TSTR_MNU_1                       ((uint_fast32_t)0x00000200)
#define RTC_TSTR_MNU_2                       ((uint_fast32_t)0x00000400)
#define RTC_TSTR_MNU_3                       ((uint_fast32_t)0x00000800)
#define RTC_TSTR_ST                          ((uint_fast32_t)0x00000070)
#define RTC_TSTR_ST_0                        ((uint_fast32_t)0x00000010)
#define RTC_TSTR_ST_1                        ((uint_fast32_t)0x00000020)
#define RTC_TSTR_ST_2                        ((uint_fast32_t)0x00000040)
#define RTC_TSTR_SU                          ((uint_fast32_t)0x0000000F)
#define RTC_TSTR_SU_0                        ((uint_fast32_t)0x00000001)
#define RTC_TSTR_SU_1                        ((uint_fast32_t)0x00000002)
#define RTC_TSTR_SU_2                        ((uint_fast32_t)0x00000004)
#define RTC_TSTR_SU_3                        ((uint_fast32_t)0x00000008)

/********************  Bits definition for RTC_TSDR register  *****************/
#define RTC_TSDR_WDU                         ((uint_fast32_t)0x0000E000)
#define RTC_TSDR_WDU_0                       ((uint_fast32_t)0x00002000)
#define RTC_TSDR_WDU_1                       ((uint_fast32_t)0x00004000)
#define RTC_TSDR_WDU_2                       ((uint_fast32_t)0x00008000)
#define RTC_TSDR_MT                          ((uint_fast32_t)0x00001000)
#define RTC_TSDR_MU                          ((uint_fast32_t)0x00000F00)
#define RTC_TSDR_MU_0                        ((uint_fast32_t)0x00000100)
#define RTC_TSDR_MU_1                        ((uint_fast32_t)0x00000200)
#define RTC_TSDR_MU_2                        ((uint_fast32_t)0x00000400)
#define RTC_TSDR_MU_3                        ((uint_fast32_t)0x00000800)
#define RTC_TSDR_DT                          ((uint_fast32_t)0x00000030)
#define RTC_TSDR_DT_0                        ((uint_fast32_t)0x00000010)
#define RTC_TSDR_DT_1                        ((uint_fast32_t)0x00000020)
#define RTC_TSDR_DU                          ((uint_fast32_t)0x0000000F)
#define RTC_TSDR_DU_0                        ((uint_fast32_t)0x00000001)
#define RTC_TSDR_DU_1                        ((uint_fast32_t)0x00000002)
#define RTC_TSDR_DU_2                        ((uint_fast32_t)0x00000004)
#define RTC_TSDR_DU_3                        ((uint_fast32_t)0x00000008)

/********************  Bits definition for RTC_TSSSR register  ****************/
#define RTC_TSSSR_SS                         ((uint_fast32_t)0x0000FFFF)

/********************  Bits definition for RTC_CAL register  *****************/
#define RTC_CALR_CALP                        ((uint_fast32_t)0x00008000)
#define RTC_CALR_CALW8                       ((uint_fast32_t)0x00004000)
#define RTC_CALR_CALW16                      ((uint_fast32_t)0x00002000)
#define RTC_CALR_CALM                        ((uint_fast32_t)0x000001FF)
#define RTC_CALR_CALM_0                      ((uint_fast32_t)0x00000001)
#define RTC_CALR_CALM_1                      ((uint_fast32_t)0x00000002)
#define RTC_CALR_CALM_2                      ((uint_fast32_t)0x00000004)
#define RTC_CALR_CALM_3                      ((uint_fast32_t)0x00000008)
#define RTC_CALR_CALM_4                      ((uint_fast32_t)0x00000010)
#define RTC_CALR_CALM_5                      ((uint_fast32_t)0x00000020)
#define RTC_CALR_CALM_6                      ((uint_fast32_t)0x00000040)
#define RTC_CALR_CALM_7                      ((uint_fast32_t)0x00000080)
#define RTC_CALR_CALM_8                      ((uint_fast32_t)0x00000100)

/********************  Bits definition for RTC_TAFCR register  ****************/
#define RTC_TAFCR_ALARMOUTTYPE               ((uint_fast32_t)0x00040000)
#define RTC_TAFCR_TSINSEL                    ((uint_fast32_t)0x00020000)
#define RTC_TAFCR_TAMPINSEL                  ((uint_fast32_t)0x00010000)
#define RTC_TAFCR_TAMPPUDIS                  ((uint_fast32_t)0x00008000)
#define RTC_TAFCR_TAMPPRCH                   ((uint_fast32_t)0x00006000)
#define RTC_TAFCR_TAMPPRCH_0                 ((uint_fast32_t)0x00002000)
#define RTC_TAFCR_TAMPPRCH_1                 ((uint_fast32_t)0x00004000)
#define RTC_TAFCR_TAMPFLT                    ((uint_fast32_t)0x00001800)
#define RTC_TAFCR_TAMPFLT_0                  ((uint_fast32_t)0x00000800)
#define RTC_TAFCR_TAMPFLT_1                  ((uint_fast32_t)0x00001000)
#define RTC_TAFCR_TAMPFREQ                   ((uint_fast32_t)0x00000700)
#define RTC_TAFCR_TAMPFREQ_0                 ((uint_fast32_t)0x00000100)
#define RTC_TAFCR_TAMPFREQ_1                 ((uint_fast32_t)0x00000200)
#define RTC_TAFCR_TAMPFREQ_2                 ((uint_fast32_t)0x00000400)
#define RTC_TAFCR_TAMPTS                     ((uint_fast32_t)0x00000080)
#define RTC_TAFCR_TAMP2TRG                   ((uint_fast32_t)0x00000010)
#define RTC_TAFCR_TAMP2E                     ((uint_fast32_t)0x00000008)
#define RTC_TAFCR_TAMPIE                     ((uint_fast32_t)0x00000004)
#define RTC_TAFCR_TAMP1TRG                   ((uint_fast32_t)0x00000002)
#define RTC_TAFCR_TAMP1E                     ((uint_fast32_t)0x00000001)

/********************  Bits definition for RTC_ALRMASSR register  *************/
#define RTC_ALRMASSR_MASKSS                  ((uint_fast32_t)0x0F000000)
#define RTC_ALRMASSR_MASKSS_0                ((uint_fast32_t)0x01000000)
#define RTC_ALRMASSR_MASKSS_1                ((uint_fast32_t)0x02000000)
#define RTC_ALRMASSR_MASKSS_2                ((uint_fast32_t)0x04000000)
#define RTC_ALRMASSR_MASKSS_3                ((uint_fast32_t)0x08000000)
#define RTC_ALRMASSR_SS                      ((uint_fast32_t)0x00007FFF)

/********************  Bits definition for RTC_ALRMBSSR register  *************/
#define RTC_ALRMBSSR_MASKSS                  ((uint_fast32_t)0x0F000000)
#define RTC_ALRMBSSR_MASKSS_0                ((uint_fast32_t)0x01000000)
#define RTC_ALRMBSSR_MASKSS_1                ((uint_fast32_t)0x02000000)
#define RTC_ALRMBSSR_MASKSS_2                ((uint_fast32_t)0x04000000)
#define RTC_ALRMBSSR_MASKSS_3                ((uint_fast32_t)0x08000000)
#define RTC_ALRMBSSR_SS                      ((uint_fast32_t)0x00007FFF)

/********************  Bits definition for RTC_BKP0R register  ****************/
#define RTC_BKP0R                            ((uint_fast32_t)0xFFFFFFFF)

/********************  Bits definition for RTC_BKP1R register  ****************/
#define RTC_BKP1R                            ((uint_fast32_t)0xFFFFFFFF)

/********************  Bits definition for RTC_BKP2R register  ****************/
#define RTC_BKP2R                            ((uint_fast32_t)0xFFFFFFFF)

/********************  Bits definition for RTC_BKP3R register  ****************/
#define RTC_BKP3R                            ((uint_fast32_t)0xFFFFFFFF)

/********************  Bits definition for RTC_BKP4R register  ****************/
#define RTC_BKP4R                            ((uint_fast32_t)0xFFFFFFFF)

/********************  Bits definition for RTC_BKP5R register  ****************/
#define RTC_BKP5R                            ((uint_fast32_t)0xFFFFFFFF)

/********************  Bits definition for RTC_BKP6R register  ****************/
#define RTC_BKP6R                            ((uint_fast32_t)0xFFFFFFFF)

/********************  Bits definition for RTC_BKP7R register  ****************/
#define RTC_BKP7R                            ((uint_fast32_t)0xFFFFFFFF)

/********************  Bits definition for RTC_BKP8R register  ****************/
#define RTC_BKP8R                            ((uint_fast32_t)0xFFFFFFFF)

/********************  Bits definition for RTC_BKP9R register  ****************/
#define RTC_BKP9R                            ((uint_fast32_t)0xFFFFFFFF)

/********************  Bits definition for RTC_BKP10R register  ***************/
#define RTC_BKP10R                           ((uint_fast32_t)0xFFFFFFFF)

/********************  Bits definition for RTC_BKP11R register  ***************/
#define RTC_BKP11R                           ((uint_fast32_t)0xFFFFFFFF)

/********************  Bits definition for RTC_BKP12R register  ***************/
#define RTC_BKP12R                           ((uint_fast32_t)0xFFFFFFFF)

/********************  Bits definition for RTC_BKP13R register  ***************/
#define RTC_BKP13R                           ((uint_fast32_t)0xFFFFFFFF)

/********************  Bits definition for RTC_BKP14R register  ***************/
#define RTC_BKP14R                           ((uint_fast32_t)0xFFFFFFFF)

/********************  Bits definition for RTC_BKP15R register  ***************/
#define RTC_BKP15R                           ((uint_fast32_t)0xFFFFFFFF)

/********************  Bits definition for RTC_BKP16R register  ***************/
#define RTC_BKP16R                           ((uint_fast32_t)0xFFFFFFFF)

/********************  Bits definition for RTC_BKP17R register  ***************/
#define RTC_BKP17R                           ((uint_fast32_t)0xFFFFFFFF)

/********************  Bits definition for RTC_BKP18R register  ***************/
#define RTC_BKP18R                           ((uint_fast32_t)0xFFFFFFFF)

/********************  Bits definition for RTC_BKP19R register  ***************/
#define RTC_BKP19R                           ((uint_fast32_t)0xFFFFFFFF)


/**
  * @brief Real-Time Clock
  */

typedef struct
{
  __IO uint_fast32_t TR;      /*!< RTC time register,                                        Address offset: 0x00 */
  __IO uint_fast32_t DR;      /*!< RTC date register,                                        Address offset: 0x04 */
  __IO uint_fast32_t CR;      /*!< RTC control register,                                     Address offset: 0x08 */
  __IO uint_fast32_t ISR;     /*!< RTC initialization and status register,                   Address offset: 0x0C */
  __IO uint_fast32_t PRER;    /*!< RTC prescaler register,                                   Address offset: 0x10 */
  __IO uint_fast32_t WUTR;    /*!< RTC wakeup timer register,                                Address offset: 0x14 */
  __IO uint_fast32_t CALIBR;  /*!< RTC calibration register,                                 Address offset: 0x18 */
  __IO uint_fast32_t ALRMAR;  /*!< RTC alarm A register,                                     Address offset: 0x1C */
  __IO uint_fast32_t ALRMBR;  /*!< RTC alarm B register,                                     Address offset: 0x20 */
  __IO uint_fast32_t WPR;     /*!< RTC write protection register,                            Address offset: 0x24 */
  __IO uint_fast32_t SSR;     /*!< RTC sub second register,                                  Address offset: 0x28 */
  __IO uint_fast32_t SHIFTR;  /*!< RTC shift control register,                               Address offset: 0x2C */
  __IO uint_fast32_t TSTR;    /*!< RTC time stamp time register,                             Address offset: 0x30 */
  __IO uint_fast32_t TSDR;    /*!< RTC time stamp date register,                             Address offset: 0x34 */
  __IO uint_fast32_t TSSSR;   /*!< RTC time-stamp sub second register,                       Address offset: 0x38 */
  __IO uint_fast32_t CALR;    /*!< RTC calibration register,                                 Address offset: 0x3C */
  __IO uint_fast32_t TAFCR;   /*!< RTC tamper and alternate function configuration register, Address offset: 0x40 */
  __IO uint_fast32_t ALRMASSR;/*!< RTC alarm A sub second register,                          Address offset: 0x44 */
  __IO uint_fast32_t ALRMBSSR;/*!< RTC alarm B sub second register,                          Address offset: 0x48 */
  uint_fast32_t RESERVED7;    /*!< Reserved, 0x4C                                                                 */
  __IO uint_fast32_t BKP0R;   /*!< RTC backup register 1,                                    Address offset: 0x50 */
  __IO uint_fast32_t BKP1R;   /*!< RTC backup register 1,                                    Address offset: 0x54 */
  __IO uint_fast32_t BKP2R;   /*!< RTC backup register 2,                                    Address offset: 0x58 */
  __IO uint_fast32_t BKP3R;   /*!< RTC backup register 3,                                    Address offset: 0x5C */
  __IO uint_fast32_t BKP4R;   /*!< RTC backup register 4,                                    Address offset: 0x60 */
  __IO uint_fast32_t BKP5R;   /*!< RTC backup register 5,                                    Address offset: 0x64 */
  __IO uint_fast32_t BKP6R;   /*!< RTC backup register 6,                                    Address offset: 0x68 */
  __IO uint_fast32_t BKP7R;   /*!< RTC backup register 7,                                    Address offset: 0x6C */
  __IO uint_fast32_t BKP8R;   /*!< RTC backup register 8,                                    Address offset: 0x70 */
  __IO uint_fast32_t BKP9R;   /*!< RTC backup register 9,                                    Address offset: 0x74 */
  __IO uint_fast32_t BKP10R;  /*!< RTC backup register 10,                                   Address offset: 0x78 */
  __IO uint_fast32_t BKP11R;  /*!< RTC backup register 11,                                   Address offset: 0x7C */
  __IO uint_fast32_t BKP12R;  /*!< RTC backup register 12,                                   Address offset: 0x80 */
  __IO uint_fast32_t BKP13R;  /*!< RTC backup register 13,                                   Address offset: 0x84 */
  __IO uint_fast32_t BKP14R;  /*!< RTC backup register 14,                                   Address offset: 0x88 */
  __IO uint_fast32_t BKP15R;  /*!< RTC backup register 15,                                   Address offset: 0x8C */
  __IO uint_fast32_t BKP16R;  /*!< RTC backup register 16,                                   Address offset: 0x90 */
  __IO uint_fast32_t BKP17R;  /*!< RTC backup register 17,                                   Address offset: 0x94 */
  __IO uint_fast32_t BKP18R;  /*!< RTC backup register 18,                                   Address offset: 0x98 */
  __IO uint_fast32_t BKP19R;  /*!< RTC backup register 19,                                   Address offset: 0x9C */
} RTC_TypeDef;

#define RTC_BASE              (APB1PERIPH_BASE + 0x2800)

#define RTC                 ((RTC_TypeDef *) RTC_BASE)

#endif /* HAL_ST_STM32F4_RTC_H_ */
