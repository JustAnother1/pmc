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

#ifndef HAL_ST_STM32F4_GPIO_H_
#define HAL_ST_STM32F4_GPIO_H_

#include "hal_common.h"

/******************************************************************************/
/*                                                                            */
/*                            General Purpose I/O                             */
/*                                                                            */
/******************************************************************************/
/******************  Bits definition for GPIO_MODER register  *****************/

#define GPIO_MODER_0_INPUT_0                 ((uint32_t)0x00000003)
#define GPIO_MODER_0_INPUT_1                 ((uint32_t)0x00000000)
#define GPIO_MODER_0_OUTPUT_0                ((uint32_t)0x00000002)
#define GPIO_MODER_0_OUTPUT_1                ((uint32_t)0x00000001)
#define GPIO_MODER_0_ALTFUNC_0               ((uint32_t)0x00000001)
#define GPIO_MODER_0_ALTFUNC_1               ((uint32_t)0x00000002)
#define GPIO_MODER_0_ANALOG_0                ((uint32_t)0x00000000)
#define GPIO_MODER_0_ANALOG_1                ((uint32_t)0x00000003)

#define GPIO_MODER_1_INPUT_0                 ((uint32_t)0x0000000c)
#define GPIO_MODER_1_INPUT_1                 ((uint32_t)0x00000000)
#define GPIO_MODER_1_OUTPUT_0                ((uint32_t)0x00000008)
#define GPIO_MODER_1_OUTPUT_1                ((uint32_t)0x00000004)
#define GPIO_MODER_1_ALTFUNC_0               ((uint32_t)0x00000004)
#define GPIO_MODER_1_ALTFUNC_1               ((uint32_t)0x00000008)
#define GPIO_MODER_1_ANALOG_0                ((uint32_t)0x00000000)
#define GPIO_MODER_1_ANALOG_1                ((uint32_t)0x0000000c)

#define GPIO_MODER_2_INPUT_0                 ((uint32_t)0x00000030)
#define GPIO_MODER_2_INPUT_1                 ((uint32_t)0x00000000)
#define GPIO_MODER_2_OUTPUT_0                ((uint32_t)0x00000020)
#define GPIO_MODER_2_OUTPUT_1                ((uint32_t)0x00000010)
#define GPIO_MODER_2_ALTFUNC_0               ((uint32_t)0x00000010)
#define GPIO_MODER_2_ALTFUNC_1               ((uint32_t)0x00000020)
#define GPIO_MODER_2_ANALOG_0                ((uint32_t)0x00000000)
#define GPIO_MODER_2_ANALOG_1                ((uint32_t)0x00000030)

#define GPIO_MODER_3_INPUT_0                 ((uint32_t)0x000000c0)
#define GPIO_MODER_3_INPUT_1                 ((uint32_t)0x00000000)
#define GPIO_MODER_3_OUTPUT_0                ((uint32_t)0x00000080)
#define GPIO_MODER_3_OUTPUT_1                ((uint32_t)0x00000040)
#define GPIO_MODER_3_ALTFUNC_0               ((uint32_t)0x00000040)
#define GPIO_MODER_3_ALTFUNC_1               ((uint32_t)0x00000080)
#define GPIO_MODER_3_ANALOG_0                ((uint32_t)0x00000000)
#define GPIO_MODER_3_ANALOG_1                ((uint32_t)0x000000c0)

#define GPIO_MODER_4_INPUT_0                 ((uint32_t)0x00000300)
#define GPIO_MODER_4_INPUT_1                 ((uint32_t)0x00000000)
#define GPIO_MODER_4_OUTPUT_0                ((uint32_t)0x00000200)
#define GPIO_MODER_4_OUTPUT_1                ((uint32_t)0x00000100)
#define GPIO_MODER_4_ALTFUNC_0               ((uint32_t)0x00000100)
#define GPIO_MODER_4_ALTFUNC_1               ((uint32_t)0x00000200)
#define GPIO_MODER_4_ANALOG_0                ((uint32_t)0x00000000)
#define GPIO_MODER_4_ANALOG_1                ((uint32_t)0x00000300)

#define GPIO_MODER_5_INPUT_0                 ((uint32_t)0x00000c00)
#define GPIO_MODER_5_INPUT_1                 ((uint32_t)0x00000000)
#define GPIO_MODER_5_OUTPUT_0                ((uint32_t)0x00000800)
#define GPIO_MODER_5_OUTPUT_1                ((uint32_t)0x00000400)
#define GPIO_MODER_5_ALTFUNC_0               ((uint32_t)0x00000400)
#define GPIO_MODER_5_ALTFUNC_1               ((uint32_t)0x00000800)
#define GPIO_MODER_5_ANALOG_0                ((uint32_t)0x00000000)
#define GPIO_MODER_5_ANALOG_1                ((uint32_t)0x00000c00)

#define GPIO_MODER_6_INPUT_0                 ((uint32_t)0x00003000)
#define GPIO_MODER_6_INPUT_1                 ((uint32_t)0x00000000)
#define GPIO_MODER_6_OUTPUT_0                ((uint32_t)0x00002000)
#define GPIO_MODER_6_OUTPUT_1                ((uint32_t)0x00001000)
#define GPIO_MODER_6_ALTFUNC_0               ((uint32_t)0x00001000)
#define GPIO_MODER_6_ALTFUNC_1               ((uint32_t)0x00002000)
#define GPIO_MODER_6_ANALOG_0                ((uint32_t)0x00000000)
#define GPIO_MODER_6_ANALOG_1                ((uint32_t)0x00003000)

#define GPIO_MODER_7_INPUT_0                 ((uint32_t)0x0000c000)
#define GPIO_MODER_7_INPUT_1                 ((uint32_t)0x00000000)
#define GPIO_MODER_7_OUTPUT_0                ((uint32_t)0x00008000)
#define GPIO_MODER_7_OUTPUT_1                ((uint32_t)0x00004000)
#define GPIO_MODER_7_ALTFUNC_0               ((uint32_t)0x00004000)
#define GPIO_MODER_7_ALTFUNC_1               ((uint32_t)0x00008000)
#define GPIO_MODER_7_ANALOG_0                ((uint32_t)0x00000000)
#define GPIO_MODER_7_ANALOG_1                ((uint32_t)0x0000c000)

#define GPIO_MODER_8_INPUT_0                 ((uint32_t)0x00030000)
#define GPIO_MODER_8_INPUT_1                 ((uint32_t)0x00000000)
#define GPIO_MODER_8_OUTPUT_0                ((uint32_t)0x00020000)
#define GPIO_MODER_8_OUTPUT_1                ((uint32_t)0x00010000)
#define GPIO_MODER_8_ALTFUNC_0               ((uint32_t)0x00010000)
#define GPIO_MODER_8_ALTFUNC_1               ((uint32_t)0x00020000)
#define GPIO_MODER_8_ANALOG_0                ((uint32_t)0x00000000)
#define GPIO_MODER_8_ANALOG_1                ((uint32_t)0x00030000)

#define GPIO_MODER_9_INPUT_0                 ((uint32_t)0x000c0000)
#define GPIO_MODER_9_INPUT_1                 ((uint32_t)0x00000000)
#define GPIO_MODER_9_OUTPUT_0                ((uint32_t)0x00080000)
#define GPIO_MODER_9_OUTPUT_1                ((uint32_t)0x00040000)
#define GPIO_MODER_9_ALTFUNC_0               ((uint32_t)0x00040000)
#define GPIO_MODER_9_ALTFUNC_1               ((uint32_t)0x00080000)
#define GPIO_MODER_9_ANALOG_0                ((uint32_t)0x00000000)
#define GPIO_MODER_9_ANALOG_1                ((uint32_t)0x000c0000)

#define GPIO_MODER_10_INPUT_0                ((uint32_t)0x00300000)
#define GPIO_MODER_10_INPUT_1                ((uint32_t)0x00000000)
#define GPIO_MODER_10_OUTPUT_0               ((uint32_t)0x00200000)
#define GPIO_MODER_10_OUTPUT_1               ((uint32_t)0x00100000)
#define GPIO_MODER_10_ALTFUNC_0              ((uint32_t)0x00100000)
#define GPIO_MODER_10_ALTFUNC_1              ((uint32_t)0x00200000)
#define GPIO_MODER_10_ANALOG_0               ((uint32_t)0x00000000)
#define GPIO_MODER_10_ANALOG_1               ((uint32_t)0x00300000)

#define GPIO_MODER_11_INPUT_0                ((uint32_t)0x00c00000)
#define GPIO_MODER_11_INPUT_1                ((uint32_t)0x00000000)
#define GPIO_MODER_11_OUTPUT_0               ((uint32_t)0x00800000)
#define GPIO_MODER_11_OUTPUT_1               ((uint32_t)0x00400000)
#define GPIO_MODER_11_ALTFUNC_0              ((uint32_t)0x00400000)
#define GPIO_MODER_11_ALTFUNC_1              ((uint32_t)0x00800000)
#define GPIO_MODER_11_ANALOG_0               ((uint32_t)0x00000000)
#define GPIO_MODER_11_ANALOG_1               ((uint32_t)0x00c00000)

#define GPIO_MODER_12_INPUT_0                ((uint32_t)0x03000000)
#define GPIO_MODER_12_INPUT_1                ((uint32_t)0x00000000)
#define GPIO_MODER_12_OUTPUT_0               ((uint32_t)0x02000000)
#define GPIO_MODER_12_OUTPUT_1               ((uint32_t)0x01000000)
#define GPIO_MODER_12_ALTFUNC_0              ((uint32_t)0x01000000)
#define GPIO_MODER_12_ALTFUNC_1              ((uint32_t)0x02000000)
#define GPIO_MODER_12_ANALOG_0               ((uint32_t)0x00000000)
#define GPIO_MODER_12_ANALOG_1               ((uint32_t)0x03000000)

#define GPIO_MODER_13_INPUT_0                ((uint32_t)0x0c000000)
#define GPIO_MODER_13_INPUT_1                ((uint32_t)0x00000000)
#define GPIO_MODER_13_OUTPUT_0               ((uint32_t)0x08000000)
#define GPIO_MODER_13_OUTPUT_1               ((uint32_t)0x04000000)
#define GPIO_MODER_13_ALTFUNC_0              ((uint32_t)0x04000000)
#define GPIO_MODER_13_ALTFUNC_1              ((uint32_t)0x08000000)
#define GPIO_MODER_13_ANALOG_0               ((uint32_t)0x00000000)
#define GPIO_MODER_13_ANALOG_1               ((uint32_t)0x0c000000)

#define GPIO_MODER_14_INPUT_0                ((uint32_t)0x30000000)
#define GPIO_MODER_14_INPUT_1                ((uint32_t)0x00000000)
#define GPIO_MODER_14_OUTPUT_0               ((uint32_t)0x20000000)
#define GPIO_MODER_14_OUTPUT_1               ((uint32_t)0x10000000)
#define GPIO_MODER_14_ALTFUNC_0              ((uint32_t)0x10000000)
#define GPIO_MODER_14_ALTFUNC_1              ((uint32_t)0x20000000)
#define GPIO_MODER_14_ANALOG_0               ((uint32_t)0x00000000)
#define GPIO_MODER_14_ANALOG_1               ((uint32_t)0x30000000)

#define GPIO_MODER_15_INPUT_0                ((uint32_t)0xc0000000)
#define GPIO_MODER_15_INPUT_1                ((uint32_t)0x00000000)
#define GPIO_MODER_15_OUTPUT_0               ((uint32_t)0x80000000)
#define GPIO_MODER_15_OUTPUT_1               ((uint32_t)0x40000000)
#define GPIO_MODER_15_ALTFUNC_0              ((uint32_t)0x40000000)
#define GPIO_MODER_15_ALTFUNC_1              ((uint32_t)0x80000000)
#define GPIO_MODER_15_ANALOG_0               ((uint32_t)0x00000000)
#define GPIO_MODER_15_ANALOG_1               ((uint32_t)0xc0000000)

/******************  Bits definition for GPIO_OTYPER register  ****************/

#define GPIO_OTYPER_0_PUSH_PULL_0            ((uint32_t)0x00000001)
#define GPIO_OTYPER_0_PUSH_PULL_1            ((uint32_t)0x00000000)
#define GPIO_OTYPER_0_OPEN_DRAIN_0           ((uint32_t)0x00000000)
#define GPIO_OTYPER_0_OPEN_DRAIN_1           ((uint32_t)0x00000001)

#define GPIO_OTYPER_1_PUSH_PULL_0            ((uint32_t)0x00000002)
#define GPIO_OTYPER_1_PUSH_PULL_1            ((uint32_t)0x00000000)
#define GPIO_OTYPER_1_OPEN_DRAIN_0           ((uint32_t)0x00000000)
#define GPIO_OTYPER_1_OPEN_DRAIN_1           ((uint32_t)0x00000002)

#define GPIO_OTYPER_2_PUSH_PULL_0            ((uint32_t)0x00000004)
#define GPIO_OTYPER_2_PUSH_PULL_1            ((uint32_t)0x00000000)
#define GPIO_OTYPER_2_OPEN_DRAIN_0           ((uint32_t)0x00000000)
#define GPIO_OTYPER_2_OPEN_DRAIN_1           ((uint32_t)0x00000004)

#define GPIO_OTYPER_3_PUSH_PULL_0            ((uint32_t)0x00000008)
#define GPIO_OTYPER_3_PUSH_PULL_1            ((uint32_t)0x00000000)
#define GPIO_OTYPER_3_OPEN_DRAIN_0           ((uint32_t)0x00000000)
#define GPIO_OTYPER_3_OPEN_DRAIN_1           ((uint32_t)0x00000008)

#define GPIO_OTYPER_4_PUSH_PULL_0            ((uint32_t)0x00000010)
#define GPIO_OTYPER_4_PUSH_PULL_1            ((uint32_t)0x00000000)
#define GPIO_OTYPER_4_OPEN_DRAIN_0           ((uint32_t)0x00000000)
#define GPIO_OTYPER_4_OPEN_DRAIN_1           ((uint32_t)0x00000010)

#define GPIO_OTYPER_5_PUSH_PULL_0            ((uint32_t)0x00000020)
#define GPIO_OTYPER_5_PUSH_PULL_1            ((uint32_t)0x00000000)
#define GPIO_OTYPER_5_OPEN_DRAIN_0           ((uint32_t)0x00000000)
#define GPIO_OTYPER_5_OPEN_DRAIN_1           ((uint32_t)0x00000020)

#define GPIO_OTYPER_6_PUSH_PULL_0            ((uint32_t)0x00000040)
#define GPIO_OTYPER_6_PUSH_PULL_1            ((uint32_t)0x00000000)
#define GPIO_OTYPER_6_OPEN_DRAIN_0           ((uint32_t)0x00000000)
#define GPIO_OTYPER_6_OPEN_DRAIN_1           ((uint32_t)0x00000040)

#define GPIO_OTYPER_7_PUSH_PULL_0            ((uint32_t)0x00000080)
#define GPIO_OTYPER_7_PUSH_PULL_1            ((uint32_t)0x00000000)
#define GPIO_OTYPER_7_OPEN_DRAIN_0           ((uint32_t)0x00000000)
#define GPIO_OTYPER_7_OPEN_DRAIN_1           ((uint32_t)0x00000080)

#define GPIO_OTYPER_8_PUSH_PULL_0            ((uint32_t)0x00000100)
#define GPIO_OTYPER_8_PUSH_PULL_1            ((uint32_t)0x00000000)
#define GPIO_OTYPER_8_OPEN_DRAIN_0           ((uint32_t)0x00000000)
#define GPIO_OTYPER_8_OPEN_DRAIN_1           ((uint32_t)0x00000100)

#define GPIO_OTYPER_9_PUSH_PULL_0            ((uint32_t)0x00000200)
#define GPIO_OTYPER_9_PUSH_PULL_1            ((uint32_t)0x00000000)
#define GPIO_OTYPER_9_OPEN_DRAIN_0           ((uint32_t)0x00000000)
#define GPIO_OTYPER_9_OPEN_DRAIN_1           ((uint32_t)0x00000200)

#define GPIO_OTYPER_10_PUSH_PULL_0           ((uint32_t)0x00000400)
#define GPIO_OTYPER_10_PUSH_PULL_1           ((uint32_t)0x00000000)
#define GPIO_OTYPER_10_OPEN_DRAIN_0          ((uint32_t)0x00000000)
#define GPIO_OTYPER_10_OPEN_DRAIN_1          ((uint32_t)0x00000400)

#define GPIO_OTYPER_11_PUSH_PULL_0           ((uint32_t)0x00000800)
#define GPIO_OTYPER_11_PUSH_PULL_1           ((uint32_t)0x00000000)
#define GPIO_OTYPER_11_OPEN_DRAIN_0          ((uint32_t)0x00000000)
#define GPIO_OTYPER_11_OPEN_DRAIN_1          ((uint32_t)0x00000800)

#define GPIO_OTYPER_12_PUSH_PULL_0           ((uint32_t)0x00001000)
#define GPIO_OTYPER_12_PUSH_PULL_1           ((uint32_t)0x00000000)
#define GPIO_OTYPER_12_OPEN_DRAIN_0          ((uint32_t)0x00000000)
#define GPIO_OTYPER_12_OPEN_DRAIN_1          ((uint32_t)0x00001000)

#define GPIO_OTYPER_13_PUSH_PULL_0           ((uint32_t)0x00002000)
#define GPIO_OTYPER_13_PUSH_PULL_1           ((uint32_t)0x00000000)
#define GPIO_OTYPER_13_OPEN_DRAIN_0          ((uint32_t)0x00000000)
#define GPIO_OTYPER_13_OPEN_DRAIN_1          ((uint32_t)0x00002000)

#define GPIO_OTYPER_14_PUSH_PULL_0           ((uint32_t)0x00004000)
#define GPIO_OTYPER_14_PUSH_PULL_1           ((uint32_t)0x00000000)
#define GPIO_OTYPER_14_OPEN_DRAIN_0          ((uint32_t)0x00000000)
#define GPIO_OTYPER_14_OPEN_DRAIN_1          ((uint32_t)0x00004000)

#define GPIO_OTYPER_15_PUSH_PULL_0           ((uint32_t)0x00008000)
#define GPIO_OTYPER_15_PUSH_PULL_1           ((uint32_t)0x00000000)
#define GPIO_OTYPER_15_OPEN_DRAIN_0          ((uint32_t)0x00000000)
#define GPIO_OTYPER_15_OPEN_DRAIN_1          ((uint32_t)0x00008000)


/******************  Bits definition for GPIO_OSPEEDR register  ***************/
#define GPIO_OSPEEDER_0_LOW_SPEED_0          ((uint32_t)0x00000003)
#define GPIO_OSPEEDER_0_LOW_SPEED_1          ((uint32_t)0x00000000)
#define GPIO_OSPEEDER_0_MEDIUM_SPEED_0       ((uint32_t)0x00000002)
#define GPIO_OSPEEDER_0_MEDIUM_SPEED_1       ((uint32_t)0x00000001)
#define GPIO_OSPEEDER_0_FAST_SPEED_0         ((uint32_t)0x00000001)
#define GPIO_OSPEEDER_0_FAST_SPEED_1         ((uint32_t)0x00000002)
#define GPIO_OSPEEDER_0_HIGH_SPEED_0         ((uint32_t)0x00000000)
#define GPIO_OSPEEDER_0_HIGH_SPEED_1         ((uint32_t)0x00000003)

#define GPIO_OSPEEDER_1_LOW_SPEED_0          ((uint32_t)0x0000000c)
#define GPIO_OSPEEDER_1_LOW_SPEED_1          ((uint32_t)0x00000000)
#define GPIO_OSPEEDER_1_MEDIUM_SPEED_0       ((uint32_t)0x00000008)
#define GPIO_OSPEEDER_1_MEDIUM_SPEED_1       ((uint32_t)0x00000004)
#define GPIO_OSPEEDER_1_FAST_SPEED_0         ((uint32_t)0x00000004)
#define GPIO_OSPEEDER_1_FAST_SPEED_1         ((uint32_t)0x00000008)
#define GPIO_OSPEEDER_1_HIGH_SPEED_0         ((uint32_t)0x00000000)
#define GPIO_OSPEEDER_1_HIGH_SPEED_1         ((uint32_t)0x0000000c)

#define GPIO_OSPEEDER_2_LOW_SPEED_0          ((uint32_t)0x00000030)
#define GPIO_OSPEEDER_2_LOW_SPEED_1          ((uint32_t)0x00000000)
#define GPIO_OSPEEDER_2_MEDIUM_SPEED_0       ((uint32_t)0x00000020)
#define GPIO_OSPEEDER_2_MEDIUM_SPEED_1       ((uint32_t)0x00000010)
#define GPIO_OSPEEDER_2_FAST_SPEED_0         ((uint32_t)0x00000010)
#define GPIO_OSPEEDER_2_FAST_SPEED_1         ((uint32_t)0x00000020)
#define GPIO_OSPEEDER_2_HIGH_SPEED_0         ((uint32_t)0x00000000)
#define GPIO_OSPEEDER_2_HIGH_SPEED_1         ((uint32_t)0x00000030)

#define GPIO_OSPEEDER_3_LOW_SPEED_0          ((uint32_t)0x000000c0)
#define GPIO_OSPEEDER_3_LOW_SPEED_1          ((uint32_t)0x00000000)
#define GPIO_OSPEEDER_3_MEDIUM_SPEED_0       ((uint32_t)0x00000080)
#define GPIO_OSPEEDER_3_MEDIUM_SPEED_1       ((uint32_t)0x00000040)
#define GPIO_OSPEEDER_3_FAST_SPEED_0         ((uint32_t)0x00000040)
#define GPIO_OSPEEDER_3_FAST_SPEED_1         ((uint32_t)0x00000080)
#define GPIO_OSPEEDER_3_HIGH_SPEED_0         ((uint32_t)0x00000000)
#define GPIO_OSPEEDER_3_HIGH_SPEED_1         ((uint32_t)0x000000c0)

#define GPIO_OSPEEDER_4_LOW_SPEED_0          ((uint32_t)0x00000300)
#define GPIO_OSPEEDER_4_LOW_SPEED_1          ((uint32_t)0x00000000)
#define GPIO_OSPEEDER_4_MEDIUM_SPEED_0       ((uint32_t)0x00000200)
#define GPIO_OSPEEDER_4_MEDIUM_SPEED_1       ((uint32_t)0x00000100)
#define GPIO_OSPEEDER_4_FAST_SPEED_0         ((uint32_t)0x00000100)
#define GPIO_OSPEEDER_4_FAST_SPEED_1         ((uint32_t)0x00000200)
#define GPIO_OSPEEDER_4_HIGH_SPEED_0         ((uint32_t)0x00000000)
#define GPIO_OSPEEDER_4_HIGH_SPEED_1         ((uint32_t)0x00000300)

#define GPIO_OSPEEDER_5_LOW_SPEED_0          ((uint32_t)0x00000c00)
#define GPIO_OSPEEDER_5_LOW_SPEED_1          ((uint32_t)0x00000000)
#define GPIO_OSPEEDER_5_MEDIUM_SPEED_0       ((uint32_t)0x00000800)
#define GPIO_OSPEEDER_5_MEDIUM_SPEED_1       ((uint32_t)0x00000400)
#define GPIO_OSPEEDER_5_FAST_SPEED_0         ((uint32_t)0x00000400)
#define GPIO_OSPEEDER_5_FAST_SPEED_1         ((uint32_t)0x00000800)
#define GPIO_OSPEEDER_5_HIGH_SPEED_0         ((uint32_t)0x00000000)
#define GPIO_OSPEEDER_5_HIGH_SPEED_1         ((uint32_t)0x00000c00)

#define GPIO_OSPEEDER_6_LOW_SPEED_0          ((uint32_t)0x00003000)
#define GPIO_OSPEEDER_6_LOW_SPEED_1          ((uint32_t)0x00000000)
#define GPIO_OSPEEDER_6_MEDIUM_SPEED_0       ((uint32_t)0x00002000)
#define GPIO_OSPEEDER_6_MEDIUM_SPEED_1       ((uint32_t)0x00001000)
#define GPIO_OSPEEDER_6_FAST_SPEED_0         ((uint32_t)0x00001000)
#define GPIO_OSPEEDER_6_FAST_SPEED_1         ((uint32_t)0x00002000)
#define GPIO_OSPEEDER_6_HIGH_SPEED_0         ((uint32_t)0x00000000)
#define GPIO_OSPEEDER_6_HIGH_SPEED_1         ((uint32_t)0x00003000)

#define GPIO_OSPEEDER_7_LOW_SPEED_0          ((uint32_t)0x0000c000)
#define GPIO_OSPEEDER_7_LOW_SPEED_1          ((uint32_t)0x00000000)
#define GPIO_OSPEEDER_7_MEDIUM_SPEED_0       ((uint32_t)0x00008000)
#define GPIO_OSPEEDER_7_MEDIUM_SPEED_1       ((uint32_t)0x00004000)
#define GPIO_OSPEEDER_7_FAST_SPEED_0         ((uint32_t)0x00004000)
#define GPIO_OSPEEDER_7_FAST_SPEED_1         ((uint32_t)0x00008000)
#define GPIO_OSPEEDER_7_HIGH_SPEED_0         ((uint32_t)0x00000000)
#define GPIO_OSPEEDER_7_HIGH_SPEED_1         ((uint32_t)0x0000c000)

#define GPIO_OSPEEDER_8_LOW_SPEED_0          ((uint32_t)0x00030000)
#define GPIO_OSPEEDER_8_LOW_SPEED_1          ((uint32_t)0x00000000)
#define GPIO_OSPEEDER_8_MEDIUM_SPEED_0       ((uint32_t)0x00020000)
#define GPIO_OSPEEDER_8_MEDIUM_SPEED_1       ((uint32_t)0x00010000)
#define GPIO_OSPEEDER_8_FAST_SPEED_0         ((uint32_t)0x00010000)
#define GPIO_OSPEEDER_8_FAST_SPEED_1         ((uint32_t)0x00020000)
#define GPIO_OSPEEDER_8_HIGH_SPEED_0         ((uint32_t)0x00000000)
#define GPIO_OSPEEDER_8_HIGH_SPEED_1         ((uint32_t)0x00030000)

#define GPIO_OSPEEDER_9_LOW_SPEED_0          ((uint32_t)0x000c0000)
#define GPIO_OSPEEDER_9_LOW_SPEED_1          ((uint32_t)0x00000000)
#define GPIO_OSPEEDER_9_MEDIUM_SPEED_0       ((uint32_t)0x00080000)
#define GPIO_OSPEEDER_9_MEDIUM_SPEED_1       ((uint32_t)0x00040000)
#define GPIO_OSPEEDER_9_FAST_SPEED_0         ((uint32_t)0x00040000)
#define GPIO_OSPEEDER_9_FAST_SPEED_1         ((uint32_t)0x00080000)
#define GPIO_OSPEEDER_9_HIGH_SPEED_0         ((uint32_t)0x00000000)
#define GPIO_OSPEEDER_9_HIGH_SPEED_1         ((uint32_t)0x000c0000)

#define GPIO_OSPEEDER_10_LOW_SPEED_0         ((uint32_t)0x00300000)
#define GPIO_OSPEEDER_10_LOW_SPEED_1         ((uint32_t)0x00000000)
#define GPIO_OSPEEDER_10_MEDIUM_SPEED_0      ((uint32_t)0x00200000)
#define GPIO_OSPEEDER_10_MEDIUM_SPEED_1      ((uint32_t)0x00100000)
#define GPIO_OSPEEDER_10_FAST_SPEED_0        ((uint32_t)0x00100000)
#define GPIO_OSPEEDER_10_FAST_SPEED_1        ((uint32_t)0x00200000)
#define GPIO_OSPEEDER_10_HIGH_SPEED_0        ((uint32_t)0x00000000)
#define GPIO_OSPEEDER_10_HIGH_SPEED_1        ((uint32_t)0x00300000)

#define GPIO_OSPEEDER_11_LOW_SPEED_0         ((uint32_t)0x00c00000)
#define GPIO_OSPEEDER_11_LOW_SPEED_1         ((uint32_t)0x00000000)
#define GPIO_OSPEEDER_11_MEDIUM_SPEED_0      ((uint32_t)0x00800000)
#define GPIO_OSPEEDER_11_MEDIUM_SPEED_1      ((uint32_t)0x00400000)
#define GPIO_OSPEEDER_11_FAST_SPEED_0        ((uint32_t)0x00400000)
#define GPIO_OSPEEDER_11_FAST_SPEED_1        ((uint32_t)0x00800000)
#define GPIO_OSPEEDER_11_HIGH_SPEED_0        ((uint32_t)0x00000000)
#define GPIO_OSPEEDER_11_HIGH_SPEED_1        ((uint32_t)0x00c00000)

#define GPIO_OSPEEDER_12_LOW_SPEED_0         ((uint32_t)0x03000000)
#define GPIO_OSPEEDER_12_LOW_SPEED_1         ((uint32_t)0x00000000)
#define GPIO_OSPEEDER_12_MEDIUM_SPEED_0      ((uint32_t)0x02000000)
#define GPIO_OSPEEDER_12_MEDIUM_SPEED_1      ((uint32_t)0x01000000)
#define GPIO_OSPEEDER_12_FAST_SPEED_0        ((uint32_t)0x01000000)
#define GPIO_OSPEEDER_12_FAST_SPEED_1        ((uint32_t)0x02000000)
#define GPIO_OSPEEDER_12_HIGH_SPEED_0        ((uint32_t)0x00000000)
#define GPIO_OSPEEDER_12_HIGH_SPEED_1        ((uint32_t)0x03000000)

#define GPIO_OSPEEDER_13_LOW_SPEED_0         ((uint32_t)0x0c000000)
#define GPIO_OSPEEDER_13_LOW_SPEED_1         ((uint32_t)0x00000000)
#define GPIO_OSPEEDER_13_MEDIUM_SPEED_0      ((uint32_t)0x08000000)
#define GPIO_OSPEEDER_13_MEDIUM_SPEED_1      ((uint32_t)0x04000000)
#define GPIO_OSPEEDER_13_FAST_SPEED_0        ((uint32_t)0x04000000)
#define GPIO_OSPEEDER_13_FAST_SPEED_1        ((uint32_t)0x08000000)
#define GPIO_OSPEEDER_13_HIGH_SPEED_0        ((uint32_t)0x00000000)
#define GPIO_OSPEEDER_13_HIGH_SPEED_1        ((uint32_t)0x0c000000)

#define GPIO_OSPEEDER_14_LOW_SPEED_0         ((uint32_t)0x30000000)
#define GPIO_OSPEEDER_14_LOW_SPEED_1         ((uint32_t)0x00000000)
#define GPIO_OSPEEDER_14_MEDIUM_SPEED_0      ((uint32_t)0x20000000)
#define GPIO_OSPEEDER_14_MEDIUM_SPEED_1      ((uint32_t)0x10000000)
#define GPIO_OSPEEDER_14_FAST_SPEED_0        ((uint32_t)0x10000000)
#define GPIO_OSPEEDER_14_FAST_SPEED_1        ((uint32_t)0x20000000)
#define GPIO_OSPEEDER_14_HIGH_SPEED_0        ((uint32_t)0x00000000)
#define GPIO_OSPEEDER_14_HIGH_SPEED_1        ((uint32_t)0x30000000)

#define GPIO_OSPEEDER_15_LOW_SPEED_0         ((uint32_t)0xc0000000)
#define GPIO_OSPEEDER_15_LOW_SPEED_1         ((uint32_t)0x00000000)
#define GPIO_OSPEEDER_15_MEDIUM_SPEED_0      ((uint32_t)0x80000000)
#define GPIO_OSPEEDER_15_MEDIUM_SPEED_1      ((uint32_t)0x40000000)
#define GPIO_OSPEEDER_15_FAST_SPEED_0        ((uint32_t)0x40000000)
#define GPIO_OSPEEDER_15_FAST_SPEED_1        ((uint32_t)0x80000000)
#define GPIO_OSPEEDER_15_HIGH_SPEED_0        ((uint32_t)0x00000000)
#define GPIO_OSPEEDER_15_HIGH_SPEED_1        ((uint32_t)0xc0000000)

/******************  Bits definition for GPIO_PUPDR register  *****************/
#define GPIO_PUPDR_0_NONE_0                  ((uint32_t)0x00000003)
#define GPIO_PUPDR_0_NONE_1                  ((uint32_t)0x00000000)
#define GPIO_PUPDR_0_PULL_UP_0               ((uint32_t)0x00000002)
#define GPIO_PUPDR_0_PULL_UP_1               ((uint32_t)0x00000001)
#define GPIO_PUPDR_0_PULL_DOWN_0             ((uint32_t)0x00000001)
#define GPIO_PUPDR_0_PULL_DOWN_1             ((uint32_t)0x00000002)

#define GPIO_PUPDR_1_NONE_0                  ((uint32_t)0x0000000c)
#define GPIO_PUPDR_1_NONE_1                  ((uint32_t)0x00000000)
#define GPIO_PUPDR_1_PULL_UP_0               ((uint32_t)0x00000008)
#define GPIO_PUPDR_1_PULL_UP_1               ((uint32_t)0x00000004)
#define GPIO_PUPDR_1_PULL_DOWN_0             ((uint32_t)0x00000004)
#define GPIO_PUPDR_1_PULL_DOWN_1             ((uint32_t)0x00000008)

#define GPIO_PUPDR_2_NONE_0                  ((uint32_t)0x00000030)
#define GPIO_PUPDR_2_NONE_1                  ((uint32_t)0x00000000)
#define GPIO_PUPDR_2_PULL_UP_0               ((uint32_t)0x00000020)
#define GPIO_PUPDR_2_PULL_UP_1               ((uint32_t)0x00000010)
#define GPIO_PUPDR_2_PULL_DOWN_0             ((uint32_t)0x00000010)
#define GPIO_PUPDR_2_PULL_DOWN_1             ((uint32_t)0x00000020)

#define GPIO_PUPDR_3_NONE_0                  ((uint32_t)0x000000c0)
#define GPIO_PUPDR_3_NONE_1                  ((uint32_t)0x00000000)
#define GPIO_PUPDR_3_PULL_UP_0               ((uint32_t)0x00000080)
#define GPIO_PUPDR_3_PULL_UP_1               ((uint32_t)0x00000040)
#define GPIO_PUPDR_3_PULL_DOWN_0             ((uint32_t)0x00000040)
#define GPIO_PUPDR_3_PULL_DOWN_1             ((uint32_t)0x00000080)

#define GPIO_PUPDR_4_NONE_0                  ((uint32_t)0x00000300)
#define GPIO_PUPDR_4_NONE_1                  ((uint32_t)0x00000000)
#define GPIO_PUPDR_4_PULL_UP_0               ((uint32_t)0x00000200)
#define GPIO_PUPDR_4_PULL_UP_1               ((uint32_t)0x00000100)
#define GPIO_PUPDR_4_PULL_DOWN_0             ((uint32_t)0x00000100)
#define GPIO_PUPDR_4_PULL_DOWN_1             ((uint32_t)0x00000200)

#define GPIO_PUPDR_5_NONE_0                  ((uint32_t)0x00000c00)
#define GPIO_PUPDR_5_NONE_1                  ((uint32_t)0x00000000)
#define GPIO_PUPDR_5_PULL_UP_0               ((uint32_t)0x00000800)
#define GPIO_PUPDR_5_PULL_UP_1               ((uint32_t)0x00000400)
#define GPIO_PUPDR_5_PULL_DOWN_0             ((uint32_t)0x00000400)
#define GPIO_PUPDR_5_PULL_DOWN_1             ((uint32_t)0x00000800)

#define GPIO_PUPDR_6_NONE_0                  ((uint32_t)0x00003000)
#define GPIO_PUPDR_6_NONE_1                  ((uint32_t)0x00000000)
#define GPIO_PUPDR_6_PULL_UP_0               ((uint32_t)0x00002000)
#define GPIO_PUPDR_6_PULL_UP_1               ((uint32_t)0x00001000)
#define GPIO_PUPDR_6_PULL_DOWN_0             ((uint32_t)0x00001000)
#define GPIO_PUPDR_6_PULL_DOWN_1             ((uint32_t)0x00002000)

#define GPIO_PUPDR_7_NONE_0                  ((uint32_t)0x0000c000)
#define GPIO_PUPDR_7_NONE_1                  ((uint32_t)0x00000000)
#define GPIO_PUPDR_7_PULL_UP_0               ((uint32_t)0x00008000)
#define GPIO_PUPDR_7_PULL_UP_1               ((uint32_t)0x00004000)
#define GPIO_PUPDR_7_PULL_DOWN_0             ((uint32_t)0x00004000)
#define GPIO_PUPDR_7_PULL_DOWN_1             ((uint32_t)0x00008000)

#define GPIO_PUPDR_8_NONE_0                  ((uint32_t)0x00030000)
#define GPIO_PUPDR_8_NONE_1                  ((uint32_t)0x00000000)
#define GPIO_PUPDR_8_PULL_UP_0               ((uint32_t)0x00020000)
#define GPIO_PUPDR_8_PULL_UP_1               ((uint32_t)0x00010000)
#define GPIO_PUPDR_8_PULL_DOWN_0             ((uint32_t)0x00010000)
#define GPIO_PUPDR_8_PULL_DOWN_1             ((uint32_t)0x00020000)

#define GPIO_PUPDR_9_NONE_0                  ((uint32_t)0x000c0000)
#define GPIO_PUPDR_9_NONE_1                  ((uint32_t)0x00000000)
#define GPIO_PUPDR_9_PULL_UP_0               ((uint32_t)0x00080000)
#define GPIO_PUPDR_9_PULL_UP_1               ((uint32_t)0x00040000)
#define GPIO_PUPDR_9_PULL_DOWN_0             ((uint32_t)0x00040000)
#define GPIO_PUPDR_9_PULL_DOWN_1             ((uint32_t)0x00080000)

#define GPIO_PUPDR_10_NONE_0                 ((uint32_t)0x00300000)
#define GPIO_PUPDR_10_NONE_1                 ((uint32_t)0x00000000)
#define GPIO_PUPDR_10_PULL_UP_0              ((uint32_t)0x00200000)
#define GPIO_PUPDR_10_PULL_UP_1              ((uint32_t)0x00100000)
#define GPIO_PUPDR_10_PULL_DOWN_0            ((uint32_t)0x00100000)
#define GPIO_PUPDR_10_PULL_DOWN_1            ((uint32_t)0x00200000)

#define GPIO_PUPDR_11_NONE_0                 ((uint32_t)0x00c00000)
#define GPIO_PUPDR_11_NONE_1                 ((uint32_t)0x00000000)
#define GPIO_PUPDR_11_PULL_UP_0              ((uint32_t)0x00800000)
#define GPIO_PUPDR_11_PULL_UP_1              ((uint32_t)0x00400000)
#define GPIO_PUPDR_11_PULL_DOWN_0            ((uint32_t)0x00400000)
#define GPIO_PUPDR_11_PULL_DOWN_1            ((uint32_t)0x00800000)

#define GPIO_PUPDR_12_NONE_0                 ((uint32_t)0x03000000)
#define GPIO_PUPDR_12_NONE_1                 ((uint32_t)0x00000000)
#define GPIO_PUPDR_12_PULL_UP_0              ((uint32_t)0x02000000)
#define GPIO_PUPDR_12_PULL_UP_1              ((uint32_t)0x01000000)
#define GPIO_PUPDR_12_PULL_DOWN_0            ((uint32_t)0x01000000)
#define GPIO_PUPDR_12_PULL_DOWN_1            ((uint32_t)0x02000000)

#define GPIO_PUPDR_13_NONE_0                 ((uint32_t)0x0c000000)
#define GPIO_PUPDR_13_NONE_1                 ((uint32_t)0x00000000)
#define GPIO_PUPDR_13_PULL_UP_0              ((uint32_t)0x08000000)
#define GPIO_PUPDR_13_PULL_UP_1              ((uint32_t)0x04000000)
#define GPIO_PUPDR_13_PULL_DOWN_0            ((uint32_t)0x04000000)
#define GPIO_PUPDR_13_PULL_DOWN_1            ((uint32_t)0x08000000)

#define GPIO_PUPDR_14_NONE_0                 ((uint32_t)0x30000000)
#define GPIO_PUPDR_14_NONE_1                 ((uint32_t)0x00000000)
#define GPIO_PUPDR_14_PULL_UP_0              ((uint32_t)0x20000000)
#define GPIO_PUPDR_14_PULL_UP_1              ((uint32_t)0x10000000)
#define GPIO_PUPDR_14_PULL_DOWN_0            ((uint32_t)0x10000000)
#define GPIO_PUPDR_14_PULL_DOWN_1            ((uint32_t)0x20000000)

#define GPIO_PUPDR_15_NONE_0                 ((uint32_t)0xc0000000)
#define GPIO_PUPDR_15_NONE_1                 ((uint32_t)0x00000000)
#define GPIO_PUPDR_15_PULL_UP_0              ((uint32_t)0x80000000)
#define GPIO_PUPDR_15_PULL_UP_1              ((uint32_t)0x40000000)
#define GPIO_PUPDR_15_PULL_DOWN_0            ((uint32_t)0x40000000)
#define GPIO_PUPDR_15_PULL_DOWN_1            ((uint32_t)0x80000000)

/******************  Bits definition for GPIO_IDR register  *******************/
#define GPIO_IDR_0                           ((uint32_t)0x00000001)
#define GPIO_IDR_1                           ((uint32_t)0x00000002)
#define GPIO_IDR_2                           ((uint32_t)0x00000004)
#define GPIO_IDR_3                           ((uint32_t)0x00000008)
#define GPIO_IDR_4                           ((uint32_t)0x00000010)
#define GPIO_IDR_5                           ((uint32_t)0x00000020)
#define GPIO_IDR_6                           ((uint32_t)0x00000040)
#define GPIO_IDR_7                           ((uint32_t)0x00000080)
#define GPIO_IDR_8                           ((uint32_t)0x00000100)
#define GPIO_IDR_9                           ((uint32_t)0x00000200)
#define GPIO_IDR_10                          ((uint32_t)0x00000400)
#define GPIO_IDR_11                          ((uint32_t)0x00000800)
#define GPIO_IDR_12                          ((uint32_t)0x00001000)
#define GPIO_IDR_13                          ((uint32_t)0x00002000)
#define GPIO_IDR_14                          ((uint32_t)0x00004000)
#define GPIO_IDR_15                          ((uint32_t)0x00008000)

/******************  Bits definition for GPIO_ODR register  *******************/
#define GPIO_ODR_0                           ((uint32_t)0x00000001)
#define GPIO_ODR_1                           ((uint32_t)0x00000002)
#define GPIO_ODR_2                           ((uint32_t)0x00000004)
#define GPIO_ODR_3                           ((uint32_t)0x00000008)
#define GPIO_ODR_4                           ((uint32_t)0x00000010)
#define GPIO_ODR_5                           ((uint32_t)0x00000020)
#define GPIO_ODR_6                           ((uint32_t)0x00000040)
#define GPIO_ODR_7                           ((uint32_t)0x00000080)
#define GPIO_ODR_8                           ((uint32_t)0x00000100)
#define GPIO_ODR_9                           ((uint32_t)0x00000200)
#define GPIO_ODR_10                          ((uint32_t)0x00000400)
#define GPIO_ODR_11                          ((uint32_t)0x00000800)
#define GPIO_ODR_12                          ((uint32_t)0x00001000)
#define GPIO_ODR_13                          ((uint32_t)0x00002000)
#define GPIO_ODR_14                          ((uint32_t)0x00004000)
#define GPIO_ODR_15                          ((uint32_t)0x00008000)

/******************  Bits definition for GPIO_BSRR register  ******************/
#define GPIO_BSRR_0                          ((uint16_t)0x0001)
#define GPIO_BSRR_1                          ((uint16_t)0x0002)
#define GPIO_BSRR_2                          ((uint16_t)0x0004)
#define GPIO_BSRR_3                          ((uint16_t)0x0008)
#define GPIO_BSRR_4                          ((uint16_t)0x0010)
#define GPIO_BSRR_5                          ((uint16_t)0x0020)
#define GPIO_BSRR_6                          ((uint16_t)0x0040)
#define GPIO_BSRR_7                          ((uint16_t)0x0080)
#define GPIO_BSRR_8                          ((uint16_t)0x0100)
#define GPIO_BSRR_9                          ((uint16_t)0x0200)
#define GPIO_BSRR_10                         ((uint16_t)0x0400)
#define GPIO_BSRR_11                         ((uint16_t)0x0800)
#define GPIO_BSRR_12                         ((uint16_t)0x1000)
#define GPIO_BSRR_13                         ((uint16_t)0x2000)
#define GPIO_BSRR_14                         ((uint16_t)0x4000)
#define GPIO_BSRR_15                         ((uint16_t)0x8000)


/**
  * @brief General Purpose I/O
  */

typedef struct
{
  __IO uint32_t MODER;      /*!< GPIO port mode register,               Address offset: 0x00      */
  __IO uint32_t OTYPER;     /*!< GPIO port output type register,        Address offset: 0x04      */
  __IO uint32_t OSPEEDR;    /*!< GPIO port output speed register,       Address offset: 0x08      */
  __IO uint32_t PUPDR;      /*!< GPIO port pull-up/pull-down register,  Address offset: 0x0C      */
  __IO uint32_t IDR;        /*!< GPIO port input data register,         Address offset: 0x10      */
  __IO uint32_t ODR;        /*!< GPIO port output data register,        Address offset: 0x14      */
  __IO uint16_t BSRR_SET;   /*!< GPIO port bit set/reset low register,  Address offset: 0x18      */
  __IO uint16_t BSRR_RESET; /*!< GPIO port bit set/reset high register, Address offset: 0x1A      */
  __IO uint32_t LCKR;       /*!< GPIO port configuration lock register, Address offset: 0x1C      */
  __IO uint32_t AFR[2];     /*!< GPIO alternate function registers,     Address offset: 0x20-0x24 */
} GPIO_TypeDef;

#define GPIOA_BASE          (AHB1PERIPH_BASE + 0x0000)
#define GPIOB_BASE          (AHB1PERIPH_BASE + 0x0400)
#define GPIOC_BASE          (AHB1PERIPH_BASE + 0x0800)
#define GPIOD_BASE          (AHB1PERIPH_BASE + 0x0C00)
#define GPIOE_BASE          (AHB1PERIPH_BASE + 0x1000)
#define GPIOF_BASE          (AHB1PERIPH_BASE + 0x1400)
#define GPIOG_BASE          (AHB1PERIPH_BASE + 0x1800)
#define GPIOH_BASE          (AHB1PERIPH_BASE + 0x1C00)
#define GPIOI_BASE          (AHB1PERIPH_BASE + 0x2000)

#define GPIOA               ((GPIO_TypeDef *) GPIOA_BASE)
#define GPIOB               ((GPIO_TypeDef *) GPIOB_BASE)
#define GPIOC               ((GPIO_TypeDef *) GPIOC_BASE)
#define GPIOD               ((GPIO_TypeDef *) GPIOD_BASE)
#define GPIOE               ((GPIO_TypeDef *) GPIOE_BASE)
#define GPIOF               ((GPIO_TypeDef *) GPIOF_BASE)
#define GPIOG               ((GPIO_TypeDef *) GPIOG_BASE)
#define GPIOH               ((GPIO_TypeDef *) GPIOH_BASE)
#define GPIOI               ((GPIO_TypeDef *) GPIOI_BASE)

#endif /* HAL_ST_STM32F4_GPIO_H_ */
