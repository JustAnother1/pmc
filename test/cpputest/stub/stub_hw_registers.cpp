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

#include <inttypes.h>

#include "st_gpio.h"
#include "st_rcc.h"
#include "st_usart.h"
#include "st_common.h"


// GPIO
GPIO_TypeDef GPIOA_BASE;
GPIO_TypeDef GPIOB_BASE;
GPIO_TypeDef GPIOC_BASE;
GPIO_TypeDef GPIOD_BASE;
GPIO_TypeDef GPIOE_BASE;
GPIO_TypeDef GPIOF_BASE;
GPIO_TypeDef GPIOG_BASE;
GPIO_TypeDef GPIOH_BASE;
GPIO_TypeDef GPIOI_BASE;

// RCC
RCC_TypeDef RCC_BASE;

// UART
USART_TypeDef USART1_BASE;
USART_TypeDef USART2_BASE;
USART_TypeDef USART3_BASE;
USART_TypeDef UART4_BASE;
USART_TypeDef UART5_BASE;
USART_TypeDef USART6_BASE;

// common
// SPI_TypeDef I2S2ext_BASE;
// SPI_TypeDef I2S3ext_BASE;
// CAN_TypeDef CAN1_BASE;
// CAN_TypeDef CAN2_BASE;
PWR_TypeDef PWR_BASE;
DAC_TypeDef DAC_BASE;
SDIO_TypeDef SDIO_BASE;
// CRC_TypeDef CRC_BASE;
// ETH_TypeDef ETH_BASE;
// DCMI_TypeDef DCMI_BASE;
RNG_TypeDef RNG_BASE;
DBGMCU_TypeDef DBGMCU_BASE;



