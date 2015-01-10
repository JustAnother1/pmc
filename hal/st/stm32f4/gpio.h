/*
 * gpio.h
 *
 *  Created on: 28.11.2014
 *      Author: lars
 */

#ifndef GPIO_H_
#define GPIO_H_

#include "hal_common.h"
#include "core_cm4.h"

/******************************************************************************/
/*                                                                            */
/*                            General Purpose I/O                             */
/*                                                                            */
/******************************************************************************/
/******************  Bits definition for GPIO_MODER register  *****************/
#define GPIO_MODER_MODER0                    ((uint_fast32_t)0x00000003)
#define GPIO_MODER_MODER0_0                  ((uint_fast32_t)0x00000001)
#define GPIO_MODER_MODER0_1                  ((uint_fast32_t)0x00000002)

#define GPIO_MODER_MODER1                    ((uint_fast32_t)0x0000000C)
#define GPIO_MODER_MODER1_0                  ((uint_fast32_t)0x00000004)
#define GPIO_MODER_MODER1_1                  ((uint_fast32_t)0x00000008)

#define GPIO_MODER_MODER2                    ((uint_fast32_t)0x00000030)
#define GPIO_MODER_MODER2_0                  ((uint_fast32_t)0x00000010)
#define GPIO_MODER_MODER2_1                  ((uint_fast32_t)0x00000020)

#define GPIO_MODER_MODER3                    ((uint_fast32_t)0x000000C0)
#define GPIO_MODER_MODER3_0                  ((uint_fast32_t)0x00000040)
#define GPIO_MODER_MODER3_1                  ((uint_fast32_t)0x00000080)

#define GPIO_MODER_MODER4                    ((uint_fast32_t)0x00000300)
#define GPIO_MODER_MODER4_0                  ((uint_fast32_t)0x00000100)
#define GPIO_MODER_MODER4_1                  ((uint_fast32_t)0x00000200)

#define GPIO_MODER_MODER5                    ((uint_fast32_t)0x00000C00)
#define GPIO_MODER_MODER5_0                  ((uint_fast32_t)0x00000400)
#define GPIO_MODER_MODER5_1                  ((uint_fast32_t)0x00000800)

#define GPIO_MODER_MODER6                    ((uint_fast32_t)0x00003000)
#define GPIO_MODER_MODER6_0                  ((uint_fast32_t)0x00001000)
#define GPIO_MODER_MODER6_1                  ((uint_fast32_t)0x00002000)

#define GPIO_MODER_MODER7                    ((uint_fast32_t)0x0000C000)
#define GPIO_MODER_MODER7_0                  ((uint_fast32_t)0x00004000)
#define GPIO_MODER_MODER7_1                  ((uint_fast32_t)0x00008000)

#define GPIO_MODER_MODER8                    ((uint_fast32_t)0x00030000)
#define GPIO_MODER_MODER8_0                  ((uint_fast32_t)0x00010000)
#define GPIO_MODER_MODER8_1                  ((uint_fast32_t)0x00020000)

#define GPIO_MODER_MODER9                    ((uint_fast32_t)0x000C0000)
#define GPIO_MODER_MODER9_0                  ((uint_fast32_t)0x00040000)
#define GPIO_MODER_MODER9_1                  ((uint_fast32_t)0x00080000)

#define GPIO_MODER_MODER10                   ((uint_fast32_t)0x00300000)
#define GPIO_MODER_MODER10_0                 ((uint_fast32_t)0x00100000)
#define GPIO_MODER_MODER10_1                 ((uint_fast32_t)0x00200000)

#define GPIO_MODER_MODER11                   ((uint_fast32_t)0x00C00000)
#define GPIO_MODER_MODER11_0                 ((uint_fast32_t)0x00400000)
#define GPIO_MODER_MODER11_1                 ((uint_fast32_t)0x00800000)

#define GPIO_MODER_MODER12                   ((uint_fast32_t)0x03000000)
#define GPIO_MODER_MODER12_0                 ((uint_fast32_t)0x01000000)
#define GPIO_MODER_MODER12_1                 ((uint_fast32_t)0x02000000)

#define GPIO_MODER_MODER13                   ((uint_fast32_t)0x0C000000)
#define GPIO_MODER_MODER13_0                 ((uint_fast32_t)0x04000000)
#define GPIO_MODER_MODER13_1                 ((uint_fast32_t)0x08000000)

#define GPIO_MODER_MODER14                   ((uint_fast32_t)0x30000000)
#define GPIO_MODER_MODER14_0                 ((uint_fast32_t)0x10000000)
#define GPIO_MODER_MODER14_1                 ((uint_fast32_t)0x20000000)

#define GPIO_MODER_MODER15                   ((uint_fast32_t)0xC0000000)
#define GPIO_MODER_MODER15_0                 ((uint_fast32_t)0x40000000)
#define GPIO_MODER_MODER15_1                 ((uint_fast32_t)0x80000000)

/******************  Bits definition for GPIO_OTYPER register  ****************/
#define GPIO_OTYPER_OT_0                     ((uint_fast32_t)0x00000001)
#define GPIO_OTYPER_OT_1                     ((uint_fast32_t)0x00000002)
#define GPIO_OTYPER_OT_2                     ((uint_fast32_t)0x00000004)
#define GPIO_OTYPER_OT_3                     ((uint_fast32_t)0x00000008)
#define GPIO_OTYPER_OT_4                     ((uint_fast32_t)0x00000010)
#define GPIO_OTYPER_OT_5                     ((uint_fast32_t)0x00000020)
#define GPIO_OTYPER_OT_6                     ((uint_fast32_t)0x00000040)
#define GPIO_OTYPER_OT_7                     ((uint_fast32_t)0x00000080)
#define GPIO_OTYPER_OT_8                     ((uint_fast32_t)0x00000100)
#define GPIO_OTYPER_OT_9                     ((uint_fast32_t)0x00000200)
#define GPIO_OTYPER_OT_10                    ((uint_fast32_t)0x00000400)
#define GPIO_OTYPER_OT_11                    ((uint_fast32_t)0x00000800)
#define GPIO_OTYPER_OT_12                    ((uint_fast32_t)0x00001000)
#define GPIO_OTYPER_OT_13                    ((uint_fast32_t)0x00002000)
#define GPIO_OTYPER_OT_14                    ((uint_fast32_t)0x00004000)
#define GPIO_OTYPER_OT_15                    ((uint_fast32_t)0x00008000)

/******************  Bits definition for GPIO_OSPEEDR register  ***************/
#define GPIO_OSPEEDER_OSPEEDR0               ((uint_fast32_t)0x00000003)
#define GPIO_OSPEEDER_OSPEEDR0_0             ((uint_fast32_t)0x00000001)
#define GPIO_OSPEEDER_OSPEEDR0_1             ((uint_fast32_t)0x00000002)

#define GPIO_OSPEEDER_OSPEEDR1               ((uint_fast32_t)0x0000000C)
#define GPIO_OSPEEDER_OSPEEDR1_0             ((uint_fast32_t)0x00000004)
#define GPIO_OSPEEDER_OSPEEDR1_1             ((uint_fast32_t)0x00000008)

#define GPIO_OSPEEDER_OSPEEDR2               ((uint_fast32_t)0x00000030)
#define GPIO_OSPEEDER_OSPEEDR2_0             ((uint_fast32_t)0x00000010)
#define GPIO_OSPEEDER_OSPEEDR2_1             ((uint_fast32_t)0x00000020)

#define GPIO_OSPEEDER_OSPEEDR3               ((uint_fast32_t)0x000000C0)
#define GPIO_OSPEEDER_OSPEEDR3_0             ((uint_fast32_t)0x00000040)
#define GPIO_OSPEEDER_OSPEEDR3_1             ((uint_fast32_t)0x00000080)

#define GPIO_OSPEEDER_OSPEEDR4               ((uint_fast32_t)0x00000300)
#define GPIO_OSPEEDER_OSPEEDR4_0             ((uint_fast32_t)0x00000100)
#define GPIO_OSPEEDER_OSPEEDR4_1             ((uint_fast32_t)0x00000200)

#define GPIO_OSPEEDER_OSPEEDR5               ((uint_fast32_t)0x00000C00)
#define GPIO_OSPEEDER_OSPEEDR5_0             ((uint_fast32_t)0x00000400)
#define GPIO_OSPEEDER_OSPEEDR5_1             ((uint_fast32_t)0x00000800)

#define GPIO_OSPEEDER_OSPEEDR6               ((uint_fast32_t)0x00003000)
#define GPIO_OSPEEDER_OSPEEDR6_0             ((uint_fast32_t)0x00001000)
#define GPIO_OSPEEDER_OSPEEDR6_1             ((uint_fast32_t)0x00002000)

#define GPIO_OSPEEDER_OSPEEDR7               ((uint_fast32_t)0x0000C000)
#define GPIO_OSPEEDER_OSPEEDR7_0             ((uint_fast32_t)0x00004000)
#define GPIO_OSPEEDER_OSPEEDR7_1             ((uint_fast32_t)0x00008000)

#define GPIO_OSPEEDER_OSPEEDR8               ((uint_fast32_t)0x00030000)
#define GPIO_OSPEEDER_OSPEEDR8_0             ((uint_fast32_t)0x00010000)
#define GPIO_OSPEEDER_OSPEEDR8_1             ((uint_fast32_t)0x00020000)

#define GPIO_OSPEEDER_OSPEEDR9               ((uint_fast32_t)0x000C0000)
#define GPIO_OSPEEDER_OSPEEDR9_0             ((uint_fast32_t)0x00040000)
#define GPIO_OSPEEDER_OSPEEDR9_1             ((uint_fast32_t)0x00080000)

#define GPIO_OSPEEDER_OSPEEDR10              ((uint_fast32_t)0x00300000)
#define GPIO_OSPEEDER_OSPEEDR10_0            ((uint_fast32_t)0x00100000)
#define GPIO_OSPEEDER_OSPEEDR10_1            ((uint_fast32_t)0x00200000)

#define GPIO_OSPEEDER_OSPEEDR11              ((uint_fast32_t)0x00C00000)
#define GPIO_OSPEEDER_OSPEEDR11_0            ((uint_fast32_t)0x00400000)
#define GPIO_OSPEEDER_OSPEEDR11_1            ((uint_fast32_t)0x00800000)

#define GPIO_OSPEEDER_OSPEEDR12              ((uint_fast32_t)0x03000000)
#define GPIO_OSPEEDER_OSPEEDR12_0            ((uint_fast32_t)0x01000000)
#define GPIO_OSPEEDER_OSPEEDR12_1            ((uint_fast32_t)0x02000000)

#define GPIO_OSPEEDER_OSPEEDR13              ((uint_fast32_t)0x0C000000)
#define GPIO_OSPEEDER_OSPEEDR13_0            ((uint_fast32_t)0x04000000)
#define GPIO_OSPEEDER_OSPEEDR13_1            ((uint_fast32_t)0x08000000)

#define GPIO_OSPEEDER_OSPEEDR14              ((uint_fast32_t)0x30000000)
#define GPIO_OSPEEDER_OSPEEDR14_0            ((uint_fast32_t)0x10000000)
#define GPIO_OSPEEDER_OSPEEDR14_1            ((uint_fast32_t)0x20000000)

#define GPIO_OSPEEDER_OSPEEDR15              ((uint_fast32_t)0xC0000000)
#define GPIO_OSPEEDER_OSPEEDR15_0            ((uint_fast32_t)0x40000000)
#define GPIO_OSPEEDER_OSPEEDR15_1            ((uint_fast32_t)0x80000000)

/******************  Bits definition for GPIO_PUPDR register  *****************/
#define GPIO_PUPDR_PUPDR0                    ((uint_fast32_t)0x00000003)
#define GPIO_PUPDR_PUPDR0_0                  ((uint_fast32_t)0x00000001)
#define GPIO_PUPDR_PUPDR0_1                  ((uint_fast32_t)0x00000002)

#define GPIO_PUPDR_PUPDR1                    ((uint_fast32_t)0x0000000C)
#define GPIO_PUPDR_PUPDR1_0                  ((uint_fast32_t)0x00000004)
#define GPIO_PUPDR_PUPDR1_1                  ((uint_fast32_t)0x00000008)

#define GPIO_PUPDR_PUPDR2                    ((uint_fast32_t)0x00000030)
#define GPIO_PUPDR_PUPDR2_0                  ((uint_fast32_t)0x00000010)
#define GPIO_PUPDR_PUPDR2_1                  ((uint_fast32_t)0x00000020)

#define GPIO_PUPDR_PUPDR3                    ((uint_fast32_t)0x000000C0)
#define GPIO_PUPDR_PUPDR3_0                  ((uint_fast32_t)0x00000040)
#define GPIO_PUPDR_PUPDR3_1                  ((uint_fast32_t)0x00000080)

#define GPIO_PUPDR_PUPDR4                    ((uint_fast32_t)0x00000300)
#define GPIO_PUPDR_PUPDR4_0                  ((uint_fast32_t)0x00000100)
#define GPIO_PUPDR_PUPDR4_1                  ((uint_fast32_t)0x00000200)

#define GPIO_PUPDR_PUPDR5                    ((uint_fast32_t)0x00000C00)
#define GPIO_PUPDR_PUPDR5_0                  ((uint_fast32_t)0x00000400)
#define GPIO_PUPDR_PUPDR5_1                  ((uint_fast32_t)0x00000800)

#define GPIO_PUPDR_PUPDR6                    ((uint_fast32_t)0x00003000)
#define GPIO_PUPDR_PUPDR6_0                  ((uint_fast32_t)0x00001000)
#define GPIO_PUPDR_PUPDR6_1                  ((uint_fast32_t)0x00002000)

#define GPIO_PUPDR_PUPDR7                    ((uint_fast32_t)0x0000C000)
#define GPIO_PUPDR_PUPDR7_0                  ((uint_fast32_t)0x00004000)
#define GPIO_PUPDR_PUPDR7_1                  ((uint_fast32_t)0x00008000)

#define GPIO_PUPDR_PUPDR8                    ((uint_fast32_t)0x00030000)
#define GPIO_PUPDR_PUPDR8_0                  ((uint_fast32_t)0x00010000)
#define GPIO_PUPDR_PUPDR8_1                  ((uint_fast32_t)0x00020000)

#define GPIO_PUPDR_PUPDR9                    ((uint_fast32_t)0x000C0000)
#define GPIO_PUPDR_PUPDR9_0                  ((uint_fast32_t)0x00040000)
#define GPIO_PUPDR_PUPDR9_1                  ((uint_fast32_t)0x00080000)

#define GPIO_PUPDR_PUPDR10                   ((uint_fast32_t)0x00300000)
#define GPIO_PUPDR_PUPDR10_0                 ((uint_fast32_t)0x00100000)
#define GPIO_PUPDR_PUPDR10_1                 ((uint_fast32_t)0x00200000)

#define GPIO_PUPDR_PUPDR11                   ((uint_fast32_t)0x00C00000)
#define GPIO_PUPDR_PUPDR11_0                 ((uint_fast32_t)0x00400000)
#define GPIO_PUPDR_PUPDR11_1                 ((uint_fast32_t)0x00800000)

#define GPIO_PUPDR_PUPDR12                   ((uint_fast32_t)0x03000000)
#define GPIO_PUPDR_PUPDR12_0                 ((uint_fast32_t)0x01000000)
#define GPIO_PUPDR_PUPDR12_1                 ((uint_fast32_t)0x02000000)

#define GPIO_PUPDR_PUPDR13                   ((uint_fast32_t)0x0C000000)
#define GPIO_PUPDR_PUPDR13_0                 ((uint_fast32_t)0x04000000)
#define GPIO_PUPDR_PUPDR13_1                 ((uint_fast32_t)0x08000000)

#define GPIO_PUPDR_PUPDR14                   ((uint_fast32_t)0x30000000)
#define GPIO_PUPDR_PUPDR14_0                 ((uint_fast32_t)0x10000000)
#define GPIO_PUPDR_PUPDR14_1                 ((uint_fast32_t)0x20000000)

#define GPIO_PUPDR_PUPDR15                   ((uint_fast32_t)0xC0000000)
#define GPIO_PUPDR_PUPDR15_0                 ((uint_fast32_t)0x40000000)
#define GPIO_PUPDR_PUPDR15_1                 ((uint_fast32_t)0x80000000)

/******************  Bits definition for GPIO_IDR register  *******************/
#define GPIO_IDR_IDR_0                       ((uint_fast32_t)0x00000001)
#define GPIO_IDR_IDR_1                       ((uint_fast32_t)0x00000002)
#define GPIO_IDR_IDR_2                       ((uint_fast32_t)0x00000004)
#define GPIO_IDR_IDR_3                       ((uint_fast32_t)0x00000008)
#define GPIO_IDR_IDR_4                       ((uint_fast32_t)0x00000010)
#define GPIO_IDR_IDR_5                       ((uint_fast32_t)0x00000020)
#define GPIO_IDR_IDR_6                       ((uint_fast32_t)0x00000040)
#define GPIO_IDR_IDR_7                       ((uint_fast32_t)0x00000080)
#define GPIO_IDR_IDR_8                       ((uint_fast32_t)0x00000100)
#define GPIO_IDR_IDR_9                       ((uint_fast32_t)0x00000200)
#define GPIO_IDR_IDR_10                      ((uint_fast32_t)0x00000400)
#define GPIO_IDR_IDR_11                      ((uint_fast32_t)0x00000800)
#define GPIO_IDR_IDR_12                      ((uint_fast32_t)0x00001000)
#define GPIO_IDR_IDR_13                      ((uint_fast32_t)0x00002000)
#define GPIO_IDR_IDR_14                      ((uint_fast32_t)0x00004000)
#define GPIO_IDR_IDR_15                      ((uint_fast32_t)0x00008000)
/* Old GPIO_IDR register bits definition, maintained for legacy purpose */
#define GPIO_OTYPER_IDR_0                    GPIO_IDR_IDR_0
#define GPIO_OTYPER_IDR_1                    GPIO_IDR_IDR_1
#define GPIO_OTYPER_IDR_2                    GPIO_IDR_IDR_2
#define GPIO_OTYPER_IDR_3                    GPIO_IDR_IDR_3
#define GPIO_OTYPER_IDR_4                    GPIO_IDR_IDR_4
#define GPIO_OTYPER_IDR_5                    GPIO_IDR_IDR_5
#define GPIO_OTYPER_IDR_6                    GPIO_IDR_IDR_6
#define GPIO_OTYPER_IDR_7                    GPIO_IDR_IDR_7
#define GPIO_OTYPER_IDR_8                    GPIO_IDR_IDR_8
#define GPIO_OTYPER_IDR_9                    GPIO_IDR_IDR_9
#define GPIO_OTYPER_IDR_10                   GPIO_IDR_IDR_10
#define GPIO_OTYPER_IDR_11                   GPIO_IDR_IDR_11
#define GPIO_OTYPER_IDR_12                   GPIO_IDR_IDR_12
#define GPIO_OTYPER_IDR_13                   GPIO_IDR_IDR_13
#define GPIO_OTYPER_IDR_14                   GPIO_IDR_IDR_14
#define GPIO_OTYPER_IDR_15                   GPIO_IDR_IDR_15

/******************  Bits definition for GPIO_ODR register  *******************/
#define GPIO_ODR_ODR_0                       ((uint_fast32_t)0x00000001)
#define GPIO_ODR_ODR_1                       ((uint_fast32_t)0x00000002)
#define GPIO_ODR_ODR_2                       ((uint_fast32_t)0x00000004)
#define GPIO_ODR_ODR_3                       ((uint_fast32_t)0x00000008)
#define GPIO_ODR_ODR_4                       ((uint_fast32_t)0x00000010)
#define GPIO_ODR_ODR_5                       ((uint_fast32_t)0x00000020)
#define GPIO_ODR_ODR_6                       ((uint_fast32_t)0x00000040)
#define GPIO_ODR_ODR_7                       ((uint_fast32_t)0x00000080)
#define GPIO_ODR_ODR_8                       ((uint_fast32_t)0x00000100)
#define GPIO_ODR_ODR_9                       ((uint_fast32_t)0x00000200)
#define GPIO_ODR_ODR_10                      ((uint_fast32_t)0x00000400)
#define GPIO_ODR_ODR_11                      ((uint_fast32_t)0x00000800)
#define GPIO_ODR_ODR_12                      ((uint_fast32_t)0x00001000)
#define GPIO_ODR_ODR_13                      ((uint_fast32_t)0x00002000)
#define GPIO_ODR_ODR_14                      ((uint_fast32_t)0x00004000)
#define GPIO_ODR_ODR_15                      ((uint_fast32_t)0x00008000)
/* Old GPIO_ODR register bits definition, maintained for legacy purpose */
#define GPIO_OTYPER_ODR_0                    GPIO_ODR_ODR_0
#define GPIO_OTYPER_ODR_1                    GPIO_ODR_ODR_1
#define GPIO_OTYPER_ODR_2                    GPIO_ODR_ODR_2
#define GPIO_OTYPER_ODR_3                    GPIO_ODR_ODR_3
#define GPIO_OTYPER_ODR_4                    GPIO_ODR_ODR_4
#define GPIO_OTYPER_ODR_5                    GPIO_ODR_ODR_5
#define GPIO_OTYPER_ODR_6                    GPIO_ODR_ODR_6
#define GPIO_OTYPER_ODR_7                    GPIO_ODR_ODR_7
#define GPIO_OTYPER_ODR_8                    GPIO_ODR_ODR_8
#define GPIO_OTYPER_ODR_9                    GPIO_ODR_ODR_9
#define GPIO_OTYPER_ODR_10                   GPIO_ODR_ODR_10
#define GPIO_OTYPER_ODR_11                   GPIO_ODR_ODR_11
#define GPIO_OTYPER_ODR_12                   GPIO_ODR_ODR_12
#define GPIO_OTYPER_ODR_13                   GPIO_ODR_ODR_13
#define GPIO_OTYPER_ODR_14                   GPIO_ODR_ODR_14
#define GPIO_OTYPER_ODR_15                   GPIO_ODR_ODR_15

/******************  Bits definition for GPIO_BSRR register  ******************/
#define GPIO_BSRR_BS_0                       ((uint_fast32_t)0x00000001)
#define GPIO_BSRR_BS_1                       ((uint_fast32_t)0x00000002)
#define GPIO_BSRR_BS_2                       ((uint_fast32_t)0x00000004)
#define GPIO_BSRR_BS_3                       ((uint_fast32_t)0x00000008)
#define GPIO_BSRR_BS_4                       ((uint_fast32_t)0x00000010)
#define GPIO_BSRR_BS_5                       ((uint_fast32_t)0x00000020)
#define GPIO_BSRR_BS_6                       ((uint_fast32_t)0x00000040)
#define GPIO_BSRR_BS_7                       ((uint_fast32_t)0x00000080)
#define GPIO_BSRR_BS_8                       ((uint_fast32_t)0x00000100)
#define GPIO_BSRR_BS_9                       ((uint_fast32_t)0x00000200)
#define GPIO_BSRR_BS_10                      ((uint_fast32_t)0x00000400)
#define GPIO_BSRR_BS_11                      ((uint_fast32_t)0x00000800)
#define GPIO_BSRR_BS_12                      ((uint_fast32_t)0x00001000)
#define GPIO_BSRR_BS_13                      ((uint_fast32_t)0x00002000)
#define GPIO_BSRR_BS_14                      ((uint_fast32_t)0x00004000)
#define GPIO_BSRR_BS_15                      ((uint_fast32_t)0x00008000)
#define GPIO_BSRR_BR_0                       ((uint_fast32_t)0x00010000)
#define GPIO_BSRR_BR_1                       ((uint_fast32_t)0x00020000)
#define GPIO_BSRR_BR_2                       ((uint_fast32_t)0x00040000)
#define GPIO_BSRR_BR_3                       ((uint_fast32_t)0x00080000)
#define GPIO_BSRR_BR_4                       ((uint_fast32_t)0x00100000)
#define GPIO_BSRR_BR_5                       ((uint_fast32_t)0x00200000)
#define GPIO_BSRR_BR_6                       ((uint_fast32_t)0x00400000)
#define GPIO_BSRR_BR_7                       ((uint_fast32_t)0x00800000)
#define GPIO_BSRR_BR_8                       ((uint_fast32_t)0x01000000)
#define GPIO_BSRR_BR_9                       ((uint_fast32_t)0x02000000)
#define GPIO_BSRR_BR_10                      ((uint_fast32_t)0x04000000)
#define GPIO_BSRR_BR_11                      ((uint_fast32_t)0x08000000)
#define GPIO_BSRR_BR_12                      ((uint_fast32_t)0x10000000)
#define GPIO_BSRR_BR_13                      ((uint_fast32_t)0x20000000)
#define GPIO_BSRR_BR_14                      ((uint_fast32_t)0x40000000)
#define GPIO_BSRR_BR_15                      ((uint_fast32_t)0x80000000)

/****************** Bit definition for GPIO_LCKR register *********************/
#define GPIO_LCKR_LCK0                       ((uint_fast32_t)0x00000001)
#define GPIO_LCKR_LCK1                       ((uint_fast32_t)0x00000002)
#define GPIO_LCKR_LCK2                       ((uint_fast32_t)0x00000004)
#define GPIO_LCKR_LCK3                       ((uint_fast32_t)0x00000008)
#define GPIO_LCKR_LCK4                       ((uint_fast32_t)0x00000010)
#define GPIO_LCKR_LCK5                       ((uint_fast32_t)0x00000020)
#define GPIO_LCKR_LCK6                       ((uint_fast32_t)0x00000040)
#define GPIO_LCKR_LCK7                       ((uint_fast32_t)0x00000080)
#define GPIO_LCKR_LCK8                       ((uint_fast32_t)0x00000100)
#define GPIO_LCKR_LCK9                       ((uint_fast32_t)0x00000200)
#define GPIO_LCKR_LCK10                      ((uint_fast32_t)0x00000400)
#define GPIO_LCKR_LCK11                      ((uint_fast32_t)0x00000800)
#define GPIO_LCKR_LCK12                      ((uint_fast32_t)0x00001000)
#define GPIO_LCKR_LCK13                      ((uint_fast32_t)0x00002000)
#define GPIO_LCKR_LCK14                      ((uint_fast32_t)0x00004000)
#define GPIO_LCKR_LCK15                      ((uint_fast32_t)0x00008000)
#define GPIO_LCKR_LCKK                       ((uint_fast32_t)0x00010000)


/**
  * @brief General Purpose I/O
  */

typedef struct
{
  __IO uint_fast32_t MODER;    /*!< GPIO port mode register,               Address offset: 0x00      */
  __IO uint_fast32_t OTYPER;   /*!< GPIO port output type register,        Address offset: 0x04      */
  __IO uint_fast32_t OSPEEDR;  /*!< GPIO port output speed register,       Address offset: 0x08      */
  __IO uint_fast32_t PUPDR;    /*!< GPIO port pull-up/pull-down register,  Address offset: 0x0C      */
  __IO uint_fast32_t IDR;      /*!< GPIO port input data register,         Address offset: 0x10      */
  __IO uint_fast32_t ODR;      /*!< GPIO port output data register,        Address offset: 0x14      */
  __IO uint_fast16_t BSRRL;    /*!< GPIO port bit set/reset low register,  Address offset: 0x18      */
  __IO uint_fast16_t BSRRH;    /*!< GPIO port bit set/reset high register, Address offset: 0x1A      */
  __IO uint_fast32_t LCKR;     /*!< GPIO port configuration lock register, Address offset: 0x1C      */
  __IO uint_fast32_t AFR[2];   /*!< GPIO alternate function registers,     Address offset: 0x20-0x24 */
} GPIO_TypeDef;

#define GPIOA               ((GPIO_TypeDef *) GPIOA_BASE)
#define GPIOB               ((GPIO_TypeDef *) GPIOB_BASE)
#define GPIOC               ((GPIO_TypeDef *) GPIOC_BASE)
#define GPIOD               ((GPIO_TypeDef *) GPIOD_BASE)
#define GPIOE               ((GPIO_TypeDef *) GPIOE_BASE)
#define GPIOF               ((GPIO_TypeDef *) GPIOF_BASE)
#define GPIOG               ((GPIO_TypeDef *) GPIOG_BASE)
#define GPIOH               ((GPIO_TypeDef *) GPIOH_BASE)
#define GPIOI               ((GPIO_TypeDef *) GPIOI_BASE)

#endif /* GPIO_H_ */
