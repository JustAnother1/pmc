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

#ifndef HAL_COMMON_H_
#define HAL_COMMON_H_

#include <inttypes.h>

typedef enum
{
  RESET = 0,
  SET = !RESET
} FlagStatus, ITStatus;

typedef enum
{
  DISABLE = 0,
  ENABLE = !DISABLE
} FunctionalState;
#define IS_FUNCTIONAL_STATE(STATE) (((STATE) == DISABLE) || ((STATE) == ENABLE))

typedef enum
{
  ERROR = 0,
  SUCCESS = !ERROR
} ErrorStatus;


/** @addtogroup Exported_macro
  * @{
  */
#define SET_BIT(REG, BIT)     ((REG) |= (BIT))

#define CLEAR_BIT(REG, BIT)   ((REG) &= ~(BIT))

#define READ_BIT(REG, BIT)    ((REG) & (BIT))

#define CLEAR_REG(REG)        ((REG) = (0x0))

#define WRITE_REG(REG, VAL)   ((REG) = (VAL))

#define READ_REG(REG)         ((REG))

#define MODIFY_REG(REG, CLEARMASK, SETMASK)  WRITE_REG((REG), (((READ_REG(REG)) & (~(CLEARMASK))) | (SETMASK)))

#define POSITION_VAL(VAL)     (__CLZ(__RBIT(VAL)))

/**
  * @brief Configuration of the Cortex-M4 Processor and Core Peripherals
  */
#define __CM4_REV                 0x0001  /*!< Core revision r0p1                            */
#define __MPU_PRESENT             1       /*!< STM32F4XX provides an MPU                     */
#define __NVIC_PRIO_BITS          4       /*!< STM32F4XX uses 4 Bits for the Priority Levels */
#define __Vendor_SysTickConfig    0       /*!< Set to 1 if different SysTick Config is used  */
#define __FPU_PRESENT             1       /*!< FPU present                                   */


/**
 * @brief STM32F4XX Interrupt Number Definition, according to the selected device
 *        in @ref Library_configuration_section
 */
typedef enum
{
/******  Cortex-M4 Processor Exceptions Numbers ****************************************************************/
  NonMaskableInt_IRQn         = -14,    /*!< 2 Non Maskable Interrupt                                          */
  MemoryManagement_IRQn       = -12,    /*!< 4 Cortex-M4 Memory Management Interrupt                           */
  BusFault_IRQn               = -11,    /*!< 5 Cortex-M4 Bus Fault Interrupt                                   */
  UsageFault_IRQn             = -10,    /*!< 6 Cortex-M4 Usage Fault Interrupt                                 */
  SVCall_IRQn                 = -5,     /*!< 11 Cortex-M4 SV Call Interrupt                                    */
  DebugMonitor_IRQn           = -4,     /*!< 12 Cortex-M4 Debug Monitor Interrupt                              */
  PendSV_IRQn                 = -2,     /*!< 14 Cortex-M4 Pend SV Interrupt                                    */
  SysTick_IRQn                = -1,     /*!< 15 Cortex-M4 System Tick Interrupt                                */
/******  STM32 specific Interrupt Numbers **********************************************************************/
  WWDG_IRQn                   = 0,      /*!< Window WatchDog Interrupt                                         */
  PVD_IRQn                    = 1,      /*!< PVD through EXTI Line detection Interrupt                         */
  TAMP_STAMP_IRQn             = 2,      /*!< Tamper and TimeStamp interrupts through the EXTI line             */
  RTC_WKUP_IRQn               = 3,      /*!< RTC Wakeup interrupt through the EXTI line                        */
  FLASH_IRQn                  = 4,      /*!< FLASH global Interrupt                                            */
  RCC_IRQn                    = 5,      /*!< RCC global Interrupt                                              */
  EXTI0_IRQn                  = 6,      /*!< EXTI Line0 Interrupt                                              */
  EXTI1_IRQn                  = 7,      /*!< EXTI Line1 Interrupt                                              */
  EXTI2_IRQn                  = 8,      /*!< EXTI Line2 Interrupt                                              */
  EXTI3_IRQn                  = 9,      /*!< EXTI Line3 Interrupt                                              */
  EXTI4_IRQn                  = 10,     /*!< EXTI Line4 Interrupt                                              */
  DMA1_Stream0_IRQn           = 11,     /*!< DMA1 Stream 0 global Interrupt                                    */
  DMA1_Stream1_IRQn           = 12,     /*!< DMA1 Stream 1 global Interrupt                                    */
  DMA1_Stream2_IRQn           = 13,     /*!< DMA1 Stream 2 global Interrupt                                    */
  DMA1_Stream3_IRQn           = 14,     /*!< DMA1 Stream 3 global Interrupt                                    */
  DMA1_Stream4_IRQn           = 15,     /*!< DMA1 Stream 4 global Interrupt                                    */
  DMA1_Stream5_IRQn           = 16,     /*!< DMA1 Stream 5 global Interrupt                                    */
  DMA1_Stream6_IRQn           = 17,     /*!< DMA1 Stream 6 global Interrupt                                    */
  ADC_IRQn                    = 18,     /*!< ADC1, ADC2 and ADC3 global Interrupts                             */
  CAN1_TX_IRQn                = 19,     /*!< CAN1 TX Interrupt                                                 */
  CAN1_RX0_IRQn               = 20,     /*!< CAN1 RX0 Interrupt                                                */
  CAN1_RX1_IRQn               = 21,     /*!< CAN1 RX1 Interrupt                                                */
  CAN1_SCE_IRQn               = 22,     /*!< CAN1 SCE Interrupt                                                */
  EXTI9_5_IRQn                = 23,     /*!< External Line[9:5] Interrupts                                     */
  TIM1_BRK_TIM9_IRQn          = 24,     /*!< TIM1 Break interrupt and TIM9 global interrupt                    */
  TIM1_UP_TIM10_IRQn          = 25,     /*!< TIM1 Update Interrupt and TIM10 global interrupt                  */
  TIM1_TRG_COM_TIM11_IRQn     = 26,     /*!< TIM1 Trigger and Commutation Interrupt and TIM11 global interrupt */
  TIM1_CC_IRQn                = 27,     /*!< TIM1 Capture Compare Interrupt                                    */
  TIM2_IRQn                   = 28,     /*!< TIM2 global Interrupt                                             */
  TIM3_IRQn                   = 29,     /*!< TIM3 global Interrupt                                             */
  TIM4_IRQn                   = 30,     /*!< TIM4 global Interrupt                                             */
  I2C1_EV_IRQn                = 31,     /*!< I2C1 Event Interrupt                                              */
  I2C1_ER_IRQn                = 32,     /*!< I2C1 Error Interrupt                                              */
  I2C2_EV_IRQn                = 33,     /*!< I2C2 Event Interrupt                                              */
  I2C2_ER_IRQn                = 34,     /*!< I2C2 Error Interrupt                                              */
  SPI1_IRQn                   = 35,     /*!< SPI1 global Interrupt                                             */
  SPI2_IRQn                   = 36,     /*!< SPI2 global Interrupt                                             */
  USART1_IRQn                 = 37,     /*!< USART1 global Interrupt                                           */
  USART2_IRQn                 = 38,     /*!< USART2 global Interrupt                                           */
  USART3_IRQn                 = 39,     /*!< USART3 global Interrupt                                           */
  EXTI15_10_IRQn              = 40,     /*!< External Line[15:10] Interrupts                                   */
  RTC_Alarm_IRQn              = 41,     /*!< RTC Alarm (A and B) through EXTI Line Interrupt                   */
  OTG_FS_WKUP_IRQn            = 42,     /*!< USB OTG FS Wakeup through EXTI line interrupt                     */
  TIM8_BRK_TIM12_IRQn         = 43,     /*!< TIM8 Break Interrupt and TIM12 global interrupt                   */
  TIM8_UP_TIM13_IRQn          = 44,     /*!< TIM8 Update Interrupt and TIM13 global interrupt                  */
  TIM8_TRG_COM_TIM14_IRQn     = 45,     /*!< TIM8 Trigger and Commutation Interrupt and TIM14 global interrupt */
  TIM8_CC_IRQn                = 46,     /*!< TIM8 Capture Compare Interrupt                                    */
  DMA1_Stream7_IRQn           = 47,     /*!< DMA1 Stream7 Interrupt                                            */
  FSMC_IRQn                   = 48,     /*!< FSMC global Interrupt                                             */
  SDIO_IRQn                   = 49,     /*!< SDIO global Interrupt                                             */
  TIM5_IRQn                   = 50,     /*!< TIM5 global Interrupt                                             */
  SPI3_IRQn                   = 51,     /*!< SPI3 global Interrupt                                             */
  UART4_IRQn                  = 52,     /*!< UART4 global Interrupt                                            */
  UART5_IRQn                  = 53,     /*!< UART5 global Interrupt                                            */
  TIM6_DAC_IRQn               = 54,     /*!< TIM6 global and DAC1&2 underrun error  interrupts                 */
  TIM7_IRQn                   = 55,     /*!< TIM7 global interrupt                                             */
  DMA2_Stream0_IRQn           = 56,     /*!< DMA2 Stream 0 global Interrupt                                    */
  DMA2_Stream1_IRQn           = 57,     /*!< DMA2 Stream 1 global Interrupt                                    */
  DMA2_Stream2_IRQn           = 58,     /*!< DMA2 Stream 2 global Interrupt                                    */
  DMA2_Stream3_IRQn           = 59,     /*!< DMA2 Stream 3 global Interrupt                                    */
  DMA2_Stream4_IRQn           = 60,     /*!< DMA2 Stream 4 global Interrupt                                    */
  ETH_IRQn                    = 61,     /*!< Ethernet global Interrupt                                         */
  ETH_WKUP_IRQn               = 62,     /*!< Ethernet Wakeup through EXTI line Interrupt                       */
  CAN2_TX_IRQn                = 63,     /*!< CAN2 TX Interrupt                                                 */
  CAN2_RX0_IRQn               = 64,     /*!< CAN2 RX0 Interrupt                                                */
  CAN2_RX1_IRQn               = 65,     /*!< CAN2 RX1 Interrupt                                                */
  CAN2_SCE_IRQn               = 66,     /*!< CAN2 SCE Interrupt                                                */
  OTG_FS_IRQn                 = 67,     /*!< USB OTG FS global Interrupt                                       */
  DMA2_Stream5_IRQn           = 68,     /*!< DMA2 Stream 5 global interrupt                                    */
  DMA2_Stream6_IRQn           = 69,     /*!< DMA2 Stream 6 global interrupt                                    */
  DMA2_Stream7_IRQn           = 70,     /*!< DMA2 Stream 7 global interrupt                                    */
  USART6_IRQn                 = 71,     /*!< USART6 global interrupt                                           */
  I2C3_EV_IRQn                = 72,     /*!< I2C3 event interrupt                                              */
  I2C3_ER_IRQn                = 73,     /*!< I2C3 error interrupt                                              */
  OTG_HS_EP1_OUT_IRQn         = 74,     /*!< USB OTG HS End Point 1 Out global interrupt                       */
  OTG_HS_EP1_IN_IRQn          = 75,     /*!< USB OTG HS End Point 1 In global interrupt                        */
  OTG_HS_WKUP_IRQn            = 76,     /*!< USB OTG HS Wakeup through EXTI interrupt                          */
  OTG_HS_IRQn                 = 77,     /*!< USB OTG HS global interrupt                                       */
  DCMI_IRQn                   = 78,     /*!< DCMI global interrupt                                             */
  HASH_RNG_IRQn               = 80,     /*!< Hash and RNG global interrupt                                     */
  FPU_IRQn                    = 81      /*!< FPU global interrupt                                              */
} IRQn_Type;


#include "core_cm4.h"             /* Cortex-M4 processor and core peripherals */


/**
  * @brief Analog to Digital Converter
  */

typedef struct
{
  __IO uint_fast32_t SR;     /*!< ADC status register,                         Address offset: 0x00 */
  __IO uint_fast32_t CR1;    /*!< ADC control register 1,                      Address offset: 0x04 */
  __IO uint_fast32_t CR2;    /*!< ADC control register 2,                      Address offset: 0x08 */
  __IO uint_fast32_t SMPR1;  /*!< ADC sample time register 1,                  Address offset: 0x0C */
  __IO uint_fast32_t SMPR2;  /*!< ADC sample time register 2,                  Address offset: 0x10 */
  __IO uint_fast32_t JOFR1;  /*!< ADC injected channel data offset register 1, Address offset: 0x14 */
  __IO uint_fast32_t JOFR2;  /*!< ADC injected channel data offset register 2, Address offset: 0x18 */
  __IO uint_fast32_t JOFR3;  /*!< ADC injected channel data offset register 3, Address offset: 0x1C */
  __IO uint_fast32_t JOFR4;  /*!< ADC injected channel data offset register 4, Address offset: 0x20 */
  __IO uint_fast32_t HTR;    /*!< ADC watchdog higher threshold register,      Address offset: 0x24 */
  __IO uint_fast32_t LTR;    /*!< ADC watchdog lower threshold register,       Address offset: 0x28 */
  __IO uint_fast32_t SQR1;   /*!< ADC regular sequence register 1,             Address offset: 0x2C */
  __IO uint_fast32_t SQR2;   /*!< ADC regular sequence register 2,             Address offset: 0x30 */
  __IO uint_fast32_t SQR3;   /*!< ADC regular sequence register 3,             Address offset: 0x34 */
  __IO uint_fast32_t JSQR;   /*!< ADC injected sequence register,              Address offset: 0x38*/
  __IO uint_fast32_t JDR1;   /*!< ADC injected data register 1,                Address offset: 0x3C */
  __IO uint_fast32_t JDR2;   /*!< ADC injected data register 2,                Address offset: 0x40 */
  __IO uint_fast32_t JDR3;   /*!< ADC injected data register 3,                Address offset: 0x44 */
  __IO uint_fast32_t JDR4;   /*!< ADC injected data register 4,                Address offset: 0x48 */
  __IO uint_fast32_t DR;     /*!< ADC regular data register,                   Address offset: 0x4C */
} ADC_TypeDef;

typedef struct
{
  __IO uint_fast32_t CSR;    /*!< ADC Common status register,                  Address offset: ADC1 base address + 0x300 */
  __IO uint_fast32_t CCR;    /*!< ADC common control register,                 Address offset: ADC1 base address + 0x304 */
  __IO uint_fast32_t CDR;    /*!< ADC common regular data register for dual
                             AND triple modes,                            Address offset: ADC1 base address + 0x308 */
} ADC_Common_TypeDef;

/**
  * @brief Digital to Analog Converter
  */

typedef struct
{
  __IO uint_fast32_t CR;       /*!< DAC control register,                                    Address offset: 0x00 */
  __IO uint_fast32_t SWTRIGR;  /*!< DAC software trigger register,                           Address offset: 0x04 */
  __IO uint_fast32_t DHR12R1;  /*!< DAC channel1 12-bit right-aligned data holding register, Address offset: 0x08 */
  __IO uint_fast32_t DHR12L1;  /*!< DAC channel1 12-bit left aligned data holding register,  Address offset: 0x0C */
  __IO uint_fast32_t DHR8R1;   /*!< DAC channel1 8-bit right aligned data holding register,  Address offset: 0x10 */
  __IO uint_fast32_t DHR12R2;  /*!< DAC channel2 12-bit right aligned data holding register, Address offset: 0x14 */
  __IO uint_fast32_t DHR12L2;  /*!< DAC channel2 12-bit left aligned data holding register,  Address offset: 0x18 */
  __IO uint_fast32_t DHR8R2;   /*!< DAC channel2 8-bit right-aligned data holding register,  Address offset: 0x1C */
  __IO uint_fast32_t DHR12RD;  /*!< Dual DAC 12-bit right-aligned data holding register,     Address offset: 0x20 */
  __IO uint_fast32_t DHR12LD;  /*!< DUAL DAC 12-bit left aligned data holding register,      Address offset: 0x24 */
  __IO uint_fast32_t DHR8RD;   /*!< DUAL DAC 8-bit right aligned data holding register,      Address offset: 0x28 */
  __IO uint_fast32_t DOR1;     /*!< DAC channel1 data output register,                       Address offset: 0x2C */
  __IO uint_fast32_t DOR2;     /*!< DAC channel2 data output register,                       Address offset: 0x30 */
  __IO uint_fast32_t SR;       /*!< DAC status register,                                     Address offset: 0x34 */
} DAC_TypeDef;

/**
  * @brief Debug MCU
  */

typedef struct
{
  __IO uint_fast32_t IDCODE;  /*!< MCU device ID code,               Address offset: 0x00 */
  __IO uint_fast32_t CR;      /*!< Debug MCU configuration register, Address offset: 0x04 */
  __IO uint_fast32_t APB1FZ;  /*!< Debug MCU APB1 freeze register,   Address offset: 0x08 */
  __IO uint_fast32_t APB2FZ;  /*!< Debug MCU APB2 freeze register,   Address offset: 0x0C */
}DBGMCU_TypeDef;



/**
  * @brief DMA Controller
  */

typedef struct
{
  __IO uint_fast32_t CR;     /*!< DMA stream x configuration register      */
  __IO uint_fast32_t NDTR;   /*!< DMA stream x number of data register     */
  __IO uint_fast32_t PAR;    /*!< DMA stream x peripheral address register */
  __IO uint_fast32_t M0AR;   /*!< DMA stream x memory 0 address register   */
  __IO uint_fast32_t M1AR;   /*!< DMA stream x memory 1 address register   */
  __IO uint_fast32_t FCR;    /*!< DMA stream x FIFO control register       */
} DMA_Stream_TypeDef;

typedef struct
{
  __IO uint_fast32_t LISR;   /*!< DMA low interrupt status register,      Address offset: 0x00 */
  __IO uint_fast32_t HISR;   /*!< DMA high interrupt status register,     Address offset: 0x04 */
  __IO uint_fast32_t LIFCR;  /*!< DMA low interrupt flag clear register,  Address offset: 0x08 */
  __IO uint_fast32_t HIFCR;  /*!< DMA high interrupt flag clear register, Address offset: 0x0C */
} DMA_TypeDef;



/**
  * @brief External Interrupt/Event Controller
  */

typedef struct
{
  __IO uint_fast32_t IMR;    /*!< EXTI Interrupt mask register,            Address offset: 0x00 */
  __IO uint_fast32_t EMR;    /*!< EXTI Event mask register,                Address offset: 0x04 */
  __IO uint_fast32_t RTSR;   /*!< EXTI Rising trigger selection register,  Address offset: 0x08 */
  __IO uint_fast32_t FTSR;   /*!< EXTI Falling trigger selection register, Address offset: 0x0C */
  __IO uint_fast32_t SWIER;  /*!< EXTI Software interrupt event register,  Address offset: 0x10 */
  __IO uint_fast32_t PR;     /*!< EXTI Pending register,                   Address offset: 0x14 */
} EXTI_TypeDef;


/**
  * @brief System configuration controller
  */

typedef struct
{
  __IO uint_fast32_t MEMRMP;       /*!< SYSCFG memory remap register,                      Address offset: 0x00      */
  __IO uint_fast32_t PMC;          /*!< SYSCFG peripheral mode configuration register,     Address offset: 0x04      */
  __IO uint_fast32_t EXTICR[4];    /*!< SYSCFG external interrupt configuration registers, Address offset: 0x08-0x14 */
  uint_fast32_t      RESERVED[2];  /*!< Reserved, 0x18-0x1C                                                          */
  __IO uint_fast32_t CMPCR;        /*!< SYSCFG Compensation cell control register,         Address offset: 0x20      */
} SYSCFG_TypeDef;

/**
  * @brief Inter-integrated Circuit Interface
  */

typedef struct
{
  __IO uint_fast32_t CR1;        /*!< I2C Control register 1,     Address offset: 0x00 */
  __IO uint_fast32_t CR2;        /*!< I2C Control register 2,     Address offset: 0x04 */
  __IO uint_fast32_t OAR1;       /*!< I2C Own address register 1, Address offset: 0x08 */
  __IO uint_fast32_t OAR2;       /*!< I2C Own address register 2, Address offset: 0x0C */
  __IO uint_fast32_t DR;         /*!< I2C Data register,          Address offset: 0x10 */
  __IO uint_fast32_t SR1;        /*!< I2C Status register 1,      Address offset: 0x14 */
  __IO uint_fast32_t SR2;        /*!< I2C Status register 2,      Address offset: 0x18 */
  __IO uint_fast32_t CCR;        /*!< I2C Clock control register, Address offset: 0x1C */
  __IO uint_fast32_t TRISE;      /*!< I2C TRISE register,         Address offset: 0x20 */
  __IO uint_fast32_t FLTR;       /*!< I2C FLTR register,          Address offset: 0x24 */
} I2C_TypeDef;

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
  * @brief Power Control
  */

typedef struct
{
  __IO uint_fast32_t CR;   /*!< PWR power control register,        Address offset: 0x00 */
  __IO uint_fast32_t CSR;  /*!< PWR power control/status register, Address offset: 0x04 */
} PWR_TypeDef;

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


/**
  * @brief SD host Interface
  */

typedef struct
{
  __IO uint_fast32_t POWER;          /*!< SDIO power control register,    Address offset: 0x00 */
  __IO uint_fast32_t CLKCR;          /*!< SDI clock control register,     Address offset: 0x04 */
  __IO uint_fast32_t ARG;            /*!< SDIO argument register,         Address offset: 0x08 */
  __IO uint_fast32_t CMD;            /*!< SDIO command register,          Address offset: 0x0C */
  __I uint_fast32_t  RESPCMD;        /*!< SDIO command response register, Address offset: 0x10 */
  __I uint_fast32_t  RESP1;          /*!< SDIO response 1 register,       Address offset: 0x14 */
  __I uint_fast32_t  RESP2;          /*!< SDIO response 2 register,       Address offset: 0x18 */
  __I uint_fast32_t  RESP3;          /*!< SDIO response 3 register,       Address offset: 0x1C */
  __I uint_fast32_t  RESP4;          /*!< SDIO response 4 register,       Address offset: 0x20 */
  __IO uint_fast32_t DTIMER;         /*!< SDIO data timer register,       Address offset: 0x24 */
  __IO uint_fast32_t DLEN;           /*!< SDIO data length register,      Address offset: 0x28 */
  __IO uint_fast32_t DCTRL;          /*!< SDIO data control register,     Address offset: 0x2C */
  __I uint_fast32_t  DCOUNT;         /*!< SDIO data counter register,     Address offset: 0x30 */
  __I uint_fast32_t  STA;            /*!< SDIO status register,           Address offset: 0x34 */
  __IO uint_fast32_t ICR;            /*!< SDIO interrupt clear register,  Address offset: 0x38 */
  __IO uint_fast32_t MASK;           /*!< SDIO mask register,             Address offset: 0x3C */
  uint_fast32_t      RESERVED0[2];   /*!< Reserved, 0x40-0x44                                  */
  __I uint_fast32_t  FIFOCNT;        /*!< SDIO FIFO counter register,     Address offset: 0x48 */
  uint_fast32_t      RESERVED1[13];  /*!< Reserved, 0x4C-0x7C                                  */
  __IO uint_fast32_t FIFO;           /*!< SDIO data FIFO register,        Address offset: 0x80 */
} SDIO_TypeDef;

/**
  * @brief Serial Peripheral Interface
  */

typedef struct
{
  __IO uint_fast32_t CR1;        /*!< SPI control register 1 (not used in I2S mode),      Address offset: 0x00 */
  __IO uint_fast32_t CR2;        /*!< SPI control register 2,                             Address offset: 0x04 */
  __IO uint_fast32_t SR;         /*!< SPI status register,                                Address offset: 0x08 */
  __IO uint_fast32_t DR;         /*!< SPI data register,                                  Address offset: 0x0C */
  __IO uint_fast32_t CRCPR;      /*!< SPI CRC polynomial register (not used in I2S mode), Address offset: 0x10 */
  __IO uint_fast32_t RXCRCR;     /*!< SPI RX CRC register (not used in I2S mode),         Address offset: 0x14 */
  __IO uint_fast32_t TXCRCR;     /*!< SPI TX CRC register (not used in I2S mode),         Address offset: 0x18 */
  __IO uint_fast32_t I2SCFGR;    /*!< SPI_I2S configuration register,                     Address offset: 0x1C */
  __IO uint_fast32_t I2SPR;      /*!< SPI_I2S prescaler register,                         Address offset: 0x20 */
} SPI_TypeDef;

/**
  * @brief TIM
  */

typedef struct
{
  __IO uint_fast32_t CR1;         /*!< TIM control register 1,              Address offset: 0x00 */
  __IO uint_fast32_t CR2;         /*!< TIM control register 2,              Address offset: 0x04 */
  __IO uint_fast32_t SMCR;        /*!< TIM slave mode control register,     Address offset: 0x08 */
  __IO uint_fast32_t DIER;        /*!< TIM DMA/interrupt enable register,   Address offset: 0x0C */
  __IO uint_fast32_t SR;          /*!< TIM status register,                 Address offset: 0x10 */
  __IO uint_fast32_t EGR;         /*!< TIM event generation register,       Address offset: 0x14 */
  __IO uint_fast32_t CCMR1;       /*!< TIM capture/compare mode register 1, Address offset: 0x18 */
  __IO uint_fast32_t CCMR2;       /*!< TIM capture/compare mode register 2, Address offset: 0x1C */
  __IO uint_fast32_t CCER;        /*!< TIM capture/compare enable register, Address offset: 0x20 */
  __IO uint_fast32_t CNT;         /*!< TIM counter register,                Address offset: 0x24 */
  __IO uint_fast32_t PSC;         /*!< TIM prescaler,                       Address offset: 0x28 */
  __IO uint_fast32_t ARR;         /*!< TIM auto-reload register,            Address offset: 0x2C */
  __IO uint_fast32_t RCR;         /*!< TIM repetition counter register,     Address offset: 0x30 */
  __IO uint_fast32_t CCR1;        /*!< TIM capture/compare register 1,      Address offset: 0x34 */
  __IO uint_fast32_t CCR2;        /*!< TIM capture/compare register 2,      Address offset: 0x38 */
  __IO uint_fast32_t CCR3;        /*!< TIM capture/compare register 3,      Address offset: 0x3C */
  __IO uint_fast32_t CCR4;        /*!< TIM capture/compare register 4,      Address offset: 0x40 */
  __IO uint_fast32_t BDTR;        /*!< TIM break and dead-time register,    Address offset: 0x44 */
  __IO uint_fast32_t DCR;         /*!< TIM DMA control register,            Address offset: 0x48 */
  __IO uint_fast32_t DMAR;        /*!< TIM DMA address for full transfer,   Address offset: 0x4C */
  __IO uint_fast32_t OR;          /*!< TIM option register,                 Address offset: 0x50 */
} TIM_TypeDef;

/**
  * @brief Universal Synchronous Asynchronous Receiver Transmitter
  */

typedef struct
{
  __IO uint_fast32_t SR;         /*!< USART Status register,                   Address offset: 0x00 */
  __IO uint_fast32_t DR;         /*!< USART Data register,                     Address offset: 0x04 */
  __IO uint_fast32_t BRR;        /*!< USART Baud rate register,                Address offset: 0x08 */
  __IO uint_fast32_t CR1;        /*!< USART Control register 1,                Address offset: 0x0C */
  __IO uint_fast32_t CR2;        /*!< USART Control register 2,                Address offset: 0x10 */
  __IO uint_fast32_t CR3;        /*!< USART Control register 3,                Address offset: 0x14 */
  __IO uint_fast32_t GTPR;       /*!< USART Guard time and prescaler register, Address offset: 0x18 */
} USART_TypeDef;

/**
  * @brief Window WATCHDOG
  */

typedef struct
{
  __IO uint_fast32_t CR;   /*!< WWDG Control register,       Address offset: 0x00 */
  __IO uint_fast32_t CFR;  /*!< WWDG Configuration register, Address offset: 0x04 */
  __IO uint_fast32_t SR;   /*!< WWDG Status register,        Address offset: 0x08 */
} WWDG_TypeDef;

/**
  * @brief RNG
  */

typedef struct
{
  __IO uint_fast32_t CR;  /*!< RNG control register, Address offset: 0x00 */
  __IO uint_fast32_t SR;  /*!< RNG status register,  Address offset: 0x04 */
  __IO uint_fast32_t DR;  /*!< RNG data register,    Address offset: 0x08 */
} RNG_TypeDef;



/**
  * @brief __USB_OTG_Core_register
  */
typedef struct
{
  __IO uint_fast32_t GOTGCTL;      /*!<  USB_OTG Control and Status Register    000h*/
  __IO uint_fast32_t GOTGINT;      /*!<  USB_OTG Interrupt Register             004h*/
  __IO uint_fast32_t GAHBCFG;      /*!<  Core AHB Configuration Register    008h*/
  __IO uint_fast32_t GUSBCFG;      /*!<  Core USB Configuration Register    00Ch*/
  __IO uint_fast32_t GRSTCTL;      /*!<  Core Reset Register                010h*/
  __IO uint_fast32_t GINTSTS;      /*!<  Core Interrupt Register            014h*/
  __IO uint_fast32_t GINTMSK;      /*!<  Core Interrupt Mask Register       018h*/
  __IO uint_fast32_t GRXSTSR;      /*!<  Receive Sts Q Read Register        01Ch*/
  __IO uint_fast32_t GRXSTSP;      /*!<  Receive Sts Q Read & POP Register  020h*/
  __IO uint_fast32_t GRXFSIZ;      /* Receive FIFO Size Register         024h*/
  __IO uint_fast32_t DIEPTXF0_HNPTXFSIZ;   /*!<  EP0 / Non Periodic Tx FIFO Size Register 028h*/
  __IO uint_fast32_t HNPTXSTS;     /*!<  Non Periodic Tx FIFO/Queue Sts reg 02Ch*/
  uint_fast32_t Reserved30[2];     /* Reserved                           030h*/
  __IO uint_fast32_t GCCFG;        /* General Purpose IO Register        038h*/
  __IO uint_fast32_t CID;          /* User ID Register                   03Ch*/
  uint_fast32_t  Reserved40[48];   /* Reserved                      040h-0FFh*/
  __IO uint_fast32_t HPTXFSIZ; /* Host Periodic Tx FIFO Size Reg     100h*/
  __IO uint_fast32_t DIEPTXF[0x0F];/* dev Periodic Transmit FIFO */
}
USB_OTG_GlobalTypeDef;



/**
  * @brief __device_Registers
  */
typedef struct
{
  __IO uint_fast32_t DCFG;         /* dev Configuration Register   800h*/
  __IO uint_fast32_t DCTL;         /* dev Control Register         804h*/
  __IO uint_fast32_t DSTS;         /* dev Status Register (RO)     808h*/
  uint_fast32_t Reserved0C;           /* Reserved                     80Ch*/
  __IO uint_fast32_t DIEPMSK;   /* dev IN Endpoint Mask         810h*/
  __IO uint_fast32_t DOEPMSK;  /* dev OUT Endpoint Mask        814h*/
  __IO uint_fast32_t DAINT;     /* dev All Endpoints Itr Reg    818h*/
  __IO uint_fast32_t DAINTMSK; /* dev All Endpoints Itr Mask   81Ch*/
  uint_fast32_t  Reserved20;          /* Reserved                     820h*/
  uint_fast32_t Reserved9;       /* Reserved                     824h*/
  __IO uint_fast32_t DVBUSDIS;    /* dev VBUS discharge Register  828h*/
  __IO uint_fast32_t DVBUSPULSE;  /* dev VBUS Pulse Register      82Ch*/
  __IO uint_fast32_t DTHRCTL;     /* dev thr                      830h*/
  __IO uint_fast32_t DIEPEMPMSK; /* dev empty msk             834h*/
  __IO uint_fast32_t DEACHINT;    /* dedicated EP interrupt       838h*/
  __IO uint_fast32_t DEACHMSK;    /* dedicated EP msk             83Ch*/
  uint_fast32_t Reserved40;      /* dedicated EP mask           840h*/
  __IO uint_fast32_t DINEP1MSK;  /* dedicated EP mask           844h*/
  uint_fast32_t  Reserved44[15];      /* Reserved                 844-87Ch*/
  __IO uint_fast32_t DOUTEP1MSK; /* dedicated EP msk            884h*/
}
USB_OTG_DeviceTypeDef;


/**
  * @brief __IN_Endpoint-Specific_Register
  */
typedef struct
{
  __IO uint_fast32_t DIEPCTL; /* dev IN Endpoint Control Reg 900h + (ep_num * 20h) + 00h*/
  uint_fast32_t Reserved04;             /* Reserved                       900h + (ep_num * 20h) + 04h*/
  __IO uint_fast32_t DIEPINT; /* dev IN Endpoint Itr Reg     900h + (ep_num * 20h) + 08h*/
  uint_fast32_t Reserved0C;             /* Reserved                       900h + (ep_num * 20h) + 0Ch*/
  __IO uint_fast32_t DIEPTSIZ; /* IN Endpoint Txfer Size   900h + (ep_num * 20h) + 10h*/
  __IO uint_fast32_t DIEPDMA; /* IN Endpoint DMA Address Reg    900h + (ep_num * 20h) + 14h*/
  __IO uint_fast32_t DTXFSTS;/*IN Endpoint Tx FIFO Status Reg 900h + (ep_num * 20h) + 18h*/
  uint_fast32_t Reserved18;             /* Reserved  900h+(ep_num*20h)+1Ch-900h+ (ep_num * 20h) + 1Ch*/
}
USB_OTG_INEndpointTypeDef;


/**
  * @brief __OUT_Endpoint-Specific_Registers
  */
typedef struct
{
  __IO uint_fast32_t DOEPCTL;       /* dev OUT Endpoint Control Reg  B00h + (ep_num * 20h) + 00h*/
  uint_fast32_t Reserved04;         /* Reserved                      B00h + (ep_num * 20h) + 04h*/
  __IO uint_fast32_t DOEPINT;       /* dev OUT Endpoint Itr Reg      B00h + (ep_num * 20h) + 08h*/
  uint_fast32_t Reserved0C;         /* Reserved                      B00h + (ep_num * 20h) + 0Ch*/
  __IO uint_fast32_t DOEPTSIZ;      /* dev OUT Endpoint Txfer Size   B00h + (ep_num * 20h) + 10h*/
  __IO uint_fast32_t DOEPDMA;       /* dev OUT Endpoint DMA Address  B00h + (ep_num * 20h) + 14h*/
  uint_fast32_t Reserved18[2];      /* Reserved B00h + (ep_num * 20h) + 18h - B00h + (ep_num * 20h) + 1Ch*/
}
USB_OTG_OUTEndpointTypeDef;


/**
  * @brief __Host_Mode_Register_Structures
  */
typedef struct
{
  __IO uint_fast32_t HCFG;             /* Host Configuration Register    400h*/
  __IO uint_fast32_t HFIR;      /* Host Frame Interval Register   404h*/
  __IO uint_fast32_t HFNUM;         /* Host Frame Nbr/Frame Remaining 408h*/
  uint_fast32_t Reserved40C;                   /* Reserved                       40Ch*/
  __IO uint_fast32_t HPTXSTS;   /* Host Periodic Tx FIFO/ Queue Status 410h*/
  __IO uint_fast32_t HAINT;   /* Host All Channels Interrupt Register 414h*/
  __IO uint_fast32_t HAINTMSK;   /* Host All Channels Interrupt Mask 418h*/
}
USB_OTG_HostTypeDef;


/**
  * @brief __Host_Channel_Specific_Registers
  */
typedef struct
{
  __IO uint_fast32_t HCCHAR;
  __IO uint_fast32_t HCSPLT;
  __IO uint_fast32_t HCINT;
  __IO uint_fast32_t HCINTMSK;
  __IO uint_fast32_t HCTSIZ;
  __IO uint_fast32_t HCDMA;
  uint_fast32_t Reserved[2];
}
USB_OTG_HostChannelTypeDef;


/**
  * @brief Peripheral_memory_map
  */
#define FLASH_BASE            ((uint_fast32_t)0x08000000) /*!< FLASH(up to 1 MB) base address in the alias region                         */
#define CCMDATARAM_BASE       ((uint_fast32_t)0x10000000) /*!< CCM(core coupled memory) data RAM(64 KB) base address in the alias region  */
#define SRAM1_BASE            ((uint_fast32_t)0x20000000) /*!< SRAM1(112 KB) base address in the alias region                             */
#define SRAM2_BASE            ((uint_fast32_t)0x2001C000) /*!< SRAM2(16 KB) base address in the alias region                              */
#define SRAM3_BASE            ((uint_fast32_t)0x20020000) /*!< SRAM3(64 KB) base address in the alias region                              */
#define PERIPH_BASE           ((uint_fast32_t)0x40000000) /*!< Peripheral base address in the alias region                                */
#define BKPSRAM_BASE          ((uint_fast32_t)0x40024000) /*!< Backup SRAM(4 KB) base address in the alias region                         */
#define FSMC_R_BASE           ((uint_fast32_t)0xA0000000) /*!< FSMC registers base address                                                */
#define CCMDATARAM_BB_BASE    ((uint_fast32_t)0x12000000) /*!< CCM(core coupled memory) data RAM(64 KB) base address in the bit-band region  */
#define SRAM1_BB_BASE         ((uint_fast32_t)0x22000000) /*!< SRAM1(112 KB) base address in the bit-band region                             */
#define SRAM2_BB_BASE         ((uint_fast32_t)0x2201C000) /*!< SRAM2(16 KB) base address in the bit-band region                              */
#define SRAM3_BB_BASE         ((uint_fast32_t)0x22020000) /*!< SRAM3(64 KB) base address in the bit-band region                              */
#define PERIPH_BB_BASE        ((uint_fast32_t)0x42000000) /*!< Peripheral base address in the bit-band region                                */
#define BKPSRAM_BB_BASE       ((uint_fast32_t)0x42024000) /*!< Backup SRAM(4 KB) base address in the bit-band region                         */
#define FLASH_END             ((uint_fast32_t)0x080FFFFF) /*!< FLASH end address */
#define CCMDATARAM_END        ((uint_fast32_t)0x1000FFFF) /*!< CCM data RAM end address */

/* Legacy defines */
#define SRAM_BASE             SRAM1_BASE
#define SRAM_BB_BASE          SRAM1_BB_BASE


/*!< Peripheral memory map */
#define APB1PERIPH_BASE       PERIPH_BASE
#define APB2PERIPH_BASE       (PERIPH_BASE + 0x00010000)
#define AHB1PERIPH_BASE       (PERIPH_BASE + 0x00020000)
#define AHB2PERIPH_BASE       (PERIPH_BASE + 0x10000000)

/*!< APB1 peripherals */
#define TIM2_BASE             (APB1PERIPH_BASE + 0x0000)
#define TIM3_BASE             (APB1PERIPH_BASE + 0x0400)
#define TIM4_BASE             (APB1PERIPH_BASE + 0x0800)
#define TIM5_BASE             (APB1PERIPH_BASE + 0x0C00)
#define TIM6_BASE             (APB1PERIPH_BASE + 0x1000)
#define TIM7_BASE             (APB1PERIPH_BASE + 0x1400)
#define TIM12_BASE            (APB1PERIPH_BASE + 0x1800)
#define TIM13_BASE            (APB1PERIPH_BASE + 0x1C00)
#define TIM14_BASE            (APB1PERIPH_BASE + 0x2000)
#define RTC_BASE              (APB1PERIPH_BASE + 0x2800)
#define WWDG_BASE             (APB1PERIPH_BASE + 0x2C00)
#define IWDG_BASE             (APB1PERIPH_BASE + 0x3000)
#define I2S2ext_BASE          (APB1PERIPH_BASE + 0x3400)
#define SPI2_BASE             (APB1PERIPH_BASE + 0x3800)
#define SPI3_BASE             (APB1PERIPH_BASE + 0x3C00)
#define I2S3ext_BASE          (APB1PERIPH_BASE + 0x4000)
#define USART2_BASE           (APB1PERIPH_BASE + 0x4400)
#define USART3_BASE           (APB1PERIPH_BASE + 0x4800)
#define UART4_BASE            (APB1PERIPH_BASE + 0x4C00)
#define UART5_BASE            (APB1PERIPH_BASE + 0x5000)
#define I2C1_BASE             (APB1PERIPH_BASE + 0x5400)
#define I2C2_BASE             (APB1PERIPH_BASE + 0x5800)
#define I2C3_BASE             (APB1PERIPH_BASE + 0x5C00)
#define CAN1_BASE             (APB1PERIPH_BASE + 0x6400)
#define CAN2_BASE             (APB1PERIPH_BASE + 0x6800)
#define PWR_BASE              (APB1PERIPH_BASE + 0x7000)
#define DAC_BASE              (APB1PERIPH_BASE + 0x7400)

/*!< APB2 peripherals */
#define TIM1_BASE             (APB2PERIPH_BASE + 0x0000)
#define TIM8_BASE             (APB2PERIPH_BASE + 0x0400)
#define USART1_BASE           (APB2PERIPH_BASE + 0x1000)
#define USART6_BASE           (APB2PERIPH_BASE + 0x1400)
#define ADC1_BASE             (APB2PERIPH_BASE + 0x2000)
#define ADC2_BASE             (APB2PERIPH_BASE + 0x2100)
#define ADC3_BASE             (APB2PERIPH_BASE + 0x2200)
#define ADC_BASE              (APB2PERIPH_BASE + 0x2300)
#define SDIO_BASE             (APB2PERIPH_BASE + 0x2C00)
#define SPI1_BASE             (APB2PERIPH_BASE + 0x3000)
#define SYSCFG_BASE           (APB2PERIPH_BASE + 0x3800)
#define EXTI_BASE             (APB2PERIPH_BASE + 0x3C00)
#define TIM9_BASE             (APB2PERIPH_BASE + 0x4000)
#define TIM10_BASE            (APB2PERIPH_BASE + 0x4400)
#define TIM11_BASE            (APB2PERIPH_BASE + 0x4800)

/*!< AHB1 peripherals */
#define GPIOA_BASE            (AHB1PERIPH_BASE + 0x0000)
#define GPIOB_BASE            (AHB1PERIPH_BASE + 0x0400)
#define GPIOC_BASE            (AHB1PERIPH_BASE + 0x0800)
#define GPIOD_BASE            (AHB1PERIPH_BASE + 0x0C00)
#define GPIOE_BASE            (AHB1PERIPH_BASE + 0x1000)
#define GPIOF_BASE            (AHB1PERIPH_BASE + 0x1400)
#define GPIOG_BASE            (AHB1PERIPH_BASE + 0x1800)
#define GPIOH_BASE            (AHB1PERIPH_BASE + 0x1C00)
#define GPIOI_BASE            (AHB1PERIPH_BASE + 0x2000)
#define CRC_BASE              (AHB1PERIPH_BASE + 0x3000)
#define RCC_BASE              (AHB1PERIPH_BASE + 0x3800)
#define FLASH_R_BASE          (AHB1PERIPH_BASE + 0x3C00)
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
#define ETH_BASE              (AHB1PERIPH_BASE + 0x8000)
#define ETH_MAC_BASE          (ETH_BASE)
#define ETH_MMC_BASE          (ETH_BASE + 0x0100)
#define ETH_PTP_BASE          (ETH_BASE + 0x0700)
#define ETH_DMA_BASE          (ETH_BASE + 0x1000)

/*!< AHB2 peripherals */
#define DCMI_BASE             (AHB2PERIPH_BASE + 0x50000)
#define RNG_BASE              (AHB2PERIPH_BASE + 0x60800)

/*!< FSMC Bankx registers base address */
#define FSMC_Bank1_R_BASE     (FSMC_R_BASE + 0x0000)
#define FSMC_Bank1E_R_BASE    (FSMC_R_BASE + 0x0104)
#define FSMC_Bank2_3_R_BASE   (FSMC_R_BASE + 0x0060)
#define FSMC_Bank4_R_BASE     (FSMC_R_BASE + 0x00A0)

/* Debug MCU registers base address */
#define DBGMCU_BASE           ((uint_fast32_t )0xE0042000)

/*!< USB registers base address */
#define USB_OTG_HS_PERIPH_BASE               ((uint_fast32_t )0x40040000)
#define USB_OTG_FS_PERIPH_BASE               ((uint_fast32_t )0x50000000)

#define USB_OTG_GLOBAL_BASE                  ((uint_fast32_t )0x000)
#define USB_OTG_DEVICE_BASE                  ((uint_fast32_t )0x800)
#define USB_OTG_IN_ENDPOINT_BASE             ((uint_fast32_t )0x900)
#define USB_OTG_OUT_ENDPOINT_BASE            ((uint_fast32_t )0xB00)
#define USB_OTG_EP_REG_SIZE                  ((uint_fast32_t )0x20)
#define USB_OTG_HOST_BASE                    ((uint_fast32_t )0x400)
#define USB_OTG_HOST_PORT_BASE               ((uint_fast32_t )0x440)
#define USB_OTG_HOST_CHANNEL_BASE            ((uint_fast32_t )0x500)
#define USB_OTG_HOST_CHANNEL_SIZE            ((uint_fast32_t )0x20)
#define USB_OTG_PCGCCTL_BASE                 ((uint_fast32_t )0xE00)
#define USB_OTG_FIFO_BASE                    ((uint_fast32_t )0x1000)
#define USB_OTG_FIFO_SIZE                    ((uint_fast32_t )0x1000)

/**
  * @}
  */

/** @addtogroup Peripheral_declaration
  * @{
  */
#define TIM2                ((TIM_TypeDef *) TIM2_BASE)
#define TIM3                ((TIM_TypeDef *) TIM3_BASE)
#define TIM4                ((TIM_TypeDef *) TIM4_BASE)
#define TIM5                ((TIM_TypeDef *) TIM5_BASE)
#define TIM6                ((TIM_TypeDef *) TIM6_BASE)
#define TIM7                ((TIM_TypeDef *) TIM7_BASE)
#define TIM12               ((TIM_TypeDef *) TIM12_BASE)
#define TIM13               ((TIM_TypeDef *) TIM13_BASE)
#define TIM14               ((TIM_TypeDef *) TIM14_BASE)
#define RTC                 ((RTC_TypeDef *) RTC_BASE)
#define WWDG                ((WWDG_TypeDef *) WWDG_BASE)
#define IWDG                ((IWDG_TypeDef *) IWDG_BASE)
#define I2S2ext             ((SPI_TypeDef *) I2S2ext_BASE)
#define SPI2                ((SPI_TypeDef *) SPI2_BASE)
#define SPI3                ((SPI_TypeDef *) SPI3_BASE)
#define I2S3ext             ((SPI_TypeDef *) I2S3ext_BASE)
#define USART2              ((USART_TypeDef *) USART2_BASE)
#define USART3              ((USART_TypeDef *) USART3_BASE)
#define UART4               ((USART_TypeDef *) UART4_BASE)
#define UART5               ((USART_TypeDef *) UART5_BASE)
#define I2C1                ((I2C_TypeDef *) I2C1_BASE)
#define I2C2                ((I2C_TypeDef *) I2C2_BASE)
#define I2C3                ((I2C_TypeDef *) I2C3_BASE)
#define CAN1                ((CAN_TypeDef *) CAN1_BASE)
#define CAN2                ((CAN_TypeDef *) CAN2_BASE)
#define PWR                 ((PWR_TypeDef *) PWR_BASE)
#define DAC                 ((DAC_TypeDef *) DAC_BASE)
#define TIM1                ((TIM_TypeDef *) TIM1_BASE)
#define TIM8                ((TIM_TypeDef *) TIM8_BASE)
#define USART1              ((USART_TypeDef *) USART1_BASE)
#define USART6              ((USART_TypeDef *) USART6_BASE)
#define ADC                 ((ADC_Common_TypeDef *) ADC_BASE)
#define ADC1                ((ADC_TypeDef *) ADC1_BASE)
#define ADC2                ((ADC_TypeDef *) ADC2_BASE)
#define ADC3                ((ADC_TypeDef *) ADC3_BASE)
#define SDIO                ((SDIO_TypeDef *) SDIO_BASE)
#define SPI1                ((SPI_TypeDef *) SPI1_BASE)
#define SYSCFG              ((SYSCFG_TypeDef *) SYSCFG_BASE)
#define EXTI                ((EXTI_TypeDef *) EXTI_BASE)
#define TIM9                ((TIM_TypeDef *) TIM9_BASE)
#define TIM10               ((TIM_TypeDef *) TIM10_BASE)
#define TIM11               ((TIM_TypeDef *) TIM11_BASE)
#define CRC                 ((CRC_TypeDef *) CRC_BASE)
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
#define ETH                 ((ETH_TypeDef *) ETH_BASE)
#define DCMI                ((DCMI_TypeDef *) DCMI_BASE)
#define RNG                 ((RNG_TypeDef *) RNG_BASE)
#define FSMC_Bank1          ((FSMC_Bank1_TypeDef *) FSMC_Bank1_R_BASE)
#define FSMC_Bank1E         ((FSMC_Bank1E_TypeDef *) FSMC_Bank1E_R_BASE)
#define FSMC_Bank2_3        ((FSMC_Bank2_3_TypeDef *) FSMC_Bank2_3_R_BASE)
#define FSMC_Bank4          ((FSMC_Bank4_TypeDef *) FSMC_Bank4_R_BASE)

#define DBGMCU              ((DBGMCU_TypeDef *) DBGMCU_BASE)

#define USB_OTG_FS          ((USB_OTG_GlobalTypeDef *) USB_OTG_FS_PERIPH_BASE)
#define USB_OTG_HS          ((USB_OTG_GlobalTypeDef *) USB_OTG_HS_PERIPH_BASE)

/**
  * @}
  */

/** @addtogroup Exported_constants
  * @{
  */

  /** @addtogroup Peripheral_Registers_Bits_Definition
  * @{
  */

/******************************************************************************/
/*                         Peripheral Registers_Bits_Definition               */
/******************************************************************************/

/******************************************************************************/
/*                                                                            */
/*                        Analog to Digital Converter                         */
/*                                                                            */
/******************************************************************************/
/********************  Bit definition for ADC_SR register  ********************/
#define  ADC_SR_AWD                          ((uint_fast32_t)0x00000001)       /*!<Analog watchdog flag */
#define  ADC_SR_EOC                          ((uint_fast32_t)0x00000002)       /*!<End of conversion */
#define  ADC_SR_JEOC                         ((uint_fast32_t)0x00000004)       /*!<Injected channel end of conversion */
#define  ADC_SR_JSTRT                        ((uint_fast32_t)0x00000008)       /*!<Injected channel Start flag */
#define  ADC_SR_STRT                         ((uint_fast32_t)0x00000010)       /*!<Regular channel Start flag */
#define  ADC_SR_OVR                          ((uint_fast32_t)0x00000020)       /*!<Overrun flag */

/*******************  Bit definition for ADC_CR1 register  ********************/
#define  ADC_CR1_AWDCH                       ((uint_fast32_t)0x0000001F)        /*!<AWDCH[4:0] bits (Analog watchdog channel select bits) */
#define  ADC_CR1_AWDCH_0                     ((uint_fast32_t)0x00000001)        /*!<Bit 0 */
#define  ADC_CR1_AWDCH_1                     ((uint_fast32_t)0x00000002)        /*!<Bit 1 */
#define  ADC_CR1_AWDCH_2                     ((uint_fast32_t)0x00000004)        /*!<Bit 2 */
#define  ADC_CR1_AWDCH_3                     ((uint_fast32_t)0x00000008)        /*!<Bit 3 */
#define  ADC_CR1_AWDCH_4                     ((uint_fast32_t)0x00000010)        /*!<Bit 4 */
#define  ADC_CR1_EOCIE                       ((uint_fast32_t)0x00000020)        /*!<Interrupt enable for EOC */
#define  ADC_CR1_AWDIE                       ((uint_fast32_t)0x00000040)        /*!<AAnalog Watchdog interrupt enable */
#define  ADC_CR1_JEOCIE                      ((uint_fast32_t)0x00000080)        /*!<Interrupt enable for injected channels */
#define  ADC_CR1_SCAN                        ((uint_fast32_t)0x00000100)        /*!<Scan mode */
#define  ADC_CR1_AWDSGL                      ((uint_fast32_t)0x00000200)        /*!<Enable the watchdog on a single channel in scan mode */
#define  ADC_CR1_JAUTO                       ((uint_fast32_t)0x00000400)        /*!<Automatic injected group conversion */
#define  ADC_CR1_DISCEN                      ((uint_fast32_t)0x00000800)        /*!<Discontinuous mode on regular channels */
#define  ADC_CR1_JDISCEN                     ((uint_fast32_t)0x00001000)        /*!<Discontinuous mode on injected channels */
#define  ADC_CR1_DISCNUM                     ((uint_fast32_t)0x0000E000)        /*!<DISCNUM[2:0] bits (Discontinuous mode channel count) */
#define  ADC_CR1_DISCNUM_0                   ((uint_fast32_t)0x00002000)        /*!<Bit 0 */
#define  ADC_CR1_DISCNUM_1                   ((uint_fast32_t)0x00004000)        /*!<Bit 1 */
#define  ADC_CR1_DISCNUM_2                   ((uint_fast32_t)0x00008000)        /*!<Bit 2 */
#define  ADC_CR1_JAWDEN                      ((uint_fast32_t)0x00400000)        /*!<Analog watchdog enable on injected channels */
#define  ADC_CR1_AWDEN                       ((uint_fast32_t)0x00800000)        /*!<Analog watchdog enable on regular channels */
#define  ADC_CR1_RES                         ((uint_fast32_t)0x03000000)        /*!<RES[2:0] bits (Resolution) */
#define  ADC_CR1_RES_0                       ((uint_fast32_t)0x01000000)        /*!<Bit 0 */
#define  ADC_CR1_RES_1                       ((uint_fast32_t)0x02000000)        /*!<Bit 1 */
#define  ADC_CR1_OVRIE                       ((uint_fast32_t)0x04000000)         /*!<overrun interrupt enable */

/*******************  Bit definition for ADC_CR2 register  ********************/
#define  ADC_CR2_ADON                        ((uint_fast32_t)0x00000001)        /*!<A/D Converter ON / OFF */
#define  ADC_CR2_CONT                        ((uint_fast32_t)0x00000002)        /*!<Continuous Conversion */
#define  ADC_CR2_DMA                         ((uint_fast32_t)0x00000100)        /*!<Direct Memory access mode */
#define  ADC_CR2_DDS                         ((uint_fast32_t)0x00000200)        /*!<DMA disable selection (Single ADC) */
#define  ADC_CR2_EOCS                        ((uint_fast32_t)0x00000400)        /*!<End of conversion selection */
#define  ADC_CR2_ALIGN                       ((uint_fast32_t)0x00000800)        /*!<Data Alignment */
#define  ADC_CR2_JEXTSEL                     ((uint_fast32_t)0x000F0000)        /*!<JEXTSEL[3:0] bits (External event select for injected group) */
#define  ADC_CR2_JEXTSEL_0                   ((uint_fast32_t)0x00010000)        /*!<Bit 0 */
#define  ADC_CR2_JEXTSEL_1                   ((uint_fast32_t)0x00020000)        /*!<Bit 1 */
#define  ADC_CR2_JEXTSEL_2                   ((uint_fast32_t)0x00040000)        /*!<Bit 2 */
#define  ADC_CR2_JEXTSEL_3                   ((uint_fast32_t)0x00080000)        /*!<Bit 3 */
#define  ADC_CR2_JEXTEN                      ((uint_fast32_t)0x00300000)        /*!<JEXTEN[1:0] bits (External Trigger Conversion mode for injected channelsp) */
#define  ADC_CR2_JEXTEN_0                    ((uint_fast32_t)0x00100000)        /*!<Bit 0 */
#define  ADC_CR2_JEXTEN_1                    ((uint_fast32_t)0x00200000)        /*!<Bit 1 */
#define  ADC_CR2_JSWSTART                    ((uint_fast32_t)0x00400000)        /*!<Start Conversion of injected channels */
#define  ADC_CR2_EXTSEL                      ((uint_fast32_t)0x0F000000)        /*!<EXTSEL[3:0] bits (External Event Select for regular group) */
#define  ADC_CR2_EXTSEL_0                    ((uint_fast32_t)0x01000000)        /*!<Bit 0 */
#define  ADC_CR2_EXTSEL_1                    ((uint_fast32_t)0x02000000)        /*!<Bit 1 */
#define  ADC_CR2_EXTSEL_2                    ((uint_fast32_t)0x04000000)        /*!<Bit 2 */
#define  ADC_CR2_EXTSEL_3                    ((uint_fast32_t)0x08000000)        /*!<Bit 3 */
#define  ADC_CR2_EXTEN                       ((uint_fast32_t)0x30000000)        /*!<EXTEN[1:0] bits (External Trigger Conversion mode for regular channelsp) */
#define  ADC_CR2_EXTEN_0                     ((uint_fast32_t)0x10000000)        /*!<Bit 0 */
#define  ADC_CR2_EXTEN_1                     ((uint_fast32_t)0x20000000)        /*!<Bit 1 */
#define  ADC_CR2_SWSTART                     ((uint_fast32_t)0x40000000)        /*!<Start Conversion of regular channels */

/******************  Bit definition for ADC_SMPR1 register  *******************/
#define  ADC_SMPR1_SMP10                     ((uint_fast32_t)0x00000007)        /*!<SMP10[2:0] bits (Channel 10 Sample time selection) */
#define  ADC_SMPR1_SMP10_0                   ((uint_fast32_t)0x00000001)        /*!<Bit 0 */
#define  ADC_SMPR1_SMP10_1                   ((uint_fast32_t)0x00000002)        /*!<Bit 1 */
#define  ADC_SMPR1_SMP10_2                   ((uint_fast32_t)0x00000004)        /*!<Bit 2 */
#define  ADC_SMPR1_SMP11                     ((uint_fast32_t)0x00000038)        /*!<SMP11[2:0] bits (Channel 11 Sample time selection) */
#define  ADC_SMPR1_SMP11_0                   ((uint_fast32_t)0x00000008)        /*!<Bit 0 */
#define  ADC_SMPR1_SMP11_1                   ((uint_fast32_t)0x00000010)        /*!<Bit 1 */
#define  ADC_SMPR1_SMP11_2                   ((uint_fast32_t)0x00000020)        /*!<Bit 2 */
#define  ADC_SMPR1_SMP12                     ((uint_fast32_t)0x000001C0)        /*!<SMP12[2:0] bits (Channel 12 Sample time selection) */
#define  ADC_SMPR1_SMP12_0                   ((uint_fast32_t)0x00000040)        /*!<Bit 0 */
#define  ADC_SMPR1_SMP12_1                   ((uint_fast32_t)0x00000080)        /*!<Bit 1 */
#define  ADC_SMPR1_SMP12_2                   ((uint_fast32_t)0x00000100)        /*!<Bit 2 */
#define  ADC_SMPR1_SMP13                     ((uint_fast32_t)0x00000E00)        /*!<SMP13[2:0] bits (Channel 13 Sample time selection) */
#define  ADC_SMPR1_SMP13_0                   ((uint_fast32_t)0x00000200)        /*!<Bit 0 */
#define  ADC_SMPR1_SMP13_1                   ((uint_fast32_t)0x00000400)        /*!<Bit 1 */
#define  ADC_SMPR1_SMP13_2                   ((uint_fast32_t)0x00000800)        /*!<Bit 2 */
#define  ADC_SMPR1_SMP14                     ((uint_fast32_t)0x00007000)        /*!<SMP14[2:0] bits (Channel 14 Sample time selection) */
#define  ADC_SMPR1_SMP14_0                   ((uint_fast32_t)0x00001000)        /*!<Bit 0 */
#define  ADC_SMPR1_SMP14_1                   ((uint_fast32_t)0x00002000)        /*!<Bit 1 */
#define  ADC_SMPR1_SMP14_2                   ((uint_fast32_t)0x00004000)        /*!<Bit 2 */
#define  ADC_SMPR1_SMP15                     ((uint_fast32_t)0x00038000)        /*!<SMP15[2:0] bits (Channel 15 Sample time selection) */
#define  ADC_SMPR1_SMP15_0                   ((uint_fast32_t)0x00008000)        /*!<Bit 0 */
#define  ADC_SMPR1_SMP15_1                   ((uint_fast32_t)0x00010000)        /*!<Bit 1 */
#define  ADC_SMPR1_SMP15_2                   ((uint_fast32_t)0x00020000)        /*!<Bit 2 */
#define  ADC_SMPR1_SMP16                     ((uint_fast32_t)0x001C0000)        /*!<SMP16[2:0] bits (Channel 16 Sample time selection) */
#define  ADC_SMPR1_SMP16_0                   ((uint_fast32_t)0x00040000)        /*!<Bit 0 */
#define  ADC_SMPR1_SMP16_1                   ((uint_fast32_t)0x00080000)        /*!<Bit 1 */
#define  ADC_SMPR1_SMP16_2                   ((uint_fast32_t)0x00100000)        /*!<Bit 2 */
#define  ADC_SMPR1_SMP17                     ((uint_fast32_t)0x00E00000)        /*!<SMP17[2:0] bits (Channel 17 Sample time selection) */
#define  ADC_SMPR1_SMP17_0                   ((uint_fast32_t)0x00200000)        /*!<Bit 0 */
#define  ADC_SMPR1_SMP17_1                   ((uint_fast32_t)0x00400000)        /*!<Bit 1 */
#define  ADC_SMPR1_SMP17_2                   ((uint_fast32_t)0x00800000)        /*!<Bit 2 */
#define  ADC_SMPR1_SMP18                     ((uint_fast32_t)0x07000000)        /*!<SMP18[2:0] bits (Channel 18 Sample time selection) */
#define  ADC_SMPR1_SMP18_0                   ((uint_fast32_t)0x01000000)        /*!<Bit 0 */
#define  ADC_SMPR1_SMP18_1                   ((uint_fast32_t)0x02000000)        /*!<Bit 1 */
#define  ADC_SMPR1_SMP18_2                   ((uint_fast32_t)0x04000000)        /*!<Bit 2 */

/******************  Bit definition for ADC_SMPR2 register  *******************/
#define  ADC_SMPR2_SMP0                      ((uint_fast32_t)0x00000007)        /*!<SMP0[2:0] bits (Channel 0 Sample time selection) */
#define  ADC_SMPR2_SMP0_0                    ((uint_fast32_t)0x00000001)        /*!<Bit 0 */
#define  ADC_SMPR2_SMP0_1                    ((uint_fast32_t)0x00000002)        /*!<Bit 1 */
#define  ADC_SMPR2_SMP0_2                    ((uint_fast32_t)0x00000004)        /*!<Bit 2 */
#define  ADC_SMPR2_SMP1                      ((uint_fast32_t)0x00000038)        /*!<SMP1[2:0] bits (Channel 1 Sample time selection) */
#define  ADC_SMPR2_SMP1_0                    ((uint_fast32_t)0x00000008)        /*!<Bit 0 */
#define  ADC_SMPR2_SMP1_1                    ((uint_fast32_t)0x00000010)        /*!<Bit 1 */
#define  ADC_SMPR2_SMP1_2                    ((uint_fast32_t)0x00000020)        /*!<Bit 2 */
#define  ADC_SMPR2_SMP2                      ((uint_fast32_t)0x000001C0)        /*!<SMP2[2:0] bits (Channel 2 Sample time selection) */
#define  ADC_SMPR2_SMP2_0                    ((uint_fast32_t)0x00000040)        /*!<Bit 0 */
#define  ADC_SMPR2_SMP2_1                    ((uint_fast32_t)0x00000080)        /*!<Bit 1 */
#define  ADC_SMPR2_SMP2_2                    ((uint_fast32_t)0x00000100)        /*!<Bit 2 */
#define  ADC_SMPR2_SMP3                      ((uint_fast32_t)0x00000E00)        /*!<SMP3[2:0] bits (Channel 3 Sample time selection) */
#define  ADC_SMPR2_SMP3_0                    ((uint_fast32_t)0x00000200)        /*!<Bit 0 */
#define  ADC_SMPR2_SMP3_1                    ((uint_fast32_t)0x00000400)        /*!<Bit 1 */
#define  ADC_SMPR2_SMP3_2                    ((uint_fast32_t)0x00000800)        /*!<Bit 2 */
#define  ADC_SMPR2_SMP4                      ((uint_fast32_t)0x00007000)        /*!<SMP4[2:0] bits (Channel 4 Sample time selection) */
#define  ADC_SMPR2_SMP4_0                    ((uint_fast32_t)0x00001000)        /*!<Bit 0 */
#define  ADC_SMPR2_SMP4_1                    ((uint_fast32_t)0x00002000)        /*!<Bit 1 */
#define  ADC_SMPR2_SMP4_2                    ((uint_fast32_t)0x00004000)        /*!<Bit 2 */
#define  ADC_SMPR2_SMP5                      ((uint_fast32_t)0x00038000)        /*!<SMP5[2:0] bits (Channel 5 Sample time selection) */
#define  ADC_SMPR2_SMP5_0                    ((uint_fast32_t)0x00008000)        /*!<Bit 0 */
#define  ADC_SMPR2_SMP5_1                    ((uint_fast32_t)0x00010000)        /*!<Bit 1 */
#define  ADC_SMPR2_SMP5_2                    ((uint_fast32_t)0x00020000)        /*!<Bit 2 */
#define  ADC_SMPR2_SMP6                      ((uint_fast32_t)0x001C0000)        /*!<SMP6[2:0] bits (Channel 6 Sample time selection) */
#define  ADC_SMPR2_SMP6_0                    ((uint_fast32_t)0x00040000)        /*!<Bit 0 */
#define  ADC_SMPR2_SMP6_1                    ((uint_fast32_t)0x00080000)        /*!<Bit 1 */
#define  ADC_SMPR2_SMP6_2                    ((uint_fast32_t)0x00100000)        /*!<Bit 2 */
#define  ADC_SMPR2_SMP7                      ((uint_fast32_t)0x00E00000)        /*!<SMP7[2:0] bits (Channel 7 Sample time selection) */
#define  ADC_SMPR2_SMP7_0                    ((uint_fast32_t)0x00200000)        /*!<Bit 0 */
#define  ADC_SMPR2_SMP7_1                    ((uint_fast32_t)0x00400000)        /*!<Bit 1 */
#define  ADC_SMPR2_SMP7_2                    ((uint_fast32_t)0x00800000)        /*!<Bit 2 */
#define  ADC_SMPR2_SMP8                      ((uint_fast32_t)0x07000000)        /*!<SMP8[2:0] bits (Channel 8 Sample time selection) */
#define  ADC_SMPR2_SMP8_0                    ((uint_fast32_t)0x01000000)        /*!<Bit 0 */
#define  ADC_SMPR2_SMP8_1                    ((uint_fast32_t)0x02000000)        /*!<Bit 1 */
#define  ADC_SMPR2_SMP8_2                    ((uint_fast32_t)0x04000000)        /*!<Bit 2 */
#define  ADC_SMPR2_SMP9                      ((uint_fast32_t)0x38000000)        /*!<SMP9[2:0] bits (Channel 9 Sample time selection) */
#define  ADC_SMPR2_SMP9_0                    ((uint_fast32_t)0x08000000)        /*!<Bit 0 */
#define  ADC_SMPR2_SMP9_1                    ((uint_fast32_t)0x10000000)        /*!<Bit 1 */
#define  ADC_SMPR2_SMP9_2                    ((uint_fast32_t)0x20000000)        /*!<Bit 2 */

/******************  Bit definition for ADC_JOFR1 register  *******************/
#define  ADC_JOFR1_JOFFSET1                  ((uint_fast32_t)0x0FFF)            /*!<Data offset for injected channel 1 */

/******************  Bit definition for ADC_JOFR2 register  *******************/
#define  ADC_JOFR2_JOFFSET2                  ((uint_fast32_t)0x0FFF)            /*!<Data offset for injected channel 2 */

/******************  Bit definition for ADC_JOFR3 register  *******************/
#define  ADC_JOFR3_JOFFSET3                  ((uint_fast32_t)0x0FFF)            /*!<Data offset for injected channel 3 */

/******************  Bit definition for ADC_JOFR4 register  *******************/
#define  ADC_JOFR4_JOFFSET4                  ((uint_fast32_t)0x0FFF)            /*!<Data offset for injected channel 4 */

/*******************  Bit definition for ADC_HTR register  ********************/
#define  ADC_HTR_HT                          ((uint_fast32_t)0x0FFF)            /*!<Analog watchdog high threshold */

/*******************  Bit definition for ADC_LTR register  ********************/
#define  ADC_LTR_LT                          ((uint_fast32_t)0x0FFF)            /*!<Analog watchdog low threshold */

/*******************  Bit definition for ADC_SQR1 register  *******************/
#define  ADC_SQR1_SQ13                       ((uint_fast32_t)0x0000001F)        /*!<SQ13[4:0] bits (13th conversion in regular sequence) */
#define  ADC_SQR1_SQ13_0                     ((uint_fast32_t)0x00000001)        /*!<Bit 0 */
#define  ADC_SQR1_SQ13_1                     ((uint_fast32_t)0x00000002)        /*!<Bit 1 */
#define  ADC_SQR1_SQ13_2                     ((uint_fast32_t)0x00000004)        /*!<Bit 2 */
#define  ADC_SQR1_SQ13_3                     ((uint_fast32_t)0x00000008)        /*!<Bit 3 */
#define  ADC_SQR1_SQ13_4                     ((uint_fast32_t)0x00000010)        /*!<Bit 4 */
#define  ADC_SQR1_SQ14                       ((uint_fast32_t)0x000003E0)        /*!<SQ14[4:0] bits (14th conversion in regular sequence) */
#define  ADC_SQR1_SQ14_0                     ((uint_fast32_t)0x00000020)        /*!<Bit 0 */
#define  ADC_SQR1_SQ14_1                     ((uint_fast32_t)0x00000040)        /*!<Bit 1 */
#define  ADC_SQR1_SQ14_2                     ((uint_fast32_t)0x00000080)        /*!<Bit 2 */
#define  ADC_SQR1_SQ14_3                     ((uint_fast32_t)0x00000100)        /*!<Bit 3 */
#define  ADC_SQR1_SQ14_4                     ((uint_fast32_t)0x00000200)        /*!<Bit 4 */
#define  ADC_SQR1_SQ15                       ((uint_fast32_t)0x00007C00)        /*!<SQ15[4:0] bits (15th conversion in regular sequence) */
#define  ADC_SQR1_SQ15_0                     ((uint_fast32_t)0x00000400)        /*!<Bit 0 */
#define  ADC_SQR1_SQ15_1                     ((uint_fast32_t)0x00000800)        /*!<Bit 1 */
#define  ADC_SQR1_SQ15_2                     ((uint_fast32_t)0x00001000)        /*!<Bit 2 */
#define  ADC_SQR1_SQ15_3                     ((uint_fast32_t)0x00002000)        /*!<Bit 3 */
#define  ADC_SQR1_SQ15_4                     ((uint_fast32_t)0x00004000)        /*!<Bit 4 */
#define  ADC_SQR1_SQ16                       ((uint_fast32_t)0x000F8000)        /*!<SQ16[4:0] bits (16th conversion in regular sequence) */
#define  ADC_SQR1_SQ16_0                     ((uint_fast32_t)0x00008000)        /*!<Bit 0 */
#define  ADC_SQR1_SQ16_1                     ((uint_fast32_t)0x00010000)        /*!<Bit 1 */
#define  ADC_SQR1_SQ16_2                     ((uint_fast32_t)0x00020000)        /*!<Bit 2 */
#define  ADC_SQR1_SQ16_3                     ((uint_fast32_t)0x00040000)        /*!<Bit 3 */
#define  ADC_SQR1_SQ16_4                     ((uint_fast32_t)0x00080000)        /*!<Bit 4 */
#define  ADC_SQR1_L                          ((uint_fast32_t)0x00F00000)        /*!<L[3:0] bits (Regular channel sequence length) */
#define  ADC_SQR1_L_0                        ((uint_fast32_t)0x00100000)        /*!<Bit 0 */
#define  ADC_SQR1_L_1                        ((uint_fast32_t)0x00200000)        /*!<Bit 1 */
#define  ADC_SQR1_L_2                        ((uint_fast32_t)0x00400000)        /*!<Bit 2 */
#define  ADC_SQR1_L_3                        ((uint_fast32_t)0x00800000)        /*!<Bit 3 */

/*******************  Bit definition for ADC_SQR2 register  *******************/
#define  ADC_SQR2_SQ7                        ((uint_fast32_t)0x0000001F)        /*!<SQ7[4:0] bits (7th conversion in regular sequence) */
#define  ADC_SQR2_SQ7_0                      ((uint_fast32_t)0x00000001)        /*!<Bit 0 */
#define  ADC_SQR2_SQ7_1                      ((uint_fast32_t)0x00000002)        /*!<Bit 1 */
#define  ADC_SQR2_SQ7_2                      ((uint_fast32_t)0x00000004)        /*!<Bit 2 */
#define  ADC_SQR2_SQ7_3                      ((uint_fast32_t)0x00000008)        /*!<Bit 3 */
#define  ADC_SQR2_SQ7_4                      ((uint_fast32_t)0x00000010)        /*!<Bit 4 */
#define  ADC_SQR2_SQ8                        ((uint_fast32_t)0x000003E0)        /*!<SQ8[4:0] bits (8th conversion in regular sequence) */
#define  ADC_SQR2_SQ8_0                      ((uint_fast32_t)0x00000020)        /*!<Bit 0 */
#define  ADC_SQR2_SQ8_1                      ((uint_fast32_t)0x00000040)        /*!<Bit 1 */
#define  ADC_SQR2_SQ8_2                      ((uint_fast32_t)0x00000080)        /*!<Bit 2 */
#define  ADC_SQR2_SQ8_3                      ((uint_fast32_t)0x00000100)        /*!<Bit 3 */
#define  ADC_SQR2_SQ8_4                      ((uint_fast32_t)0x00000200)        /*!<Bit 4 */
#define  ADC_SQR2_SQ9                        ((uint_fast32_t)0x00007C00)        /*!<SQ9[4:0] bits (9th conversion in regular sequence) */
#define  ADC_SQR2_SQ9_0                      ((uint_fast32_t)0x00000400)        /*!<Bit 0 */
#define  ADC_SQR2_SQ9_1                      ((uint_fast32_t)0x00000800)        /*!<Bit 1 */
#define  ADC_SQR2_SQ9_2                      ((uint_fast32_t)0x00001000)        /*!<Bit 2 */
#define  ADC_SQR2_SQ9_3                      ((uint_fast32_t)0x00002000)        /*!<Bit 3 */
#define  ADC_SQR2_SQ9_4                      ((uint_fast32_t)0x00004000)        /*!<Bit 4 */
#define  ADC_SQR2_SQ10                       ((uint_fast32_t)0x000F8000)        /*!<SQ10[4:0] bits (10th conversion in regular sequence) */
#define  ADC_SQR2_SQ10_0                     ((uint_fast32_t)0x00008000)        /*!<Bit 0 */
#define  ADC_SQR2_SQ10_1                     ((uint_fast32_t)0x00010000)        /*!<Bit 1 */
#define  ADC_SQR2_SQ10_2                     ((uint_fast32_t)0x00020000)        /*!<Bit 2 */
#define  ADC_SQR2_SQ10_3                     ((uint_fast32_t)0x00040000)        /*!<Bit 3 */
#define  ADC_SQR2_SQ10_4                     ((uint_fast32_t)0x00080000)        /*!<Bit 4 */
#define  ADC_SQR2_SQ11                       ((uint_fast32_t)0x01F00000)        /*!<SQ11[4:0] bits (11th conversion in regular sequence) */
#define  ADC_SQR2_SQ11_0                     ((uint_fast32_t)0x00100000)        /*!<Bit 0 */
#define  ADC_SQR2_SQ11_1                     ((uint_fast32_t)0x00200000)        /*!<Bit 1 */
#define  ADC_SQR2_SQ11_2                     ((uint_fast32_t)0x00400000)        /*!<Bit 2 */
#define  ADC_SQR2_SQ11_3                     ((uint_fast32_t)0x00800000)        /*!<Bit 3 */
#define  ADC_SQR2_SQ11_4                     ((uint_fast32_t)0x01000000)        /*!<Bit 4 */
#define  ADC_SQR2_SQ12                       ((uint_fast32_t)0x3E000000)        /*!<SQ12[4:0] bits (12th conversion in regular sequence) */
#define  ADC_SQR2_SQ12_0                     ((uint_fast32_t)0x02000000)        /*!<Bit 0 */
#define  ADC_SQR2_SQ12_1                     ((uint_fast32_t)0x04000000)        /*!<Bit 1 */
#define  ADC_SQR2_SQ12_2                     ((uint_fast32_t)0x08000000)        /*!<Bit 2 */
#define  ADC_SQR2_SQ12_3                     ((uint_fast32_t)0x10000000)        /*!<Bit 3 */
#define  ADC_SQR2_SQ12_4                     ((uint_fast32_t)0x20000000)        /*!<Bit 4 */

/*******************  Bit definition for ADC_SQR3 register  *******************/
#define  ADC_SQR3_SQ1                        ((uint_fast32_t)0x0000001F)        /*!<SQ1[4:0] bits (1st conversion in regular sequence) */
#define  ADC_SQR3_SQ1_0                      ((uint_fast32_t)0x00000001)        /*!<Bit 0 */
#define  ADC_SQR3_SQ1_1                      ((uint_fast32_t)0x00000002)        /*!<Bit 1 */
#define  ADC_SQR3_SQ1_2                      ((uint_fast32_t)0x00000004)        /*!<Bit 2 */
#define  ADC_SQR3_SQ1_3                      ((uint_fast32_t)0x00000008)        /*!<Bit 3 */
#define  ADC_SQR3_SQ1_4                      ((uint_fast32_t)0x00000010)        /*!<Bit 4 */
#define  ADC_SQR3_SQ2                        ((uint_fast32_t)0x000003E0)        /*!<SQ2[4:0] bits (2nd conversion in regular sequence) */
#define  ADC_SQR3_SQ2_0                      ((uint_fast32_t)0x00000020)        /*!<Bit 0 */
#define  ADC_SQR3_SQ2_1                      ((uint_fast32_t)0x00000040)        /*!<Bit 1 */
#define  ADC_SQR3_SQ2_2                      ((uint_fast32_t)0x00000080)        /*!<Bit 2 */
#define  ADC_SQR3_SQ2_3                      ((uint_fast32_t)0x00000100)        /*!<Bit 3 */
#define  ADC_SQR3_SQ2_4                      ((uint_fast32_t)0x00000200)        /*!<Bit 4 */
#define  ADC_SQR3_SQ3                        ((uint_fast32_t)0x00007C00)        /*!<SQ3[4:0] bits (3rd conversion in regular sequence) */
#define  ADC_SQR3_SQ3_0                      ((uint_fast32_t)0x00000400)        /*!<Bit 0 */
#define  ADC_SQR3_SQ3_1                      ((uint_fast32_t)0x00000800)        /*!<Bit 1 */
#define  ADC_SQR3_SQ3_2                      ((uint_fast32_t)0x00001000)        /*!<Bit 2 */
#define  ADC_SQR3_SQ3_3                      ((uint_fast32_t)0x00002000)        /*!<Bit 3 */
#define  ADC_SQR3_SQ3_4                      ((uint_fast32_t)0x00004000)        /*!<Bit 4 */
#define  ADC_SQR3_SQ4                        ((uint_fast32_t)0x000F8000)        /*!<SQ4[4:0] bits (4th conversion in regular sequence) */
#define  ADC_SQR3_SQ4_0                      ((uint_fast32_t)0x00008000)        /*!<Bit 0 */
#define  ADC_SQR3_SQ4_1                      ((uint_fast32_t)0x00010000)        /*!<Bit 1 */
#define  ADC_SQR3_SQ4_2                      ((uint_fast32_t)0x00020000)        /*!<Bit 2 */
#define  ADC_SQR3_SQ4_3                      ((uint_fast32_t)0x00040000)        /*!<Bit 3 */
#define  ADC_SQR3_SQ4_4                      ((uint_fast32_t)0x00080000)        /*!<Bit 4 */
#define  ADC_SQR3_SQ5                        ((uint_fast32_t)0x01F00000)        /*!<SQ5[4:0] bits (5th conversion in regular sequence) */
#define  ADC_SQR3_SQ5_0                      ((uint_fast32_t)0x00100000)        /*!<Bit 0 */
#define  ADC_SQR3_SQ5_1                      ((uint_fast32_t)0x00200000)        /*!<Bit 1 */
#define  ADC_SQR3_SQ5_2                      ((uint_fast32_t)0x00400000)        /*!<Bit 2 */
#define  ADC_SQR3_SQ5_3                      ((uint_fast32_t)0x00800000)        /*!<Bit 3 */
#define  ADC_SQR3_SQ5_4                      ((uint_fast32_t)0x01000000)        /*!<Bit 4 */
#define  ADC_SQR3_SQ6                        ((uint_fast32_t)0x3E000000)        /*!<SQ6[4:0] bits (6th conversion in regular sequence) */
#define  ADC_SQR3_SQ6_0                      ((uint_fast32_t)0x02000000)        /*!<Bit 0 */
#define  ADC_SQR3_SQ6_1                      ((uint_fast32_t)0x04000000)        /*!<Bit 1 */
#define  ADC_SQR3_SQ6_2                      ((uint_fast32_t)0x08000000)        /*!<Bit 2 */
#define  ADC_SQR3_SQ6_3                      ((uint_fast32_t)0x10000000)        /*!<Bit 3 */
#define  ADC_SQR3_SQ6_4                      ((uint_fast32_t)0x20000000)        /*!<Bit 4 */

/*******************  Bit definition for ADC_JSQR register  *******************/
#define  ADC_JSQR_JSQ1                       ((uint_fast32_t)0x0000001F)        /*!<JSQ1[4:0] bits (1st conversion in injected sequence) */
#define  ADC_JSQR_JSQ1_0                     ((uint_fast32_t)0x00000001)        /*!<Bit 0 */
#define  ADC_JSQR_JSQ1_1                     ((uint_fast32_t)0x00000002)        /*!<Bit 1 */
#define  ADC_JSQR_JSQ1_2                     ((uint_fast32_t)0x00000004)        /*!<Bit 2 */
#define  ADC_JSQR_JSQ1_3                     ((uint_fast32_t)0x00000008)        /*!<Bit 3 */
#define  ADC_JSQR_JSQ1_4                     ((uint_fast32_t)0x00000010)        /*!<Bit 4 */
#define  ADC_JSQR_JSQ2                       ((uint_fast32_t)0x000003E0)        /*!<JSQ2[4:0] bits (2nd conversion in injected sequence) */
#define  ADC_JSQR_JSQ2_0                     ((uint_fast32_t)0x00000020)        /*!<Bit 0 */
#define  ADC_JSQR_JSQ2_1                     ((uint_fast32_t)0x00000040)        /*!<Bit 1 */
#define  ADC_JSQR_JSQ2_2                     ((uint_fast32_t)0x00000080)        /*!<Bit 2 */
#define  ADC_JSQR_JSQ2_3                     ((uint_fast32_t)0x00000100)        /*!<Bit 3 */
#define  ADC_JSQR_JSQ2_4                     ((uint_fast32_t)0x00000200)        /*!<Bit 4 */
#define  ADC_JSQR_JSQ3                       ((uint_fast32_t)0x00007C00)        /*!<JSQ3[4:0] bits (3rd conversion in injected sequence) */
#define  ADC_JSQR_JSQ3_0                     ((uint_fast32_t)0x00000400)        /*!<Bit 0 */
#define  ADC_JSQR_JSQ3_1                     ((uint_fast32_t)0x00000800)        /*!<Bit 1 */
#define  ADC_JSQR_JSQ3_2                     ((uint_fast32_t)0x00001000)        /*!<Bit 2 */
#define  ADC_JSQR_JSQ3_3                     ((uint_fast32_t)0x00002000)        /*!<Bit 3 */
#define  ADC_JSQR_JSQ3_4                     ((uint_fast32_t)0x00004000)        /*!<Bit 4 */
#define  ADC_JSQR_JSQ4                       ((uint_fast32_t)0x000F8000)        /*!<JSQ4[4:0] bits (4th conversion in injected sequence) */
#define  ADC_JSQR_JSQ4_0                     ((uint_fast32_t)0x00008000)        /*!<Bit 0 */
#define  ADC_JSQR_JSQ4_1                     ((uint_fast32_t)0x00010000)        /*!<Bit 1 */
#define  ADC_JSQR_JSQ4_2                     ((uint_fast32_t)0x00020000)        /*!<Bit 2 */
#define  ADC_JSQR_JSQ4_3                     ((uint_fast32_t)0x00040000)        /*!<Bit 3 */
#define  ADC_JSQR_JSQ4_4                     ((uint_fast32_t)0x00080000)        /*!<Bit 4 */
#define  ADC_JSQR_JL                         ((uint_fast32_t)0x00300000)        /*!<JL[1:0] bits (Injected Sequence length) */
#define  ADC_JSQR_JL_0                       ((uint_fast32_t)0x00100000)        /*!<Bit 0 */
#define  ADC_JSQR_JL_1                       ((uint_fast32_t)0x00200000)        /*!<Bit 1 */

/*******************  Bit definition for ADC_JDR1 register  *******************/
#define  ADC_JDR1_JDATA                      ((uint_fast32_t)0xFFFF)            /*!<Injected data */

/*******************  Bit definition for ADC_JDR2 register  *******************/
#define  ADC_JDR2_JDATA                      ((uint_fast32_t)0xFFFF)            /*!<Injected data */

/*******************  Bit definition for ADC_JDR3 register  *******************/
#define  ADC_JDR3_JDATA                      ((uint_fast32_t)0xFFFF)            /*!<Injected data */

/*******************  Bit definition for ADC_JDR4 register  *******************/
#define  ADC_JDR4_JDATA                      ((uint_fast32_t)0xFFFF)            /*!<Injected data */

/********************  Bit definition for ADC_DR register  ********************/
#define  ADC_DR_DATA                         ((uint_fast32_t)0x0000FFFF)        /*!<Regular data */
#define  ADC_DR_ADC2DATA                     ((uint_fast32_t)0xFFFF0000)        /*!<ADC2 data */

/*******************  Bit definition for ADC_CSR register  ********************/
#define  ADC_CSR_AWD1                        ((uint_fast32_t)0x00000001)        /*!<ADC1 Analog watchdog flag */
#define  ADC_CSR_EOC1                        ((uint_fast32_t)0x00000002)        /*!<ADC1 End of conversion */
#define  ADC_CSR_JEOC1                       ((uint_fast32_t)0x00000004)        /*!<ADC1 Injected channel end of conversion */
#define  ADC_CSR_JSTRT1                      ((uint_fast32_t)0x00000008)        /*!<ADC1 Injected channel Start flag */
#define  ADC_CSR_STRT1                       ((uint_fast32_t)0x00000010)        /*!<ADC1 Regular channel Start flag */
#define  ADC_CSR_DOVR1                       ((uint_fast32_t)0x00000020)        /*!<ADC1 DMA overrun  flag */
#define  ADC_CSR_AWD2                        ((uint_fast32_t)0x00000100)        /*!<ADC2 Analog watchdog flag */
#define  ADC_CSR_EOC2                        ((uint_fast32_t)0x00000200)        /*!<ADC2 End of conversion */
#define  ADC_CSR_JEOC2                       ((uint_fast32_t)0x00000400)        /*!<ADC2 Injected channel end of conversion */
#define  ADC_CSR_JSTRT2                      ((uint_fast32_t)0x00000800)        /*!<ADC2 Injected channel Start flag */
#define  ADC_CSR_STRT2                       ((uint_fast32_t)0x00001000)        /*!<ADC2 Regular channel Start flag */
#define  ADC_CSR_DOVR2                       ((uint_fast32_t)0x00002000)        /*!<ADC2 DMA overrun  flag */
#define  ADC_CSR_AWD3                        ((uint_fast32_t)0x00010000)        /*!<ADC3 Analog watchdog flag */
#define  ADC_CSR_EOC3                        ((uint_fast32_t)0x00020000)        /*!<ADC3 End of conversion */
#define  ADC_CSR_JEOC3                       ((uint_fast32_t)0x00040000)        /*!<ADC3 Injected channel end of conversion */
#define  ADC_CSR_JSTRT3                      ((uint_fast32_t)0x00080000)        /*!<ADC3 Injected channel Start flag */
#define  ADC_CSR_STRT3                       ((uint_fast32_t)0x00100000)        /*!<ADC3 Regular channel Start flag */
#define  ADC_CSR_DOVR3                       ((uint_fast32_t)0x00200000)        /*!<ADC3 DMA overrun  flag */

/*******************  Bit definition for ADC_CCR register  ********************/
#define  ADC_CCR_MULTI                       ((uint_fast32_t)0x0000001F)        /*!<MULTI[4:0] bits (Multi-ADC mode selection) */
#define  ADC_CCR_MULTI_0                     ((uint_fast32_t)0x00000001)        /*!<Bit 0 */
#define  ADC_CCR_MULTI_1                     ((uint_fast32_t)0x00000002)        /*!<Bit 1 */
#define  ADC_CCR_MULTI_2                     ((uint_fast32_t)0x00000004)        /*!<Bit 2 */
#define  ADC_CCR_MULTI_3                     ((uint_fast32_t)0x00000008)        /*!<Bit 3 */
#define  ADC_CCR_MULTI_4                     ((uint_fast32_t)0x00000010)        /*!<Bit 4 */
#define  ADC_CCR_DELAY                       ((uint_fast32_t)0x00000F00)        /*!<DELAY[3:0] bits (Delay between 2 sampling phases) */
#define  ADC_CCR_DELAY_0                     ((uint_fast32_t)0x00000100)        /*!<Bit 0 */
#define  ADC_CCR_DELAY_1                     ((uint_fast32_t)0x00000200)        /*!<Bit 1 */
#define  ADC_CCR_DELAY_2                     ((uint_fast32_t)0x00000400)        /*!<Bit 2 */
#define  ADC_CCR_DELAY_3                     ((uint_fast32_t)0x00000800)        /*!<Bit 3 */
#define  ADC_CCR_DDS                         ((uint_fast32_t)0x00002000)        /*!<DMA disable selection (Multi-ADC mode) */
#define  ADC_CCR_DMA                         ((uint_fast32_t)0x0000C000)        /*!<DMA[1:0] bits (Direct Memory Access mode for multimode) */
#define  ADC_CCR_DMA_0                       ((uint_fast32_t)0x00004000)        /*!<Bit 0 */
#define  ADC_CCR_DMA_1                       ((uint_fast32_t)0x00008000)        /*!<Bit 1 */
#define  ADC_CCR_ADCPRE                      ((uint_fast32_t)0x00030000)        /*!<ADCPRE[1:0] bits (ADC prescaler) */
#define  ADC_CCR_ADCPRE_0                    ((uint_fast32_t)0x00010000)        /*!<Bit 0 */
#define  ADC_CCR_ADCPRE_1                    ((uint_fast32_t)0x00020000)        /*!<Bit 1 */
#define  ADC_CCR_VBATE                       ((uint_fast32_t)0x00400000)        /*!<VBAT Enable */
#define  ADC_CCR_TSVREFE                     ((uint_fast32_t)0x00800000)        /*!<Temperature Sensor and VREFINT Enable */

/*******************  Bit definition for ADC_CDR register  ********************/
#define  ADC_CDR_DATA1                      ((uint_fast32_t)0x0000FFFF)         /*!<1st data of a pair of regular conversions */
#define  ADC_CDR_DATA2                      ((uint_fast32_t)0xFFFF0000)         /*!<2nd data of a pair of regular conversions */


/******************************************************************************/
/*                                                                            */
/*                          CRC calculation unit                              */
/*                                                                            */
/******************************************************************************/
/*******************  Bit definition for CRC_DR register  *********************/
#define  CRC_DR_DR                           ((uint_fast32_t)0xFFFFFFFF) /*!< Data register bits */


/*******************  Bit definition for CRC_IDR register  ********************/
#define  CRC_IDR_IDR                         ((uint_fast32_t)0xFF)        /*!< General-purpose 8-bit data register bits */


/********************  Bit definition for CRC_CR register  ********************/
#define  CRC_CR_RESET                        ((uint_fast32_t)0x01)        /*!< RESET bit */


/******************************************************************************/
/*                                                                            */
/*                      Digital to Analog Converter                           */
/*                                                                            */
/******************************************************************************/
/********************  Bit definition for DAC_CR register  ********************/
#define  DAC_CR_EN1                          ((uint_fast32_t)0x00000001)        /*!<DAC channel1 enable */
#define  DAC_CR_BOFF1                        ((uint_fast32_t)0x00000002)        /*!<DAC channel1 output buffer disable */
#define  DAC_CR_TEN1                         ((uint_fast32_t)0x00000004)        /*!<DAC channel1 Trigger enable */

#define  DAC_CR_TSEL1                        ((uint_fast32_t)0x00000038)        /*!<TSEL1[2:0] (DAC channel1 Trigger selection) */
#define  DAC_CR_TSEL1_0                      ((uint_fast32_t)0x00000008)        /*!<Bit 0 */
#define  DAC_CR_TSEL1_1                      ((uint_fast32_t)0x00000010)        /*!<Bit 1 */
#define  DAC_CR_TSEL1_2                      ((uint_fast32_t)0x00000020)        /*!<Bit 2 */

#define  DAC_CR_WAVE1                        ((uint_fast32_t)0x000000C0)        /*!<WAVE1[1:0] (DAC channel1 noise/triangle wave generation enable) */
#define  DAC_CR_WAVE1_0                      ((uint_fast32_t)0x00000040)        /*!<Bit 0 */
#define  DAC_CR_WAVE1_1                      ((uint_fast32_t)0x00000080)        /*!<Bit 1 */

#define  DAC_CR_MAMP1                        ((uint_fast32_t)0x00000F00)        /*!<MAMP1[3:0] (DAC channel1 Mask/Amplitude selector) */
#define  DAC_CR_MAMP1_0                      ((uint_fast32_t)0x00000100)        /*!<Bit 0 */
#define  DAC_CR_MAMP1_1                      ((uint_fast32_t)0x00000200)        /*!<Bit 1 */
#define  DAC_CR_MAMP1_2                      ((uint_fast32_t)0x00000400)        /*!<Bit 2 */
#define  DAC_CR_MAMP1_3                      ((uint_fast32_t)0x00000800)        /*!<Bit 3 */

#define  DAC_CR_DMAEN1                       ((uint_fast32_t)0x00001000)        /*!<DAC channel1 DMA enable */
#define  DAC_CR_EN2                          ((uint_fast32_t)0x00010000)        /*!<DAC channel2 enable */
#define  DAC_CR_BOFF2                        ((uint_fast32_t)0x00020000)        /*!<DAC channel2 output buffer disable */
#define  DAC_CR_TEN2                         ((uint_fast32_t)0x00040000)        /*!<DAC channel2 Trigger enable */

#define  DAC_CR_TSEL2                        ((uint_fast32_t)0x00380000)        /*!<TSEL2[2:0] (DAC channel2 Trigger selection) */
#define  DAC_CR_TSEL2_0                      ((uint_fast32_t)0x00080000)        /*!<Bit 0 */
#define  DAC_CR_TSEL2_1                      ((uint_fast32_t)0x00100000)        /*!<Bit 1 */
#define  DAC_CR_TSEL2_2                      ((uint_fast32_t)0x00200000)        /*!<Bit 2 */

#define  DAC_CR_WAVE2                        ((uint_fast32_t)0x00C00000)        /*!<WAVE2[1:0] (DAC channel2 noise/triangle wave generation enable) */
#define  DAC_CR_WAVE2_0                      ((uint_fast32_t)0x00400000)        /*!<Bit 0 */
#define  DAC_CR_WAVE2_1                      ((uint_fast32_t)0x00800000)        /*!<Bit 1 */

#define  DAC_CR_MAMP2                        ((uint_fast32_t)0x0F000000)        /*!<MAMP2[3:0] (DAC channel2 Mask/Amplitude selector) */
#define  DAC_CR_MAMP2_0                      ((uint_fast32_t)0x01000000)        /*!<Bit 0 */
#define  DAC_CR_MAMP2_1                      ((uint_fast32_t)0x02000000)        /*!<Bit 1 */
#define  DAC_CR_MAMP2_2                      ((uint_fast32_t)0x04000000)        /*!<Bit 2 */
#define  DAC_CR_MAMP2_3                      ((uint_fast32_t)0x08000000)        /*!<Bit 3 */

#define  DAC_CR_DMAEN2                       ((uint_fast32_t)0x10000000)        /*!<DAC channel2 DMA enabled */

/*****************  Bit definition for DAC_SWTRIGR register  ******************/
#define  DAC_SWTRIGR_SWTRIG1                 ((uint_fast32_t)0x01)               /*!<DAC channel1 software trigger */
#define  DAC_SWTRIGR_SWTRIG2                 ((uint_fast32_t)0x02)               /*!<DAC channel2 software trigger */

/*****************  Bit definition for DAC_DHR12R1 register  ******************/
#define  DAC_DHR12R1_DACC1DHR                ((uint_fast32_t)0x0FFF)            /*!<DAC channel1 12-bit Right aligned data */

/*****************  Bit definition for DAC_DHR12L1 register  ******************/
#define  DAC_DHR12L1_DACC1DHR                ((uint_fast32_t)0xFFF0)            /*!<DAC channel1 12-bit Left aligned data */

/******************  Bit definition for DAC_DHR8R1 register  ******************/
#define  DAC_DHR8R1_DACC1DHR                 ((uint_fast32_t)0xFF)               /*!<DAC channel1 8-bit Right aligned data */

/*****************  Bit definition for DAC_DHR12R2 register  ******************/
#define  DAC_DHR12R2_DACC2DHR                ((uint_fast32_t)0x0FFF)            /*!<DAC channel2 12-bit Right aligned data */

/*****************  Bit definition for DAC_DHR12L2 register  ******************/
#define  DAC_DHR12L2_DACC2DHR                ((uint_fast32_t)0xFFF0)            /*!<DAC channel2 12-bit Left aligned data */

/******************  Bit definition for DAC_DHR8R2 register  ******************/
#define  DAC_DHR8R2_DACC2DHR                 ((uint_fast32_t)0xFF)               /*!<DAC channel2 8-bit Right aligned data */

/*****************  Bit definition for DAC_DHR12RD register  ******************/
#define  DAC_DHR12RD_DACC1DHR                ((uint_fast32_t)0x00000FFF)        /*!<DAC channel1 12-bit Right aligned data */
#define  DAC_DHR12RD_DACC2DHR                ((uint_fast32_t)0x0FFF0000)        /*!<DAC channel2 12-bit Right aligned data */

/*****************  Bit definition for DAC_DHR12LD register  ******************/
#define  DAC_DHR12LD_DACC1DHR                ((uint_fast32_t)0x0000FFF0)        /*!<DAC channel1 12-bit Left aligned data */
#define  DAC_DHR12LD_DACC2DHR                ((uint_fast32_t)0xFFF00000)        /*!<DAC channel2 12-bit Left aligned data */

/******************  Bit definition for DAC_DHR8RD register  ******************/
#define  DAC_DHR8RD_DACC1DHR                 ((uint_fast32_t)0x00FF)            /*!<DAC channel1 8-bit Right aligned data */
#define  DAC_DHR8RD_DACC2DHR                 ((uint_fast32_t)0xFF00)            /*!<DAC channel2 8-bit Right aligned data */

/*******************  Bit definition for DAC_DOR1 register  *******************/
#define  DAC_DOR1_DACC1DOR                   ((uint_fast32_t)0x0FFF)            /*!<DAC channel1 data output */

/*******************  Bit definition for DAC_DOR2 register  *******************/
#define  DAC_DOR2_DACC2DOR                   ((uint_fast32_t)0x0FFF)            /*!<DAC channel2 data output */

/********************  Bit definition for DAC_SR register  ********************/
#define  DAC_SR_DMAUDR1                      ((uint_fast32_t)0x00002000)        /*!<DAC channel1 DMA underrun flag */
#define  DAC_SR_DMAUDR2                      ((uint_fast32_t)0x20000000)        /*!<DAC channel2 DMA underrun flag */

/******************************************************************************/
/*                                                                            */
/*                                 Debug MCU                                  */
/*                                                                            */
/******************************************************************************/

/******************************************************************************/
/*                                                                            */
/*                                    DCMI                                    */
/*                                                                            */
/******************************************************************************/
/********************  Bits definition for DCMI_CR register  ******************/
#define DCMI_CR_CAPTURE                      ((uint_fast32_t)0x00000001)
#define DCMI_CR_CM                           ((uint_fast32_t)0x00000002)
#define DCMI_CR_CROP                         ((uint_fast32_t)0x00000004)
#define DCMI_CR_JPEG                         ((uint_fast32_t)0x00000008)
#define DCMI_CR_ESS                          ((uint_fast32_t)0x00000010)
#define DCMI_CR_PCKPOL                       ((uint_fast32_t)0x00000020)
#define DCMI_CR_HSPOL                        ((uint_fast32_t)0x00000040)
#define DCMI_CR_VSPOL                        ((uint_fast32_t)0x00000080)
#define DCMI_CR_FCRC_0                       ((uint_fast32_t)0x00000100)
#define DCMI_CR_FCRC_1                       ((uint_fast32_t)0x00000200)
#define DCMI_CR_EDM_0                        ((uint_fast32_t)0x00000400)
#define DCMI_CR_EDM_1                        ((uint_fast32_t)0x00000800)
#define DCMI_CR_CRE                          ((uint_fast32_t)0x00001000)
#define DCMI_CR_ENABLE                       ((uint_fast32_t)0x00004000)

/********************  Bits definition for DCMI_SR register  ******************/
#define DCMI_SR_HSYNC                        ((uint_fast32_t)0x00000001)
#define DCMI_SR_VSYNC                        ((uint_fast32_t)0x00000002)
#define DCMI_SR_FNE                          ((uint_fast32_t)0x00000004)

/********************  Bits definition for DCMI_RISR register  ****************/
#define DCMI_RISR_FRAME_RIS                  ((uint_fast32_t)0x00000001)
#define DCMI_RISR_OVF_RIS                    ((uint_fast32_t)0x00000002)
#define DCMI_RISR_ERR_RIS                    ((uint_fast32_t)0x00000004)
#define DCMI_RISR_VSYNC_RIS                  ((uint_fast32_t)0x00000008)
#define DCMI_RISR_LINE_RIS                   ((uint_fast32_t)0x00000010)

/********************  Bits definition for DCMI_IER register  *****************/
#define DCMI_IER_FRAME_IE                    ((uint_fast32_t)0x00000001)
#define DCMI_IER_OVF_IE                      ((uint_fast32_t)0x00000002)
#define DCMI_IER_ERR_IE                      ((uint_fast32_t)0x00000004)
#define DCMI_IER_VSYNC_IE                    ((uint_fast32_t)0x00000008)
#define DCMI_IER_LINE_IE                     ((uint_fast32_t)0x00000010)

/********************  Bits definition for DCMI_MISR register  ****************/
#define DCMI_MISR_FRAME_MIS                  ((uint_fast32_t)0x00000001)
#define DCMI_MISR_OVF_MIS                    ((uint_fast32_t)0x00000002)
#define DCMI_MISR_ERR_MIS                    ((uint_fast32_t)0x00000004)
#define DCMI_MISR_VSYNC_MIS                  ((uint_fast32_t)0x00000008)
#define DCMI_MISR_LINE_MIS                   ((uint_fast32_t)0x00000010)

/********************  Bits definition for DCMI_ICR register  *****************/
#define DCMI_ICR_FRAME_ISC                   ((uint_fast32_t)0x00000001)
#define DCMI_ICR_OVF_ISC                     ((uint_fast32_t)0x00000002)
#define DCMI_ICR_ERR_ISC                     ((uint_fast32_t)0x00000004)
#define DCMI_ICR_VSYNC_ISC                   ((uint_fast32_t)0x00000008)
#define DCMI_ICR_LINE_ISC                    ((uint_fast32_t)0x00000010)

/******************************************************************************/
/*                                                                            */
/*                             DMA Controller                                 */
/*                                                                            */
/******************************************************************************/
/********************  Bits definition for DMA_SxCR register  *****************/
#define DMA_SxCR_CHSEL                       ((uint_fast32_t)0x0E000000)
#define DMA_SxCR_CHSEL_0                     ((uint_fast32_t)0x02000000)
#define DMA_SxCR_CHSEL_1                     ((uint_fast32_t)0x04000000)
#define DMA_SxCR_CHSEL_2                     ((uint_fast32_t)0x08000000)
#define DMA_SxCR_MBURST                      ((uint_fast32_t)0x01800000)
#define DMA_SxCR_MBURST_0                    ((uint_fast32_t)0x00800000)
#define DMA_SxCR_MBURST_1                    ((uint_fast32_t)0x01000000)
#define DMA_SxCR_PBURST                      ((uint_fast32_t)0x00600000)
#define DMA_SxCR_PBURST_0                    ((uint_fast32_t)0x00200000)
#define DMA_SxCR_PBURST_1                    ((uint_fast32_t)0x00400000)
#define DMA_SxCR_ACK                         ((uint_fast32_t)0x00100000)
#define DMA_SxCR_CT                          ((uint_fast32_t)0x00080000)
#define DMA_SxCR_DBM                         ((uint_fast32_t)0x00040000)
#define DMA_SxCR_PL                          ((uint_fast32_t)0x00030000)
#define DMA_SxCR_PL_0                        ((uint_fast32_t)0x00010000)
#define DMA_SxCR_PL_1                        ((uint_fast32_t)0x00020000)
#define DMA_SxCR_PINCOS                      ((uint_fast32_t)0x00008000)
#define DMA_SxCR_MSIZE                       ((uint_fast32_t)0x00006000)
#define DMA_SxCR_MSIZE_0                     ((uint_fast32_t)0x00002000)
#define DMA_SxCR_MSIZE_1                     ((uint_fast32_t)0x00004000)
#define DMA_SxCR_PSIZE                       ((uint_fast32_t)0x00001800)
#define DMA_SxCR_PSIZE_0                     ((uint_fast32_t)0x00000800)
#define DMA_SxCR_PSIZE_1                     ((uint_fast32_t)0x00001000)
#define DMA_SxCR_MINC                        ((uint_fast32_t)0x00000400)
#define DMA_SxCR_PINC                        ((uint_fast32_t)0x00000200)
#define DMA_SxCR_CIRC                        ((uint_fast32_t)0x00000100)
#define DMA_SxCR_DIR                         ((uint_fast32_t)0x000000C0)
#define DMA_SxCR_DIR_0                       ((uint_fast32_t)0x00000040)
#define DMA_SxCR_DIR_1                       ((uint_fast32_t)0x00000080)
#define DMA_SxCR_PFCTRL                      ((uint_fast32_t)0x00000020)
#define DMA_SxCR_TCIE                        ((uint_fast32_t)0x00000010)
#define DMA_SxCR_HTIE                        ((uint_fast32_t)0x00000008)
#define DMA_SxCR_TEIE                        ((uint_fast32_t)0x00000004)
#define DMA_SxCR_DMEIE                       ((uint_fast32_t)0x00000002)
#define DMA_SxCR_EN                          ((uint_fast32_t)0x00000001)

/********************  Bits definition for DMA_SxCNDTR register  **************/
#define DMA_SxNDT                            ((uint_fast32_t)0x0000FFFF)
#define DMA_SxNDT_0                          ((uint_fast32_t)0x00000001)
#define DMA_SxNDT_1                          ((uint_fast32_t)0x00000002)
#define DMA_SxNDT_2                          ((uint_fast32_t)0x00000004)
#define DMA_SxNDT_3                          ((uint_fast32_t)0x00000008)
#define DMA_SxNDT_4                          ((uint_fast32_t)0x00000010)
#define DMA_SxNDT_5                          ((uint_fast32_t)0x00000020)
#define DMA_SxNDT_6                          ((uint_fast32_t)0x00000040)
#define DMA_SxNDT_7                          ((uint_fast32_t)0x00000080)
#define DMA_SxNDT_8                          ((uint_fast32_t)0x00000100)
#define DMA_SxNDT_9                          ((uint_fast32_t)0x00000200)
#define DMA_SxNDT_10                         ((uint_fast32_t)0x00000400)
#define DMA_SxNDT_11                         ((uint_fast32_t)0x00000800)
#define DMA_SxNDT_12                         ((uint_fast32_t)0x00001000)
#define DMA_SxNDT_13                         ((uint_fast32_t)0x00002000)
#define DMA_SxNDT_14                         ((uint_fast32_t)0x00004000)
#define DMA_SxNDT_15                         ((uint_fast32_t)0x00008000)

/********************  Bits definition for DMA_SxFCR register  ****************/
#define DMA_SxFCR_FEIE                       ((uint_fast32_t)0x00000080)
#define DMA_SxFCR_FS                         ((uint_fast32_t)0x00000038)
#define DMA_SxFCR_FS_0                       ((uint_fast32_t)0x00000008)
#define DMA_SxFCR_FS_1                       ((uint_fast32_t)0x00000010)
#define DMA_SxFCR_FS_2                       ((uint_fast32_t)0x00000020)
#define DMA_SxFCR_DMDIS                      ((uint_fast32_t)0x00000004)
#define DMA_SxFCR_FTH                        ((uint_fast32_t)0x00000003)
#define DMA_SxFCR_FTH_0                      ((uint_fast32_t)0x00000001)
#define DMA_SxFCR_FTH_1                      ((uint_fast32_t)0x00000002)

/********************  Bits definition for DMA_LISR register  *****************/
#define DMA_LISR_TCIF3                       ((uint_fast32_t)0x08000000)
#define DMA_LISR_HTIF3                       ((uint_fast32_t)0x04000000)
#define DMA_LISR_TEIF3                       ((uint_fast32_t)0x02000000)
#define DMA_LISR_DMEIF3                      ((uint_fast32_t)0x01000000)
#define DMA_LISR_FEIF3                       ((uint_fast32_t)0x00400000)
#define DMA_LISR_TCIF2                       ((uint_fast32_t)0x00200000)
#define DMA_LISR_HTIF2                       ((uint_fast32_t)0x00100000)
#define DMA_LISR_TEIF2                       ((uint_fast32_t)0x00080000)
#define DMA_LISR_DMEIF2                      ((uint_fast32_t)0x00040000)
#define DMA_LISR_FEIF2                       ((uint_fast32_t)0x00010000)
#define DMA_LISR_TCIF1                       ((uint_fast32_t)0x00000800)
#define DMA_LISR_HTIF1                       ((uint_fast32_t)0x00000400)
#define DMA_LISR_TEIF1                       ((uint_fast32_t)0x00000200)
#define DMA_LISR_DMEIF1                      ((uint_fast32_t)0x00000100)
#define DMA_LISR_FEIF1                       ((uint_fast32_t)0x00000040)
#define DMA_LISR_TCIF0                       ((uint_fast32_t)0x00000020)
#define DMA_LISR_HTIF0                       ((uint_fast32_t)0x00000010)
#define DMA_LISR_TEIF0                       ((uint_fast32_t)0x00000008)
#define DMA_LISR_DMEIF0                      ((uint_fast32_t)0x00000004)
#define DMA_LISR_FEIF0                       ((uint_fast32_t)0x00000001)

/********************  Bits definition for DMA_HISR register  *****************/
#define DMA_HISR_TCIF7                       ((uint_fast32_t)0x08000000)
#define DMA_HISR_HTIF7                       ((uint_fast32_t)0x04000000)
#define DMA_HISR_TEIF7                       ((uint_fast32_t)0x02000000)
#define DMA_HISR_DMEIF7                      ((uint_fast32_t)0x01000000)
#define DMA_HISR_FEIF7                       ((uint_fast32_t)0x00400000)
#define DMA_HISR_TCIF6                       ((uint_fast32_t)0x00200000)
#define DMA_HISR_HTIF6                       ((uint_fast32_t)0x00100000)
#define DMA_HISR_TEIF6                       ((uint_fast32_t)0x00080000)
#define DMA_HISR_DMEIF6                      ((uint_fast32_t)0x00040000)
#define DMA_HISR_FEIF6                       ((uint_fast32_t)0x00010000)
#define DMA_HISR_TCIF5                       ((uint_fast32_t)0x00000800)
#define DMA_HISR_HTIF5                       ((uint_fast32_t)0x00000400)
#define DMA_HISR_TEIF5                       ((uint_fast32_t)0x00000200)
#define DMA_HISR_DMEIF5                      ((uint_fast32_t)0x00000100)
#define DMA_HISR_FEIF5                       ((uint_fast32_t)0x00000040)
#define DMA_HISR_TCIF4                       ((uint_fast32_t)0x00000020)
#define DMA_HISR_HTIF4                       ((uint_fast32_t)0x00000010)
#define DMA_HISR_TEIF4                       ((uint_fast32_t)0x00000008)
#define DMA_HISR_DMEIF4                      ((uint_fast32_t)0x00000004)
#define DMA_HISR_FEIF4                       ((uint_fast32_t)0x00000001)

/********************  Bits definition for DMA_LIFCR register  ****************/
#define DMA_LIFCR_CTCIF3                     ((uint_fast32_t)0x08000000)
#define DMA_LIFCR_CHTIF3                     ((uint_fast32_t)0x04000000)
#define DMA_LIFCR_CTEIF3                     ((uint_fast32_t)0x02000000)
#define DMA_LIFCR_CDMEIF3                    ((uint_fast32_t)0x01000000)
#define DMA_LIFCR_CFEIF3                     ((uint_fast32_t)0x00400000)
#define DMA_LIFCR_CTCIF2                     ((uint_fast32_t)0x00200000)
#define DMA_LIFCR_CHTIF2                     ((uint_fast32_t)0x00100000)
#define DMA_LIFCR_CTEIF2                     ((uint_fast32_t)0x00080000)
#define DMA_LIFCR_CDMEIF2                    ((uint_fast32_t)0x00040000)
#define DMA_LIFCR_CFEIF2                     ((uint_fast32_t)0x00010000)
#define DMA_LIFCR_CTCIF1                     ((uint_fast32_t)0x00000800)
#define DMA_LIFCR_CHTIF1                     ((uint_fast32_t)0x00000400)
#define DMA_LIFCR_CTEIF1                     ((uint_fast32_t)0x00000200)
#define DMA_LIFCR_CDMEIF1                    ((uint_fast32_t)0x00000100)
#define DMA_LIFCR_CFEIF1                     ((uint_fast32_t)0x00000040)
#define DMA_LIFCR_CTCIF0                     ((uint_fast32_t)0x00000020)
#define DMA_LIFCR_CHTIF0                     ((uint_fast32_t)0x00000010)
#define DMA_LIFCR_CTEIF0                     ((uint_fast32_t)0x00000008)
#define DMA_LIFCR_CDMEIF0                    ((uint_fast32_t)0x00000004)
#define DMA_LIFCR_CFEIF0                     ((uint_fast32_t)0x00000001)

/********************  Bits definition for DMA_HIFCR  register  ****************/
#define DMA_HIFCR_CTCIF7                     ((uint_fast32_t)0x08000000)
#define DMA_HIFCR_CHTIF7                     ((uint_fast32_t)0x04000000)
#define DMA_HIFCR_CTEIF7                     ((uint_fast32_t)0x02000000)
#define DMA_HIFCR_CDMEIF7                    ((uint_fast32_t)0x01000000)
#define DMA_HIFCR_CFEIF7                     ((uint_fast32_t)0x00400000)
#define DMA_HIFCR_CTCIF6                     ((uint_fast32_t)0x00200000)
#define DMA_HIFCR_CHTIF6                     ((uint_fast32_t)0x00100000)
#define DMA_HIFCR_CTEIF6                     ((uint_fast32_t)0x00080000)
#define DMA_HIFCR_CDMEIF6                    ((uint_fast32_t)0x00040000)
#define DMA_HIFCR_CFEIF6                     ((uint_fast32_t)0x00010000)
#define DMA_HIFCR_CTCIF5                     ((uint_fast32_t)0x00000800)
#define DMA_HIFCR_CHTIF5                     ((uint_fast32_t)0x00000400)
#define DMA_HIFCR_CTEIF5                     ((uint_fast32_t)0x00000200)
#define DMA_HIFCR_CDMEIF5                    ((uint_fast32_t)0x00000100)
#define DMA_HIFCR_CFEIF5                     ((uint_fast32_t)0x00000040)
#define DMA_HIFCR_CTCIF4                     ((uint_fast32_t)0x00000020)
#define DMA_HIFCR_CHTIF4                     ((uint_fast32_t)0x00000010)
#define DMA_HIFCR_CTEIF4                     ((uint_fast32_t)0x00000008)
#define DMA_HIFCR_CDMEIF4                    ((uint_fast32_t)0x00000004)
#define DMA_HIFCR_CFEIF4                     ((uint_fast32_t)0x00000001)


/******************************************************************************/
/*                                                                            */
/*                    External Interrupt/Event Controller                     */
/*                                                                            */
/******************************************************************************/
/*******************  Bit definition for EXTI_IMR register  *******************/
#define  EXTI_IMR_MR0                        ((uint_fast32_t)0x00000001)        /*!< Interrupt Mask on line 0 */
#define  EXTI_IMR_MR1                        ((uint_fast32_t)0x00000002)        /*!< Interrupt Mask on line 1 */
#define  EXTI_IMR_MR2                        ((uint_fast32_t)0x00000004)        /*!< Interrupt Mask on line 2 */
#define  EXTI_IMR_MR3                        ((uint_fast32_t)0x00000008)        /*!< Interrupt Mask on line 3 */
#define  EXTI_IMR_MR4                        ((uint_fast32_t)0x00000010)        /*!< Interrupt Mask on line 4 */
#define  EXTI_IMR_MR5                        ((uint_fast32_t)0x00000020)        /*!< Interrupt Mask on line 5 */
#define  EXTI_IMR_MR6                        ((uint_fast32_t)0x00000040)        /*!< Interrupt Mask on line 6 */
#define  EXTI_IMR_MR7                        ((uint_fast32_t)0x00000080)        /*!< Interrupt Mask on line 7 */
#define  EXTI_IMR_MR8                        ((uint_fast32_t)0x00000100)        /*!< Interrupt Mask on line 8 */
#define  EXTI_IMR_MR9                        ((uint_fast32_t)0x00000200)        /*!< Interrupt Mask on line 9 */
#define  EXTI_IMR_MR10                       ((uint_fast32_t)0x00000400)        /*!< Interrupt Mask on line 10 */
#define  EXTI_IMR_MR11                       ((uint_fast32_t)0x00000800)        /*!< Interrupt Mask on line 11 */
#define  EXTI_IMR_MR12                       ((uint_fast32_t)0x00001000)        /*!< Interrupt Mask on line 12 */
#define  EXTI_IMR_MR13                       ((uint_fast32_t)0x00002000)        /*!< Interrupt Mask on line 13 */
#define  EXTI_IMR_MR14                       ((uint_fast32_t)0x00004000)        /*!< Interrupt Mask on line 14 */
#define  EXTI_IMR_MR15                       ((uint_fast32_t)0x00008000)        /*!< Interrupt Mask on line 15 */
#define  EXTI_IMR_MR16                       ((uint_fast32_t)0x00010000)        /*!< Interrupt Mask on line 16 */
#define  EXTI_IMR_MR17                       ((uint_fast32_t)0x00020000)        /*!< Interrupt Mask on line 17 */
#define  EXTI_IMR_MR18                       ((uint_fast32_t)0x00040000)        /*!< Interrupt Mask on line 18 */
#define  EXTI_IMR_MR19                       ((uint_fast32_t)0x00080000)        /*!< Interrupt Mask on line 19 */

/*******************  Bit definition for EXTI_EMR register  *******************/
#define  EXTI_EMR_MR0                        ((uint_fast32_t)0x00000001)        /*!< Event Mask on line 0 */
#define  EXTI_EMR_MR1                        ((uint_fast32_t)0x00000002)        /*!< Event Mask on line 1 */
#define  EXTI_EMR_MR2                        ((uint_fast32_t)0x00000004)        /*!< Event Mask on line 2 */
#define  EXTI_EMR_MR3                        ((uint_fast32_t)0x00000008)        /*!< Event Mask on line 3 */
#define  EXTI_EMR_MR4                        ((uint_fast32_t)0x00000010)        /*!< Event Mask on line 4 */
#define  EXTI_EMR_MR5                        ((uint_fast32_t)0x00000020)        /*!< Event Mask on line 5 */
#define  EXTI_EMR_MR6                        ((uint_fast32_t)0x00000040)        /*!< Event Mask on line 6 */
#define  EXTI_EMR_MR7                        ((uint_fast32_t)0x00000080)        /*!< Event Mask on line 7 */
#define  EXTI_EMR_MR8                        ((uint_fast32_t)0x00000100)        /*!< Event Mask on line 8 */
#define  EXTI_EMR_MR9                        ((uint_fast32_t)0x00000200)        /*!< Event Mask on line 9 */
#define  EXTI_EMR_MR10                       ((uint_fast32_t)0x00000400)        /*!< Event Mask on line 10 */
#define  EXTI_EMR_MR11                       ((uint_fast32_t)0x00000800)        /*!< Event Mask on line 11 */
#define  EXTI_EMR_MR12                       ((uint_fast32_t)0x00001000)        /*!< Event Mask on line 12 */
#define  EXTI_EMR_MR13                       ((uint_fast32_t)0x00002000)        /*!< Event Mask on line 13 */
#define  EXTI_EMR_MR14                       ((uint_fast32_t)0x00004000)        /*!< Event Mask on line 14 */
#define  EXTI_EMR_MR15                       ((uint_fast32_t)0x00008000)        /*!< Event Mask on line 15 */
#define  EXTI_EMR_MR16                       ((uint_fast32_t)0x00010000)        /*!< Event Mask on line 16 */
#define  EXTI_EMR_MR17                       ((uint_fast32_t)0x00020000)        /*!< Event Mask on line 17 */
#define  EXTI_EMR_MR18                       ((uint_fast32_t)0x00040000)        /*!< Event Mask on line 18 */
#define  EXTI_EMR_MR19                       ((uint_fast32_t)0x00080000)        /*!< Event Mask on line 19 */

/******************  Bit definition for EXTI_RTSR register  *******************/
#define  EXTI_RTSR_TR0                       ((uint_fast32_t)0x00000001)        /*!< Rising trigger event configuration bit of line 0 */
#define  EXTI_RTSR_TR1                       ((uint_fast32_t)0x00000002)        /*!< Rising trigger event configuration bit of line 1 */
#define  EXTI_RTSR_TR2                       ((uint_fast32_t)0x00000004)        /*!< Rising trigger event configuration bit of line 2 */
#define  EXTI_RTSR_TR3                       ((uint_fast32_t)0x00000008)        /*!< Rising trigger event configuration bit of line 3 */
#define  EXTI_RTSR_TR4                       ((uint_fast32_t)0x00000010)        /*!< Rising trigger event configuration bit of line 4 */
#define  EXTI_RTSR_TR5                       ((uint_fast32_t)0x00000020)        /*!< Rising trigger event configuration bit of line 5 */
#define  EXTI_RTSR_TR6                       ((uint_fast32_t)0x00000040)        /*!< Rising trigger event configuration bit of line 6 */
#define  EXTI_RTSR_TR7                       ((uint_fast32_t)0x00000080)        /*!< Rising trigger event configuration bit of line 7 */
#define  EXTI_RTSR_TR8                       ((uint_fast32_t)0x00000100)        /*!< Rising trigger event configuration bit of line 8 */
#define  EXTI_RTSR_TR9                       ((uint_fast32_t)0x00000200)        /*!< Rising trigger event configuration bit of line 9 */
#define  EXTI_RTSR_TR10                      ((uint_fast32_t)0x00000400)        /*!< Rising trigger event configuration bit of line 10 */
#define  EXTI_RTSR_TR11                      ((uint_fast32_t)0x00000800)        /*!< Rising trigger event configuration bit of line 11 */
#define  EXTI_RTSR_TR12                      ((uint_fast32_t)0x00001000)        /*!< Rising trigger event configuration bit of line 12 */
#define  EXTI_RTSR_TR13                      ((uint_fast32_t)0x00002000)        /*!< Rising trigger event configuration bit of line 13 */
#define  EXTI_RTSR_TR14                      ((uint_fast32_t)0x00004000)        /*!< Rising trigger event configuration bit of line 14 */
#define  EXTI_RTSR_TR15                      ((uint_fast32_t)0x00008000)        /*!< Rising trigger event configuration bit of line 15 */
#define  EXTI_RTSR_TR16                      ((uint_fast32_t)0x00010000)        /*!< Rising trigger event configuration bit of line 16 */
#define  EXTI_RTSR_TR17                      ((uint_fast32_t)0x00020000)        /*!< Rising trigger event configuration bit of line 17 */
#define  EXTI_RTSR_TR18                      ((uint_fast32_t)0x00040000)        /*!< Rising trigger event configuration bit of line 18 */
#define  EXTI_RTSR_TR19                      ((uint_fast32_t)0x00080000)        /*!< Rising trigger event configuration bit of line 19 */

/******************  Bit definition for EXTI_FTSR register  *******************/
#define  EXTI_FTSR_TR0                       ((uint_fast32_t)0x00000001)        /*!< Falling trigger event configuration bit of line 0 */
#define  EXTI_FTSR_TR1                       ((uint_fast32_t)0x00000002)        /*!< Falling trigger event configuration bit of line 1 */
#define  EXTI_FTSR_TR2                       ((uint_fast32_t)0x00000004)        /*!< Falling trigger event configuration bit of line 2 */
#define  EXTI_FTSR_TR3                       ((uint_fast32_t)0x00000008)        /*!< Falling trigger event configuration bit of line 3 */
#define  EXTI_FTSR_TR4                       ((uint_fast32_t)0x00000010)        /*!< Falling trigger event configuration bit of line 4 */
#define  EXTI_FTSR_TR5                       ((uint_fast32_t)0x00000020)        /*!< Falling trigger event configuration bit of line 5 */
#define  EXTI_FTSR_TR6                       ((uint_fast32_t)0x00000040)        /*!< Falling trigger event configuration bit of line 6 */
#define  EXTI_FTSR_TR7                       ((uint_fast32_t)0x00000080)        /*!< Falling trigger event configuration bit of line 7 */
#define  EXTI_FTSR_TR8                       ((uint_fast32_t)0x00000100)        /*!< Falling trigger event configuration bit of line 8 */
#define  EXTI_FTSR_TR9                       ((uint_fast32_t)0x00000200)        /*!< Falling trigger event configuration bit of line 9 */
#define  EXTI_FTSR_TR10                      ((uint_fast32_t)0x00000400)        /*!< Falling trigger event configuration bit of line 10 */
#define  EXTI_FTSR_TR11                      ((uint_fast32_t)0x00000800)        /*!< Falling trigger event configuration bit of line 11 */
#define  EXTI_FTSR_TR12                      ((uint_fast32_t)0x00001000)        /*!< Falling trigger event configuration bit of line 12 */
#define  EXTI_FTSR_TR13                      ((uint_fast32_t)0x00002000)        /*!< Falling trigger event configuration bit of line 13 */
#define  EXTI_FTSR_TR14                      ((uint_fast32_t)0x00004000)        /*!< Falling trigger event configuration bit of line 14 */
#define  EXTI_FTSR_TR15                      ((uint_fast32_t)0x00008000)        /*!< Falling trigger event configuration bit of line 15 */
#define  EXTI_FTSR_TR16                      ((uint_fast32_t)0x00010000)        /*!< Falling trigger event configuration bit of line 16 */
#define  EXTI_FTSR_TR17                      ((uint_fast32_t)0x00020000)        /*!< Falling trigger event configuration bit of line 17 */
#define  EXTI_FTSR_TR18                      ((uint_fast32_t)0x00040000)        /*!< Falling trigger event configuration bit of line 18 */
#define  EXTI_FTSR_TR19                      ((uint_fast32_t)0x00080000)        /*!< Falling trigger event configuration bit of line 19 */

/******************  Bit definition for EXTI_SWIER register  ******************/
#define  EXTI_SWIER_SWIER0                   ((uint_fast32_t)0x00000001)        /*!< Software Interrupt on line 0 */
#define  EXTI_SWIER_SWIER1                   ((uint_fast32_t)0x00000002)        /*!< Software Interrupt on line 1 */
#define  EXTI_SWIER_SWIER2                   ((uint_fast32_t)0x00000004)        /*!< Software Interrupt on line 2 */
#define  EXTI_SWIER_SWIER3                   ((uint_fast32_t)0x00000008)        /*!< Software Interrupt on line 3 */
#define  EXTI_SWIER_SWIER4                   ((uint_fast32_t)0x00000010)        /*!< Software Interrupt on line 4 */
#define  EXTI_SWIER_SWIER5                   ((uint_fast32_t)0x00000020)        /*!< Software Interrupt on line 5 */
#define  EXTI_SWIER_SWIER6                   ((uint_fast32_t)0x00000040)        /*!< Software Interrupt on line 6 */
#define  EXTI_SWIER_SWIER7                   ((uint_fast32_t)0x00000080)        /*!< Software Interrupt on line 7 */
#define  EXTI_SWIER_SWIER8                   ((uint_fast32_t)0x00000100)        /*!< Software Interrupt on line 8 */
#define  EXTI_SWIER_SWIER9                   ((uint_fast32_t)0x00000200)        /*!< Software Interrupt on line 9 */
#define  EXTI_SWIER_SWIER10                  ((uint_fast32_t)0x00000400)        /*!< Software Interrupt on line 10 */
#define  EXTI_SWIER_SWIER11                  ((uint_fast32_t)0x00000800)        /*!< Software Interrupt on line 11 */
#define  EXTI_SWIER_SWIER12                  ((uint_fast32_t)0x00001000)        /*!< Software Interrupt on line 12 */
#define  EXTI_SWIER_SWIER13                  ((uint_fast32_t)0x00002000)        /*!< Software Interrupt on line 13 */
#define  EXTI_SWIER_SWIER14                  ((uint_fast32_t)0x00004000)        /*!< Software Interrupt on line 14 */
#define  EXTI_SWIER_SWIER15                  ((uint_fast32_t)0x00008000)        /*!< Software Interrupt on line 15 */
#define  EXTI_SWIER_SWIER16                  ((uint_fast32_t)0x00010000)        /*!< Software Interrupt on line 16 */
#define  EXTI_SWIER_SWIER17                  ((uint_fast32_t)0x00020000)        /*!< Software Interrupt on line 17 */
#define  EXTI_SWIER_SWIER18                  ((uint_fast32_t)0x00040000)        /*!< Software Interrupt on line 18 */
#define  EXTI_SWIER_SWIER19                  ((uint_fast32_t)0x00080000)        /*!< Software Interrupt on line 19 */

/*******************  Bit definition for EXTI_PR register  ********************/
#define  EXTI_PR_PR0                         ((uint_fast32_t)0x00000001)        /*!< Pending bit for line 0 */
#define  EXTI_PR_PR1                         ((uint_fast32_t)0x00000002)        /*!< Pending bit for line 1 */
#define  EXTI_PR_PR2                         ((uint_fast32_t)0x00000004)        /*!< Pending bit for line 2 */
#define  EXTI_PR_PR3                         ((uint_fast32_t)0x00000008)        /*!< Pending bit for line 3 */
#define  EXTI_PR_PR4                         ((uint_fast32_t)0x00000010)        /*!< Pending bit for line 4 */
#define  EXTI_PR_PR5                         ((uint_fast32_t)0x00000020)        /*!< Pending bit for line 5 */
#define  EXTI_PR_PR6                         ((uint_fast32_t)0x00000040)        /*!< Pending bit for line 6 */
#define  EXTI_PR_PR7                         ((uint_fast32_t)0x00000080)        /*!< Pending bit for line 7 */
#define  EXTI_PR_PR8                         ((uint_fast32_t)0x00000100)        /*!< Pending bit for line 8 */
#define  EXTI_PR_PR9                         ((uint_fast32_t)0x00000200)        /*!< Pending bit for line 9 */
#define  EXTI_PR_PR10                        ((uint_fast32_t)0x00000400)        /*!< Pending bit for line 10 */
#define  EXTI_PR_PR11                        ((uint_fast32_t)0x00000800)        /*!< Pending bit for line 11 */
#define  EXTI_PR_PR12                        ((uint_fast32_t)0x00001000)        /*!< Pending bit for line 12 */
#define  EXTI_PR_PR13                        ((uint_fast32_t)0x00002000)        /*!< Pending bit for line 13 */
#define  EXTI_PR_PR14                        ((uint_fast32_t)0x00004000)        /*!< Pending bit for line 14 */
#define  EXTI_PR_PR15                        ((uint_fast32_t)0x00008000)        /*!< Pending bit for line 15 */
#define  EXTI_PR_PR16                        ((uint_fast32_t)0x00010000)        /*!< Pending bit for line 16 */
#define  EXTI_PR_PR17                        ((uint_fast32_t)0x00020000)        /*!< Pending bit for line 17 */
#define  EXTI_PR_PR18                        ((uint_fast32_t)0x00040000)        /*!< Pending bit for line 18 */
#define  EXTI_PR_PR19                        ((uint_fast32_t)0x00080000)        /*!< Pending bit for line 19 */

/******************************************************************************/
/*                                                                            */
/*                      Inter-integrated Circuit Interface                    */
/*                                                                            */
/******************************************************************************/
/*******************  Bit definition for I2C_CR1 register  ********************/
#define  I2C_CR1_PE                          ((uint_fast32_t)0x00000001)     /*!<Peripheral Enable                             */
#define  I2C_CR1_SMBUS                       ((uint_fast32_t)0x00000002)     /*!<SMBus Mode                                    */
#define  I2C_CR1_SMBTYPE                     ((uint_fast32_t)0x00000008)     /*!<SMBus Type                                    */
#define  I2C_CR1_ENARP                       ((uint_fast32_t)0x00000010)     /*!<ARP Enable                                    */
#define  I2C_CR1_ENPEC                       ((uint_fast32_t)0x00000020)     /*!<PEC Enable                                    */
#define  I2C_CR1_ENGC                        ((uint_fast32_t)0x00000040)     /*!<General Call Enable                           */
#define  I2C_CR1_NOSTRETCH                   ((uint_fast32_t)0x00000080)     /*!<Clock Stretching Disable (Slave mode)  */
#define  I2C_CR1_START                       ((uint_fast32_t)0x00000100)     /*!<Start Generation                              */
#define  I2C_CR1_STOP                        ((uint_fast32_t)0x00000200)     /*!<Stop Generation                               */
#define  I2C_CR1_ACK                         ((uint_fast32_t)0x00000400)     /*!<Acknowledge Enable                            */
#define  I2C_CR1_POS                         ((uint_fast32_t)0x00000800)     /*!<Acknowledge/PEC Position (for data reception) */
#define  I2C_CR1_PEC                         ((uint_fast32_t)0x00001000)     /*!<Packet Error Checking                         */
#define  I2C_CR1_ALERT                       ((uint_fast32_t)0x00002000)     /*!<SMBus Alert                                   */
#define  I2C_CR1_SWRST                       ((uint_fast32_t)0x00008000)     /*!<Software Reset                                */

/*******************  Bit definition for I2C_CR2 register  ********************/
#define  I2C_CR2_FREQ                        ((uint_fast32_t)0x0000003F)     /*!<FREQ[5:0] bits (Peripheral Clock Frequency)   */
#define  I2C_CR2_FREQ_0                      ((uint_fast32_t)0x00000001)     /*!<Bit 0 */
#define  I2C_CR2_FREQ_1                      ((uint_fast32_t)0x00000002)     /*!<Bit 1 */
#define  I2C_CR2_FREQ_2                      ((uint_fast32_t)0x00000004)     /*!<Bit 2 */
#define  I2C_CR2_FREQ_3                      ((uint_fast32_t)0x00000008)     /*!<Bit 3 */
#define  I2C_CR2_FREQ_4                      ((uint_fast32_t)0x00000010)     /*!<Bit 4 */
#define  I2C_CR2_FREQ_5                      ((uint_fast32_t)0x00000020)     /*!<Bit 5 */

#define  I2C_CR2_ITERREN                     ((uint_fast32_t)0x00000100)     /*!<Error Interrupt Enable  */
#define  I2C_CR2_ITEVTEN                     ((uint_fast32_t)0x00000200)     /*!<Event Interrupt Enable  */
#define  I2C_CR2_ITBUFEN                     ((uint_fast32_t)0x00000400)     /*!<Buffer Interrupt Enable */
#define  I2C_CR2_DMAEN                       ((uint_fast32_t)0x00000800)     /*!<DMA Requests Enable     */
#define  I2C_CR2_LAST                        ((uint_fast32_t)0x00001000)     /*!<DMA Last Transfer       */

/*******************  Bit definition for I2C_OAR1 register  *******************/
#define  I2C_OAR1_ADD1_7                     ((uint_fast32_t)0x000000FE)     /*!<Interface Address */
#define  I2C_OAR1_ADD8_9                     ((uint_fast32_t)0x00000300)     /*!<Interface Address */

#define  I2C_OAR1_ADD0                       ((uint_fast32_t)0x00000001)     /*!<Bit 0 */
#define  I2C_OAR1_ADD1                       ((uint_fast32_t)0x00000002)     /*!<Bit 1 */
#define  I2C_OAR1_ADD2                       ((uint_fast32_t)0x00000004)     /*!<Bit 2 */
#define  I2C_OAR1_ADD3                       ((uint_fast32_t)0x00000008)     /*!<Bit 3 */
#define  I2C_OAR1_ADD4                       ((uint_fast32_t)0x00000010)     /*!<Bit 4 */
#define  I2C_OAR1_ADD5                       ((uint_fast32_t)0x00000020)     /*!<Bit 5 */
#define  I2C_OAR1_ADD6                       ((uint_fast32_t)0x00000040)     /*!<Bit 6 */
#define  I2C_OAR1_ADD7                       ((uint_fast32_t)0x00000080)     /*!<Bit 7 */
#define  I2C_OAR1_ADD8                       ((uint_fast32_t)0x00000100)     /*!<Bit 8 */
#define  I2C_OAR1_ADD9                       ((uint_fast32_t)0x00000200)     /*!<Bit 9 */

#define  I2C_OAR1_ADDMODE                    ((uint_fast32_t)0x00008000)     /*!<Addressing Mode (Slave mode) */

/*******************  Bit definition for I2C_OAR2 register  *******************/
#define  I2C_OAR2_ENDUAL                     ((uint_fast32_t)0x00000001)        /*!<Dual addressing mode enable */
#define  I2C_OAR2_ADD2                       ((uint_fast32_t)0x000000FE)        /*!<Interface address           */

/********************  Bit definition for I2C_DR register  ********************/
#define  I2C_DR_DR                           ((uint_fast32_t)0x000000FF)        /*!<8-bit Data Register         */

/*******************  Bit definition for I2C_SR1 register  ********************/
#define  I2C_SR1_SB                          ((uint_fast32_t)0x00000001)     /*!<Start Bit (Master mode)                  */
#define  I2C_SR1_ADDR                        ((uint_fast32_t)0x00000002)     /*!<Address sent (master mode)/matched (slave mode) */
#define  I2C_SR1_BTF                         ((uint_fast32_t)0x00000004)     /*!<Byte Transfer Finished                          */
#define  I2C_SR1_ADD10                       ((uint_fast32_t)0x00000008)     /*!<10-bit header sent (Master mode)         */
#define  I2C_SR1_STOPF                       ((uint_fast32_t)0x00000010)     /*!<Stop detection (Slave mode)              */
#define  I2C_SR1_RXNE                        ((uint_fast32_t)0x00000040)     /*!<Data Register not Empty (receivers)      */
#define  I2C_SR1_TXE                         ((uint_fast32_t)0x00000080)     /*!<Data Register Empty (transmitters)       */
#define  I2C_SR1_BERR                        ((uint_fast32_t)0x00000100)     /*!<Bus Error                                       */
#define  I2C_SR1_ARLO                        ((uint_fast32_t)0x00000200)     /*!<Arbitration Lost (master mode)           */
#define  I2C_SR1_AF                          ((uint_fast32_t)0x00000400)     /*!<Acknowledge Failure                             */
#define  I2C_SR1_OVR                         ((uint_fast32_t)0x00000800)     /*!<Overrun/Underrun                                */
#define  I2C_SR1_PECERR                      ((uint_fast32_t)0x00001000)     /*!<PEC Error in reception                          */
#define  I2C_SR1_TIMEOUT                     ((uint_fast32_t)0x00004000)     /*!<Timeout or Tlow Error                           */
#define  I2C_SR1_SMBALERT                    ((uint_fast32_t)0x00008000)     /*!<SMBus Alert                                     */

/*******************  Bit definition for I2C_SR2 register  ********************/
#define  I2C_SR2_MSL                         ((uint_fast32_t)0x00000001)     /*!<Master/Slave                              */
#define  I2C_SR2_BUSY                        ((uint_fast32_t)0x00000002)     /*!<Bus Busy                                  */
#define  I2C_SR2_TRA                         ((uint_fast32_t)0x00000004)     /*!<Transmitter/Receiver                      */
#define  I2C_SR2_GENCALL                     ((uint_fast32_t)0x00000010)     /*!<General Call Address (Slave mode)  */
#define  I2C_SR2_SMBDEFAULT                  ((uint_fast32_t)0x00000020)     /*!<SMBus Device Default Address (Slave mode) */
#define  I2C_SR2_SMBHOST                     ((uint_fast32_t)0x00000040)     /*!<SMBus Host Header (Slave mode)     */
#define  I2C_SR2_DUALF                       ((uint_fast32_t)0x00000080)     /*!<Dual Flag (Slave mode)             */
#define  I2C_SR2_PEC                         ((uint_fast32_t)0x0000FF00)     /*!<Packet Error Checking Register            */

/*******************  Bit definition for I2C_CCR register  ********************/
#define  I2C_CCR_CCR                         ((uint_fast32_t)0x00000FFF)     /*!<Clock Control Register in Fast/Standard mode (Master mode) */
#define  I2C_CCR_DUTY                        ((uint_fast32_t)0x00004000)     /*!<Fast Mode Duty Cycle                                       */
#define  I2C_CCR_FS                          ((uint_fast32_t)0x00008000)     /*!<I2C Master Mode Selection                                  */

/******************  Bit definition for I2C_TRISE register  *******************/
#define  I2C_TRISE_TRISE                     ((uint_fast32_t)0x0000003F)     /*!<Maximum Rise Time in Fast/Standard mode (Master mode) */

/******************  Bit definition for I2C_FLTR register  *******************/
#define  I2C_FLTR_DNF                        ((uint_fast32_t)0x0000000F)     /*!<Digital Noise Filter */
#define  I2C_FLTR_ANOFF                      ((uint_fast32_t)0x00000010)     /*!<Analog Noise Filter OFF */

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
/*                             Power Control                                  */
/*                                                                            */
/******************************************************************************/
/********************  Bit definition for PWR_CR register  ********************/
#define  PWR_CR_LPDS                         ((uint_fast32_t)0x00000001)     /*!< Low-Power Deepsleep                 */
#define  PWR_CR_PDDS                         ((uint_fast32_t)0x00000002)     /*!< Power Down Deepsleep                */
#define  PWR_CR_CWUF                         ((uint_fast32_t)0x00000004)     /*!< Clear Wakeup Flag                   */
#define  PWR_CR_CSBF                         ((uint_fast32_t)0x00000008)     /*!< Clear Standby Flag                  */
#define  PWR_CR_PVDE                         ((uint_fast32_t)0x00000010)     /*!< Power Voltage Detector Enable       */

#define  PWR_CR_PLS                          ((uint_fast32_t)0x000000E0)     /*!< PLS[2:0] bits (PVD Level Selection) */
#define  PWR_CR_PLS_0                        ((uint_fast32_t)0x00000020)     /*!< Bit 0 */
#define  PWR_CR_PLS_1                        ((uint_fast32_t)0x00000040)     /*!< Bit 1 */
#define  PWR_CR_PLS_2                        ((uint_fast32_t)0x00000080)     /*!< Bit 2 */

/*!< PVD level configuration */
#define  PWR_CR_PLS_LEV0                     ((uint_fast32_t)0x00000000)     /*!< PVD level 0 */
#define  PWR_CR_PLS_LEV1                     ((uint_fast32_t)0x00000020)     /*!< PVD level 1 */
#define  PWR_CR_PLS_LEV2                     ((uint_fast32_t)0x00000040)     /*!< PVD level 2 */
#define  PWR_CR_PLS_LEV3                     ((uint_fast32_t)0x00000060)     /*!< PVD level 3 */
#define  PWR_CR_PLS_LEV4                     ((uint_fast32_t)0x00000080)     /*!< PVD level 4 */
#define  PWR_CR_PLS_LEV5                     ((uint_fast32_t)0x000000A0)     /*!< PVD level 5 */
#define  PWR_CR_PLS_LEV6                     ((uint_fast32_t)0x000000C0)     /*!< PVD level 6 */
#define  PWR_CR_PLS_LEV7                     ((uint_fast32_t)0x000000E0)     /*!< PVD level 7 */

#define  PWR_CR_DBP                          ((uint_fast32_t)0x00000100)     /*!< Disable Backup Domain write protection                     */
#define  PWR_CR_FPDS                         ((uint_fast32_t)0x00000200)     /*!< Flash power down in Stop mode                              */
#define  PWR_CR_VOS                          ((uint_fast32_t)0x0000C000)     /*!< VOS[1:0] bits (Regulator voltage scaling output selection) */
#define  PWR_CR_VOS_0                        ((uint_fast32_t)0x00004000)     /*!< Bit 0 */
#define  PWR_CR_VOS_1                        ((uint_fast32_t)0x00008000)     /*!< Bit 1 */

/* Legacy define */
#define  PWR_CR_PMODE                        PWR_CR_VOS

/*******************  Bit definition for PWR_CSR register  ********************/
#define  PWR_CSR_WUF                         ((uint_fast32_t)0x00000001)     /*!< Wakeup Flag                                      */
#define  PWR_CSR_SBF                         ((uint_fast32_t)0x00000002)     /*!< Standby Flag                                     */
#define  PWR_CSR_PVDO                        ((uint_fast32_t)0x00000004)     /*!< PVD Output                                       */
#define  PWR_CSR_BRR                         ((uint_fast32_t)0x00000008)     /*!< Backup regulator ready                           */
#define  PWR_CSR_EWUP                        ((uint_fast32_t)0x00000100)     /*!< Enable WKUP pin                                  */
#define  PWR_CSR_BRE                         ((uint_fast32_t)0x00000200)     /*!< Backup regulator enable                          */
#define  PWR_CSR_VOSRDY                      ((uint_fast32_t)0x00004000)     /*!< Regulator voltage scaling output selection ready */

/* Legacy define */
#define  PWR_CSR_REGRDY                      PWR_CSR_VOSRDY

/******************************************************************************/
/*                                                                            */
/*                                    RNG                                     */
/*                                                                            */
/******************************************************************************/
/********************  Bits definition for RNG_CR register  *******************/
#define RNG_CR_RNGEN                         ((uint_fast32_t)0x00000004)
#define RNG_CR_IE                            ((uint_fast32_t)0x00000008)

/********************  Bits definition for RNG_SR register  *******************/
#define RNG_SR_DRDY                          ((uint_fast32_t)0x00000001)
#define RNG_SR_CECS                          ((uint_fast32_t)0x00000002)
#define RNG_SR_SECS                          ((uint_fast32_t)0x00000004)
#define RNG_SR_CEIS                          ((uint_fast32_t)0x00000020)
#define RNG_SR_SEIS                          ((uint_fast32_t)0x00000040)

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



/******************************************************************************/
/*                                                                            */
/*                          SD host Interface                                 */
/*                                                                            */
/******************************************************************************/
/******************  Bit definition for SDIO_POWER register  ******************/
#define  SDIO_POWER_PWRCTRL                  ((uint_fast32_t)0x03)               /*!<PWRCTRL[1:0] bits (Power supply control bits) */
#define  SDIO_POWER_PWRCTRL_0                ((uint_fast32_t)0x01)               /*!<Bit 0 */
#define  SDIO_POWER_PWRCTRL_1                ((uint_fast32_t)0x02)               /*!<Bit 1 */

/******************  Bit definition for SDIO_CLKCR register  ******************/
#define  SDIO_CLKCR_CLKDIV                   ((uint_fast32_t)0x00FF)            /*!<Clock divide factor             */
#define  SDIO_CLKCR_CLKEN                    ((uint_fast32_t)0x0100)            /*!<Clock enable bit                */
#define  SDIO_CLKCR_PWRSAV                   ((uint_fast32_t)0x0200)            /*!<Power saving configuration bit  */
#define  SDIO_CLKCR_BYPASS                   ((uint_fast32_t)0x0400)            /*!<Clock divider bypass enable bit */

#define  SDIO_CLKCR_WIDBUS                   ((uint_fast32_t)0x1800)            /*!<WIDBUS[1:0] bits (Wide bus mode enable bit) */
#define  SDIO_CLKCR_WIDBUS_0                 ((uint_fast32_t)0x0800)            /*!<Bit 0 */
#define  SDIO_CLKCR_WIDBUS_1                 ((uint_fast32_t)0x1000)            /*!<Bit 1 */

#define  SDIO_CLKCR_NEGEDGE                  ((uint_fast32_t)0x2000)            /*!<SDIO_CK dephasing selection bit */
#define  SDIO_CLKCR_HWFC_EN                  ((uint_fast32_t)0x4000)            /*!<HW Flow Control enable          */

/*******************  Bit definition for SDIO_ARG register  *******************/
#define  SDIO_ARG_CMDARG                     ((uint_fast32_t)0xFFFFFFFF)            /*!<Command argument */

/*******************  Bit definition for SDIO_CMD register  *******************/
#define  SDIO_CMD_CMDINDEX                   ((uint_fast32_t)0x003F)            /*!<Command Index                               */

#define  SDIO_CMD_WAITRESP                   ((uint_fast32_t)0x00C0)            /*!<WAITRESP[1:0] bits (Wait for response bits) */
#define  SDIO_CMD_WAITRESP_0                 ((uint_fast32_t)0x0040)            /*!< Bit 0 */
#define  SDIO_CMD_WAITRESP_1                 ((uint_fast32_t)0x0080)            /*!< Bit 1 */

#define  SDIO_CMD_WAITINT                    ((uint_fast32_t)0x0100)            /*!<CPSM Waits for Interrupt Request                               */
#define  SDIO_CMD_WAITPEND                   ((uint_fast32_t)0x0200)            /*!<CPSM Waits for ends of data transfer (CmdPend internal signal) */
#define  SDIO_CMD_CPSMEN                     ((uint_fast32_t)0x0400)            /*!<Command path state machine (CPSM) Enable bit                   */
#define  SDIO_CMD_SDIOSUSPEND                ((uint_fast32_t)0x0800)            /*!<SD I/O suspend command                                         */
#define  SDIO_CMD_ENCMDCOMPL                 ((uint_fast32_t)0x1000)            /*!<Enable CMD completion                                          */
#define  SDIO_CMD_NIEN                       ((uint_fast32_t)0x2000)            /*!<Not Interrupt Enable */
#define  SDIO_CMD_CEATACMD                   ((uint_fast32_t)0x4000)            /*!<CE-ATA command       */

/*****************  Bit definition for SDIO_RESPCMD register  *****************/
#define  SDIO_RESPCMD_RESPCMD                ((uint_fast32_t)0x3F)               /*!<Response command index */

/******************  Bit definition for SDIO_RESP0 register  ******************/
#define  SDIO_RESP0_CARDSTATUS0              ((uint_fast32_t)0xFFFFFFFF)        /*!<Card Status */

/******************  Bit definition for SDIO_RESP1 register  ******************/
#define  SDIO_RESP1_CARDSTATUS1              ((uint_fast32_t)0xFFFFFFFF)        /*!<Card Status */

/******************  Bit definition for SDIO_RESP2 register  ******************/
#define  SDIO_RESP2_CARDSTATUS2              ((uint_fast32_t)0xFFFFFFFF)        /*!<Card Status */

/******************  Bit definition for SDIO_RESP3 register  ******************/
#define  SDIO_RESP3_CARDSTATUS3              ((uint_fast32_t)0xFFFFFFFF)        /*!<Card Status */

/******************  Bit definition for SDIO_RESP4 register  ******************/
#define  SDIO_RESP4_CARDSTATUS4              ((uint_fast32_t)0xFFFFFFFF)        /*!<Card Status */

/******************  Bit definition for SDIO_DTIMER register  *****************/
#define  SDIO_DTIMER_DATATIME                ((uint_fast32_t)0xFFFFFFFF)        /*!<Data timeout period. */

/******************  Bit definition for SDIO_DLEN register  *******************/
#define  SDIO_DLEN_DATALENGTH                ((uint_fast32_t)0x01FFFFFF)        /*!<Data length value    */

/******************  Bit definition for SDIO_DCTRL register  ******************/
#define  SDIO_DCTRL_DTEN                     ((uint_fast32_t)0x0001)            /*!<Data transfer enabled bit         */
#define  SDIO_DCTRL_DTDIR                    ((uint_fast32_t)0x0002)            /*!<Data transfer direction selection */
#define  SDIO_DCTRL_DTMODE                   ((uint_fast32_t)0x0004)            /*!<Data transfer mode selection      */
#define  SDIO_DCTRL_DMAEN                    ((uint_fast32_t)0x0008)            /*!<DMA enabled bit                   */

#define  SDIO_DCTRL_DBLOCKSIZE               ((uint_fast32_t)0x00F0)            /*!<DBLOCKSIZE[3:0] bits (Data block size) */
#define  SDIO_DCTRL_DBLOCKSIZE_0             ((uint_fast32_t)0x0010)            /*!<Bit 0 */
#define  SDIO_DCTRL_DBLOCKSIZE_1             ((uint_fast32_t)0x0020)            /*!<Bit 1 */
#define  SDIO_DCTRL_DBLOCKSIZE_2             ((uint_fast32_t)0x0040)            /*!<Bit 2 */
#define  SDIO_DCTRL_DBLOCKSIZE_3             ((uint_fast32_t)0x0080)            /*!<Bit 3 */

#define  SDIO_DCTRL_RWSTART                  ((uint_fast32_t)0x0100)            /*!<Read wait start         */
#define  SDIO_DCTRL_RWSTOP                   ((uint_fast32_t)0x0200)            /*!<Read wait stop          */
#define  SDIO_DCTRL_RWMOD                    ((uint_fast32_t)0x0400)            /*!<Read wait mode          */
#define  SDIO_DCTRL_SDIOEN                   ((uint_fast32_t)0x0800)            /*!<SD I/O enable functions */

/******************  Bit definition for SDIO_DCOUNT register  *****************/
#define  SDIO_DCOUNT_DATACOUNT               ((uint_fast32_t)0x01FFFFFF)        /*!<Data count value */

/******************  Bit definition for SDIO_STA register  ********************/
#define  SDIO_STA_CCRCFAIL                   ((uint_fast32_t)0x00000001)        /*!<Command response received (CRC check failed)  */
#define  SDIO_STA_DCRCFAIL                   ((uint_fast32_t)0x00000002)        /*!<Data block sent/received (CRC check failed)   */
#define  SDIO_STA_CTIMEOUT                   ((uint_fast32_t)0x00000004)        /*!<Command response timeout                      */
#define  SDIO_STA_DTIMEOUT                   ((uint_fast32_t)0x00000008)        /*!<Data timeout                                  */
#define  SDIO_STA_TXUNDERR                   ((uint_fast32_t)0x00000010)        /*!<Transmit FIFO underrun error                  */
#define  SDIO_STA_RXOVERR                    ((uint_fast32_t)0x00000020)        /*!<Received FIFO overrun error                   */
#define  SDIO_STA_CMDREND                    ((uint_fast32_t)0x00000040)        /*!<Command response received (CRC check passed)  */
#define  SDIO_STA_CMDSENT                    ((uint_fast32_t)0x00000080)        /*!<Command sent (no response required)           */
#define  SDIO_STA_DATAEND                    ((uint_fast32_t)0x00000100)        /*!<Data end (data counter, SDIDCOUNT, is zero)   */
#define  SDIO_STA_STBITERR                   ((uint_fast32_t)0x00000200)        /*!<Start bit not detected on all data signals in wide bus mode */
#define  SDIO_STA_DBCKEND                    ((uint_fast32_t)0x00000400)        /*!<Data block sent/received (CRC check passed)   */
#define  SDIO_STA_CMDACT                     ((uint_fast32_t)0x00000800)        /*!<Command transfer in progress                  */
#define  SDIO_STA_TXACT                      ((uint_fast32_t)0x00001000)        /*!<Data transmit in progress                     */
#define  SDIO_STA_RXACT                      ((uint_fast32_t)0x00002000)        /*!<Data receive in progress                      */
#define  SDIO_STA_TXFIFOHE                   ((uint_fast32_t)0x00004000)        /*!<Transmit FIFO Half Empty: at least 8 words can be written into the FIFO */
#define  SDIO_STA_RXFIFOHF                   ((uint_fast32_t)0x00008000)        /*!<Receive FIFO Half Full: there are at least 8 words in the FIFO */
#define  SDIO_STA_TXFIFOF                    ((uint_fast32_t)0x00010000)        /*!<Transmit FIFO full                            */
#define  SDIO_STA_RXFIFOF                    ((uint_fast32_t)0x00020000)        /*!<Receive FIFO full                             */
#define  SDIO_STA_TXFIFOE                    ((uint_fast32_t)0x00040000)        /*!<Transmit FIFO empty                           */
#define  SDIO_STA_RXFIFOE                    ((uint_fast32_t)0x00080000)        /*!<Receive FIFO empty                            */
#define  SDIO_STA_TXDAVL                     ((uint_fast32_t)0x00100000)        /*!<Data available in transmit FIFO               */
#define  SDIO_STA_RXDAVL                     ((uint_fast32_t)0x00200000)        /*!<Data available in receive FIFO                */
#define  SDIO_STA_SDIOIT                     ((uint_fast32_t)0x00400000)        /*!<SDIO interrupt received                       */
#define  SDIO_STA_CEATAEND                   ((uint_fast32_t)0x00800000)        /*!<CE-ATA command completion signal received for CMD61 */

/*******************  Bit definition for SDIO_ICR register  *******************/
#define  SDIO_ICR_CCRCFAILC                  ((uint_fast32_t)0x00000001)        /*!<CCRCFAIL flag clear bit */
#define  SDIO_ICR_DCRCFAILC                  ((uint_fast32_t)0x00000002)        /*!<DCRCFAIL flag clear bit */
#define  SDIO_ICR_CTIMEOUTC                  ((uint_fast32_t)0x00000004)        /*!<CTIMEOUT flag clear bit */
#define  SDIO_ICR_DTIMEOUTC                  ((uint_fast32_t)0x00000008)        /*!<DTIMEOUT flag clear bit */
#define  SDIO_ICR_TXUNDERRC                  ((uint_fast32_t)0x00000010)        /*!<TXUNDERR flag clear bit */
#define  SDIO_ICR_RXOVERRC                   ((uint_fast32_t)0x00000020)        /*!<RXOVERR flag clear bit  */
#define  SDIO_ICR_CMDRENDC                   ((uint_fast32_t)0x00000040)        /*!<CMDREND flag clear bit  */
#define  SDIO_ICR_CMDSENTC                   ((uint_fast32_t)0x00000080)        /*!<CMDSENT flag clear bit  */
#define  SDIO_ICR_DATAENDC                   ((uint_fast32_t)0x00000100)        /*!<DATAEND flag clear bit  */
#define  SDIO_ICR_STBITERRC                  ((uint_fast32_t)0x00000200)        /*!<STBITERR flag clear bit */
#define  SDIO_ICR_DBCKENDC                   ((uint_fast32_t)0x00000400)        /*!<DBCKEND flag clear bit  */
#define  SDIO_ICR_SDIOITC                    ((uint_fast32_t)0x00400000)        /*!<SDIOIT flag clear bit   */
#define  SDIO_ICR_CEATAENDC                  ((uint_fast32_t)0x00800000)        /*!<CEATAEND flag clear bit */

/******************  Bit definition for SDIO_MASK register  *******************/
#define  SDIO_MASK_CCRCFAILIE                ((uint_fast32_t)0x00000001)        /*!<Command CRC Fail Interrupt Enable          */
#define  SDIO_MASK_DCRCFAILIE                ((uint_fast32_t)0x00000002)        /*!<Data CRC Fail Interrupt Enable             */
#define  SDIO_MASK_CTIMEOUTIE                ((uint_fast32_t)0x00000004)        /*!<Command TimeOut Interrupt Enable           */
#define  SDIO_MASK_DTIMEOUTIE                ((uint_fast32_t)0x00000008)        /*!<Data TimeOut Interrupt Enable              */
#define  SDIO_MASK_TXUNDERRIE                ((uint_fast32_t)0x00000010)        /*!<Tx FIFO UnderRun Error Interrupt Enable    */
#define  SDIO_MASK_RXOVERRIE                 ((uint_fast32_t)0x00000020)        /*!<Rx FIFO OverRun Error Interrupt Enable     */
#define  SDIO_MASK_CMDRENDIE                 ((uint_fast32_t)0x00000040)        /*!<Command Response Received Interrupt Enable */
#define  SDIO_MASK_CMDSENTIE                 ((uint_fast32_t)0x00000080)        /*!<Command Sent Interrupt Enable              */
#define  SDIO_MASK_DATAENDIE                 ((uint_fast32_t)0x00000100)        /*!<Data End Interrupt Enable                  */
#define  SDIO_MASK_STBITERRIE                ((uint_fast32_t)0x00000200)        /*!<Start Bit Error Interrupt Enable           */
#define  SDIO_MASK_DBCKENDIE                 ((uint_fast32_t)0x00000400)        /*!<Data Block End Interrupt Enable            */
#define  SDIO_MASK_CMDACTIE                  ((uint_fast32_t)0x00000800)        /*!<CCommand Acting Interrupt Enable           */
#define  SDIO_MASK_TXACTIE                   ((uint_fast32_t)0x00001000)        /*!<Data Transmit Acting Interrupt Enable      */
#define  SDIO_MASK_RXACTIE                   ((uint_fast32_t)0x00002000)        /*!<Data receive acting interrupt enabled      */
#define  SDIO_MASK_TXFIFOHEIE                ((uint_fast32_t)0x00004000)        /*!<Tx FIFO Half Empty interrupt Enable        */
#define  SDIO_MASK_RXFIFOHFIE                ((uint_fast32_t)0x00008000)        /*!<Rx FIFO Half Full interrupt Enable         */
#define  SDIO_MASK_TXFIFOFIE                 ((uint_fast32_t)0x00010000)        /*!<Tx FIFO Full interrupt Enable              */
#define  SDIO_MASK_RXFIFOFIE                 ((uint_fast32_t)0x00020000)        /*!<Rx FIFO Full interrupt Enable              */
#define  SDIO_MASK_TXFIFOEIE                 ((uint_fast32_t)0x00040000)        /*!<Tx FIFO Empty interrupt Enable             */
#define  SDIO_MASK_RXFIFOEIE                 ((uint_fast32_t)0x00080000)        /*!<Rx FIFO Empty interrupt Enable             */
#define  SDIO_MASK_TXDAVLIE                  ((uint_fast32_t)0x00100000)        /*!<Data available in Tx FIFO interrupt Enable */
#define  SDIO_MASK_RXDAVLIE                  ((uint_fast32_t)0x00200000)        /*!<Data available in Rx FIFO interrupt Enable */
#define  SDIO_MASK_SDIOITIE                  ((uint_fast32_t)0x00400000)        /*!<SDIO Mode Interrupt Received interrupt Enable */
#define  SDIO_MASK_CEATAENDIE                ((uint_fast32_t)0x00800000)        /*!<CE-ATA command completion signal received Interrupt Enable */

/*****************  Bit definition for SDIO_FIFOCNT register  *****************/
#define  SDIO_FIFOCNT_FIFOCOUNT              ((uint_fast32_t)0x00FFFFFF)        /*!<Remaining number of words to be written to or read from the FIFO */

/******************  Bit definition for SDIO_FIFO register  *******************/
#define  SDIO_FIFO_FIFODATA                  ((uint_fast32_t)0xFFFFFFFF)        /*!<Receive and transmit FIFO data */

/******************************************************************************/
/*                                                                            */
/*                        Serial Peripheral Interface                         */
/*                                                                            */
/******************************************************************************/
/*******************  Bit definition for SPI_CR1 register  ********************/
#define  SPI_CR1_CPHA                        ((uint_fast32_t)0x00000001)            /*!<Clock Phase      */
#define  SPI_CR1_CPOL                        ((uint_fast32_t)0x00000002)            /*!<Clock Polarity   */
#define  SPI_CR1_MSTR                        ((uint_fast32_t)0x00000004)            /*!<Master Selection */

#define  SPI_CR1_BR                          ((uint_fast32_t)0x00000038)            /*!<BR[2:0] bits (Baud Rate Control) */
#define  SPI_CR1_BR_0                        ((uint_fast32_t)0x00000008)            /*!<Bit 0 */
#define  SPI_CR1_BR_1                        ((uint_fast32_t)0x00000010)            /*!<Bit 1 */
#define  SPI_CR1_BR_2                        ((uint_fast32_t)0x00000020)            /*!<Bit 2 */

#define  SPI_CR1_SPE                         ((uint_fast32_t)0x00000040)            /*!<SPI Enable                          */
#define  SPI_CR1_LSBFIRST                    ((uint_fast32_t)0x00000080)            /*!<Frame Format                        */
#define  SPI_CR1_SSI                         ((uint_fast32_t)0x00000100)            /*!<Internal slave select               */
#define  SPI_CR1_SSM                         ((uint_fast32_t)0x00000200)            /*!<Software slave management           */
#define  SPI_CR1_RXONLY                      ((uint_fast32_t)0x00000400)            /*!<Receive only                        */
#define  SPI_CR1_DFF                         ((uint_fast32_t)0x00000800)            /*!<Data Frame Format                   */
#define  SPI_CR1_CRCNEXT                     ((uint_fast32_t)0x00001000)            /*!<Transmit CRC next                   */
#define  SPI_CR1_CRCEN                       ((uint_fast32_t)0x00002000)            /*!<Hardware CRC calculation enable     */
#define  SPI_CR1_BIDIOE                      ((uint_fast32_t)0x00004000)            /*!<Output enable in bidirectional mode */
#define  SPI_CR1_BIDIMODE                    ((uint_fast32_t)0x00008000)            /*!<Bidirectional data mode enable      */

/*******************  Bit definition for SPI_CR2 register  ********************/
#define  SPI_CR2_RXDMAEN                     ((uint_fast32_t)0x00000001)               /*!<Rx Buffer DMA Enable                 */
#define  SPI_CR2_TXDMAEN                     ((uint_fast32_t)0x00000002)               /*!<Tx Buffer DMA Enable                 */
#define  SPI_CR2_SSOE                        ((uint_fast32_t)0x00000004)               /*!<SS Output Enable                     */
#define  SPI_CR2_FRF                         ((uint_fast32_t)0x00000010)               /*!<Frame Format                         */
#define  SPI_CR2_ERRIE                       ((uint_fast32_t)0x00000020)               /*!<Error Interrupt Enable               */
#define  SPI_CR2_RXNEIE                      ((uint_fast32_t)0x00000040)               /*!<RX buffer Not Empty Interrupt Enable */
#define  SPI_CR2_TXEIE                       ((uint_fast32_t)0x00000080)               /*!<Tx buffer Empty Interrupt Enable     */

/********************  Bit definition for SPI_SR register  ********************/
#define  SPI_SR_RXNE                         ((uint_fast32_t)0x00000001)               /*!<Receive buffer Not Empty */
#define  SPI_SR_TXE                          ((uint_fast32_t)0x00000002)               /*!<Transmit buffer Empty    */
#define  SPI_SR_CHSIDE                       ((uint_fast32_t)0x00000004)               /*!<Channel side             */
#define  SPI_SR_UDR                          ((uint_fast32_t)0x00000008)               /*!<Underrun flag            */
#define  SPI_SR_CRCERR                       ((uint_fast32_t)0x00000010)               /*!<CRC Error flag           */
#define  SPI_SR_MODF                         ((uint_fast32_t)0x00000020)               /*!<Mode fault               */
#define  SPI_SR_OVR                          ((uint_fast32_t)0x00000040)               /*!<Overrun flag             */
#define  SPI_SR_BSY                          ((uint_fast32_t)0x00000080)               /*!<Busy flag                */
#define  SPI_SR_FRE                          ((uint_fast32_t)0x00000100)               /*!<Frame format error flag  */

/********************  Bit definition for SPI_DR register  ********************/
#define  SPI_DR_DR                           ((uint_fast32_t)0x0000FFFF)            /*!<Data Register           */

/*******************  Bit definition for SPI_CRCPR register  ******************/
#define  SPI_CRCPR_CRCPOLY                   ((uint_fast32_t)0x0000FFFF)            /*!<CRC polynomial register */

/******************  Bit definition for SPI_RXCRCR register  ******************/
#define  SPI_RXCRCR_RXCRC                    ((uint_fast32_t)0x0000FFFF)            /*!<Rx CRC Register         */

/******************  Bit definition for SPI_TXCRCR register  ******************/
#define  SPI_TXCRCR_TXCRC                    ((uint_fast32_t)0x0000FFFF)            /*!<Tx CRC Register         */

/******************  Bit definition for SPI_I2SCFGR register  *****************/
#define  SPI_I2SCFGR_CHLEN                   ((uint_fast32_t)0x00000001)            /*!<Channel length (number of bits per audio channel) */

#define  SPI_I2SCFGR_DATLEN                  ((uint_fast32_t)0x00000006)            /*!<DATLEN[1:0] bits (Data length to be transferred)  */
#define  SPI_I2SCFGR_DATLEN_0                ((uint_fast32_t)0x00000002)            /*!<Bit 0 */
#define  SPI_I2SCFGR_DATLEN_1                ((uint_fast32_t)0x00000004)            /*!<Bit 1 */

#define  SPI_I2SCFGR_CKPOL                   ((uint_fast32_t)0x00000008)            /*!<steady state clock polarity               */

#define  SPI_I2SCFGR_I2SSTD                  ((uint_fast32_t)0x00000030)            /*!<I2SSTD[1:0] bits (I2S standard selection) */
#define  SPI_I2SCFGR_I2SSTD_0                ((uint_fast32_t)0x00000010)            /*!<Bit 0 */
#define  SPI_I2SCFGR_I2SSTD_1                ((uint_fast32_t)0x00000020)            /*!<Bit 1 */

#define  SPI_I2SCFGR_PCMSYNC                 ((uint_fast32_t)0x00000080)            /*!<PCM frame synchronization                 */

#define  SPI_I2SCFGR_I2SCFG                  ((uint_fast32_t)0x00000300)            /*!<I2SCFG[1:0] bits (I2S configuration mode) */
#define  SPI_I2SCFGR_I2SCFG_0                ((uint_fast32_t)0x00000100)            /*!<Bit 0 */
#define  SPI_I2SCFGR_I2SCFG_1                ((uint_fast32_t)0x00000200)            /*!<Bit 1 */

#define  SPI_I2SCFGR_I2SE                    ((uint_fast32_t)0x00000400)            /*!<I2S Enable         */
#define  SPI_I2SCFGR_I2SMOD                  ((uint_fast32_t)0x00000800)            /*!<I2S mode selection */

/******************  Bit definition for SPI_I2SPR register  *******************/
#define  SPI_I2SPR_I2SDIV                    ((uint_fast32_t)0x000000FF)            /*!<I2S Linear prescaler         */
#define  SPI_I2SPR_ODD                       ((uint_fast32_t)0x00000100)            /*!<Odd factor for the prescaler */
#define  SPI_I2SPR_MCKOE                     ((uint_fast32_t)0x00000200)            /*!<Master Clock Output Enable   */

/******************************************************************************/
/*                                                                            */
/*                                 SYSCFG                                     */
/*                                                                            */
/******************************************************************************/
/******************  Bit definition for SYSCFG_MEMRMP register  ***************/
#define SYSCFG_MEMRMP_MEM_MODE          ((uint_fast32_t)0x00000007) /*!< SYSCFG_Memory Remap Config */
#define SYSCFG_MEMRMP_MEM_MODE_0        ((uint_fast32_t)0x00000001)
#define SYSCFG_MEMRMP_MEM_MODE_1        ((uint_fast32_t)0x00000002)
#define SYSCFG_MEMRMP_MEM_MODE_2        ((uint_fast32_t)0x00000004)

/******************  Bit definition for SYSCFG_PMC register  ******************/
#define SYSCFG_PMC_MII_RMII_SEL         ((uint_fast32_t)0x00800000) /*!<Ethernet PHY interface selection */
/* Old MII_RMII_SEL bit definition, maintained for legacy purpose */
#define SYSCFG_PMC_MII_RMII             SYSCFG_PMC_MII_RMII_SEL

/*****************  Bit definition for SYSCFG_EXTICR1 register  ***************/
#define SYSCFG_EXTICR1_EXTI0            ((uint_fast32_t)0x000F) /*!<EXTI 0 configuration */
#define SYSCFG_EXTICR1_EXTI1            ((uint_fast32_t)0x00F0) /*!<EXTI 1 configuration */
#define SYSCFG_EXTICR1_EXTI2            ((uint_fast32_t)0x0F00) /*!<EXTI 2 configuration */
#define SYSCFG_EXTICR1_EXTI3            ((uint_fast32_t)0xF000) /*!<EXTI 3 configuration */
/**
  * @brief   EXTI0 configuration
  */
#define SYSCFG_EXTICR1_EXTI0_PA         ((uint_fast32_t)0x0000) /*!<PA[0] pin */
#define SYSCFG_EXTICR1_EXTI0_PB         ((uint_fast32_t)0x0001) /*!<PB[0] pin */
#define SYSCFG_EXTICR1_EXTI0_PC         ((uint_fast32_t)0x0002) /*!<PC[0] pin */
#define SYSCFG_EXTICR1_EXTI0_PD         ((uint_fast32_t)0x0003) /*!<PD[0] pin */
#define SYSCFG_EXTICR1_EXTI0_PE         ((uint_fast32_t)0x0004) /*!<PE[0] pin */
#define SYSCFG_EXTICR1_EXTI0_PF         ((uint_fast32_t)0x0005) /*!<PF[0] pin */
#define SYSCFG_EXTICR1_EXTI0_PG         ((uint_fast32_t)0x0006) /*!<PG[0] pin */
#define SYSCFG_EXTICR1_EXTI0_PH         ((uint_fast32_t)0x0007) /*!<PH[0] pin */
#define SYSCFG_EXTICR1_EXTI0_PI         ((uint_fast32_t)0x0008) /*!<PI[0] pin */

/**
  * @brief   EXTI1 configuration
  */
#define SYSCFG_EXTICR1_EXTI1_PA         ((uint_fast32_t)0x0000) /*!<PA[1] pin */
#define SYSCFG_EXTICR1_EXTI1_PB         ((uint_fast32_t)0x0010) /*!<PB[1] pin */
#define SYSCFG_EXTICR1_EXTI1_PC         ((uint_fast32_t)0x0020) /*!<PC[1] pin */
#define SYSCFG_EXTICR1_EXTI1_PD         ((uint_fast32_t)0x0030) /*!<PD[1] pin */
#define SYSCFG_EXTICR1_EXTI1_PE         ((uint_fast32_t)0x0040) /*!<PE[1] pin */
#define SYSCFG_EXTICR1_EXTI1_PF         ((uint_fast32_t)0x0050) /*!<PF[1] pin */
#define SYSCFG_EXTICR1_EXTI1_PG         ((uint_fast32_t)0x0060) /*!<PG[1] pin */
#define SYSCFG_EXTICR1_EXTI1_PH         ((uint_fast32_t)0x0070) /*!<PH[1] pin */
#define SYSCFG_EXTICR1_EXTI1_PI         ((uint_fast32_t)0x0080) /*!<PI[1] pin */

/**
  * @brief   EXTI2 configuration
  */
#define SYSCFG_EXTICR1_EXTI2_PA         ((uint_fast32_t)0x0000) /*!<PA[2] pin */
#define SYSCFG_EXTICR1_EXTI2_PB         ((uint_fast32_t)0x0100) /*!<PB[2] pin */
#define SYSCFG_EXTICR1_EXTI2_PC         ((uint_fast32_t)0x0200) /*!<PC[2] pin */
#define SYSCFG_EXTICR1_EXTI2_PD         ((uint_fast32_t)0x0300) /*!<PD[2] pin */
#define SYSCFG_EXTICR1_EXTI2_PE         ((uint_fast32_t)0x0400) /*!<PE[2] pin */
#define SYSCFG_EXTICR1_EXTI2_PF         ((uint_fast32_t)0x0500) /*!<PF[2] pin */
#define SYSCFG_EXTICR1_EXTI2_PG         ((uint_fast32_t)0x0600) /*!<PG[2] pin */
#define SYSCFG_EXTICR1_EXTI2_PH         ((uint_fast32_t)0x0700) /*!<PH[2] pin */
#define SYSCFG_EXTICR1_EXTI2_PI         ((uint_fast32_t)0x0800) /*!<PI[2] pin */

/**
  * @brief   EXTI3 configuration
  */
#define SYSCFG_EXTICR1_EXTI3_PA         ((uint_fast32_t)0x0000) /*!<PA[3] pin */
#define SYSCFG_EXTICR1_EXTI3_PB         ((uint_fast32_t)0x1000) /*!<PB[3] pin */
#define SYSCFG_EXTICR1_EXTI3_PC         ((uint_fast32_t)0x2000) /*!<PC[3] pin */
#define SYSCFG_EXTICR1_EXTI3_PD         ((uint_fast32_t)0x3000) /*!<PD[3] pin */
#define SYSCFG_EXTICR1_EXTI3_PE         ((uint_fast32_t)0x4000) /*!<PE[3] pin */
#define SYSCFG_EXTICR1_EXTI3_PF         ((uint_fast32_t)0x5000) /*!<PF[3] pin */
#define SYSCFG_EXTICR1_EXTI3_PG         ((uint_fast32_t)0x6000) /*!<PG[3] pin */
#define SYSCFG_EXTICR1_EXTI3_PH         ((uint_fast32_t)0x7000) /*!<PH[3] pin */
#define SYSCFG_EXTICR1_EXTI3_PI         ((uint_fast32_t)0x8000) /*!<PI[3] pin */

/*****************  Bit definition for SYSCFG_EXTICR2 register  ***************/
#define SYSCFG_EXTICR2_EXTI4            ((uint_fast32_t)0x000F) /*!<EXTI 4 configuration */
#define SYSCFG_EXTICR2_EXTI5            ((uint_fast32_t)0x00F0) /*!<EXTI 5 configuration */
#define SYSCFG_EXTICR2_EXTI6            ((uint_fast32_t)0x0F00) /*!<EXTI 6 configuration */
#define SYSCFG_EXTICR2_EXTI7            ((uint_fast32_t)0xF000) /*!<EXTI 7 configuration */
/**
  * @brief   EXTI4 configuration
  */
#define SYSCFG_EXTICR2_EXTI4_PA         ((uint_fast32_t)0x0000) /*!<PA[4] pin */
#define SYSCFG_EXTICR2_EXTI4_PB         ((uint_fast32_t)0x0001) /*!<PB[4] pin */
#define SYSCFG_EXTICR2_EXTI4_PC         ((uint_fast32_t)0x0002) /*!<PC[4] pin */
#define SYSCFG_EXTICR2_EXTI4_PD         ((uint_fast32_t)0x0003) /*!<PD[4] pin */
#define SYSCFG_EXTICR2_EXTI4_PE         ((uint_fast32_t)0x0004) /*!<PE[4] pin */
#define SYSCFG_EXTICR2_EXTI4_PF         ((uint_fast32_t)0x0005) /*!<PF[4] pin */
#define SYSCFG_EXTICR2_EXTI4_PG         ((uint_fast32_t)0x0006) /*!<PG[4] pin */
#define SYSCFG_EXTICR2_EXTI4_PH         ((uint_fast32_t)0x0007) /*!<PH[4] pin */
#define SYSCFG_EXTICR2_EXTI4_PI         ((uint_fast32_t)0x0008) /*!<PI[4] pin */

/**
  * @brief   EXTI5 configuration
  */
#define SYSCFG_EXTICR2_EXTI5_PA         ((uint_fast32_t)0x0000) /*!<PA[5] pin */
#define SYSCFG_EXTICR2_EXTI5_PB         ((uint_fast32_t)0x0010) /*!<PB[5] pin */
#define SYSCFG_EXTICR2_EXTI5_PC         ((uint_fast32_t)0x0020) /*!<PC[5] pin */
#define SYSCFG_EXTICR2_EXTI5_PD         ((uint_fast32_t)0x0030) /*!<PD[5] pin */
#define SYSCFG_EXTICR2_EXTI5_PE         ((uint_fast32_t)0x0040) /*!<PE[5] pin */
#define SYSCFG_EXTICR2_EXTI5_PF         ((uint_fast32_t)0x0050) /*!<PF[5] pin */
#define SYSCFG_EXTICR2_EXTI5_PG         ((uint_fast32_t)0x0060) /*!<PG[5] pin */
#define SYSCFG_EXTICR2_EXTI5_PH         ((uint_fast32_t)0x0070) /*!<PH[5] pin */
#define SYSCFG_EXTICR2_EXTI5_PI         ((uint_fast32_t)0x0080) /*!<PI[5] pin */

/**
  * @brief   EXTI6 configuration
  */
#define SYSCFG_EXTICR2_EXTI6_PA         ((uint_fast32_t)0x0000) /*!<PA[6] pin */
#define SYSCFG_EXTICR2_EXTI6_PB         ((uint_fast32_t)0x0100) /*!<PB[6] pin */
#define SYSCFG_EXTICR2_EXTI6_PC         ((uint_fast32_t)0x0200) /*!<PC[6] pin */
#define SYSCFG_EXTICR2_EXTI6_PD         ((uint_fast32_t)0x0300) /*!<PD[6] pin */
#define SYSCFG_EXTICR2_EXTI6_PE         ((uint_fast32_t)0x0400) /*!<PE[6] pin */
#define SYSCFG_EXTICR2_EXTI6_PF         ((uint_fast32_t)0x0500) /*!<PF[6] pin */
#define SYSCFG_EXTICR2_EXTI6_PG         ((uint_fast32_t)0x0600) /*!<PG[6] pin */
#define SYSCFG_EXTICR2_EXTI6_PH         ((uint_fast32_t)0x0700) /*!<PH[6] pin */
#define SYSCFG_EXTICR2_EXTI6_PI         ((uint_fast32_t)0x0800) /*!<PI[6] pin */

/**
  * @brief   EXTI7 configuration
  */
#define SYSCFG_EXTICR2_EXTI7_PA         ((uint_fast32_t)0x0000) /*!<PA[7] pin */
#define SYSCFG_EXTICR2_EXTI7_PB         ((uint_fast32_t)0x1000) /*!<PB[7] pin */
#define SYSCFG_EXTICR2_EXTI7_PC         ((uint_fast32_t)0x2000) /*!<PC[7] pin */
#define SYSCFG_EXTICR2_EXTI7_PD         ((uint_fast32_t)0x3000) /*!<PD[7] pin */
#define SYSCFG_EXTICR2_EXTI7_PE         ((uint_fast32_t)0x4000) /*!<PE[7] pin */
#define SYSCFG_EXTICR2_EXTI7_PF         ((uint_fast32_t)0x5000) /*!<PF[7] pin */
#define SYSCFG_EXTICR2_EXTI7_PG         ((uint_fast32_t)0x6000) /*!<PG[7] pin */
#define SYSCFG_EXTICR2_EXTI7_PH         ((uint_fast32_t)0x7000) /*!<PH[7] pin */
#define SYSCFG_EXTICR2_EXTI7_PI         ((uint_fast32_t)0x8000) /*!<PI[7] pin */


/*****************  Bit definition for SYSCFG_EXTICR3 register  ***************/
#define SYSCFG_EXTICR3_EXTI8            ((uint_fast32_t)0x000F) /*!<EXTI 8 configuration */
#define SYSCFG_EXTICR3_EXTI9            ((uint_fast32_t)0x00F0) /*!<EXTI 9 configuration */
#define SYSCFG_EXTICR3_EXTI10           ((uint_fast32_t)0x0F00) /*!<EXTI 10 configuration */
#define SYSCFG_EXTICR3_EXTI11           ((uint_fast32_t)0xF000) /*!<EXTI 11 configuration */

/**
  * @brief   EXTI8 configuration
  */
#define SYSCFG_EXTICR3_EXTI8_PA         ((uint_fast32_t)0x0000) /*!<PA[8] pin */
#define SYSCFG_EXTICR3_EXTI8_PB         ((uint_fast32_t)0x0001) /*!<PB[8] pin */
#define SYSCFG_EXTICR3_EXTI8_PC         ((uint_fast32_t)0x0002) /*!<PC[8] pin */
#define SYSCFG_EXTICR3_EXTI8_PD         ((uint_fast32_t)0x0003) /*!<PD[8] pin */
#define SYSCFG_EXTICR3_EXTI8_PE         ((uint_fast32_t)0x0004) /*!<PE[8] pin */
#define SYSCFG_EXTICR3_EXTI8_PF         ((uint_fast32_t)0x0005) /*!<PF[8] pin */
#define SYSCFG_EXTICR3_EXTI8_PG         ((uint_fast32_t)0x0006) /*!<PG[8] pin */
#define SYSCFG_EXTICR3_EXTI8_PH         ((uint_fast32_t)0x0007) /*!<PH[8] pin */
#define SYSCFG_EXTICR3_EXTI8_PI         ((uint_fast32_t)0x0008) /*!<PI[8] pin */

/**
  * @brief   EXTI9 configuration
  */
#define SYSCFG_EXTICR3_EXTI9_PA         ((uint_fast32_t)0x0000) /*!<PA[9] pin */
#define SYSCFG_EXTICR3_EXTI9_PB         ((uint_fast32_t)0x0010) /*!<PB[9] pin */
#define SYSCFG_EXTICR3_EXTI9_PC         ((uint_fast32_t)0x0020) /*!<PC[9] pin */
#define SYSCFG_EXTICR3_EXTI9_PD         ((uint_fast32_t)0x0030) /*!<PD[9] pin */
#define SYSCFG_EXTICR3_EXTI9_PE         ((uint_fast32_t)0x0040) /*!<PE[9] pin */
#define SYSCFG_EXTICR3_EXTI9_PF         ((uint_fast32_t)0x0050) /*!<PF[9] pin */
#define SYSCFG_EXTICR3_EXTI9_PG         ((uint_fast32_t)0x0060) /*!<PG[9] pin */
#define SYSCFG_EXTICR3_EXTI9_PH         ((uint_fast32_t)0x0070) /*!<PH[9] pin */
#define SYSCFG_EXTICR3_EXTI9_PI         ((uint_fast32_t)0x0080) /*!<PI[9] pin */

/**
  * @brief   EXTI10 configuration
  */
#define SYSCFG_EXTICR3_EXTI10_PA        ((uint_fast32_t)0x0000) /*!<PA[10] pin */
#define SYSCFG_EXTICR3_EXTI10_PB        ((uint_fast32_t)0x0100) /*!<PB[10] pin */
#define SYSCFG_EXTICR3_EXTI10_PC        ((uint_fast32_t)0x0200) /*!<PC[10] pin */
#define SYSCFG_EXTICR3_EXTI10_PD        ((uint_fast32_t)0x0300) /*!<PD[10] pin */
#define SYSCFG_EXTICR3_EXTI10_PE        ((uint_fast32_t)0x0400) /*!<PE[10] pin */
#define SYSCFG_EXTICR3_EXTI10_PF        ((uint_fast32_t)0x0500) /*!<PF[10] pin */
#define SYSCFG_EXTICR3_EXTI10_PG        ((uint_fast32_t)0x0600) /*!<PG[10] pin */
#define SYSCFG_EXTICR3_EXTI10_PH        ((uint_fast32_t)0x0700) /*!<PH[10] pin */
#define SYSCFG_EXTICR3_EXTI10_PI        ((uint_fast32_t)0x0800) /*!<PI[10] pin */

/**
  * @brief   EXTI11 configuration
  */
#define SYSCFG_EXTICR3_EXTI11_PA        ((uint_fast32_t)0x0000) /*!<PA[11] pin */
#define SYSCFG_EXTICR3_EXTI11_PB        ((uint_fast32_t)0x1000) /*!<PB[11] pin */
#define SYSCFG_EXTICR3_EXTI11_PC        ((uint_fast32_t)0x2000) /*!<PC[11] pin */
#define SYSCFG_EXTICR3_EXTI11_PD        ((uint_fast32_t)0x3000) /*!<PD[11] pin */
#define SYSCFG_EXTICR3_EXTI11_PE        ((uint_fast32_t)0x4000) /*!<PE[11] pin */
#define SYSCFG_EXTICR3_EXTI11_PF        ((uint_fast32_t)0x5000) /*!<PF[11] pin */
#define SYSCFG_EXTICR3_EXTI11_PG        ((uint_fast32_t)0x6000) /*!<PG[11] pin */
#define SYSCFG_EXTICR3_EXTI11_PH        ((uint_fast32_t)0x7000) /*!<PH[11] pin */
#define SYSCFG_EXTICR3_EXTI11_PI        ((uint_fast32_t)0x8000) /*!<PI[11] pin */

/*****************  Bit definition for SYSCFG_EXTICR4 register  ***************/
#define SYSCFG_EXTICR4_EXTI12           ((uint_fast32_t)0x000F) /*!<EXTI 12 configuration */
#define SYSCFG_EXTICR4_EXTI13           ((uint_fast32_t)0x00F0) /*!<EXTI 13 configuration */
#define SYSCFG_EXTICR4_EXTI14           ((uint_fast32_t)0x0F00) /*!<EXTI 14 configuration */
#define SYSCFG_EXTICR4_EXTI15           ((uint_fast32_t)0xF000) /*!<EXTI 15 configuration */
/**
  * @brief   EXTI12 configuration
  */
#define SYSCFG_EXTICR4_EXTI12_PA        ((uint_fast32_t)0x0000) /*!<PA[12] pin */
#define SYSCFG_EXTICR4_EXTI12_PB        ((uint_fast32_t)0x0001) /*!<PB[12] pin */
#define SYSCFG_EXTICR4_EXTI12_PC        ((uint_fast32_t)0x0002) /*!<PC[12] pin */
#define SYSCFG_EXTICR4_EXTI12_PD        ((uint_fast32_t)0x0003) /*!<PD[12] pin */
#define SYSCFG_EXTICR4_EXTI12_PE        ((uint_fast32_t)0x0004) /*!<PE[12] pin */
#define SYSCFG_EXTICR4_EXTI12_PF        ((uint_fast32_t)0x0005) /*!<PF[12] pin */
#define SYSCFG_EXTICR4_EXTI12_PG        ((uint_fast32_t)0x0006) /*!<PG[12] pin */
#define SYSCFG_EXTICR4_EXTI12_PH        ((uint_fast32_t)0x0007) /*!<PH[12] pin */

/**
  * @brief   EXTI13 configuration
  */
#define SYSCFG_EXTICR4_EXTI13_PA        ((uint_fast32_t)0x0000) /*!<PA[13] pin */
#define SYSCFG_EXTICR4_EXTI13_PB        ((uint_fast32_t)0x0010) /*!<PB[13] pin */
#define SYSCFG_EXTICR4_EXTI13_PC        ((uint_fast32_t)0x0020) /*!<PC[13] pin */
#define SYSCFG_EXTICR4_EXTI13_PD        ((uint_fast32_t)0x0030) /*!<PD[13] pin */
#define SYSCFG_EXTICR4_EXTI13_PE        ((uint_fast32_t)0x0040) /*!<PE[13] pin */
#define SYSCFG_EXTICR4_EXTI13_PF        ((uint_fast32_t)0x0050) /*!<PF[13] pin */
#define SYSCFG_EXTICR4_EXTI13_PG        ((uint_fast32_t)0x0060) /*!<PG[13] pin */
#define SYSCFG_EXTICR4_EXTI13_PH        ((uint_fast32_t)0x0070) /*!<PH[13] pin */

/**
  * @brief   EXTI14 configuration
  */
#define SYSCFG_EXTICR4_EXTI14_PA        ((uint_fast32_t)0x0000) /*!<PA[14] pin */
#define SYSCFG_EXTICR4_EXTI14_PB        ((uint_fast32_t)0x0100) /*!<PB[14] pin */
#define SYSCFG_EXTICR4_EXTI14_PC        ((uint_fast32_t)0x0200) /*!<PC[14] pin */
#define SYSCFG_EXTICR4_EXTI14_PD        ((uint_fast32_t)0x0300) /*!<PD[14] pin */
#define SYSCFG_EXTICR4_EXTI14_PE        ((uint_fast32_t)0x0400) /*!<PE[14] pin */
#define SYSCFG_EXTICR4_EXTI14_PF        ((uint_fast32_t)0x0500) /*!<PF[14] pin */
#define SYSCFG_EXTICR4_EXTI14_PG        ((uint_fast32_t)0x0600) /*!<PG[14] pin */
#define SYSCFG_EXTICR4_EXTI14_PH        ((uint_fast32_t)0x0700) /*!<PH[14] pin */

/**
  * @brief   EXTI15 configuration
  */
#define SYSCFG_EXTICR4_EXTI15_PA        ((uint_fast32_t)0x0000) /*!<PA[15] pin */
#define SYSCFG_EXTICR4_EXTI15_PB        ((uint_fast32_t)0x1000) /*!<PB[15] pin */
#define SYSCFG_EXTICR4_EXTI15_PC        ((uint_fast32_t)0x2000) /*!<PC[15] pin */
#define SYSCFG_EXTICR4_EXTI15_PD        ((uint_fast32_t)0x3000) /*!<PD[15] pin */
#define SYSCFG_EXTICR4_EXTI15_PE        ((uint_fast32_t)0x4000) /*!<PE[15] pin */
#define SYSCFG_EXTICR4_EXTI15_PF        ((uint_fast32_t)0x5000) /*!<PF[15] pin */
#define SYSCFG_EXTICR4_EXTI15_PG        ((uint_fast32_t)0x6000) /*!<PG[15] pin */
#define SYSCFG_EXTICR4_EXTI15_PH        ((uint_fast32_t)0x7000) /*!<PH[15] pin */

/******************  Bit definition for SYSCFG_CMPCR register  ****************/
#define SYSCFG_CMPCR_CMP_PD             ((uint_fast32_t)0x00000001) /*!<Compensation cell ready flag */
#define SYSCFG_CMPCR_READY              ((uint_fast32_t)0x00000100) /*!<Compensation cell power-down */

/******************************************************************************/
/*                                                                            */
/*                                    TIM                                     */
/*                                                                            */
/******************************************************************************/
/*******************  Bit definition for TIM_CR1 register  ********************/
#define  TIM_CR1_CEN                         ((uint_fast32_t)0x0001)            /*!<Counter enable        */
#define  TIM_CR1_UDIS                        ((uint_fast32_t)0x0002)            /*!<Update disable        */
#define  TIM_CR1_URS                         ((uint_fast32_t)0x0004)            /*!<Update request source */
#define  TIM_CR1_OPM                         ((uint_fast32_t)0x0008)            /*!<One pulse mode        */
#define  TIM_CR1_DIR                         ((uint_fast32_t)0x0010)            /*!<Direction             */

#define  TIM_CR1_CMS                         ((uint_fast32_t)0x0060)            /*!<CMS[1:0] bits (Center-aligned mode selection) */
#define  TIM_CR1_CMS_0                       ((uint_fast32_t)0x0020)            /*!<Bit 0 */
#define  TIM_CR1_CMS_1                       ((uint_fast32_t)0x0040)            /*!<Bit 1 */

#define  TIM_CR1_ARPE                        ((uint_fast32_t)0x0080)            /*!<Auto-reload preload enable     */

#define  TIM_CR1_CKD                         ((uint_fast32_t)0x0300)            /*!<CKD[1:0] bits (clock division) */
#define  TIM_CR1_CKD_0                       ((uint_fast32_t)0x0100)            /*!<Bit 0 */
#define  TIM_CR1_CKD_1                       ((uint_fast32_t)0x0200)            /*!<Bit 1 */

/*******************  Bit definition for TIM_CR2 register  ********************/
#define  TIM_CR2_CCPC                        ((uint_fast32_t)0x0001)            /*!<Capture/Compare Preloaded Control        */
#define  TIM_CR2_CCUS                        ((uint_fast32_t)0x0004)            /*!<Capture/Compare Control Update Selection */
#define  TIM_CR2_CCDS                        ((uint_fast32_t)0x0008)            /*!<Capture/Compare DMA Selection            */

#define  TIM_CR2_MMS                         ((uint_fast32_t)0x0070)            /*!<MMS[2:0] bits (Master Mode Selection) */
#define  TIM_CR2_MMS_0                       ((uint_fast32_t)0x0010)            /*!<Bit 0 */
#define  TIM_CR2_MMS_1                       ((uint_fast32_t)0x0020)            /*!<Bit 1 */
#define  TIM_CR2_MMS_2                       ((uint_fast32_t)0x0040)            /*!<Bit 2 */

#define  TIM_CR2_TI1S                        ((uint_fast32_t)0x0080)            /*!<TI1 Selection */
#define  TIM_CR2_OIS1                        ((uint_fast32_t)0x0100)            /*!<Output Idle state 1 (OC1 output)  */
#define  TIM_CR2_OIS1N                       ((uint_fast32_t)0x0200)            /*!<Output Idle state 1 (OC1N output) */
#define  TIM_CR2_OIS2                        ((uint_fast32_t)0x0400)            /*!<Output Idle state 2 (OC2 output)  */
#define  TIM_CR2_OIS2N                       ((uint_fast32_t)0x0800)            /*!<Output Idle state 2 (OC2N output) */
#define  TIM_CR2_OIS3                        ((uint_fast32_t)0x1000)            /*!<Output Idle state 3 (OC3 output)  */
#define  TIM_CR2_OIS3N                       ((uint_fast32_t)0x2000)            /*!<Output Idle state 3 (OC3N output) */
#define  TIM_CR2_OIS4                        ((uint_fast32_t)0x4000)            /*!<Output Idle state 4 (OC4 output)  */

/*******************  Bit definition for TIM_SMCR register  *******************/
#define  TIM_SMCR_SMS                        ((uint_fast32_t)0x0007)            /*!<SMS[2:0] bits (Slave mode selection)    */
#define  TIM_SMCR_SMS_0                      ((uint_fast32_t)0x0001)            /*!<Bit 0 */
#define  TIM_SMCR_SMS_1                      ((uint_fast32_t)0x0002)            /*!<Bit 1 */
#define  TIM_SMCR_SMS_2                      ((uint_fast32_t)0x0004)            /*!<Bit 2 */

#define  TIM_SMCR_TS                         ((uint_fast32_t)0x0070)            /*!<TS[2:0] bits (Trigger selection)        */
#define  TIM_SMCR_TS_0                       ((uint_fast32_t)0x0010)            /*!<Bit 0 */
#define  TIM_SMCR_TS_1                       ((uint_fast32_t)0x0020)            /*!<Bit 1 */
#define  TIM_SMCR_TS_2                       ((uint_fast32_t)0x0040)            /*!<Bit 2 */

#define  TIM_SMCR_MSM                        ((uint_fast32_t)0x0080)            /*!<Master/slave mode                       */

#define  TIM_SMCR_ETF                        ((uint_fast32_t)0x0F00)            /*!<ETF[3:0] bits (External trigger filter) */
#define  TIM_SMCR_ETF_0                      ((uint_fast32_t)0x0100)            /*!<Bit 0 */
#define  TIM_SMCR_ETF_1                      ((uint_fast32_t)0x0200)            /*!<Bit 1 */
#define  TIM_SMCR_ETF_2                      ((uint_fast32_t)0x0400)            /*!<Bit 2 */
#define  TIM_SMCR_ETF_3                      ((uint_fast32_t)0x0800)            /*!<Bit 3 */

#define  TIM_SMCR_ETPS                       ((uint_fast32_t)0x3000)            /*!<ETPS[1:0] bits (External trigger prescaler) */
#define  TIM_SMCR_ETPS_0                     ((uint_fast32_t)0x1000)            /*!<Bit 0 */
#define  TIM_SMCR_ETPS_1                     ((uint_fast32_t)0x2000)            /*!<Bit 1 */

#define  TIM_SMCR_ECE                        ((uint_fast32_t)0x4000)            /*!<External clock enable     */
#define  TIM_SMCR_ETP                        ((uint_fast32_t)0x8000)            /*!<External trigger polarity */

/*******************  Bit definition for TIM_DIER register  *******************/
#define  TIM_DIER_UIE                        ((uint_fast32_t)0x0001)            /*!<Update interrupt enable */
#define  TIM_DIER_CC1IE                      ((uint_fast32_t)0x0002)            /*!<Capture/Compare 1 interrupt enable   */
#define  TIM_DIER_CC2IE                      ((uint_fast32_t)0x0004)            /*!<Capture/Compare 2 interrupt enable   */
#define  TIM_DIER_CC3IE                      ((uint_fast32_t)0x0008)            /*!<Capture/Compare 3 interrupt enable   */
#define  TIM_DIER_CC4IE                      ((uint_fast32_t)0x0010)            /*!<Capture/Compare 4 interrupt enable   */
#define  TIM_DIER_COMIE                      ((uint_fast32_t)0x0020)            /*!<COM interrupt enable                 */
#define  TIM_DIER_TIE                        ((uint_fast32_t)0x0040)            /*!<Trigger interrupt enable             */
#define  TIM_DIER_BIE                        ((uint_fast32_t)0x0080)            /*!<Break interrupt enable               */
#define  TIM_DIER_UDE                        ((uint_fast32_t)0x0100)            /*!<Update DMA request enable            */
#define  TIM_DIER_CC1DE                      ((uint_fast32_t)0x0200)            /*!<Capture/Compare 1 DMA request enable */
#define  TIM_DIER_CC2DE                      ((uint_fast32_t)0x0400)            /*!<Capture/Compare 2 DMA request enable */
#define  TIM_DIER_CC3DE                      ((uint_fast32_t)0x0800)            /*!<Capture/Compare 3 DMA request enable */
#define  TIM_DIER_CC4DE                      ((uint_fast32_t)0x1000)            /*!<Capture/Compare 4 DMA request enable */
#define  TIM_DIER_COMDE                      ((uint_fast32_t)0x2000)            /*!<COM DMA request enable               */
#define  TIM_DIER_TDE                        ((uint_fast32_t)0x4000)            /*!<Trigger DMA request enable           */

/********************  Bit definition for TIM_SR register  ********************/
#define  TIM_SR_UIF                          ((uint_fast32_t)0x0001)            /*!<Update interrupt Flag              */
#define  TIM_SR_CC1IF                        ((uint_fast32_t)0x0002)            /*!<Capture/Compare 1 interrupt Flag   */
#define  TIM_SR_CC2IF                        ((uint_fast32_t)0x0004)            /*!<Capture/Compare 2 interrupt Flag   */
#define  TIM_SR_CC3IF                        ((uint_fast32_t)0x0008)            /*!<Capture/Compare 3 interrupt Flag   */
#define  TIM_SR_CC4IF                        ((uint_fast32_t)0x0010)            /*!<Capture/Compare 4 interrupt Flag   */
#define  TIM_SR_COMIF                        ((uint_fast32_t)0x0020)            /*!<COM interrupt Flag                 */
#define  TIM_SR_TIF                          ((uint_fast32_t)0x0040)            /*!<Trigger interrupt Flag             */
#define  TIM_SR_BIF                          ((uint_fast32_t)0x0080)            /*!<Break interrupt Flag               */
#define  TIM_SR_CC1OF                        ((uint_fast32_t)0x0200)            /*!<Capture/Compare 1 Overcapture Flag */
#define  TIM_SR_CC2OF                        ((uint_fast32_t)0x0400)            /*!<Capture/Compare 2 Overcapture Flag */
#define  TIM_SR_CC3OF                        ((uint_fast32_t)0x0800)            /*!<Capture/Compare 3 Overcapture Flag */
#define  TIM_SR_CC4OF                        ((uint_fast32_t)0x1000)            /*!<Capture/Compare 4 Overcapture Flag */

/*******************  Bit definition for TIM_EGR register  ********************/
#define  TIM_EGR_UG                          ((uint_fast32_t)0x01)               /*!<Update Generation                         */
#define  TIM_EGR_CC1G                        ((uint_fast32_t)0x02)               /*!<Capture/Compare 1 Generation              */
#define  TIM_EGR_CC2G                        ((uint_fast32_t)0x04)               /*!<Capture/Compare 2 Generation              */
#define  TIM_EGR_CC3G                        ((uint_fast32_t)0x08)               /*!<Capture/Compare 3 Generation              */
#define  TIM_EGR_CC4G                        ((uint_fast32_t)0x10)               /*!<Capture/Compare 4 Generation              */
#define  TIM_EGR_COMG                        ((uint_fast32_t)0x20)               /*!<Capture/Compare Control Update Generation */
#define  TIM_EGR_TG                          ((uint_fast32_t)0x40)               /*!<Trigger Generation                        */
#define  TIM_EGR_BG                          ((uint_fast32_t)0x80)               /*!<Break Generation                          */

/******************  Bit definition for TIM_CCMR1 register  *******************/
#define  TIM_CCMR1_CC1S                      ((uint_fast32_t)0x0003)            /*!<CC1S[1:0] bits (Capture/Compare 1 Selection) */
#define  TIM_CCMR1_CC1S_0                    ((uint_fast32_t)0x0001)            /*!<Bit 0 */
#define  TIM_CCMR1_CC1S_1                    ((uint_fast32_t)0x0002)            /*!<Bit 1 */

#define  TIM_CCMR1_OC1FE                     ((uint_fast32_t)0x0004)            /*!<Output Compare 1 Fast enable                 */
#define  TIM_CCMR1_OC1PE                     ((uint_fast32_t)0x0008)            /*!<Output Compare 1 Preload enable              */

#define  TIM_CCMR1_OC1M                      ((uint_fast32_t)0x0070)            /*!<OC1M[2:0] bits (Output Compare 1 Mode)       */
#define  TIM_CCMR1_OC1M_0                    ((uint_fast32_t)0x0010)            /*!<Bit 0 */
#define  TIM_CCMR1_OC1M_1                    ((uint_fast32_t)0x0020)            /*!<Bit 1 */
#define  TIM_CCMR1_OC1M_2                    ((uint_fast32_t)0x0040)            /*!<Bit 2 */

#define  TIM_CCMR1_OC1CE                     ((uint_fast32_t)0x0080)            /*!<Output Compare 1Clear Enable                 */

#define  TIM_CCMR1_CC2S                      ((uint_fast32_t)0x0300)            /*!<CC2S[1:0] bits (Capture/Compare 2 Selection) */
#define  TIM_CCMR1_CC2S_0                    ((uint_fast32_t)0x0100)            /*!<Bit 0 */
#define  TIM_CCMR1_CC2S_1                    ((uint_fast32_t)0x0200)            /*!<Bit 1 */

#define  TIM_CCMR1_OC2FE                     ((uint_fast32_t)0x0400)            /*!<Output Compare 2 Fast enable                 */
#define  TIM_CCMR1_OC2PE                     ((uint_fast32_t)0x0800)            /*!<Output Compare 2 Preload enable              */

#define  TIM_CCMR1_OC2M                      ((uint_fast32_t)0x7000)            /*!<OC2M[2:0] bits (Output Compare 2 Mode)       */
#define  TIM_CCMR1_OC2M_0                    ((uint_fast32_t)0x1000)            /*!<Bit 0 */
#define  TIM_CCMR1_OC2M_1                    ((uint_fast32_t)0x2000)            /*!<Bit 1 */
#define  TIM_CCMR1_OC2M_2                    ((uint_fast32_t)0x4000)            /*!<Bit 2 */

#define  TIM_CCMR1_OC2CE                     ((uint_fast32_t)0x8000)            /*!<Output Compare 2 Clear Enable */

/*----------------------------------------------------------------------------*/

#define  TIM_CCMR1_IC1PSC                    ((uint_fast32_t)0x000C)            /*!<IC1PSC[1:0] bits (Input Capture 1 Prescaler) */
#define  TIM_CCMR1_IC1PSC_0                  ((uint_fast32_t)0x0004)            /*!<Bit 0 */
#define  TIM_CCMR1_IC1PSC_1                  ((uint_fast32_t)0x0008)            /*!<Bit 1 */

#define  TIM_CCMR1_IC1F                      ((uint_fast32_t)0x00F0)            /*!<IC1F[3:0] bits (Input Capture 1 Filter)      */
#define  TIM_CCMR1_IC1F_0                    ((uint_fast32_t)0x0010)            /*!<Bit 0 */
#define  TIM_CCMR1_IC1F_1                    ((uint_fast32_t)0x0020)            /*!<Bit 1 */
#define  TIM_CCMR1_IC1F_2                    ((uint_fast32_t)0x0040)            /*!<Bit 2 */
#define  TIM_CCMR1_IC1F_3                    ((uint_fast32_t)0x0080)            /*!<Bit 3 */

#define  TIM_CCMR1_IC2PSC                    ((uint_fast32_t)0x0C00)            /*!<IC2PSC[1:0] bits (Input Capture 2 Prescaler)  */
#define  TIM_CCMR1_IC2PSC_0                  ((uint_fast32_t)0x0400)            /*!<Bit 0 */
#define  TIM_CCMR1_IC2PSC_1                  ((uint_fast32_t)0x0800)            /*!<Bit 1 */

#define  TIM_CCMR1_IC2F                      ((uint_fast32_t)0xF000)            /*!<IC2F[3:0] bits (Input Capture 2 Filter)       */
#define  TIM_CCMR1_IC2F_0                    ((uint_fast32_t)0x1000)            /*!<Bit 0 */
#define  TIM_CCMR1_IC2F_1                    ((uint_fast32_t)0x2000)            /*!<Bit 1 */
#define  TIM_CCMR1_IC2F_2                    ((uint_fast32_t)0x4000)            /*!<Bit 2 */
#define  TIM_CCMR1_IC2F_3                    ((uint_fast32_t)0x8000)            /*!<Bit 3 */

/******************  Bit definition for TIM_CCMR2 register  *******************/
#define  TIM_CCMR2_CC3S                      ((uint_fast32_t)0x0003)            /*!<CC3S[1:0] bits (Capture/Compare 3 Selection)  */
#define  TIM_CCMR2_CC3S_0                    ((uint_fast32_t)0x0001)            /*!<Bit 0 */
#define  TIM_CCMR2_CC3S_1                    ((uint_fast32_t)0x0002)            /*!<Bit 1 */

#define  TIM_CCMR2_OC3FE                     ((uint_fast32_t)0x0004)            /*!<Output Compare 3 Fast enable           */
#define  TIM_CCMR2_OC3PE                     ((uint_fast32_t)0x0008)            /*!<Output Compare 3 Preload enable        */

#define  TIM_CCMR2_OC3M                      ((uint_fast32_t)0x0070)            /*!<OC3M[2:0] bits (Output Compare 3 Mode) */
#define  TIM_CCMR2_OC3M_0                    ((uint_fast32_t)0x0010)            /*!<Bit 0 */
#define  TIM_CCMR2_OC3M_1                    ((uint_fast32_t)0x0020)            /*!<Bit 1 */
#define  TIM_CCMR2_OC3M_2                    ((uint_fast32_t)0x0040)            /*!<Bit 2 */

#define  TIM_CCMR2_OC3CE                     ((uint_fast32_t)0x0080)            /*!<Output Compare 3 Clear Enable */

#define  TIM_CCMR2_CC4S                      ((uint_fast32_t)0x0300)            /*!<CC4S[1:0] bits (Capture/Compare 4 Selection) */
#define  TIM_CCMR2_CC4S_0                    ((uint_fast32_t)0x0100)            /*!<Bit 0 */
#define  TIM_CCMR2_CC4S_1                    ((uint_fast32_t)0x0200)            /*!<Bit 1 */

#define  TIM_CCMR2_OC4FE                     ((uint_fast32_t)0x0400)            /*!<Output Compare 4 Fast enable    */
#define  TIM_CCMR2_OC4PE                     ((uint_fast32_t)0x0800)            /*!<Output Compare 4 Preload enable */

#define  TIM_CCMR2_OC4M                      ((uint_fast32_t)0x7000)            /*!<OC4M[2:0] bits (Output Compare 4 Mode) */
#define  TIM_CCMR2_OC4M_0                    ((uint_fast32_t)0x1000)            /*!<Bit 0 */
#define  TIM_CCMR2_OC4M_1                    ((uint_fast32_t)0x2000)            /*!<Bit 1 */
#define  TIM_CCMR2_OC4M_2                    ((uint_fast32_t)0x4000)            /*!<Bit 2 */

#define  TIM_CCMR2_OC4CE                     ((uint_fast32_t)0x8000)            /*!<Output Compare 4 Clear Enable */

/*----------------------------------------------------------------------------*/

#define  TIM_CCMR2_IC3PSC                    ((uint_fast32_t)0x000C)            /*!<IC3PSC[1:0] bits (Input Capture 3 Prescaler) */
#define  TIM_CCMR2_IC3PSC_0                  ((uint_fast32_t)0x0004)            /*!<Bit 0 */
#define  TIM_CCMR2_IC3PSC_1                  ((uint_fast32_t)0x0008)            /*!<Bit 1 */

#define  TIM_CCMR2_IC3F                      ((uint_fast32_t)0x00F0)            /*!<IC3F[3:0] bits (Input Capture 3 Filter) */
#define  TIM_CCMR2_IC3F_0                    ((uint_fast32_t)0x0010)            /*!<Bit 0 */
#define  TIM_CCMR2_IC3F_1                    ((uint_fast32_t)0x0020)            /*!<Bit 1 */
#define  TIM_CCMR2_IC3F_2                    ((uint_fast32_t)0x0040)            /*!<Bit 2 */
#define  TIM_CCMR2_IC3F_3                    ((uint_fast32_t)0x0080)            /*!<Bit 3 */

#define  TIM_CCMR2_IC4PSC                    ((uint_fast32_t)0x0C00)            /*!<IC4PSC[1:0] bits (Input Capture 4 Prescaler) */
#define  TIM_CCMR2_IC4PSC_0                  ((uint_fast32_t)0x0400)            /*!<Bit 0 */
#define  TIM_CCMR2_IC4PSC_1                  ((uint_fast32_t)0x0800)            /*!<Bit 1 */

#define  TIM_CCMR2_IC4F                      ((uint_fast32_t)0xF000)            /*!<IC4F[3:0] bits (Input Capture 4 Filter) */
#define  TIM_CCMR2_IC4F_0                    ((uint_fast32_t)0x1000)            /*!<Bit 0 */
#define  TIM_CCMR2_IC4F_1                    ((uint_fast32_t)0x2000)            /*!<Bit 1 */
#define  TIM_CCMR2_IC4F_2                    ((uint_fast32_t)0x4000)            /*!<Bit 2 */
#define  TIM_CCMR2_IC4F_3                    ((uint_fast32_t)0x8000)            /*!<Bit 3 */

/*******************  Bit definition for TIM_CCER register  *******************/
#define  TIM_CCER_CC1E                       ((uint_fast32_t)0x0001)            /*!<Capture/Compare 1 output enable                 */
#define  TIM_CCER_CC1P                       ((uint_fast32_t)0x0002)            /*!<Capture/Compare 1 output Polarity               */
#define  TIM_CCER_CC1NE                      ((uint_fast32_t)0x0004)            /*!<Capture/Compare 1 Complementary output enable   */
#define  TIM_CCER_CC1NP                      ((uint_fast32_t)0x0008)            /*!<Capture/Compare 1 Complementary output Polarity */
#define  TIM_CCER_CC2E                       ((uint_fast32_t)0x0010)            /*!<Capture/Compare 2 output enable                 */
#define  TIM_CCER_CC2P                       ((uint_fast32_t)0x0020)            /*!<Capture/Compare 2 output Polarity               */
#define  TIM_CCER_CC2NE                      ((uint_fast32_t)0x0040)            /*!<Capture/Compare 2 Complementary output enable   */
#define  TIM_CCER_CC2NP                      ((uint_fast32_t)0x0080)            /*!<Capture/Compare 2 Complementary output Polarity */
#define  TIM_CCER_CC3E                       ((uint_fast32_t)0x0100)            /*!<Capture/Compare 3 output enable                 */
#define  TIM_CCER_CC3P                       ((uint_fast32_t)0x0200)            /*!<Capture/Compare 3 output Polarity               */
#define  TIM_CCER_CC3NE                      ((uint_fast32_t)0x0400)            /*!<Capture/Compare 3 Complementary output enable   */
#define  TIM_CCER_CC3NP                      ((uint_fast32_t)0x0800)            /*!<Capture/Compare 3 Complementary output Polarity */
#define  TIM_CCER_CC4E                       ((uint_fast32_t)0x1000)            /*!<Capture/Compare 4 output enable                 */
#define  TIM_CCER_CC4P                       ((uint_fast32_t)0x2000)            /*!<Capture/Compare 4 output Polarity               */
#define  TIM_CCER_CC4NP                      ((uint_fast32_t)0x8000)            /*!<Capture/Compare 4 Complementary output Polarity */

/*******************  Bit definition for TIM_CNT register  ********************/
#define  TIM_CNT_CNT                         ((uint_fast32_t)0xFFFF)            /*!<Counter Value            */

/*******************  Bit definition for TIM_PSC register  ********************/
#define  TIM_PSC_PSC                         ((uint_fast32_t)0xFFFF)            /*!<Prescaler Value          */

/*******************  Bit definition for TIM_ARR register  ********************/
#define  TIM_ARR_ARR                         ((uint_fast32_t)0xFFFF)            /*!<actual auto-reload Value */

/*******************  Bit definition for TIM_RCR register  ********************/
#define  TIM_RCR_REP                         ((uint_fast32_t)0xFF)               /*!<Repetition Counter Value */

/*******************  Bit definition for TIM_CCR1 register  *******************/
#define  TIM_CCR1_CCR1                       ((uint_fast32_t)0xFFFF)            /*!<Capture/Compare 1 Value  */

/*******************  Bit definition for TIM_CCR2 register  *******************/
#define  TIM_CCR2_CCR2                       ((uint_fast32_t)0xFFFF)            /*!<Capture/Compare 2 Value  */

/*******************  Bit definition for TIM_CCR3 register  *******************/
#define  TIM_CCR3_CCR3                       ((uint_fast32_t)0xFFFF)            /*!<Capture/Compare 3 Value  */

/*******************  Bit definition for TIM_CCR4 register  *******************/
#define  TIM_CCR4_CCR4                       ((uint_fast32_t)0xFFFF)            /*!<Capture/Compare 4 Value  */

/*******************  Bit definition for TIM_BDTR register  *******************/
#define  TIM_BDTR_DTG                        ((uint_fast32_t)0x00FF)            /*!<DTG[0:7] bits (Dead-Time Generator set-up) */
#define  TIM_BDTR_DTG_0                      ((uint_fast32_t)0x0001)            /*!<Bit 0 */
#define  TIM_BDTR_DTG_1                      ((uint_fast32_t)0x0002)            /*!<Bit 1 */
#define  TIM_BDTR_DTG_2                      ((uint_fast32_t)0x0004)            /*!<Bit 2 */
#define  TIM_BDTR_DTG_3                      ((uint_fast32_t)0x0008)            /*!<Bit 3 */
#define  TIM_BDTR_DTG_4                      ((uint_fast32_t)0x0010)            /*!<Bit 4 */
#define  TIM_BDTR_DTG_5                      ((uint_fast32_t)0x0020)            /*!<Bit 5 */
#define  TIM_BDTR_DTG_6                      ((uint_fast32_t)0x0040)            /*!<Bit 6 */
#define  TIM_BDTR_DTG_7                      ((uint_fast32_t)0x0080)            /*!<Bit 7 */

#define  TIM_BDTR_LOCK                       ((uint_fast32_t)0x0300)            /*!<LOCK[1:0] bits (Lock Configuration) */
#define  TIM_BDTR_LOCK_0                     ((uint_fast32_t)0x0100)            /*!<Bit 0 */
#define  TIM_BDTR_LOCK_1                     ((uint_fast32_t)0x0200)            /*!<Bit 1 */

#define  TIM_BDTR_OSSI                       ((uint_fast32_t)0x0400)            /*!<Off-State Selection for Idle mode */
#define  TIM_BDTR_OSSR                       ((uint_fast32_t)0x0800)            /*!<Off-State Selection for Run mode  */
#define  TIM_BDTR_BKE                        ((uint_fast32_t)0x1000)            /*!<Break enable                      */
#define  TIM_BDTR_BKP                        ((uint_fast32_t)0x2000)            /*!<Break Polarity                    */
#define  TIM_BDTR_AOE                        ((uint_fast32_t)0x4000)            /*!<Automatic Output enable           */
#define  TIM_BDTR_MOE                        ((uint_fast32_t)0x8000)            /*!<Main Output enable                */

/*******************  Bit definition for TIM_DCR register  ********************/
#define  TIM_DCR_DBA                         ((uint_fast32_t)0x001F)            /*!<DBA[4:0] bits (DMA Base Address) */
#define  TIM_DCR_DBA_0                       ((uint_fast32_t)0x0001)            /*!<Bit 0 */
#define  TIM_DCR_DBA_1                       ((uint_fast32_t)0x0002)            /*!<Bit 1 */
#define  TIM_DCR_DBA_2                       ((uint_fast32_t)0x0004)            /*!<Bit 2 */
#define  TIM_DCR_DBA_3                       ((uint_fast32_t)0x0008)            /*!<Bit 3 */
#define  TIM_DCR_DBA_4                       ((uint_fast32_t)0x0010)            /*!<Bit 4 */

#define  TIM_DCR_DBL                         ((uint_fast32_t)0x1F00)            /*!<DBL[4:0] bits (DMA Burst Length) */
#define  TIM_DCR_DBL_0                       ((uint_fast32_t)0x0100)            /*!<Bit 0 */
#define  TIM_DCR_DBL_1                       ((uint_fast32_t)0x0200)            /*!<Bit 1 */
#define  TIM_DCR_DBL_2                       ((uint_fast32_t)0x0400)            /*!<Bit 2 */
#define  TIM_DCR_DBL_3                       ((uint_fast32_t)0x0800)            /*!<Bit 3 */
#define  TIM_DCR_DBL_4                       ((uint_fast32_t)0x1000)            /*!<Bit 4 */

/*******************  Bit definition for TIM_DMAR register  *******************/
#define  TIM_DMAR_DMAB                       ((uint_fast32_t)0xFFFF)            /*!<DMA register for burst accesses                    */

/*******************  Bit definition for TIM_OR register  *********************/
#define TIM_OR_TI4_RMP                       ((uint_fast32_t)0x00C0)            /*!<TI4_RMP[1:0] bits (TIM5 Input 4 remap)             */
#define TIM_OR_TI4_RMP_0                     ((uint_fast32_t)0x0040)            /*!<Bit 0 */
#define TIM_OR_TI4_RMP_1                     ((uint_fast32_t)0x0080)            /*!<Bit 1 */
#define TIM_OR_ITR1_RMP                      ((uint_fast32_t)0x0C00)            /*!<ITR1_RMP[1:0] bits (TIM2 Internal trigger 1 remap) */
#define TIM_OR_ITR1_RMP_0                    ((uint_fast32_t)0x0400)            /*!<Bit 0 */
#define TIM_OR_ITR1_RMP_1                    ((uint_fast32_t)0x0800)            /*!<Bit 1 */


/******************************************************************************/
/*                                                                            */
/*         Universal Synchronous Asynchronous Receiver Transmitter            */
/*                                                                            */
/******************************************************************************/
/*******************  Bit definition for USART_SR register  *******************/
#define  USART_SR_PE                         ((uint_fast32_t)0x0001)            /*!<Parity Error                 */
#define  USART_SR_FE                         ((uint_fast32_t)0x0002)            /*!<Framing Error                */
#define  USART_SR_NE                         ((uint_fast32_t)0x0004)            /*!<Noise Error Flag             */
#define  USART_SR_ORE                        ((uint_fast32_t)0x0008)            /*!<OverRun Error                */
#define  USART_SR_IDLE                       ((uint_fast32_t)0x0010)            /*!<IDLE line detected           */
#define  USART_SR_RXNE                       ((uint_fast32_t)0x0020)            /*!<Read Data Register Not Empty */
#define  USART_SR_TC                         ((uint_fast32_t)0x0040)            /*!<Transmission Complete        */
#define  USART_SR_TXE                        ((uint_fast32_t)0x0080)            /*!<Transmit Data Register Empty */
#define  USART_SR_LBD                        ((uint_fast32_t)0x0100)            /*!<LIN Break Detection Flag     */
#define  USART_SR_CTS                        ((uint_fast32_t)0x0200)            /*!<CTS Flag                     */

/*******************  Bit definition for USART_DR register  *******************/
#define  USART_DR_DR                         ((uint_fast32_t)0x01FF)            /*!<Data value */

/******************  Bit definition for USART_BRR register  *******************/
#define  USART_BRR_DIV_Fraction              ((uint_fast32_t)0x000F)            /*!<Fraction of USARTDIV */
#define  USART_BRR_DIV_Mantissa              ((uint_fast32_t)0xFFF0)            /*!<Mantissa of USARTDIV */

/******************  Bit definition for USART_CR1 register  *******************/
#define  USART_CR1_SBK                       ((uint_fast32_t)0x0001)            /*!<Send Break                             */
#define  USART_CR1_RWU                       ((uint_fast32_t)0x0002)            /*!<Receiver wakeup                        */
#define  USART_CR1_RE                        ((uint_fast32_t)0x0004)            /*!<Receiver Enable                        */
#define  USART_CR1_TE                        ((uint_fast32_t)0x0008)            /*!<Transmitter Enable                     */
#define  USART_CR1_IDLEIE                    ((uint_fast32_t)0x0010)            /*!<IDLE Interrupt Enable                  */
#define  USART_CR1_RXNEIE                    ((uint_fast32_t)0x0020)            /*!<RXNE Interrupt Enable                  */
#define  USART_CR1_TCIE                      ((uint_fast32_t)0x0040)            /*!<Transmission Complete Interrupt Enable */
#define  USART_CR1_TXEIE                     ((uint_fast32_t)0x0080)            /*!<PE Interrupt Enable                    */
#define  USART_CR1_PEIE                      ((uint_fast32_t)0x0100)            /*!<PE Interrupt Enable                    */
#define  USART_CR1_PS                        ((uint_fast32_t)0x0200)            /*!<Parity Selection                       */
#define  USART_CR1_PCE                       ((uint_fast32_t)0x0400)            /*!<Parity Control Enable                  */
#define  USART_CR1_WAKE                      ((uint_fast32_t)0x0800)            /*!<Wakeup method                          */
#define  USART_CR1_M                         ((uint_fast32_t)0x1000)            /*!<Word length                            */
#define  USART_CR1_UE                        ((uint_fast32_t)0x2000)            /*!<USART Enable                           */
#define  USART_CR1_OVER8                     ((uint_fast32_t)0x8000)            /*!<USART Oversampling by 8 enable         */

/******************  Bit definition for USART_CR2 register  *******************/
#define  USART_CR2_ADD                       ((uint_fast32_t)0x000F)            /*!<Address of the USART node            */
#define  USART_CR2_LBDL                      ((uint_fast32_t)0x0020)            /*!<LIN Break Detection Length           */
#define  USART_CR2_LBDIE                     ((uint_fast32_t)0x0040)            /*!<LIN Break Detection Interrupt Enable */
#define  USART_CR2_LBCL                      ((uint_fast32_t)0x0100)            /*!<Last Bit Clock pulse                 */
#define  USART_CR2_CPHA                      ((uint_fast32_t)0x0200)            /*!<Clock Phase                          */
#define  USART_CR2_CPOL                      ((uint_fast32_t)0x0400)            /*!<Clock Polarity                       */
#define  USART_CR2_CLKEN                     ((uint_fast32_t)0x0800)            /*!<Clock Enable                         */

#define  USART_CR2_STOP                      ((uint_fast32_t)0x3000)            /*!<STOP[1:0] bits (STOP bits) */
#define  USART_CR2_STOP_0                    ((uint_fast32_t)0x1000)            /*!<Bit 0 */
#define  USART_CR2_STOP_1                    ((uint_fast32_t)0x2000)            /*!<Bit 1 */

#define  USART_CR2_LINEN                     ((uint_fast32_t)0x4000)            /*!<LIN mode enable */

/******************  Bit definition for USART_CR3 register  *******************/
#define  USART_CR3_EIE                       ((uint_fast32_t)0x0001)            /*!<Error Interrupt Enable      */
#define  USART_CR3_IREN                      ((uint_fast32_t)0x0002)            /*!<IrDA mode Enable            */
#define  USART_CR3_IRLP                      ((uint_fast32_t)0x0004)            /*!<IrDA Low-Power              */
#define  USART_CR3_HDSEL                     ((uint_fast32_t)0x0008)            /*!<Half-Duplex Selection       */
#define  USART_CR3_NACK                      ((uint_fast32_t)0x0010)            /*!<Smartcard NACK enable       */
#define  USART_CR3_SCEN                      ((uint_fast32_t)0x0020)            /*!<Smartcard mode enable       */
#define  USART_CR3_DMAR                      ((uint_fast32_t)0x0040)            /*!<DMA Enable Receiver         */
#define  USART_CR3_DMAT                      ((uint_fast32_t)0x0080)            /*!<DMA Enable Transmitter      */
#define  USART_CR3_RTSE                      ((uint_fast32_t)0x0100)            /*!<RTS Enable                  */
#define  USART_CR3_CTSE                      ((uint_fast32_t)0x0200)            /*!<CTS Enable                  */
#define  USART_CR3_CTSIE                     ((uint_fast32_t)0x0400)            /*!<CTS Interrupt Enable        */
#define  USART_CR3_ONEBIT                    ((uint_fast32_t)0x0800)            /*!<USART One bit method enable */

/******************  Bit definition for USART_GTPR register  ******************/
#define  USART_GTPR_PSC                      ((uint_fast32_t)0x00FF)            /*!<PSC[7:0] bits (Prescaler value) */
#define  USART_GTPR_PSC_0                    ((uint_fast32_t)0x0001)            /*!<Bit 0 */
#define  USART_GTPR_PSC_1                    ((uint_fast32_t)0x0002)            /*!<Bit 1 */
#define  USART_GTPR_PSC_2                    ((uint_fast32_t)0x0004)            /*!<Bit 2 */
#define  USART_GTPR_PSC_3                    ((uint_fast32_t)0x0008)            /*!<Bit 3 */
#define  USART_GTPR_PSC_4                    ((uint_fast32_t)0x0010)            /*!<Bit 4 */
#define  USART_GTPR_PSC_5                    ((uint_fast32_t)0x0020)            /*!<Bit 5 */
#define  USART_GTPR_PSC_6                    ((uint_fast32_t)0x0040)            /*!<Bit 6 */
#define  USART_GTPR_PSC_7                    ((uint_fast32_t)0x0080)            /*!<Bit 7 */

#define  USART_GTPR_GT                       ((uint_fast32_t)0xFF00)            /*!<Guard time value */

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


/******************************************************************************/
/*                                                                            */
/*                                DBG                                         */
/*                                                                            */
/******************************************************************************/
/********************  Bit definition for DBGMCU_IDCODE register  *************/
#define  DBGMCU_IDCODE_DEV_ID                ((uint_fast32_t)0x00000FFF)
#define  DBGMCU_IDCODE_REV_ID                ((uint_fast32_t)0xFFFF0000)

/********************  Bit definition for DBGMCU_CR register  *****************/
#define  DBGMCU_CR_DBG_SLEEP                 ((uint_fast32_t)0x00000001)
#define  DBGMCU_CR_DBG_STOP                  ((uint_fast32_t)0x00000002)
#define  DBGMCU_CR_DBG_STANDBY               ((uint_fast32_t)0x00000004)
#define  DBGMCU_CR_TRACE_IOEN                ((uint_fast32_t)0x00000020)

#define  DBGMCU_CR_TRACE_MODE                ((uint_fast32_t)0x000000C0)
#define  DBGMCU_CR_TRACE_MODE_0              ((uint_fast32_t)0x00000040)/*!<Bit 0 */
#define  DBGMCU_CR_TRACE_MODE_1              ((uint_fast32_t)0x00000080)/*!<Bit 1 */

/********************  Bit definition for DBGMCU_APB1_FZ register  ************/
#define  DBGMCU_APB1_FZ_DBG_TIM2_STOP            ((uint_fast32_t)0x00000001)
#define  DBGMCU_APB1_FZ_DBG_TIM3_STOP            ((uint_fast32_t)0x00000002)
#define  DBGMCU_APB1_FZ_DBG_TIM4_STOP            ((uint_fast32_t)0x00000004)
#define  DBGMCU_APB1_FZ_DBG_TIM5_STOP            ((uint_fast32_t)0x00000008)
#define  DBGMCU_APB1_FZ_DBG_TIM6_STOP            ((uint_fast32_t)0x00000010)
#define  DBGMCU_APB1_FZ_DBG_TIM7_STOP            ((uint_fast32_t)0x00000020)
#define  DBGMCU_APB1_FZ_DBG_TIM12_STOP           ((uint_fast32_t)0x00000040)
#define  DBGMCU_APB1_FZ_DBG_TIM13_STOP           ((uint_fast32_t)0x00000080)
#define  DBGMCU_APB1_FZ_DBG_TIM14_STOP           ((uint_fast32_t)0x00000100)
#define  DBGMCU_APB1_FZ_DBG_RTC_STOP             ((uint_fast32_t)0x00000400)
#define  DBGMCU_APB1_FZ_DBG_WWDG_STOP            ((uint_fast32_t)0x00000800)
#define  DBGMCU_APB1_FZ_DBG_IWDG_STOP            ((uint_fast32_t)0x00001000)
#define  DBGMCU_APB1_FZ_DBG_I2C1_SMBUS_TIMEOUT   ((uint_fast32_t)0x00200000)
#define  DBGMCU_APB1_FZ_DBG_I2C2_SMBUS_TIMEOUT   ((uint_fast32_t)0x00400000)
#define  DBGMCU_APB1_FZ_DBG_I2C3_SMBUS_TIMEOUT   ((uint_fast32_t)0x00800000)
#define  DBGMCU_APB1_FZ_DBG_CAN1_STOP            ((uint_fast32_t)0x02000000)
#define  DBGMCU_APB1_FZ_DBG_CAN2_STOP            ((uint_fast32_t)0x04000000)
/* Old IWDGSTOP bit definition, maintained for legacy purpose */
#define  DBGMCU_APB1_FZ_DBG_IWDEG_STOP           DBGMCU_APB1_FZ_DBG_IWDG_STOP

/********************  Bit definition for DBGMCU_APB2_FZ register  ************/
#define  DBGMCU_APB2_FZ_DBG_TIM1_STOP        ((uint_fast32_t)0x00000001)
#define  DBGMCU_APB2_FZ_DBG_TIM8_STOP        ((uint_fast32_t)0x00000002)
#define  DBGMCU_APB2_FZ_DBG_TIM9_STOP        ((uint_fast32_t)0x00010000)
#define  DBGMCU_APB2_FZ_DBG_TIM10_STOP       ((uint_fast32_t)0x00020000)
#define  DBGMCU_APB2_FZ_DBG_TIM11_STOP       ((uint_fast32_t)0x00040000)

/******************************************************************************/
/*                                                                            */
/*                                       USB_OTG                                    */
/*                                                                            */
/******************************************************************************/
/********************  Bit definition forUSB_OTG_GOTGCTL register  ********************/
#define USB_OTG_GOTGCTL_SRQSCS                  ((uint_fast32_t)0x00000001)            /*!< Session request success */
#define USB_OTG_GOTGCTL_SRQ                     ((uint_fast32_t)0x00000002)            /*!< Session request */
#define USB_OTG_GOTGCTL_HNGSCS                  ((uint_fast32_t)0x00000100)            /*!< Host negotiation success */
#define USB_OTG_GOTGCTL_HNPRQ                   ((uint_fast32_t)0x00000200)            /*!< HNP request */
#define USB_OTG_GOTGCTL_HSHNPEN                 ((uint_fast32_t)0x00000400)            /*!< Host set HNP enable */
#define USB_OTG_GOTGCTL_DHNPEN                  ((uint_fast32_t)0x00000800)            /*!< Device HNP enabled */
#define USB_OTG_GOTGCTL_CIDSTS                  ((uint_fast32_t)0x00010000)            /*!< Connector ID status */
#define USB_OTG_GOTGCTL_DBCT                    ((uint_fast32_t)0x00020000)            /*!< Long/short debounce time */
#define USB_OTG_GOTGCTL_ASVLD                   ((uint_fast32_t)0x00040000)            /*!< A-session valid */
#define USB_OTG_GOTGCTL_BSVLD                   ((uint_fast32_t)0x00080000)            /*!< B-session valid */

/********************  Bit definition forUSB_OTG_HCFG register  ********************/

#define USB_OTG_HCFG_FSLSPCS                 ((uint_fast32_t)0x00000003)            /*!< FS/LS PHY clock select */
#define USB_OTG_HCFG_FSLSPCS_0               ((uint_fast32_t)0x00000001)            /*!<Bit 0 */
#define USB_OTG_HCFG_FSLSPCS_1               ((uint_fast32_t)0x00000002)            /*!<Bit 1 */
#define USB_OTG_HCFG_FSLSS                   ((uint_fast32_t)0x00000004)            /*!< FS- and LS-only support */

/********************  Bit definition forUSB_OTG_DCFG register  ********************/

#define USB_OTG_DCFG_DSPD                    ((uint_fast32_t)0x00000003)            /*!< Device speed */
#define USB_OTG_DCFG_DSPD_0                  ((uint_fast32_t)0x00000001)            /*!<Bit 0 */
#define USB_OTG_DCFG_DSPD_1                  ((uint_fast32_t)0x00000002)            /*!<Bit 1 */
#define USB_OTG_DCFG_NZLSOHSK                ((uint_fast32_t)0x00000004)            /*!< Nonzero-length status OUT handshake */

#define USB_OTG_DCFG_DAD                     ((uint_fast32_t)0x000007F0)            /*!< Device address */
#define USB_OTG_DCFG_DAD_0                   ((uint_fast32_t)0x00000010)            /*!<Bit 0 */
#define USB_OTG_DCFG_DAD_1                   ((uint_fast32_t)0x00000020)            /*!<Bit 1 */
#define USB_OTG_DCFG_DAD_2                   ((uint_fast32_t)0x00000040)            /*!<Bit 2 */
#define USB_OTG_DCFG_DAD_3                   ((uint_fast32_t)0x00000080)            /*!<Bit 3 */
#define USB_OTG_DCFG_DAD_4                   ((uint_fast32_t)0x00000100)            /*!<Bit 4 */
#define USB_OTG_DCFG_DAD_5                   ((uint_fast32_t)0x00000200)            /*!<Bit 5 */
#define USB_OTG_DCFG_DAD_6                   ((uint_fast32_t)0x00000400)            /*!<Bit 6 */

#define USB_OTG_DCFG_PFIVL                   ((uint_fast32_t)0x00001800)            /*!< Periodic (micro)frame interval */
#define USB_OTG_DCFG_PFIVL_0                 ((uint_fast32_t)0x00000800)            /*!<Bit 0 */
#define USB_OTG_DCFG_PFIVL_1                 ((uint_fast32_t)0x00001000)            /*!<Bit 1 */

#define USB_OTG_DCFG_PERSCHIVL               ((uint_fast32_t)0x03000000)            /*!< Periodic scheduling interval */
#define USB_OTG_DCFG_PERSCHIVL_0             ((uint_fast32_t)0x01000000)            /*!<Bit 0 */
#define USB_OTG_DCFG_PERSCHIVL_1             ((uint_fast32_t)0x02000000)            /*!<Bit 1 */

/********************  Bit definition forUSB_OTG_PCGCR register  ********************/
#define USB_OTG_PCGCR_STPPCLK                 ((uint_fast32_t)0x00000001)            /*!< Stop PHY clock */
#define USB_OTG_PCGCR_GATEHCLK                ((uint_fast32_t)0x00000002)            /*!< Gate HCLK */
#define USB_OTG_PCGCR_PHYSUSP                 ((uint_fast32_t)0x00000010)            /*!< PHY suspended */

/********************  Bit definition forUSB_OTG_GOTGINT register  ********************/
#define USB_OTG_GOTGINT_SEDET                   ((uint_fast32_t)0x00000004)            /*!< Session end detected */
#define USB_OTG_GOTGINT_SRSSCHG                 ((uint_fast32_t)0x00000100)            /*!< Session request success status change */
#define USB_OTG_GOTGINT_HNSSCHG                 ((uint_fast32_t)0x00000200)            /*!< Host negotiation success status change */
#define USB_OTG_GOTGINT_HNGDET                  ((uint_fast32_t)0x00020000)            /*!< Host negotiation detected */
#define USB_OTG_GOTGINT_ADTOCHG                 ((uint_fast32_t)0x00040000)            /*!< A-device timeout change */
#define USB_OTG_GOTGINT_DBCDNE                  ((uint_fast32_t)0x00080000)            /*!< Debounce done */

/********************  Bit definition forUSB_OTG_DCTL register  ********************/
#define USB_OTG_DCTL_RWUSIG                  ((uint_fast32_t)0x00000001)            /*!< Remote wakeup signaling */
#define USB_OTG_DCTL_SDIS                    ((uint_fast32_t)0x00000002)            /*!< Soft disconnect */
#define USB_OTG_DCTL_GINSTS                  ((uint_fast32_t)0x00000004)            /*!< Global IN NAK status */
#define USB_OTG_DCTL_GONSTS                  ((uint_fast32_t)0x00000008)            /*!< Global OUT NAK status */

#define USB_OTG_DCTL_TCTL                    ((uint_fast32_t)0x00000070)            /*!< Test control */
#define USB_OTG_DCTL_TCTL_0                  ((uint_fast32_t)0x00000010)            /*!<Bit 0 */
#define USB_OTG_DCTL_TCTL_1                  ((uint_fast32_t)0x00000020)            /*!<Bit 1 */
#define USB_OTG_DCTL_TCTL_2                  ((uint_fast32_t)0x00000040)            /*!<Bit 2 */
#define USB_OTG_DCTL_SGINAK                  ((uint_fast32_t)0x00000080)            /*!< Set global IN NAK */
#define USB_OTG_DCTL_CGINAK                  ((uint_fast32_t)0x00000100)            /*!< Clear global IN NAK */
#define USB_OTG_DCTL_SGONAK                  ((uint_fast32_t)0x00000200)            /*!< Set global OUT NAK */
#define USB_OTG_DCTL_CGONAK                  ((uint_fast32_t)0x00000400)            /*!< Clear global OUT NAK */
#define USB_OTG_DCTL_POPRGDNE                ((uint_fast32_t)0x00000800)            /*!< Power-on programming done */

/********************  Bit definition forUSB_OTG_HFIR register  ********************/
#define USB_OTG_HFIR_FRIVL                   ((uint_fast32_t)0x0000FFFF)            /*!< Frame interval */

/********************  Bit definition forUSB_OTG_HFNUM register  ********************/
#define USB_OTG_HFNUM_FRNUM                   ((uint_fast32_t)0x0000FFFF)            /*!< Frame number */
#define USB_OTG_HFNUM_FTREM                   ((uint_fast32_t)0xFFFF0000)            /*!< Frame time remaining */

/********************  Bit definition forUSB_OTG_DSTS register  ********************/
#define USB_OTG_DSTS_SUSPSTS                 ((uint_fast32_t)0x00000001)            /*!< Suspend status */

#define USB_OTG_DSTS_ENUMSPD                 ((uint_fast32_t)0x00000006)            /*!< Enumerated speed */
#define USB_OTG_DSTS_ENUMSPD_0               ((uint_fast32_t)0x00000002)            /*!<Bit 0 */
#define USB_OTG_DSTS_ENUMSPD_1               ((uint_fast32_t)0x00000004)            /*!<Bit 1 */
#define USB_OTG_DSTS_EERR                    ((uint_fast32_t)0x00000008)            /*!< Erratic error */
#define USB_OTG_DSTS_FNSOF                   ((uint_fast32_t)0x003FFF00)            /*!< Frame number of the received SOF */

/********************  Bit definition forUSB_OTG_GAHBCFG register  ********************/
#define USB_OTG_GAHBCFG_GINT                    ((uint_fast32_t)0x00000001)            /*!< Global interrupt mask */

#define USB_OTG_GAHBCFG_HBSTLEN                 ((uint_fast32_t)0x0000001E)            /*!< Burst length/type */
#define USB_OTG_GAHBCFG_HBSTLEN_0               ((uint_fast32_t)0x00000002)            /*!<Bit 0 */
#define USB_OTG_GAHBCFG_HBSTLEN_1               ((uint_fast32_t)0x00000004)            /*!<Bit 1 */
#define USB_OTG_GAHBCFG_HBSTLEN_2               ((uint_fast32_t)0x00000008)            /*!<Bit 2 */
#define USB_OTG_GAHBCFG_HBSTLEN_3               ((uint_fast32_t)0x00000010)            /*!<Bit 3 */
#define USB_OTG_GAHBCFG_DMAEN                   ((uint_fast32_t)0x00000020)            /*!< DMA enable */
#define USB_OTG_GAHBCFG_TXFELVL                 ((uint_fast32_t)0x00000080)            /*!< TxFIFO empty level */
#define USB_OTG_GAHBCFG_PTXFELVL                ((uint_fast32_t)0x00000100)            /*!< Periodic TxFIFO empty level */

/********************  Bit definition forUSB_OTG_GUSBCFG register  ********************/

#define USB_OTG_GUSBCFG_TOCAL                   ((uint_fast32_t)0x00000007)            /*!< FS timeout calibration */
#define USB_OTG_GUSBCFG_TOCAL_0                 ((uint_fast32_t)0x00000001)            /*!<Bit 0 */
#define USB_OTG_GUSBCFG_TOCAL_1                 ((uint_fast32_t)0x00000002)            /*!<Bit 1 */
#define USB_OTG_GUSBCFG_TOCAL_2                 ((uint_fast32_t)0x00000004)            /*!<Bit 2 */
#define USB_OTG_GUSBCFG_PHYSEL                  ((uint_fast32_t)0x00000040)            /*!< USB 2.0 high-speed ULPI PHY or USB 1.1 full-speed serial transceiver select */
#define USB_OTG_GUSBCFG_SRPCAP                  ((uint_fast32_t)0x00000100)            /*!< SRP-capable */
#define USB_OTG_GUSBCFG_HNPCAP                  ((uint_fast32_t)0x00000200)            /*!< HNP-capable */

#define USB_OTG_GUSBCFG_TRDT                    ((uint_fast32_t)0x00003C00)            /*!< USB turnaround time */
#define USB_OTG_GUSBCFG_TRDT_0                  ((uint_fast32_t)0x00000400)            /*!<Bit 0 */
#define USB_OTG_GUSBCFG_TRDT_1                  ((uint_fast32_t)0x00000800)            /*!<Bit 1 */
#define USB_OTG_GUSBCFG_TRDT_2                  ((uint_fast32_t)0x00001000)            /*!<Bit 2 */
#define USB_OTG_GUSBCFG_TRDT_3                  ((uint_fast32_t)0x00002000)            /*!<Bit 3 */
#define USB_OTG_GUSBCFG_PHYLPCS                 ((uint_fast32_t)0x00008000)            /*!< PHY Low-power clock select */
#define USB_OTG_GUSBCFG_ULPIFSLS                ((uint_fast32_t)0x00020000)            /*!< ULPI FS/LS select */
#define USB_OTG_GUSBCFG_ULPIAR                  ((uint_fast32_t)0x00040000)            /*!< ULPI Auto-resume */
#define USB_OTG_GUSBCFG_ULPICSM                 ((uint_fast32_t)0x00080000)            /*!< ULPI Clock SuspendM */
#define USB_OTG_GUSBCFG_ULPIEVBUSD              ((uint_fast32_t)0x00100000)            /*!< ULPI External VBUS Drive */
#define USB_OTG_GUSBCFG_ULPIEVBUSI              ((uint_fast32_t)0x00200000)            /*!< ULPI external VBUS indicator */
#define USB_OTG_GUSBCFG_TSDPS                   ((uint_fast32_t)0x00400000)            /*!< TermSel DLine pulsing selection */
#define USB_OTG_GUSBCFG_PCCI                    ((uint_fast32_t)0x00800000)            /*!< Indicator complement */
#define USB_OTG_GUSBCFG_PTCI                    ((uint_fast32_t)0x01000000)            /*!< Indicator pass through */
#define USB_OTG_GUSBCFG_ULPIIPD                 ((uint_fast32_t)0x02000000)            /*!< ULPI interface protect disable */
#define USB_OTG_GUSBCFG_FHMOD                   ((uint_fast32_t)0x20000000)            /*!< Forced host mode */
#define USB_OTG_GUSBCFG_FDMOD                   ((uint_fast32_t)0x40000000)            /*!< Forced peripheral mode */
#define USB_OTG_GUSBCFG_CTXPKT                  ((uint_fast32_t)0x80000000)            /*!< Corrupt Tx packet */

/********************  Bit definition forUSB_OTG_GRSTCTL register  ********************/
#define USB_OTG_GRSTCTL_CSRST                   ((uint_fast32_t)0x00000001)            /*!< Core soft reset */
#define USB_OTG_GRSTCTL_HSRST                   ((uint_fast32_t)0x00000002)            /*!< HCLK soft reset */
#define USB_OTG_GRSTCTL_FCRST                   ((uint_fast32_t)0x00000004)            /*!< Host frame counter reset */
#define USB_OTG_GRSTCTL_RXFFLSH                 ((uint_fast32_t)0x00000010)            /*!< RxFIFO flush */
#define USB_OTG_GRSTCTL_TXFFLSH                 ((uint_fast32_t)0x00000020)            /*!< TxFIFO flush */

#define USB_OTG_GRSTCTL_TXFNUM                  ((uint_fast32_t)0x000007C0)            /*!< TxFIFO number */
#define USB_OTG_GRSTCTL_TXFNUM_0                ((uint_fast32_t)0x00000040)            /*!<Bit 0 */
#define USB_OTG_GRSTCTL_TXFNUM_1                ((uint_fast32_t)0x00000080)            /*!<Bit 1 */
#define USB_OTG_GRSTCTL_TXFNUM_2                ((uint_fast32_t)0x00000100)            /*!<Bit 2 */
#define USB_OTG_GRSTCTL_TXFNUM_3                ((uint_fast32_t)0x00000200)            /*!<Bit 3 */
#define USB_OTG_GRSTCTL_TXFNUM_4                ((uint_fast32_t)0x00000400)            /*!<Bit 4 */
#define USB_OTG_GRSTCTL_DMAREQ                  ((uint_fast32_t)0x40000000)            /*!< DMA request signal */
#define USB_OTG_GRSTCTL_AHBIDL                  ((uint_fast32_t)0x80000000)            /*!< AHB master idle */

/********************  Bit definition forUSB_OTG_DIEPMSK register  ********************/
#define USB_OTG_DIEPMSK_XFRCM                   ((uint_fast32_t)0x00000001)            /*!< Transfer completed interrupt mask */
#define USB_OTG_DIEPMSK_EPDM                    ((uint_fast32_t)0x00000002)            /*!< Endpoint disabled interrupt mask */
#define USB_OTG_DIEPMSK_TOM                     ((uint_fast32_t)0x00000008)            /*!< Timeout condition mask (nonisochronous endpoints) */
#define USB_OTG_DIEPMSK_ITTXFEMSK               ((uint_fast32_t)0x00000010)            /*!< IN token received when TxFIFO empty mask */
#define USB_OTG_DIEPMSK_INEPNMM                 ((uint_fast32_t)0x00000020)            /*!< IN token received with EP mismatch mask */
#define USB_OTG_DIEPMSK_INEPNEM                 ((uint_fast32_t)0x00000040)            /*!< IN endpoint NAK effective mask */
#define USB_OTG_DIEPMSK_TXFURM                  ((uint_fast32_t)0x00000100)            /*!< FIFO underrun mask */
#define USB_OTG_DIEPMSK_BIM                     ((uint_fast32_t)0x00000200)            /*!< BNA interrupt mask */

/********************  Bit definition forUSB_OTG_HPTXSTS register  ********************/
#define USB_OTG_HPTXSTS_PTXFSAVL                ((uint_fast32_t)0x0000FFFF)            /*!< Periodic transmit data FIFO space available */

#define USB_OTG_HPTXSTS_PTXQSAV                 ((uint_fast32_t)0x00FF0000)            /*!< Periodic transmit request queue space available */
#define USB_OTG_HPTXSTS_PTXQSAV_0               ((uint_fast32_t)0x00010000)            /*!<Bit 0 */
#define USB_OTG_HPTXSTS_PTXQSAV_1               ((uint_fast32_t)0x00020000)            /*!<Bit 1 */
#define USB_OTG_HPTXSTS_PTXQSAV_2               ((uint_fast32_t)0x00040000)            /*!<Bit 2 */
#define USB_OTG_HPTXSTS_PTXQSAV_3               ((uint_fast32_t)0x00080000)            /*!<Bit 3 */
#define USB_OTG_HPTXSTS_PTXQSAV_4               ((uint_fast32_t)0x00100000)            /*!<Bit 4 */
#define USB_OTG_HPTXSTS_PTXQSAV_5               ((uint_fast32_t)0x00200000)            /*!<Bit 5 */
#define USB_OTG_HPTXSTS_PTXQSAV_6               ((uint_fast32_t)0x00400000)            /*!<Bit 6 */
#define USB_OTG_HPTXSTS_PTXQSAV_7               ((uint_fast32_t)0x00800000)            /*!<Bit 7 */

#define USB_OTG_HPTXSTS_PTXQTOP                 ((uint_fast32_t)0xFF000000)            /*!< Top of the periodic transmit request queue */
#define USB_OTG_HPTXSTS_PTXQTOP_0               ((uint_fast32_t)0x01000000)            /*!<Bit 0 */
#define USB_OTG_HPTXSTS_PTXQTOP_1               ((uint_fast32_t)0x02000000)            /*!<Bit 1 */
#define USB_OTG_HPTXSTS_PTXQTOP_2               ((uint_fast32_t)0x04000000)            /*!<Bit 2 */
#define USB_OTG_HPTXSTS_PTXQTOP_3               ((uint_fast32_t)0x08000000)            /*!<Bit 3 */
#define USB_OTG_HPTXSTS_PTXQTOP_4               ((uint_fast32_t)0x10000000)            /*!<Bit 4 */
#define USB_OTG_HPTXSTS_PTXQTOP_5               ((uint_fast32_t)0x20000000)            /*!<Bit 5 */
#define USB_OTG_HPTXSTS_PTXQTOP_6               ((uint_fast32_t)0x40000000)            /*!<Bit 6 */
#define USB_OTG_HPTXSTS_PTXQTOP_7               ((uint_fast32_t)0x80000000)            /*!<Bit 7 */

/********************  Bit definition forUSB_OTG_HAINT register  ********************/
#define USB_OTG_HAINT_HAINT                   ((uint_fast32_t)0x0000FFFF)            /*!< Channel interrupts */

/********************  Bit definition forUSB_OTG_DOEPMSK register  ********************/
#define USB_OTG_DOEPMSK_XFRCM                   ((uint_fast32_t)0x00000001)            /*!< Transfer completed interrupt mask */
#define USB_OTG_DOEPMSK_EPDM                    ((uint_fast32_t)0x00000002)            /*!< Endpoint disabled interrupt mask */
#define USB_OTG_DOEPMSK_STUPM                   ((uint_fast32_t)0x00000008)            /*!< SETUP phase done mask */
#define USB_OTG_DOEPMSK_OTEPDM                  ((uint_fast32_t)0x00000010)            /*!< OUT token received when endpoint disabled mask */
#define USB_OTG_DOEPMSK_B2BSTUP                 ((uint_fast32_t)0x00000040)            /*!< Back-to-back SETUP packets received mask */
#define USB_OTG_DOEPMSK_OPEM                    ((uint_fast32_t)0x00000100)            /*!< OUT packet error mask */
#define USB_OTG_DOEPMSK_BOIM                    ((uint_fast32_t)0x00000200)            /*!< BNA interrupt mask */

/********************  Bit definition forUSB_OTG_GINTSTS register  ********************/
#define USB_OTG_GINTSTS_CMOD                    ((uint_fast32_t)0x00000001)            /*!< Current mode of operation */
#define USB_OTG_GINTSTS_MMIS                    ((uint_fast32_t)0x00000002)            /*!< Mode mismatch interrupt */
#define USB_OTG_GINTSTS_OTGINT                  ((uint_fast32_t)0x00000004)            /*!< OTG interrupt */
#define USB_OTG_GINTSTS_SOF                     ((uint_fast32_t)0x00000008)            /*!< Start of frame */
#define USB_OTG_GINTSTS_RXFLVL                  ((uint_fast32_t)0x00000010)            /*!< RxFIFO nonempty */
#define USB_OTG_GINTSTS_NPTXFE                  ((uint_fast32_t)0x00000020)            /*!< Nonperiodic TxFIFO empty */
#define USB_OTG_GINTSTS_GINAKEFF                ((uint_fast32_t)0x00000040)            /*!< Global IN nonperiodic NAK effective */
#define USB_OTG_GINTSTS_BOUTNAKEFF              ((uint_fast32_t)0x00000080)            /*!< Global OUT NAK effective */
#define USB_OTG_GINTSTS_ESUSP                   ((uint_fast32_t)0x00000400)            /*!< Early suspend */
#define USB_OTG_GINTSTS_USBSUSP                 ((uint_fast32_t)0x00000800)            /*!< USB suspend */
#define USB_OTG_GINTSTS_USBRST                  ((uint_fast32_t)0x00001000)            /*!< USB reset */
#define USB_OTG_GINTSTS_ENUMDNE                 ((uint_fast32_t)0x00002000)            /*!< Enumeration done */
#define USB_OTG_GINTSTS_ISOODRP                 ((uint_fast32_t)0x00004000)            /*!< Isochronous OUT packet dropped interrupt */
#define USB_OTG_GINTSTS_EOPF                    ((uint_fast32_t)0x00008000)            /*!< End of periodic frame interrupt */
#define USB_OTG_GINTSTS_IEPINT                  ((uint_fast32_t)0x00040000)            /*!< IN endpoint interrupt */
#define USB_OTG_GINTSTS_OEPINT                  ((uint_fast32_t)0x00080000)            /*!< OUT endpoint interrupt */
#define USB_OTG_GINTSTS_IISOIXFR                ((uint_fast32_t)0x00100000)            /*!< Incomplete isochronous IN transfer */
#define USB_OTG_GINTSTS_PXFR_INCOMPISOOUT       ((uint_fast32_t)0x00200000)            /*!< Incomplete periodic transfer */
#define USB_OTG_GINTSTS_DATAFSUSP               ((uint_fast32_t)0x00400000)            /*!< Data fetch suspended */
#define USB_OTG_GINTSTS_HPRTINT                 ((uint_fast32_t)0x01000000)            /*!< Host port interrupt */
#define USB_OTG_GINTSTS_HCINT                   ((uint_fast32_t)0x02000000)            /*!< Host channels interrupt */
#define USB_OTG_GINTSTS_PTXFE                   ((uint_fast32_t)0x04000000)            /*!< Periodic TxFIFO empty */
#define USB_OTG_GINTSTS_CIDSCHG                 ((uint_fast32_t)0x10000000)            /*!< Connector ID status change */
#define USB_OTG_GINTSTS_DISCINT                 ((uint_fast32_t)0x20000000)            /*!< Disconnect detected interrupt */
#define USB_OTG_GINTSTS_SRQINT                  ((uint_fast32_t)0x40000000)            /*!< Session request/new session detected interrupt */
#define USB_OTG_GINTSTS_WKUINT                  ((uint_fast32_t)0x80000000)            /*!< Resume/remote wakeup detected interrupt */

/********************  Bit definition forUSB_OTG_GINTMSK register  ********************/
#define USB_OTG_GINTMSK_MMISM                   ((uint_fast32_t)0x00000002)            /*!< Mode mismatch interrupt mask */
#define USB_OTG_GINTMSK_OTGINT                  ((uint_fast32_t)0x00000004)            /*!< OTG interrupt mask */
#define USB_OTG_GINTMSK_SOFM                    ((uint_fast32_t)0x00000008)            /*!< Start of frame mask */
#define USB_OTG_GINTMSK_RXFLVLM                 ((uint_fast32_t)0x00000010)            /*!< Receive FIFO nonempty mask */
#define USB_OTG_GINTMSK_NPTXFEM                 ((uint_fast32_t)0x00000020)            /*!< Nonperiodic TxFIFO empty mask */
#define USB_OTG_GINTMSK_GINAKEFFM               ((uint_fast32_t)0x00000040)            /*!< Global nonperiodic IN NAK effective mask */
#define USB_OTG_GINTMSK_GONAKEFFM               ((uint_fast32_t)0x00000080)            /*!< Global OUT NAK effective mask */
#define USB_OTG_GINTMSK_ESUSPM                  ((uint_fast32_t)0x00000400)            /*!< Early suspend mask */
#define USB_OTG_GINTMSK_USBSUSPM                ((uint_fast32_t)0x00000800)            /*!< USB suspend mask */
#define USB_OTG_GINTMSK_USBRST                  ((uint_fast32_t)0x00001000)            /*!< USB reset mask */
#define USB_OTG_GINTMSK_ENUMDNEM                ((uint_fast32_t)0x00002000)            /*!< Enumeration done mask */
#define USB_OTG_GINTMSK_ISOODRPM                ((uint_fast32_t)0x00004000)            /*!< Isochronous OUT packet dropped interrupt mask */
#define USB_OTG_GINTMSK_EOPFM                   ((uint_fast32_t)0x00008000)            /*!< End of periodic frame interrupt mask */
#define USB_OTG_GINTMSK_EPMISM                  ((uint_fast32_t)0x00020000)            /*!< Endpoint mismatch interrupt mask */
#define USB_OTG_GINTMSK_IEPINT                  ((uint_fast32_t)0x00040000)            /*!< IN endpoints interrupt mask */
#define USB_OTG_GINTMSK_OEPINT                  ((uint_fast32_t)0x00080000)            /*!< OUT endpoints interrupt mask */
#define USB_OTG_GINTMSK_IISOIXFRM               ((uint_fast32_t)0x00100000)            /*!< Incomplete isochronous IN transfer mask */
#define USB_OTG_GINTMSK_PXFRM_IISOOXFRM         ((uint_fast32_t)0x00200000)            /*!< Incomplete periodic transfer mask */
#define USB_OTG_GINTMSK_FSUSPM                  ((uint_fast32_t)0x00400000)            /*!< Data fetch suspended mask */
#define USB_OTG_GINTMSK_PRTIM                   ((uint_fast32_t)0x01000000)            /*!< Host port interrupt mask */
#define USB_OTG_GINTMSK_HCIM                    ((uint_fast32_t)0x02000000)            /*!< Host channels interrupt mask */
#define USB_OTG_GINTMSK_PTXFEM                  ((uint_fast32_t)0x04000000)            /*!< Periodic TxFIFO empty mask */
#define USB_OTG_GINTMSK_CIDSCHGM                ((uint_fast32_t)0x10000000)            /*!< Connector ID status change mask */
#define USB_OTG_GINTMSK_DISCINT                 ((uint_fast32_t)0x20000000)            /*!< Disconnect detected interrupt mask */
#define USB_OTG_GINTMSK_SRQIM                   ((uint_fast32_t)0x40000000)            /*!< Session request/new session detected interrupt mask */
#define USB_OTG_GINTMSK_WUIM                    ((uint_fast32_t)0x80000000)            /*!< Resume/remote wakeup detected interrupt mask */

/********************  Bit definition forUSB_OTG_DAINT register  ********************/
#define USB_OTG_DAINT_IEPINT                  ((uint_fast32_t)0x0000FFFF)            /*!< IN endpoint interrupt bits */
#define USB_OTG_DAINT_OEPINT                  ((uint_fast32_t)0xFFFF0000)            /*!< OUT endpoint interrupt bits */

/********************  Bit definition forUSB_OTG_HAINTMSK register  ********************/
#define USB_OTG_HAINTMSK_HAINTM                  ((uint_fast32_t)0x0000FFFF)            /*!< Channel interrupt mask */

/********************  Bit definition for USB_OTG_GRXSTSP register  ********************/
#define USB_OTG_GRXSTSP_EPNUM                    ((uint_fast32_t)0x0000000F)            /*!< IN EP interrupt mask bits */
#define USB_OTG_GRXSTSP_BCNT                     ((uint_fast32_t)0x00007FF0)            /*!< OUT EP interrupt mask bits */
#define USB_OTG_GRXSTSP_DPID                     ((uint_fast32_t)0x00018000)            /*!< OUT EP interrupt mask bits */
#define USB_OTG_GRXSTSP_PKTSTS                   ((uint_fast32_t)0x001E0000)            /*!< OUT EP interrupt mask bits */

/********************  Bit definition forUSB_OTG_DAINTMSK register  ********************/
#define USB_OTG_DAINTMSK_IEPM                    ((uint_fast32_t)0x0000FFFF)            /*!< IN EP interrupt mask bits */
#define USB_OTG_DAINTMSK_OEPM                    ((uint_fast32_t)0xFFFF0000)            /*!< OUT EP interrupt mask bits */

/********************  Bit definition for OTG register  ********************/

#define USB_OTG_CHNUM                   ((uint_fast32_t)0x0000000F)            /*!< Channel number */
#define USB_OTG_CHNUM_0                 ((uint_fast32_t)0x00000001)            /*!<Bit 0 */
#define USB_OTG_CHNUM_1                 ((uint_fast32_t)0x00000002)            /*!<Bit 1 */
#define USB_OTG_CHNUM_2                 ((uint_fast32_t)0x00000004)            /*!<Bit 2 */
#define USB_OTG_CHNUM_3                 ((uint_fast32_t)0x00000008)            /*!<Bit 3 */
#define USB_OTG_BCNT                    ((uint_fast32_t)0x00007FF0)            /*!< Byte count */

#define USB_OTG_DPID                    ((uint_fast32_t)0x00018000)            /*!< Data PID */
#define USB_OTG_DPID_0                  ((uint_fast32_t)0x00008000)            /*!<Bit 0 */
#define USB_OTG_DPID_1                  ((uint_fast32_t)0x00010000)            /*!<Bit 1 */

#define USB_OTG_PKTSTS                  ((uint_fast32_t)0x001E0000)            /*!< Packet status */
#define USB_OTG_PKTSTS_0                ((uint_fast32_t)0x00020000)            /*!<Bit 0 */
#define USB_OTG_PKTSTS_1                ((uint_fast32_t)0x00040000)            /*!<Bit 1 */
#define USB_OTG_PKTSTS_2                ((uint_fast32_t)0x00080000)            /*!<Bit 2 */
#define USB_OTG_PKTSTS_3                ((uint_fast32_t)0x00100000)            /*!<Bit 3 */

#define USB_OTG_EPNUM                   ((uint_fast32_t)0x0000000F)            /*!< Endpoint number */
#define USB_OTG_EPNUM_0                 ((uint_fast32_t)0x00000001)            /*!<Bit 0 */
#define USB_OTG_EPNUM_1                 ((uint_fast32_t)0x00000002)            /*!<Bit 1 */
#define USB_OTG_EPNUM_2                 ((uint_fast32_t)0x00000004)            /*!<Bit 2 */
#define USB_OTG_EPNUM_3                 ((uint_fast32_t)0x00000008)            /*!<Bit 3 */

#define USB_OTG_FRMNUM                  ((uint_fast32_t)0x01E00000)            /*!< Frame number */
#define USB_OTG_FRMNUM_0                ((uint_fast32_t)0x00200000)            /*!<Bit 0 */
#define USB_OTG_FRMNUM_1                ((uint_fast32_t)0x00400000)            /*!<Bit 1 */
#define USB_OTG_FRMNUM_2                ((uint_fast32_t)0x00800000)            /*!<Bit 2 */
#define USB_OTG_FRMNUM_3                ((uint_fast32_t)0x01000000)            /*!<Bit 3 */

/********************  Bit definition for OTG register  ********************/

#define USB_OTG_CHNUM                   ((uint_fast32_t)0x0000000F)            /*!< Channel number */
#define USB_OTG_CHNUM_0                 ((uint_fast32_t)0x00000001)            /*!<Bit 0 */
#define USB_OTG_CHNUM_1                 ((uint_fast32_t)0x00000002)            /*!<Bit 1 */
#define USB_OTG_CHNUM_2                 ((uint_fast32_t)0x00000004)            /*!<Bit 2 */
#define USB_OTG_CHNUM_3                 ((uint_fast32_t)0x00000008)            /*!<Bit 3 */
#define USB_OTG_BCNT                    ((uint_fast32_t)0x00007FF0)            /*!< Byte count */

#define USB_OTG_DPID                    ((uint_fast32_t)0x00018000)            /*!< Data PID */
#define USB_OTG_DPID_0                  ((uint_fast32_t)0x00008000)            /*!<Bit 0 */
#define USB_OTG_DPID_1                  ((uint_fast32_t)0x00010000)            /*!<Bit 1 */

#define USB_OTG_PKTSTS                  ((uint_fast32_t)0x001E0000)            /*!< Packet status */
#define USB_OTG_PKTSTS_0                ((uint_fast32_t)0x00020000)            /*!<Bit 0 */
#define USB_OTG_PKTSTS_1                ((uint_fast32_t)0x00040000)            /*!<Bit 1 */
#define USB_OTG_PKTSTS_2                ((uint_fast32_t)0x00080000)            /*!<Bit 2 */
#define USB_OTG_PKTSTS_3                ((uint_fast32_t)0x00100000)            /*!<Bit 3 */

#define USB_OTG_EPNUM                   ((uint_fast32_t)0x0000000F)            /*!< Endpoint number */
#define USB_OTG_EPNUM_0                 ((uint_fast32_t)0x00000001)            /*!<Bit 0 */
#define USB_OTG_EPNUM_1                 ((uint_fast32_t)0x00000002)            /*!<Bit 1 */
#define USB_OTG_EPNUM_2                 ((uint_fast32_t)0x00000004)            /*!<Bit 2 */
#define USB_OTG_EPNUM_3                 ((uint_fast32_t)0x00000008)            /*!<Bit 3 */

#define USB_OTG_FRMNUM                  ((uint_fast32_t)0x01E00000)            /*!< Frame number */
#define USB_OTG_FRMNUM_0                ((uint_fast32_t)0x00200000)            /*!<Bit 0 */
#define USB_OTG_FRMNUM_1                ((uint_fast32_t)0x00400000)            /*!<Bit 1 */
#define USB_OTG_FRMNUM_2                ((uint_fast32_t)0x00800000)            /*!<Bit 2 */
#define USB_OTG_FRMNUM_3                ((uint_fast32_t)0x01000000)            /*!<Bit 3 */

/********************  Bit definition forUSB_OTG_GRXFSIZ register  ********************/
#define USB_OTG_GRXFSIZ_RXFD                    ((uint_fast32_t)0x0000FFFF)            /*!< RxFIFO depth */

/********************  Bit definition forUSB_OTG_DVBUSDIS register  ********************/
#define USB_OTG_DVBUSDIS_VBUSDT                  ((uint_fast32_t)0x0000FFFF)            /*!< Device VBUS discharge time */

/********************  Bit definition for OTG register  ********************/
#define USB_OTG_NPTXFSA                 ((uint_fast32_t)0x0000FFFF)            /*!< Nonperiodic transmit RAM start address */
#define USB_OTG_NPTXFD                  ((uint_fast32_t)0xFFFF0000)            /*!< Nonperiodic TxFIFO depth */
#define USB_OTG_TX0FSA                  ((uint_fast32_t)0x0000FFFF)            /*!< Endpoint 0 transmit RAM start address */
#define USB_OTG_TX0FD                   ((uint_fast32_t)0xFFFF0000)            /*!< Endpoint 0 TxFIFO depth */

/********************  Bit definition forUSB_OTG_DVBUSPULSE register  ********************/
#define USB_OTG_DVBUSPULSE_DVBUSP                  ((uint_fast32_t)0x00000FFF)            /*!< Device VBUS pulsing time */

/********************  Bit definition forUSB_OTG_GNPTXSTS register  ********************/
#define USB_OTG_GNPTXSTS_NPTXFSAV                ((uint_fast32_t)0x0000FFFF)            /*!< Nonperiodic TxFIFO space available */

#define USB_OTG_GNPTXSTS_NPTQXSAV                ((uint_fast32_t)0x00FF0000)            /*!< Nonperiodic transmit request queue space available */
#define USB_OTG_GNPTXSTS_NPTQXSAV_0              ((uint_fast32_t)0x00010000)            /*!<Bit 0 */
#define USB_OTG_GNPTXSTS_NPTQXSAV_1              ((uint_fast32_t)0x00020000)            /*!<Bit 1 */
#define USB_OTG_GNPTXSTS_NPTQXSAV_2              ((uint_fast32_t)0x00040000)            /*!<Bit 2 */
#define USB_OTG_GNPTXSTS_NPTQXSAV_3              ((uint_fast32_t)0x00080000)            /*!<Bit 3 */
#define USB_OTG_GNPTXSTS_NPTQXSAV_4              ((uint_fast32_t)0x00100000)            /*!<Bit 4 */
#define USB_OTG_GNPTXSTS_NPTQXSAV_5              ((uint_fast32_t)0x00200000)            /*!<Bit 5 */
#define USB_OTG_GNPTXSTS_NPTQXSAV_6              ((uint_fast32_t)0x00400000)            /*!<Bit 6 */
#define USB_OTG_GNPTXSTS_NPTQXSAV_7              ((uint_fast32_t)0x00800000)            /*!<Bit 7 */

#define USB_OTG_GNPTXSTS_NPTXQTOP                ((uint_fast32_t)0x7F000000)            /*!< Top of the nonperiodic transmit request queue */
#define USB_OTG_GNPTXSTS_NPTXQTOP_0              ((uint_fast32_t)0x01000000)            /*!<Bit 0 */
#define USB_OTG_GNPTXSTS_NPTXQTOP_1              ((uint_fast32_t)0x02000000)            /*!<Bit 1 */
#define USB_OTG_GNPTXSTS_NPTXQTOP_2              ((uint_fast32_t)0x04000000)            /*!<Bit 2 */
#define USB_OTG_GNPTXSTS_NPTXQTOP_3              ((uint_fast32_t)0x08000000)            /*!<Bit 3 */
#define USB_OTG_GNPTXSTS_NPTXQTOP_4              ((uint_fast32_t)0x10000000)            /*!<Bit 4 */
#define USB_OTG_GNPTXSTS_NPTXQTOP_5              ((uint_fast32_t)0x20000000)            /*!<Bit 5 */
#define USB_OTG_GNPTXSTS_NPTXQTOP_6              ((uint_fast32_t)0x40000000)            /*!<Bit 6 */

/********************  Bit definition forUSB_OTG_DTHRCTL register  ********************/
#define USB_OTG_DTHRCTL_NONISOTHREN             ((uint_fast32_t)0x00000001)            /*!< Nonisochronous IN endpoints threshold enable */
#define USB_OTG_DTHRCTL_ISOTHREN                ((uint_fast32_t)0x00000002)            /*!< ISO IN endpoint threshold enable */

#define USB_OTG_DTHRCTL_TXTHRLEN                ((uint_fast32_t)0x000007FC)            /*!< Transmit threshold length */
#define USB_OTG_DTHRCTL_TXTHRLEN_0              ((uint_fast32_t)0x00000004)            /*!<Bit 0 */
#define USB_OTG_DTHRCTL_TXTHRLEN_1              ((uint_fast32_t)0x00000008)            /*!<Bit 1 */
#define USB_OTG_DTHRCTL_TXTHRLEN_2              ((uint_fast32_t)0x00000010)            /*!<Bit 2 */
#define USB_OTG_DTHRCTL_TXTHRLEN_3              ((uint_fast32_t)0x00000020)            /*!<Bit 3 */
#define USB_OTG_DTHRCTL_TXTHRLEN_4              ((uint_fast32_t)0x00000040)            /*!<Bit 4 */
#define USB_OTG_DTHRCTL_TXTHRLEN_5              ((uint_fast32_t)0x00000080)            /*!<Bit 5 */
#define USB_OTG_DTHRCTL_TXTHRLEN_6              ((uint_fast32_t)0x00000100)            /*!<Bit 6 */
#define USB_OTG_DTHRCTL_TXTHRLEN_7              ((uint_fast32_t)0x00000200)            /*!<Bit 7 */
#define USB_OTG_DTHRCTL_TXTHRLEN_8              ((uint_fast32_t)0x00000400)            /*!<Bit 8 */
#define USB_OTG_DTHRCTL_RXTHREN                 ((uint_fast32_t)0x00010000)            /*!< Receive threshold enable */

#define USB_OTG_DTHRCTL_RXTHRLEN                ((uint_fast32_t)0x03FE0000)            /*!< Receive threshold length */
#define USB_OTG_DTHRCTL_RXTHRLEN_0              ((uint_fast32_t)0x00020000)            /*!<Bit 0 */
#define USB_OTG_DTHRCTL_RXTHRLEN_1              ((uint_fast32_t)0x00040000)            /*!<Bit 1 */
#define USB_OTG_DTHRCTL_RXTHRLEN_2              ((uint_fast32_t)0x00080000)            /*!<Bit 2 */
#define USB_OTG_DTHRCTL_RXTHRLEN_3              ((uint_fast32_t)0x00100000)            /*!<Bit 3 */
#define USB_OTG_DTHRCTL_RXTHRLEN_4              ((uint_fast32_t)0x00200000)            /*!<Bit 4 */
#define USB_OTG_DTHRCTL_RXTHRLEN_5              ((uint_fast32_t)0x00400000)            /*!<Bit 5 */
#define USB_OTG_DTHRCTL_RXTHRLEN_6              ((uint_fast32_t)0x00800000)            /*!<Bit 6 */
#define USB_OTG_DTHRCTL_RXTHRLEN_7              ((uint_fast32_t)0x01000000)            /*!<Bit 7 */
#define USB_OTG_DTHRCTL_RXTHRLEN_8              ((uint_fast32_t)0x02000000)            /*!<Bit 8 */
#define USB_OTG_DTHRCTL_ARPEN                   ((uint_fast32_t)0x08000000)            /*!< Arbiter parking enable */

/********************  Bit definition forUSB_OTG_DIEPEMPMSK register  ********************/
#define USB_OTG_DIEPEMPMSK_INEPTXFEM               ((uint_fast32_t)0x0000FFFF)            /*!< IN EP Tx FIFO empty interrupt mask bits */

/********************  Bit definition forUSB_OTG_DEACHINT register  ********************/
#define USB_OTG_DEACHINT_IEP1INT                 ((uint_fast32_t)0x00000002)            /*!< IN endpoint 1interrupt bit */
#define USB_OTG_DEACHINT_OEP1INT                 ((uint_fast32_t)0x00020000)            /*!< OUT endpoint 1 interrupt bit */

/********************  Bit definition forUSB_OTG_GCCFG register  ********************/
#define USB_OTG_GCCFG_PWRDWN                  ((uint_fast32_t)0x00010000)            /*!< Power down */
#define USB_OTG_GCCFG_I2CPADEN                ((uint_fast32_t)0x00020000)            /*!< Enable I2C bus connection for the external I2C PHY interface */
#define USB_OTG_GCCFG_VBUSASEN                ((uint_fast32_t)0x00040000)            /*!< Enable the VBUS sensing device */
#define USB_OTG_GCCFG_VBUSBSEN                ((uint_fast32_t)0x00080000)            /*!< Enable the VBUS sensing device */
#define USB_OTG_GCCFG_SOFOUTEN                ((uint_fast32_t)0x00100000)            /*!< SOF output enable */
#define USB_OTG_GCCFG_NOVBUSSENS              ((uint_fast32_t)0x00200000)            /*!< VBUS sensing disable option */

/********************  Bit definition forUSB_OTG_DEACHINTMSK register  ********************/
#define USB_OTG_DEACHINTMSK_IEP1INTM                ((uint_fast32_t)0x00000002)            /*!< IN Endpoint 1 interrupt mask bit */
#define USB_OTG_DEACHINTMSK_OEP1INTM                ((uint_fast32_t)0x00020000)            /*!< OUT Endpoint 1 interrupt mask bit */

/********************  Bit definition forUSB_OTG_CID register  ********************/
#define USB_OTG_CID_PRODUCT_ID              ((uint_fast32_t)0xFFFFFFFF)            /*!< Product ID field */

/********************  Bit definition forUSB_OTG_DIEPEACHMSK1 register  ********************/
#define USB_OTG_DIEPEACHMSK1_XFRCM                   ((uint_fast32_t)0x00000001)            /*!< Transfer completed interrupt mask */
#define USB_OTG_DIEPEACHMSK1_EPDM                    ((uint_fast32_t)0x00000002)            /*!< Endpoint disabled interrupt mask */
#define USB_OTG_DIEPEACHMSK1_TOM                     ((uint_fast32_t)0x00000008)            /*!< Timeout condition mask (nonisochronous endpoints) */
#define USB_OTG_DIEPEACHMSK1_ITTXFEMSK               ((uint_fast32_t)0x00000010)            /*!< IN token received when TxFIFO empty mask */
#define USB_OTG_DIEPEACHMSK1_INEPNMM                 ((uint_fast32_t)0x00000020)            /*!< IN token received with EP mismatch mask */
#define USB_OTG_DIEPEACHMSK1_INEPNEM                 ((uint_fast32_t)0x00000040)            /*!< IN endpoint NAK effective mask */
#define USB_OTG_DIEPEACHMSK1_TXFURM                  ((uint_fast32_t)0x00000100)            /*!< FIFO underrun mask */
#define USB_OTG_DIEPEACHMSK1_BIM                     ((uint_fast32_t)0x00000200)            /*!< BNA interrupt mask */
#define USB_OTG_DIEPEACHMSK1_NAKM                    ((uint_fast32_t)0x00002000)            /*!< NAK interrupt mask */

/********************  Bit definition forUSB_OTG_HPRT register  ********************/
#define USB_OTG_HPRT_PCSTS                   ((uint_fast32_t)0x00000001)            /*!< Port connect status */
#define USB_OTG_HPRT_PCDET                   ((uint_fast32_t)0x00000002)            /*!< Port connect detected */
#define USB_OTG_HPRT_PENA                    ((uint_fast32_t)0x00000004)            /*!< Port enable */
#define USB_OTG_HPRT_PENCHNG                 ((uint_fast32_t)0x00000008)            /*!< Port enable/disable change */
#define USB_OTG_HPRT_POCA                    ((uint_fast32_t)0x00000010)            /*!< Port overcurrent active */
#define USB_OTG_HPRT_POCCHNG                 ((uint_fast32_t)0x00000020)            /*!< Port overcurrent change */
#define USB_OTG_HPRT_PRES                    ((uint_fast32_t)0x00000040)            /*!< Port resume */
#define USB_OTG_HPRT_PSUSP                   ((uint_fast32_t)0x00000080)            /*!< Port suspend */
#define USB_OTG_HPRT_PRST                    ((uint_fast32_t)0x00000100)            /*!< Port reset */

#define USB_OTG_HPRT_PLSTS                   ((uint_fast32_t)0x00000C00)            /*!< Port line status */
#define USB_OTG_HPRT_PLSTS_0                 ((uint_fast32_t)0x00000400)            /*!<Bit 0 */
#define USB_OTG_HPRT_PLSTS_1                 ((uint_fast32_t)0x00000800)            /*!<Bit 1 */
#define USB_OTG_HPRT_PPWR                    ((uint_fast32_t)0x00001000)            /*!< Port power */

#define USB_OTG_HPRT_PTCTL                   ((uint_fast32_t)0x0001E000)            /*!< Port test control */
#define USB_OTG_HPRT_PTCTL_0                 ((uint_fast32_t)0x00002000)            /*!<Bit 0 */
#define USB_OTG_HPRT_PTCTL_1                 ((uint_fast32_t)0x00004000)            /*!<Bit 1 */
#define USB_OTG_HPRT_PTCTL_2                 ((uint_fast32_t)0x00008000)            /*!<Bit 2 */
#define USB_OTG_HPRT_PTCTL_3                 ((uint_fast32_t)0x00010000)            /*!<Bit 3 */

#define USB_OTG_HPRT_PSPD                    ((uint_fast32_t)0x00060000)            /*!< Port speed */
#define USB_OTG_HPRT_PSPD_0                  ((uint_fast32_t)0x00020000)            /*!<Bit 0 */
#define USB_OTG_HPRT_PSPD_1                  ((uint_fast32_t)0x00040000)            /*!<Bit 1 */

/********************  Bit definition forUSB_OTG_DOEPEACHMSK1 register  ********************/
#define USB_OTG_DOEPEACHMSK1_XFRCM                   ((uint_fast32_t)0x00000001)            /*!< Transfer completed interrupt mask */
#define USB_OTG_DOEPEACHMSK1_EPDM                    ((uint_fast32_t)0x00000002)            /*!< Endpoint disabled interrupt mask */
#define USB_OTG_DOEPEACHMSK1_TOM                     ((uint_fast32_t)0x00000008)            /*!< Timeout condition mask */
#define USB_OTG_DOEPEACHMSK1_ITTXFEMSK               ((uint_fast32_t)0x00000010)            /*!< IN token received when TxFIFO empty mask */
#define USB_OTG_DOEPEACHMSK1_INEPNMM                 ((uint_fast32_t)0x00000020)            /*!< IN token received with EP mismatch mask */
#define USB_OTG_DOEPEACHMSK1_INEPNEM                 ((uint_fast32_t)0x00000040)            /*!< IN endpoint NAK effective mask */
#define USB_OTG_DOEPEACHMSK1_TXFURM                  ((uint_fast32_t)0x00000100)            /*!< OUT packet error mask */
#define USB_OTG_DOEPEACHMSK1_BIM                     ((uint_fast32_t)0x00000200)            /*!< BNA interrupt mask */
#define USB_OTG_DOEPEACHMSK1_BERRM                   ((uint_fast32_t)0x00001000)            /*!< Bubble error interrupt mask */
#define USB_OTG_DOEPEACHMSK1_NAKM                    ((uint_fast32_t)0x00002000)            /*!< NAK interrupt mask */
#define USB_OTG_DOEPEACHMSK1_NYETM                   ((uint_fast32_t)0x00004000)            /*!< NYET interrupt mask */

/********************  Bit definition forUSB_OTG_HPTXFSIZ register  ********************/
#define USB_OTG_HPTXFSIZ_PTXSA                   ((uint_fast32_t)0x0000FFFF)            /*!< Host periodic TxFIFO start address */
#define USB_OTG_HPTXFSIZ_PTXFD                   ((uint_fast32_t)0xFFFF0000)            /*!< Host periodic TxFIFO depth */

/********************  Bit definition forUSB_OTG_DIEPCTL register  ********************/
#define USB_OTG_DIEPCTL_MPSIZ                   ((uint_fast32_t)0x000007FF)            /*!< Maximum packet size */
#define USB_OTG_DIEPCTL_USBAEP                  ((uint_fast32_t)0x00008000)            /*!< USB active endpoint */
#define USB_OTG_DIEPCTL_EONUM_DPID              ((uint_fast32_t)0x00010000)            /*!< Even/odd frame */
#define USB_OTG_DIEPCTL_NAKSTS                  ((uint_fast32_t)0x00020000)            /*!< NAK status */

#define USB_OTG_DIEPCTL_EPTYP                   ((uint_fast32_t)0x000C0000)            /*!< Endpoint type */
#define USB_OTG_DIEPCTL_EPTYP_0                 ((uint_fast32_t)0x00040000)            /*!<Bit 0 */
#define USB_OTG_DIEPCTL_EPTYP_1                 ((uint_fast32_t)0x00080000)            /*!<Bit 1 */
#define USB_OTG_DIEPCTL_STALL                   ((uint_fast32_t)0x00200000)            /*!< STALL handshake */

#define USB_OTG_DIEPCTL_TXFNUM                  ((uint_fast32_t)0x03C00000)            /*!< TxFIFO number */
#define USB_OTG_DIEPCTL_TXFNUM_0                ((uint_fast32_t)0x00400000)            /*!<Bit 0 */
#define USB_OTG_DIEPCTL_TXFNUM_1                ((uint_fast32_t)0x00800000)            /*!<Bit 1 */
#define USB_OTG_DIEPCTL_TXFNUM_2                ((uint_fast32_t)0x01000000)            /*!<Bit 2 */
#define USB_OTG_DIEPCTL_TXFNUM_3                ((uint_fast32_t)0x02000000)            /*!<Bit 3 */
#define USB_OTG_DIEPCTL_CNAK                    ((uint_fast32_t)0x04000000)            /*!< Clear NAK */
#define USB_OTG_DIEPCTL_SNAK                    ((uint_fast32_t)0x08000000)            /*!< Set NAK */
#define USB_OTG_DIEPCTL_SD0PID_SEVNFRM          ((uint_fast32_t)0x10000000)            /*!< Set DATA0 PID */
#define USB_OTG_DIEPCTL_SODDFRM                 ((uint_fast32_t)0x20000000)            /*!< Set odd frame */
#define USB_OTG_DIEPCTL_EPDIS                   ((uint_fast32_t)0x40000000)            /*!< Endpoint disable */
#define USB_OTG_DIEPCTL_EPENA                   ((uint_fast32_t)0x80000000)            /*!< Endpoint enable */

/********************  Bit definition forUSB_OTG_HCCHAR register  ********************/
#define USB_OTG_HCCHAR_MPSIZ                   ((uint_fast32_t)0x000007FF)            /*!< Maximum packet size */

#define USB_OTG_HCCHAR_EPNUM                   ((uint_fast32_t)0x00007800)            /*!< Endpoint number */
#define USB_OTG_HCCHAR_EPNUM_0                 ((uint_fast32_t)0x00000800)            /*!<Bit 0 */
#define USB_OTG_HCCHAR_EPNUM_1                 ((uint_fast32_t)0x00001000)            /*!<Bit 1 */
#define USB_OTG_HCCHAR_EPNUM_2                 ((uint_fast32_t)0x00002000)            /*!<Bit 2 */
#define USB_OTG_HCCHAR_EPNUM_3                 ((uint_fast32_t)0x00004000)            /*!<Bit 3 */
#define USB_OTG_HCCHAR_EPDIR                   ((uint_fast32_t)0x00008000)            /*!< Endpoint direction */
#define USB_OTG_HCCHAR_LSDEV                   ((uint_fast32_t)0x00020000)            /*!< Low-speed device */

#define USB_OTG_HCCHAR_EPTYP                   ((uint_fast32_t)0x000C0000)            /*!< Endpoint type */
#define USB_OTG_HCCHAR_EPTYP_0                 ((uint_fast32_t)0x00040000)            /*!<Bit 0 */
#define USB_OTG_HCCHAR_EPTYP_1                 ((uint_fast32_t)0x00080000)            /*!<Bit 1 */

#define USB_OTG_HCCHAR_MC                      ((uint_fast32_t)0x00300000)            /*!< Multi Count (MC) / Error Count (EC) */
#define USB_OTG_HCCHAR_MC_0                    ((uint_fast32_t)0x00100000)            /*!<Bit 0 */
#define USB_OTG_HCCHAR_MC_1                    ((uint_fast32_t)0x00200000)            /*!<Bit 1 */

#define USB_OTG_HCCHAR_DAD                     ((uint_fast32_t)0x1FC00000)            /*!< Device address */
#define USB_OTG_HCCHAR_DAD_0                   ((uint_fast32_t)0x00400000)            /*!<Bit 0 */
#define USB_OTG_HCCHAR_DAD_1                   ((uint_fast32_t)0x00800000)            /*!<Bit 1 */
#define USB_OTG_HCCHAR_DAD_2                   ((uint_fast32_t)0x01000000)            /*!<Bit 2 */
#define USB_OTG_HCCHAR_DAD_3                   ((uint_fast32_t)0x02000000)            /*!<Bit 3 */
#define USB_OTG_HCCHAR_DAD_4                   ((uint_fast32_t)0x04000000)            /*!<Bit 4 */
#define USB_OTG_HCCHAR_DAD_5                   ((uint_fast32_t)0x08000000)            /*!<Bit 5 */
#define USB_OTG_HCCHAR_DAD_6                   ((uint_fast32_t)0x10000000)            /*!<Bit 6 */
#define USB_OTG_HCCHAR_ODDFRM                  ((uint_fast32_t)0x20000000)            /*!< Odd frame */
#define USB_OTG_HCCHAR_CHDIS                   ((uint_fast32_t)0x40000000)            /*!< Channel disable */
#define USB_OTG_HCCHAR_CHENA                   ((uint_fast32_t)0x80000000)            /*!< Channel enable */

/********************  Bit definition forUSB_OTG_HCSPLT register  ********************/

#define USB_OTG_HCSPLT_PRTADDR                 ((uint_fast32_t)0x0000007F)            /*!< Port address */
#define USB_OTG_HCSPLT_PRTADDR_0               ((uint_fast32_t)0x00000001)            /*!<Bit 0 */
#define USB_OTG_HCSPLT_PRTADDR_1               ((uint_fast32_t)0x00000002)            /*!<Bit 1 */
#define USB_OTG_HCSPLT_PRTADDR_2               ((uint_fast32_t)0x00000004)            /*!<Bit 2 */
#define USB_OTG_HCSPLT_PRTADDR_3               ((uint_fast32_t)0x00000008)            /*!<Bit 3 */
#define USB_OTG_HCSPLT_PRTADDR_4               ((uint_fast32_t)0x00000010)            /*!<Bit 4 */
#define USB_OTG_HCSPLT_PRTADDR_5               ((uint_fast32_t)0x00000020)            /*!<Bit 5 */
#define USB_OTG_HCSPLT_PRTADDR_6               ((uint_fast32_t)0x00000040)            /*!<Bit 6 */

#define USB_OTG_HCSPLT_HUBADDR                 ((uint_fast32_t)0x00003F80)            /*!< Hub address */
#define USB_OTG_HCSPLT_HUBADDR_0               ((uint_fast32_t)0x00000080)            /*!<Bit 0 */
#define USB_OTG_HCSPLT_HUBADDR_1               ((uint_fast32_t)0x00000100)            /*!<Bit 1 */
#define USB_OTG_HCSPLT_HUBADDR_2               ((uint_fast32_t)0x00000200)            /*!<Bit 2 */
#define USB_OTG_HCSPLT_HUBADDR_3               ((uint_fast32_t)0x00000400)            /*!<Bit 3 */
#define USB_OTG_HCSPLT_HUBADDR_4               ((uint_fast32_t)0x00000800)            /*!<Bit 4 */
#define USB_OTG_HCSPLT_HUBADDR_5               ((uint_fast32_t)0x00001000)            /*!<Bit 5 */
#define USB_OTG_HCSPLT_HUBADDR_6               ((uint_fast32_t)0x00002000)            /*!<Bit 6 */

#define USB_OTG_HCSPLT_XACTPOS                 ((uint_fast32_t)0x0000C000)            /*!< XACTPOS */
#define USB_OTG_HCSPLT_XACTPOS_0               ((uint_fast32_t)0x00004000)            /*!<Bit 0 */
#define USB_OTG_HCSPLT_XACTPOS_1               ((uint_fast32_t)0x00008000)            /*!<Bit 1 */
#define USB_OTG_HCSPLT_COMPLSPLT               ((uint_fast32_t)0x00010000)            /*!< Do complete split */
#define USB_OTG_HCSPLT_SPLITEN                 ((uint_fast32_t)0x80000000)            /*!< Split enable */

/********************  Bit definition forUSB_OTG_HCINT register  ********************/
#define USB_OTG_HCINT_XFRC                    ((uint_fast32_t)0x00000001)            /*!< Transfer completed */
#define USB_OTG_HCINT_CHH                     ((uint_fast32_t)0x00000002)            /*!< Channel halted */
#define USB_OTG_HCINT_AHBERR                  ((uint_fast32_t)0x00000004)            /*!< AHB error */
#define USB_OTG_HCINT_STALL                   ((uint_fast32_t)0x00000008)            /*!< STALL response received interrupt */
#define USB_OTG_HCINT_NAK                     ((uint_fast32_t)0x00000010)            /*!< NAK response received interrupt */
#define USB_OTG_HCINT_ACK                     ((uint_fast32_t)0x00000020)            /*!< ACK response received/transmitted interrupt */
#define USB_OTG_HCINT_NYET                    ((uint_fast32_t)0x00000040)            /*!< Response received interrupt */
#define USB_OTG_HCINT_TXERR                   ((uint_fast32_t)0x00000080)            /*!< Transaction error */
#define USB_OTG_HCINT_BBERR                   ((uint_fast32_t)0x00000100)            /*!< Babble error */
#define USB_OTG_HCINT_FRMOR                   ((uint_fast32_t)0x00000200)            /*!< Frame overrun */
#define USB_OTG_HCINT_DTERR                   ((uint_fast32_t)0x00000400)            /*!< Data toggle error */

/********************  Bit definition forUSB_OTG_DIEPINT register  ********************/
#define USB_OTG_DIEPINT_XFRC                    ((uint_fast32_t)0x00000001)            /*!< Transfer completed interrupt */
#define USB_OTG_DIEPINT_EPDISD                  ((uint_fast32_t)0x00000002)            /*!< Endpoint disabled interrupt */
#define USB_OTG_DIEPINT_TOC                     ((uint_fast32_t)0x00000008)            /*!< Timeout condition */
#define USB_OTG_DIEPINT_ITTXFE                  ((uint_fast32_t)0x00000010)            /*!< IN token received when TxFIFO is empty */
#define USB_OTG_DIEPINT_INEPNE                  ((uint_fast32_t)0x00000040)            /*!< IN endpoint NAK effective */
#define USB_OTG_DIEPINT_TXFE                    ((uint_fast32_t)0x00000080)            /*!< Transmit FIFO empty */
#define USB_OTG_DIEPINT_TXFIFOUDRN              ((uint_fast32_t)0x00000100)            /*!< Transmit Fifo Underrun */
#define USB_OTG_DIEPINT_BNA                     ((uint_fast32_t)0x00000200)            /*!< Buffer not available interrupt */
#define USB_OTG_DIEPINT_PKTDRPSTS               ((uint_fast32_t)0x00000800)            /*!< Packet dropped status */
#define USB_OTG_DIEPINT_BERR                    ((uint_fast32_t)0x00001000)            /*!< Babble error interrupt */
#define USB_OTG_DIEPINT_NAK                     ((uint_fast32_t)0x00002000)            /*!< NAK interrupt */

/********************  Bit definition forUSB_OTG_HCINTMSK register  ********************/
#define USB_OTG_HCINTMSK_XFRCM                   ((uint_fast32_t)0x00000001)            /*!< Transfer completed mask */
#define USB_OTG_HCINTMSK_CHHM                    ((uint_fast32_t)0x00000002)            /*!< Channel halted mask */
#define USB_OTG_HCINTMSK_AHBERR                  ((uint_fast32_t)0x00000004)            /*!< AHB error */
#define USB_OTG_HCINTMSK_STALLM                  ((uint_fast32_t)0x00000008)            /*!< STALL response received interrupt mask */
#define USB_OTG_HCINTMSK_NAKM                    ((uint_fast32_t)0x00000010)            /*!< NAK response received interrupt mask */
#define USB_OTG_HCINTMSK_ACKM                    ((uint_fast32_t)0x00000020)            /*!< ACK response received/transmitted interrupt mask */
#define USB_OTG_HCINTMSK_NYET                    ((uint_fast32_t)0x00000040)            /*!< response received interrupt mask */
#define USB_OTG_HCINTMSK_TXERRM                  ((uint_fast32_t)0x00000080)            /*!< Transaction error mask */
#define USB_OTG_HCINTMSK_BBERRM                  ((uint_fast32_t)0x00000100)            /*!< Babble error mask */
#define USB_OTG_HCINTMSK_FRMORM                  ((uint_fast32_t)0x00000200)            /*!< Frame overrun mask */
#define USB_OTG_HCINTMSK_DTERRM                  ((uint_fast32_t)0x00000400)            /*!< Data toggle error mask */

/********************  Bit definition for USB_OTG_DIEPTSIZ register  ********************/

#define USB_OTG_DIEPTSIZ_XFRSIZ                  ((uint_fast32_t)0x0007FFFF)            /*!< Transfer size */
#define USB_OTG_DIEPTSIZ_PKTCNT                  ((uint_fast32_t)0x1FF80000)            /*!< Packet count */
#define USB_OTG_DIEPTSIZ_MULCNT                  ((uint_fast32_t)0x60000000)            /*!< Packet count */
/********************  Bit definition forUSB_OTG_HCTSIZ register  ********************/
#define USB_OTG_HCTSIZ_XFRSIZ                    ((uint_fast32_t)0x0007FFFF)            /*!< Transfer size */
#define USB_OTG_HCTSIZ_PKTCNT                    ((uint_fast32_t)0x1FF80000)            /*!< Packet count */
#define USB_OTG_HCTSIZ_DOPING                    ((uint_fast32_t)0x80000000)            /*!< Do PING */
#define USB_OTG_HCTSIZ_DPID                      ((uint_fast32_t)0x60000000)            /*!< Data PID */
#define USB_OTG_HCTSIZ_DPID_0                    ((uint_fast32_t)0x20000000)            /*!<Bit 0 */
#define USB_OTG_HCTSIZ_DPID_1                    ((uint_fast32_t)0x40000000)            /*!<Bit 1 */

/********************  Bit definition forUSB_OTG_DIEPDMA register  ********************/
#define USB_OTG_DIEPDMA_DMAADDR                  ((uint_fast32_t)0xFFFFFFFF)            /*!< DMA address */

/********************  Bit definition forUSB_OTG_HCDMA register  ********************/
#define USB_OTG_HCDMA_DMAADDR                    ((uint_fast32_t)0xFFFFFFFF)            /*!< DMA address */

/********************  Bit definition forUSB_OTG_DTXFSTS register  ********************/
#define USB_OTG_DTXFSTS_INEPTFSAV                ((uint_fast32_t)0x0000FFFF)            /*!< IN endpoint TxFIFO space avail */

/********************  Bit definition forUSB_OTG_DIEPTXF register  ********************/
#define USB_OTG_DIEPTXF_INEPTXSA                 ((uint_fast32_t)0x0000FFFF)            /*!< IN endpoint FIFOx transmit RAM start address */
#define USB_OTG_DIEPTXF_INEPTXFD                 ((uint_fast32_t)0xFFFF0000)            /*!< IN endpoint TxFIFO depth */

/********************  Bit definition forUSB_OTG_DOEPCTL register  ********************/

#define USB_OTG_DOEPCTL_MPSIZ                     ((uint_fast32_t)0x000007FF)            /*!< Maximum packet size */          /*!<Bit 1 */
#define USB_OTG_DOEPCTL_USBAEP                    ((uint_fast32_t)0x00008000)            /*!< USB active endpoint */
#define USB_OTG_DOEPCTL_NAKSTS                    ((uint_fast32_t)0x00020000)            /*!< NAK status */
#define USB_OTG_DOEPCTL_SD0PID_SEVNFRM            ((uint_fast32_t)0x10000000)            /*!< Set DATA0 PID */
#define USB_OTG_DOEPCTL_SODDFRM                   ((uint_fast32_t)0x20000000)            /*!< Set odd frame */
#define USB_OTG_DOEPCTL_EPTYP                     ((uint_fast32_t)0x000C0000)            /*!< Endpoint type */
#define USB_OTG_DOEPCTL_EPTYP_0                   ((uint_fast32_t)0x00040000)            /*!<Bit 0 */
#define USB_OTG_DOEPCTL_EPTYP_1                   ((uint_fast32_t)0x00080000)            /*!<Bit 1 */
#define USB_OTG_DOEPCTL_SNPM                      ((uint_fast32_t)0x00100000)            /*!< Snoop mode */
#define USB_OTG_DOEPCTL_STALL                     ((uint_fast32_t)0x00200000)            /*!< STALL handshake */
#define USB_OTG_DOEPCTL_CNAK                      ((uint_fast32_t)0x04000000)            /*!< Clear NAK */
#define USB_OTG_DOEPCTL_SNAK                      ((uint_fast32_t)0x08000000)            /*!< Set NAK */
#define USB_OTG_DOEPCTL_EPDIS                     ((uint_fast32_t)0x40000000)            /*!< Endpoint disable */
#define USB_OTG_DOEPCTL_EPENA                     ((uint_fast32_t)0x80000000)            /*!< Endpoint enable */

/********************  Bit definition forUSB_OTG_DOEPINT register  ********************/
#define USB_OTG_DOEPINT_XFRC                    ((uint_fast32_t)0x00000001)            /*!< Transfer completed interrupt */
#define USB_OTG_DOEPINT_EPDISD                  ((uint_fast32_t)0x00000002)            /*!< Endpoint disabled interrupt */
#define USB_OTG_DOEPINT_STUP                    ((uint_fast32_t)0x00000008)            /*!< SETUP phase done */
#define USB_OTG_DOEPINT_OTEPDIS                 ((uint_fast32_t)0x00000010)            /*!< OUT token received when endpoint disabled */
#define USB_OTG_DOEPINT_B2BSTUP                 ((uint_fast32_t)0x00000040)            /*!< Back-to-back SETUP packets received */
#define USB_OTG_DOEPINT_NYET                    ((uint_fast32_t)0x00004000)            /*!< NYET interrupt */

/********************  Bit definition forUSB_OTG_DOEPTSIZ register  ********************/

#define USB_OTG_DOEPTSIZ_XFRSIZ                  ((uint_fast32_t)0x0007FFFF)            /*!< Transfer size */
#define USB_OTG_DOEPTSIZ_PKTCNT                  ((uint_fast32_t)0x1FF80000)            /*!< Packet count */

#define USB_OTG_DOEPTSIZ_STUPCNT                 ((uint_fast32_t)0x60000000)            /*!< SETUP packet count */
#define USB_OTG_DOEPTSIZ_STUPCNT_0               ((uint_fast32_t)0x20000000)            /*!<Bit 0 */
#define USB_OTG_DOEPTSIZ_STUPCNT_1               ((uint_fast32_t)0x40000000)            /*!<Bit 1 */

/********************  Bit definition for PCGCCTL register  ********************/
#define USB_OTG_PCGCCTL_STOPCLK                 ((uint_fast32_t)0x00000001)            /*!< SETUP packet count */
#define USB_OTG_PCGCCTL_GATECLK                 ((uint_fast32_t)0x00000002)            /*!<Bit 0 */
#define USB_OTG_PCGCCTL_PHYSUSP                 ((uint_fast32_t)0x00000010)            /*!<Bit 1 */



/******************************* ADC Instances ********************************/
#define IS_ADC_ALL_INSTANCE(INSTANCE) (((INSTANCE) == ADC1) || \
                                       ((INSTANCE) == ADC2) || \
                                       ((INSTANCE) == ADC3))

/******************************* CAN Instances ********************************/
#define IS_CAN_ALL_INSTANCE(INSTANCE) (((INSTANCE) == CAN1) || \
                                       ((INSTANCE) == CAN2))

/******************************* CRC Instances ********************************/
#define IS_CRC_ALL_INSTANCE(INSTANCE) ((INSTANCE) == CRC)

/******************************* DAC Instances ********************************/
#define IS_DAC_ALL_INSTANCE(INSTANCE) ((INSTANCE) == DAC)

/******************************* DCMI Instances *******************************/
#define IS_DCMI_ALL_INSTANCE(INSTANCE) ((INSTANCE) == DCMI)

/******************************** DMA Instances *******************************/
#define IS_DMA_STREAM_ALL_INSTANCE(INSTANCE) (((INSTANCE) == DMA1_Stream0) || \
                                              ((INSTANCE) == DMA1_Stream1) || \
                                              ((INSTANCE) == DMA1_Stream2) || \
                                              ((INSTANCE) == DMA1_Stream3) || \
                                              ((INSTANCE) == DMA1_Stream4) || \
                                              ((INSTANCE) == DMA1_Stream5) || \
                                              ((INSTANCE) == DMA1_Stream6) || \
                                              ((INSTANCE) == DMA1_Stream7) || \
                                              ((INSTANCE) == DMA2_Stream0) || \
                                              ((INSTANCE) == DMA2_Stream1) || \
                                              ((INSTANCE) == DMA2_Stream2) || \
                                              ((INSTANCE) == DMA2_Stream3) || \
                                              ((INSTANCE) == DMA2_Stream4) || \
                                              ((INSTANCE) == DMA2_Stream5) || \
                                              ((INSTANCE) == DMA2_Stream6) || \
                                              ((INSTANCE) == DMA2_Stream7))

/******************************* GPIO Instances *******************************/
#define IS_GPIO_ALL_INSTANCE(INSTANCE) (((INSTANCE) == GPIOA) || \
                                        ((INSTANCE) == GPIOB) || \
                                        ((INSTANCE) == GPIOC) || \
                                        ((INSTANCE) == GPIOD) || \
                                        ((INSTANCE) == GPIOE) || \
                                        ((INSTANCE) == GPIOF) || \
                                        ((INSTANCE) == GPIOG) || \
                                        ((INSTANCE) == GPIOH) || \
                                        ((INSTANCE) == GPIOI))

/******************************** I2C Instances *******************************/
#define IS_I2C_ALL_INSTANCE(INSTANCE) (((INSTANCE) == I2C1) || \
                                       ((INSTANCE) == I2C2) || \
                                       ((INSTANCE) == I2C3))

/******************************** I2S Instances *******************************/
#define IS_I2S_INSTANCE(INSTANCE)  (((INSTANCE) == SPI2) || \
                                    ((INSTANCE) == SPI3))

/*************************** I2S Extended Instances ***************************/
#define IS_I2S_INSTANCE_EXT(PERIPH)  (((INSTANCE) == SPI2)    || \
                                      ((INSTANCE) == SPI3)    || \
                                      ((INSTANCE) == I2S2ext) || \
                                      ((INSTANCE) == I2S3ext))

/******************************* RNG Instances ********************************/
#define IS_RNG_ALL_INSTANCE(INSTANCE)  ((INSTANCE) == RNG)

/****************************** RTC Instances *********************************/
#define IS_RTC_ALL_INSTANCE(INSTANCE)  ((INSTANCE) == RTC)

/******************************** SPI Instances *******************************/
#define IS_SPI_ALL_INSTANCE(INSTANCE) (((INSTANCE) == SPI1) || \
                                       ((INSTANCE) == SPI2) || \
                                       ((INSTANCE) == SPI3))

/*************************** SPI Extended Instances ***************************/
#define IS_SPI_ALL_INSTANCE_EXT(INSTANCE) (((INSTANCE) == SPI1)    || \
                                           ((INSTANCE) == SPI2)    || \
                                           ((INSTANCE) == SPI3)    || \
                                           ((INSTANCE) == I2S2ext) || \
                                           ((INSTANCE) == I2S3ext))

/****************** TIM Instances : All supported instances *******************/
#define IS_TIM_INSTANCE(INSTANCE) (((INSTANCE) == TIM1)   || \
                                   ((INSTANCE) == TIM2)   || \
                                   ((INSTANCE) == TIM3)   || \
                                   ((INSTANCE) == TIM4)   || \
                                   ((INSTANCE) == TIM5)   || \
                                   ((INSTANCE) == TIM6)   || \
                                   ((INSTANCE) == TIM7)   || \
                                   ((INSTANCE) == TIM8)   || \
                                   ((INSTANCE) == TIM9)   || \
                                   ((INSTANCE) == TIM10)  || \
                                   ((INSTANCE) == TIM11)  || \
                                   ((INSTANCE) == TIM12)  || \
                                   ((INSTANCE) == TIM13)  || \
                                   ((INSTANCE) == TIM14))

/************* TIM Instances : at least 1 capture/compare channel *************/
#define IS_TIM_CC1_INSTANCE(INSTANCE)   (((INSTANCE) == TIM1)  || \
                                         ((INSTANCE) == TIM2)  || \
                                         ((INSTANCE) == TIM3)  || \
                                         ((INSTANCE) == TIM4)  || \
                                         ((INSTANCE) == TIM5)  || \
                                         ((INSTANCE) == TIM8)  || \
                                         ((INSTANCE) == TIM9)  || \
                                         ((INSTANCE) == TIM10) || \
                                         ((INSTANCE) == TIM11) || \
                                         ((INSTANCE) == TIM12) || \
                                         ((INSTANCE) == TIM13) || \
                                         ((INSTANCE) == TIM14))

/************ TIM Instances : at least 2 capture/compare channels *************/
#define IS_TIM_CC2_INSTANCE(INSTANCE) (((INSTANCE) == TIM1) || \
                                       ((INSTANCE) == TIM2) || \
                                       ((INSTANCE) == TIM3) || \
                                       ((INSTANCE) == TIM4) || \
                                       ((INSTANCE) == TIM5) || \
                                       ((INSTANCE) == TIM8) || \
                                       ((INSTANCE) == TIM9) || \
                                       ((INSTANCE) == TIM12))

/************ TIM Instances : at least 3 capture/compare channels *************/
#define IS_TIM_CC3_INSTANCE(INSTANCE)   (((INSTANCE) == TIM1) || \
                                         ((INSTANCE) == TIM2) || \
                                         ((INSTANCE) == TIM3) || \
                                         ((INSTANCE) == TIM4) || \
                                         ((INSTANCE) == TIM5) || \
                                         ((INSTANCE) == TIM8))

/************ TIM Instances : at least 4 capture/compare channels *************/
#define IS_TIM_CC4_INSTANCE(INSTANCE) (((INSTANCE) == TIM1) || \
                                       ((INSTANCE) == TIM2) || \
                                       ((INSTANCE) == TIM3) || \
                                       ((INSTANCE) == TIM4) || \
                                       ((INSTANCE) == TIM5) || \
                                       ((INSTANCE) == TIM8))

/******************** TIM Instances : Advanced-control timers *****************/
#define IS_TIM_ADVANCED_INSTANCE(INSTANCE) (((INSTANCE) == TIM1) || \
                                            ((INSTANCE) == TIM8))

/******************* TIM Instances : Timer input XOR function *****************/
#define IS_TIM_XOR_INSTANCE(INSTANCE)   (((INSTANCE) == TIM1) || \
                                         ((INSTANCE) == TIM2) || \
                                         ((INSTANCE) == TIM3) || \
                                         ((INSTANCE) == TIM4) || \
                                         ((INSTANCE) == TIM5) || \
                                         ((INSTANCE) == TIM8))

/****************** TIM Instances : DMA requests generation (UDE) *************/
#define IS_TIM_DMA_INSTANCE(INSTANCE) (((INSTANCE) == TIM1) || \
                                       ((INSTANCE) == TIM2) || \
                                       ((INSTANCE) == TIM3) || \
                                       ((INSTANCE) == TIM4) || \
                                       ((INSTANCE) == TIM5) || \
                                       ((INSTANCE) == TIM6) || \
                                       ((INSTANCE) == TIM7) || \
                                       ((INSTANCE) == TIM8))

/************ TIM Instances : DMA requests generation (CCxDE) *****************/
#define IS_TIM_DMA_CC_INSTANCE(INSTANCE) (((INSTANCE) == TIM1) || \
                                          ((INSTANCE) == TIM2) || \
                                          ((INSTANCE) == TIM3) || \
                                          ((INSTANCE) == TIM4) || \
                                          ((INSTANCE) == TIM5) || \
                                          ((INSTANCE) == TIM8))

/************ TIM Instances : DMA requests generation (COMDE) *****************/
#define IS_TIM_CCDMA_INSTANCE(INSTANCE)  (((INSTANCE) == TIM1) || \
                                          ((INSTANCE) == TIM2) || \
                                          ((INSTANCE) == TIM3) || \
                                          ((INSTANCE) == TIM4) || \
                                          ((INSTANCE) == TIM5) || \
                                          ((INSTANCE) == TIM8))

/******************** TIM Instances : DMA burst feature ***********************/
#define IS_TIM_DMABURST_INSTANCE(INSTANCE)  (((INSTANCE) == TIM1) || \
                                             ((INSTANCE) == TIM2) || \
                                             ((INSTANCE) == TIM3) || \
                                             ((INSTANCE) == TIM4) || \
                                             ((INSTANCE) == TIM5) || \
                                             ((INSTANCE) == TIM8))

/****** TIM Instances : master mode available (TIMx_CR2.MMS available )********/
#define IS_TIM_MASTER_INSTANCE(INSTANCE) (((INSTANCE) == TIM1) || \
                                          ((INSTANCE) == TIM2) || \
                                          ((INSTANCE) == TIM3) || \
                                          ((INSTANCE) == TIM4) || \
                                          ((INSTANCE) == TIM5) || \
                                          ((INSTANCE) == TIM6) || \
                                          ((INSTANCE) == TIM7) || \
                                          ((INSTANCE) == TIM8) || \
                                          ((INSTANCE) == TIM9) || \
                                          ((INSTANCE) == TIM12))

/*********** TIM Instances : Slave mode available (TIMx_SMCR available )*******/
#define IS_TIM_SLAVE_INSTANCE(INSTANCE) (((INSTANCE) == TIM1) || \
                                         ((INSTANCE) == TIM2) || \
                                         ((INSTANCE) == TIM3) || \
                                         ((INSTANCE) == TIM4) || \
                                         ((INSTANCE) == TIM5) || \
                                         ((INSTANCE) == TIM8) || \
                                         ((INSTANCE) == TIM9) || \
                                         ((INSTANCE) == TIM12))

/********************** TIM Instances : 32 bit Counter ************************/
#define IS_TIM_32B_COUNTER_INSTANCE(INSTANCE)(((INSTANCE) == TIM2) || \
                                              ((INSTANCE) == TIM5))

/***************** TIM Instances : external trigger input availabe ************/
#define IS_TIM_ETR_INSTANCE(INSTANCE)  (((INSTANCE) == TIM1) || \
                                        ((INSTANCE) == TIM2) || \
                                        ((INSTANCE) == TIM3) || \
                                        ((INSTANCE) == TIM4) || \
                                        ((INSTANCE) == TIM5) || \
                                        ((INSTANCE) == TIM8))

/****************** TIM Instances : remapping capability **********************/
#define IS_TIM_REMAP_INSTANCE(INSTANCE) (((INSTANCE) == TIM2)  || \
                                         ((INSTANCE) == TIM5)  || \
                                         ((INSTANCE) == TIM11))

/******************* TIM Instances : output(s) available **********************/
#define IS_TIM_CCX_INSTANCE(INSTANCE, CHANNEL) \
    ((((INSTANCE) == TIM1) &&                  \
     (((CHANNEL) == TIM_CHANNEL_1) ||          \
      ((CHANNEL) == TIM_CHANNEL_2) ||          \
      ((CHANNEL) == TIM_CHANNEL_3) ||          \
      ((CHANNEL) == TIM_CHANNEL_4)))           \
    ||                                         \
    (((INSTANCE) == TIM2) &&                   \
     (((CHANNEL) == TIM_CHANNEL_1) ||          \
      ((CHANNEL) == TIM_CHANNEL_2) ||          \
      ((CHANNEL) == TIM_CHANNEL_3) ||          \
      ((CHANNEL) == TIM_CHANNEL_4)))           \
    ||                                         \
    (((INSTANCE) == TIM3) &&                   \
     (((CHANNEL) == TIM_CHANNEL_1) ||          \
      ((CHANNEL) == TIM_CHANNEL_2) ||          \
      ((CHANNEL) == TIM_CHANNEL_3) ||          \
      ((CHANNEL) == TIM_CHANNEL_4)))           \
    ||                                         \
    (((INSTANCE) == TIM4) &&                   \
     (((CHANNEL) == TIM_CHANNEL_1) ||          \
      ((CHANNEL) == TIM_CHANNEL_2) ||          \
      ((CHANNEL) == TIM_CHANNEL_3) ||          \
      ((CHANNEL) == TIM_CHANNEL_4)))           \
    ||                                         \
    (((INSTANCE) == TIM5) &&                   \
     (((CHANNEL) == TIM_CHANNEL_1) ||          \
      ((CHANNEL) == TIM_CHANNEL_2) ||          \
      ((CHANNEL) == TIM_CHANNEL_3) ||          \
      ((CHANNEL) == TIM_CHANNEL_4)))           \
    ||                                         \
    (((INSTANCE) == TIM8) &&                   \
     (((CHANNEL) == TIM_CHANNEL_1) ||          \
      ((CHANNEL) == TIM_CHANNEL_2) ||          \
      ((CHANNEL) == TIM_CHANNEL_3) ||          \
      ((CHANNEL) == TIM_CHANNEL_4)))           \
    ||                                         \
    (((INSTANCE) == TIM9) &&                   \
     (((CHANNEL) == TIM_CHANNEL_1) ||          \
      ((CHANNEL) == TIM_CHANNEL_2)))           \
    ||                                         \
    (((INSTANCE) == TIM10) &&                  \
     (((CHANNEL) == TIM_CHANNEL_1)))           \
    ||                                         \
    (((INSTANCE) == TIM11) &&                  \
     (((CHANNEL) == TIM_CHANNEL_1)))           \
    ||                                         \
    (((INSTANCE) == TIM12) &&                  \
     (((CHANNEL) == TIM_CHANNEL_1) ||          \
      ((CHANNEL) == TIM_CHANNEL_2)))           \
    ||                                         \
    (((INSTANCE) == TIM13) &&                  \
     (((CHANNEL) == TIM_CHANNEL_1)))           \
    ||                                         \
    (((INSTANCE) == TIM14) &&                  \
     (((CHANNEL) == TIM_CHANNEL_1))))

/************ TIM Instances : complementary output(s) available ***************/
#define IS_TIM_CCXN_INSTANCE(INSTANCE, CHANNEL) \
   ((((INSTANCE) == TIM1) &&                    \
     (((CHANNEL) == TIM_CHANNEL_1) ||           \
      ((CHANNEL) == TIM_CHANNEL_2) ||           \
      ((CHANNEL) == TIM_CHANNEL_3)))            \
    ||                                          \
    (((INSTANCE) == TIM8) &&                    \
     (((CHANNEL) == TIM_CHANNEL_1) ||           \
      ((CHANNEL) == TIM_CHANNEL_2) ||           \
      ((CHANNEL) == TIM_CHANNEL_3))))

/******************** USART Instances : Synchronous mode **********************/
#define IS_USART_INSTANCE(INSTANCE) (((INSTANCE) == USART1) || \
                                     ((INSTANCE) == USART2) || \
                                     ((INSTANCE) == USART3) || \
                                     ((INSTANCE) == USART6))

/******************** UART Instances : Asynchronous mode **********************/
#define IS_UART_INSTANCE(INSTANCE) (((INSTANCE) == USART1) || \
                                    ((INSTANCE) == USART2) || \
                                    ((INSTANCE) == USART3) || \
                                    ((INSTANCE) == UART4)  || \
                                    ((INSTANCE) == UART5)  || \
                                    ((INSTANCE) == USART6))

/****************** UART Instances : Hardware Flow control ********************/
#define IS_UART_HWFLOW_INSTANCE(INSTANCE) (((INSTANCE) == USART1) || \
                                           ((INSTANCE) == USART2) || \
                                           ((INSTANCE) == USART3) || \
                                           ((INSTANCE) == USART6))

/********************* UART Instances : Smard card mode ***********************/
#define IS_SMARTCARD_INSTANCE(INSTANCE) (((INSTANCE) == USART1) || \
                                         ((INSTANCE) == USART2) || \
                                         ((INSTANCE) == USART3) || \
                                         ((INSTANCE) == USART6))

/*********************** UART Instances : IRDA mode ***************************/
#define IS_IRDA_INSTANCE(INSTANCE) (((INSTANCE) == USART1) || \
                                    ((INSTANCE) == USART2) || \
                                    ((INSTANCE) == USART3) || \
                                    ((INSTANCE) == UART4)  || \
                                    ((INSTANCE) == UART5)  || \
                                    ((INSTANCE) == USART6))

/****************************** IWDG Instances ********************************/
#define IS_IWDG_ALL_INSTANCE(INSTANCE)  ((INSTANCE) == IWDG)

/****************************** WWDG Instances ********************************/
#define IS_WWDG_ALL_INSTANCE(INSTANCE)  ((INSTANCE) == WWDG)

#endif /* HAL_COMMON_H_ */
