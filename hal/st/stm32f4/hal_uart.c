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

#include "hal_uart.h"
#include "hal_time.h"

#define RECEIVE_BUFFER_SIZE_FRAMES  2
#define RECEIVE_BUFFER_SIZE_BYTES 512
// #define RECEIVE_BUFFER_SIZE_BYTES (MAX_RECEIVE_FRAME_SIZE * RECEIVE_BUFFER_SIZE_FRAMES)
#define endPos (RECEIVE_BUFFER_SIZE_BYTES -1)


typedef struct {
    uint_fast8_t receive_buffer[RECEIVE_BUFFER_SIZE_BYTES];
    volatile uint_fast16_t read_pos;
    volatile uint_fast16_t write_pos;
}uart_device_typ;


static uart_device_typ devices[MAX_UART + 1]; // +1 as MAX_UART is the highest index into this array


uint_fast8_t hal_uart_get_byte_at_offset(uint_fast8_t device, uint_fast16_t offset)
{
    uint_fast8_t res;
    uint_fast16_t target_pos = devices[device].read_pos + offset;
    if(endPos < target_pos)
    {
        target_pos = target_pos - endPos;
    }
    res = devices[device].receive_buffer[target_pos];
    return res;
}

uint_fast16_t hal_uart_get_available_bytes(uint_fast8_t device)
{
    uint_fast16_t res = 0;
    if(devices[device].read_pos != devices[device].write_pos)
    {
        if(devices[device].write_pos > devices[device].read_pos)
        {
            res = devices[device].write_pos - devices[device].read_pos;
        }
        else
        {
            res = (endPos + 1) - devices[device].read_pos + (0 - devices[device].write_pos);
        }
    }
    // else res = 0;
    return res;
}

void hal_uart_forget_bytes(uint_fast8_t device, uint_fast16_t how_many)
{
    uint_fast16_t target_pos = devices[device].read_pos + how_many;
    if(endPos < target_pos)
    {
        target_pos = target_pos - endPos;
    }
    devices[device].read_pos = target_pos;
}

void hal_uart_send_frame(uint_fast8_t device, uint8_t * frame, uint_fast16_t length)
{
    /*
    while(HAL_BUSY == HAL_USART_Transmit_IT(&huart1, frame, length))
    {
        hal_time_ms_sleep(1);
    }
    */
}


bool hal_uart_init(uint_fast8_t device)
{
    devices[device].read_pos = 0;
    devices[device].write_pos = 0;
    /*
    huart1.Instance = USART1;
    huart1.Init.BaudRate = 115200;
    huart1.Init.WordLength = UART_WORDLENGTH_8B;
    huart1.Init.StopBits = UART_STOPBITS_1;
    huart1.Init.Parity = UART_PARITY_NONE;
    huart1.Init.Mode = UART_MODE_TX_RX;
    */
    // huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    // huart1.Init.OverSampling = UART_OVERSAMPLING_16;
    // HAL_USART_Init(&huart1);

    /* Enable the USART Data Register not empty Interrupt */
    // __USART_ENABLE_IT(&huart1, USART_IT_RXNE);

    return true;
}
/*
void HAL_UART_MspInit(UART_HandleTypeDef* huart)
{
  // GPIO_InitTypeDef GPIO_InitStruct;
  // if(huart->Instance==USART1)
  {
    // Peripheral clock enable
    // __USART1_CLK_ENABLE();

    //USART1 GPIO Configuration
    PA9     ------> USART1_TX
    PA10     ------> USART1_RX

    GPIO_InitStruct.Pin = GPIO_PIN_9|GPIO_PIN_10;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF7_USART1;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
  }
}

void HAL_UART_MspDeInit(UART_HandleTypeDef* huart)
{
  if(huart->Instance==USART1)
  {
    // Peripheral clock disable
    __USART1_CLK_DISABLE();

    //USART1 GPIO Configuration
    PA9     ------> USART1_TX
    PA10     ------> USART1_RX
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_9|GPIO_PIN_10);
  }
}

// this is the interrupt request handler (IRQ) for ALL USART1 interrupts
void USART1_IRQHandler(void)
{

    // check if the USART1 receive interrupt flag was set
    if( USART_GetITStatus(USART1, USART_IT_RXNE) )
    {
        receive_buffer[write_pos] = USART1->DR; // the byte from the USART1 data register is saved
        write_pos++;
    }
}
*/

