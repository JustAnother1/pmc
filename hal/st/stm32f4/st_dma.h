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

#ifndef HAL_ST_STM32F4_ST_DMA_H_
#define HAL_ST_STM32F4_ST_DMA_H_

#include "st_common.h"

/******************************************************************************/
/*                                                                            */
/*                             DMA Controller                                 */
/*                                                                            */
/******************************************************************************/
/********************  Bits definition for DMA_SxCR register  *****************/
#define DMA_SxCR_CHSEL                       ((uint32_t)0x0E000000)
#define DMA_SxCR_CHSEL_0                     ((uint32_t)0x02000000)
#define DMA_SxCR_CHSEL_1                     ((uint32_t)0x04000000)
#define DMA_SxCR_CHSEL_2                     ((uint32_t)0x08000000)
#define DMA_SxCR_MBURST                      ((uint32_t)0x01800000)
#define DMA_SxCR_MBURST_0                    ((uint32_t)0x00800000)
#define DMA_SxCR_MBURST_1                    ((uint32_t)0x01000000)
#define DMA_SxCR_PBURST                      ((uint32_t)0x00600000)
#define DMA_SxCR_PBURST_0                    ((uint32_t)0x00200000)
#define DMA_SxCR_PBURST_1                    ((uint32_t)0x00400000)
#define DMA_SxCR_ACK                         ((uint32_t)0x00100000)
#define DMA_SxCR_CT                          ((uint32_t)0x00080000)
#define DMA_SxCR_DBM                         ((uint32_t)0x00040000)
#define DMA_SxCR_PL                          ((uint32_t)0x00030000)
#define DMA_SxCR_PL_0                        ((uint32_t)0x00010000)
#define DMA_SxCR_PL_1                        ((uint32_t)0x00020000)
#define DMA_SxCR_PINCOS                      ((uint32_t)0x00008000)
#define DMA_SxCR_MSIZE                       ((uint32_t)0x00006000)
#define DMA_SxCR_MSIZE_0                     ((uint32_t)0x00002000)
#define DMA_SxCR_MSIZE_1                     ((uint32_t)0x00004000)
#define DMA_SxCR_PSIZE                       ((uint32_t)0x00001800)
#define DMA_SxCR_PSIZE_0                     ((uint32_t)0x00000800)
#define DMA_SxCR_PSIZE_1                     ((uint32_t)0x00001000)
#define DMA_SxCR_MINC                        ((uint32_t)0x00000400)
#define DMA_SxCR_PINC                        ((uint32_t)0x00000200)
#define DMA_SxCR_CIRC                        ((uint32_t)0x00000100)
#define DMA_SxCR_DIR                         ((uint32_t)0x000000C0)
#define DMA_SxCR_DIR_0                       ((uint32_t)0x00000040)
#define DMA_SxCR_DIR_1                       ((uint32_t)0x00000080)
#define DMA_SxCR_PFCTRL                      ((uint32_t)0x00000020)
#define DMA_SxCR_TCIE                        ((uint32_t)0x00000010)
#define DMA_SxCR_HTIE                        ((uint32_t)0x00000008)
#define DMA_SxCR_TEIE                        ((uint32_t)0x00000004)
#define DMA_SxCR_DMEIE                       ((uint32_t)0x00000002)
#define DMA_SxCR_EN                          ((uint32_t)0x00000001)

/********************  Bits definition for DMA_SxCNDTR register  **************/
#define DMA_SxNDT                            ((uint32_t)0x0000FFFF)
#define DMA_SxNDT_0                          ((uint32_t)0x00000001)
#define DMA_SxNDT_1                          ((uint32_t)0x00000002)
#define DMA_SxNDT_2                          ((uint32_t)0x00000004)
#define DMA_SxNDT_3                          ((uint32_t)0x00000008)
#define DMA_SxNDT_4                          ((uint32_t)0x00000010)
#define DMA_SxNDT_5                          ((uint32_t)0x00000020)
#define DMA_SxNDT_6                          ((uint32_t)0x00000040)
#define DMA_SxNDT_7                          ((uint32_t)0x00000080)
#define DMA_SxNDT_8                          ((uint32_t)0x00000100)
#define DMA_SxNDT_9                          ((uint32_t)0x00000200)
#define DMA_SxNDT_10                         ((uint32_t)0x00000400)
#define DMA_SxNDT_11                         ((uint32_t)0x00000800)
#define DMA_SxNDT_12                         ((uint32_t)0x00001000)
#define DMA_SxNDT_13                         ((uint32_t)0x00002000)
#define DMA_SxNDT_14                         ((uint32_t)0x00004000)
#define DMA_SxNDT_15                         ((uint32_t)0x00008000)

/********************  Bits definition for DMA_SxFCR register  ****************/
#define DMA_SxFCR_FEIE                       ((uint32_t)0x00000080)
#define DMA_SxFCR_FS                         ((uint32_t)0x00000038)
#define DMA_SxFCR_FS_0                       ((uint32_t)0x00000008)
#define DMA_SxFCR_FS_1                       ((uint32_t)0x00000010)
#define DMA_SxFCR_FS_2                       ((uint32_t)0x00000020)
#define DMA_SxFCR_DMDIS                      ((uint32_t)0x00000004)
#define DMA_SxFCR_FTH                        ((uint32_t)0x00000003)
#define DMA_SxFCR_FTH_0                      ((uint32_t)0x00000001)
#define DMA_SxFCR_FTH_1                      ((uint32_t)0x00000002)

/********************  Bits definition for DMA_LISR register  *****************/
#define DMA_LISR_TCIF3                       ((uint32_t)0x08000000)
#define DMA_LISR_HTIF3                       ((uint32_t)0x04000000)
#define DMA_LISR_TEIF3                       ((uint32_t)0x02000000)
#define DMA_LISR_DMEIF3                      ((uint32_t)0x01000000)
#define DMA_LISR_FEIF3                       ((uint32_t)0x00400000)
#define DMA_LISR_TCIF2                       ((uint32_t)0x00200000)
#define DMA_LISR_HTIF2                       ((uint32_t)0x00100000)
#define DMA_LISR_TEIF2                       ((uint32_t)0x00080000)
#define DMA_LISR_DMEIF2                      ((uint32_t)0x00040000)
#define DMA_LISR_FEIF2                       ((uint32_t)0x00010000)
#define DMA_LISR_TCIF1                       ((uint32_t)0x00000800)
#define DMA_LISR_HTIF1                       ((uint32_t)0x00000400)
#define DMA_LISR_TEIF1                       ((uint32_t)0x00000200)
#define DMA_LISR_DMEIF1                      ((uint32_t)0x00000100)
#define DMA_LISR_FEIF1                       ((uint32_t)0x00000040)
#define DMA_LISR_TCIF0                       ((uint32_t)0x00000020)
#define DMA_LISR_HTIF0                       ((uint32_t)0x00000010)
#define DMA_LISR_TEIF0                       ((uint32_t)0x00000008)
#define DMA_LISR_DMEIF0                      ((uint32_t)0x00000004)
#define DMA_LISR_FEIF0                       ((uint32_t)0x00000001)

/********************  Bits definition for DMA_HISR register  *****************/
#define DMA_HISR_TCIF7                       ((uint32_t)0x08000000)
#define DMA_HISR_HTIF7                       ((uint32_t)0x04000000)
#define DMA_HISR_TEIF7                       ((uint32_t)0x02000000)
#define DMA_HISR_DMEIF7                      ((uint32_t)0x01000000)
#define DMA_HISR_FEIF7                       ((uint32_t)0x00400000)
#define DMA_HISR_TCIF6                       ((uint32_t)0x00200000)
#define DMA_HISR_HTIF6                       ((uint32_t)0x00100000)
#define DMA_HISR_TEIF6                       ((uint32_t)0x00080000)
#define DMA_HISR_DMEIF6                      ((uint32_t)0x00040000)
#define DMA_HISR_FEIF6                       ((uint32_t)0x00010000)
#define DMA_HISR_TCIF5                       ((uint32_t)0x00000800)
#define DMA_HISR_HTIF5                       ((uint32_t)0x00000400)
#define DMA_HISR_TEIF5                       ((uint32_t)0x00000200)
#define DMA_HISR_DMEIF5                      ((uint32_t)0x00000100)
#define DMA_HISR_FEIF5                       ((uint32_t)0x00000040)
#define DMA_HISR_TCIF4                       ((uint32_t)0x00000020)
#define DMA_HISR_HTIF4                       ((uint32_t)0x00000010)
#define DMA_HISR_TEIF4                       ((uint32_t)0x00000008)
#define DMA_HISR_DMEIF4                      ((uint32_t)0x00000004)
#define DMA_HISR_FEIF4                       ((uint32_t)0x00000001)

/********************  Bits definition for DMA_LIFCR register  ****************/
#define DMA_LIFCR_CTCIF3                     ((uint32_t)0x08000000)
#define DMA_LIFCR_CHTIF3                     ((uint32_t)0x04000000)
#define DMA_LIFCR_CTEIF3                     ((uint32_t)0x02000000)
#define DMA_LIFCR_CDMEIF3                    ((uint32_t)0x01000000)
#define DMA_LIFCR_CFEIF3                     ((uint32_t)0x00400000)
#define DMA_LIFCR_CTCIF2                     ((uint32_t)0x00200000)
#define DMA_LIFCR_CHTIF2                     ((uint32_t)0x00100000)
#define DMA_LIFCR_CTEIF2                     ((uint32_t)0x00080000)
#define DMA_LIFCR_CDMEIF2                    ((uint32_t)0x00040000)
#define DMA_LIFCR_CFEIF2                     ((uint32_t)0x00010000)
#define DMA_LIFCR_CTCIF1                     ((uint32_t)0x00000800)
#define DMA_LIFCR_CHTIF1                     ((uint32_t)0x00000400)
#define DMA_LIFCR_CTEIF1                     ((uint32_t)0x00000200)
#define DMA_LIFCR_CDMEIF1                    ((uint32_t)0x00000100)
#define DMA_LIFCR_CFEIF1                     ((uint32_t)0x00000040)
#define DMA_LIFCR_CTCIF0                     ((uint32_t)0x00000020)
#define DMA_LIFCR_CHTIF0                     ((uint32_t)0x00000010)
#define DMA_LIFCR_CTEIF0                     ((uint32_t)0x00000008)
#define DMA_LIFCR_CDMEIF0                    ((uint32_t)0x00000004)
#define DMA_LIFCR_CFEIF0                     ((uint32_t)0x00000001)

/********************  Bits definition for DMA_HIFCR  register  ****************/
#define DMA_HIFCR_CTCIF7                     ((uint32_t)0x08000000)
#define DMA_HIFCR_CHTIF7                     ((uint32_t)0x04000000)
#define DMA_HIFCR_CTEIF7                     ((uint32_t)0x02000000)
#define DMA_HIFCR_CDMEIF7                    ((uint32_t)0x01000000)
#define DMA_HIFCR_CFEIF7                     ((uint32_t)0x00400000)
#define DMA_HIFCR_CTCIF6                     ((uint32_t)0x00200000)
#define DMA_HIFCR_CHTIF6                     ((uint32_t)0x00100000)
#define DMA_HIFCR_CTEIF6                     ((uint32_t)0x00080000)
#define DMA_HIFCR_CDMEIF6                    ((uint32_t)0x00040000)
#define DMA_HIFCR_CFEIF6                     ((uint32_t)0x00010000)
#define DMA_HIFCR_CTCIF5                     ((uint32_t)0x00000800)
#define DMA_HIFCR_CHTIF5                     ((uint32_t)0x00000400)
#define DMA_HIFCR_CTEIF5                     ((uint32_t)0x00000200)
#define DMA_HIFCR_CDMEIF5                    ((uint32_t)0x00000100)
#define DMA_HIFCR_CFEIF5                     ((uint32_t)0x00000040)
#define DMA_HIFCR_CTCIF4                     ((uint32_t)0x00000020)
#define DMA_HIFCR_CHTIF4                     ((uint32_t)0x00000010)
#define DMA_HIFCR_CTEIF4                     ((uint32_t)0x00000008)
#define DMA_HIFCR_CDMEIF4                    ((uint32_t)0x00000004)
#define DMA_HIFCR_CFEIF4                     ((uint32_t)0x00000001)

/**
  * @brief DMA Controller
  */

typedef struct
{
  __IO uint32_t CR;     /*!< DMA stream x configuration register      */
  __IO uint32_t NDTR;   /*!< DMA stream x number of data register     */
  __IO uint32_t PAR;    /*!< DMA stream x peripheral address register */
  __IO uint32_t M0AR;   /*!< DMA stream x memory 0 address register   */
  __IO uint32_t M1AR;   /*!< DMA stream x memory 1 address register   */
  __IO uint32_t FCR;    /*!< DMA stream x FIFO control register       */
} DMA_Stream_TypeDef;

typedef struct
{
  __IO uint32_t LISR;   /*!< DMA low interrupt status register,      Address offset: 0x00 */
  __IO uint32_t HISR;   /*!< DMA high interrupt status register,     Address offset: 0x04 */
  __IO uint32_t LIFCR;  /*!< DMA low interrupt flag clear register,  Address offset: 0x08 */
  __IO uint32_t HIFCR;  /*!< DMA high interrupt flag clear register, Address offset: 0x0C */
} DMA_TypeDef;

#define DMA1_BASE             (AHB1PERIPH_BASE + 0x6000)
#define DMA1_Stream0_BASE     (DMA1_BASE + 0x010)
#define DMA1_Stream1_BASE     (DMA1_BASE + 0x028)
#define DMA1_Stream2_BASE     (DMA1_BASE + 0x040)
#define DMA1_Stream3_BASE     (DMA1_BASE + 0x058)
#define DMA1_Stream4_BASE     (DMA1_BASE + 0x070)
#define DMA1_Stream5_BASE     (DMA1_BASE + 0x088)
#define DMA1_Stream6_BASE     (DMA1_BASE + 0x0A0)
#define DMA1_Stream7_BASE     (DMA1_BASE + 0x0B8)
#define DMA2_BASE             (AHB1PERIPH_BASE + 0x6400)
#define DMA2_Stream0_BASE     (DMA2_BASE + 0x010)
#define DMA2_Stream1_BASE     (DMA2_BASE + 0x028)
#define DMA2_Stream2_BASE     (DMA2_BASE + 0x040)
#define DMA2_Stream3_BASE     (DMA2_BASE + 0x058)
#define DMA2_Stream4_BASE     (DMA2_BASE + 0x070)
#define DMA2_Stream5_BASE     (DMA2_BASE + 0x088)
#define DMA2_Stream6_BASE     (DMA2_BASE + 0x0A0)
#define DMA2_Stream7_BASE     (DMA2_BASE + 0x0B8)


#define DMA1                ((DMA_TypeDef *) DMA1_BASE)
#define DMA1_Stream0        ((DMA_Stream_TypeDef *) DMA1_Stream0_BASE)
#define DMA1_Stream1        ((DMA_Stream_TypeDef *) DMA1_Stream1_BASE)
#define DMA1_Stream2        ((DMA_Stream_TypeDef *) DMA1_Stream2_BASE)
#define DMA1_Stream3        ((DMA_Stream_TypeDef *) DMA1_Stream3_BASE)
#define DMA1_Stream4        ((DMA_Stream_TypeDef *) DMA1_Stream4_BASE)
#define DMA1_Stream5        ((DMA_Stream_TypeDef *) DMA1_Stream5_BASE)
#define DMA1_Stream6        ((DMA_Stream_TypeDef *) DMA1_Stream6_BASE)
#define DMA1_Stream7        ((DMA_Stream_TypeDef *) DMA1_Stream7_BASE)
#define DMA2                ((DMA_TypeDef *) DMA2_BASE)
#define DMA2_Stream0        ((DMA_Stream_TypeDef *) DMA2_Stream0_BASE)
#define DMA2_Stream1        ((DMA_Stream_TypeDef *) DMA2_Stream1_BASE)
#define DMA2_Stream2        ((DMA_Stream_TypeDef *) DMA2_Stream2_BASE)
#define DMA2_Stream3        ((DMA_Stream_TypeDef *) DMA2_Stream3_BASE)
#define DMA2_Stream4        ((DMA_Stream_TypeDef *) DMA2_Stream4_BASE)
#define DMA2_Stream5        ((DMA_Stream_TypeDef *) DMA2_Stream5_BASE)
#define DMA2_Stream6        ((DMA_Stream_TypeDef *) DMA2_Stream6_BASE)
#define DMA2_Stream7        ((DMA_Stream_TypeDef *) DMA2_Stream7_BASE)


#endif /* HAL_ST_STM32F4_ST_DMA_H_ */
