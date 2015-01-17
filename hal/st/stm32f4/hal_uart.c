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
#include "rcc.h"
#include "hal_cfg.h"
#include "board_cfg.h"
#include "usart.h"
#include "gpio.h"

#define RECEIVE_BUFFER_SIZE_BYTES  512
#define endPos                     (RECEIVE_BUFFER_SIZE_BYTES -1)
// Baudrate is 115200 so a byte should transfere in less than one ms
#define UART_BYTE_TIMEOUT_MS      5


typedef struct {
    uint_fast8_t receive_buffer[RECEIVE_BUFFER_SIZE_BYTES];
    volatile uint_fast16_t read_pos;
    volatile uint_fast16_t write_pos;
    USART_TypeDef * port;
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
    uint_fast16_t bytesSend = 0;
    while(bytesSend < length)
    {
        uint32_t timeout = hal_time_get_ms_tick() + UART_BYTE_TIMEOUT_MS;
        if(timeout < hal_time_get_ms_tick())
        {
            // wrap around
            // wait for TX to be ready for the next Byte
            while(   (timeout < hal_time_get_ms_tick())
                  && (USART_SR_TXE != (devices[device].port->SR & USART_SR_TXE)) )
            {
                ; // wait
            }
        }
        // wait for TX to be ready for the next Byte
        while(   (timeout > hal_time_get_ms_tick())
              && (USART_SR_TXE != (devices[device].port->SR & USART_SR_TXE)) )
        {
            ; // wait
        }
        // TODO check if we had a timeout or not
        // send one Byte
        devices[device].port->DR = frame[bytesSend];
        bytesSend++;
    }


    /*
    // DMA:
    // ====
    // TC flag goes high after last byte has been send

    // page 319 of Ref manual
    // 1 check that EN bit is 0 else wait for it
    // 1.1 clear all statis bits in DMA_LISR and DMA_HISR
    // 2 write DMA_SxPAR
    // 3 write DMA_SxMAOR
    // 4 write num bytes to DMA_SxNDTR
    // 5 select channel n DMA_SxCR , flow contoller, stream priority
    // 8 cfg fifo
    // 9 cfg direction
    // activate En in SxCR


    // page 986 of Ref Manual:

    // 1. USART_DR is destination
    // 2. configure DMA source
    // 3. cfg num bytes
    // 4. cfg dma channel prio
    // 5. cfg dma irq
    // 6. clear TC bin in SR Register by writing it 0
    // 7. activate dma channel in register


    // 6
    switch(device)
    {
    case 0 :
        UART_0->SR &= ~USART_SR_TC;  // TODO
        break;
    case 1:
        UART_1->SR &= ~USART_SR_TC;  // TODO
        break;
    default:
        // invalid Device
        return;
    }
    */
}

void UART_0_IRQ_HANDLER(void)
{
    if(USART_SR_RXNE == (USART_SR_RXNE && UART_0->SR))
    {
        uint8_t received_byte = (UART_0->DR & 0xff);
        devices[0].receive_buffer[devices[0].write_pos] = received_byte;
        devices[0].write_pos ++;
        if(RECEIVE_BUFFER_SIZE_BYTES ==devices[0]. write_pos)
        {
            devices[0].write_pos = 0;
        }
    }
}

void UART_1_IRQ_HANDLER(void)
{
    if(USART_SR_RXNE == (USART_SR_RXNE && UART_1->SR))
    {
        uint8_t received_byte = (UART_1->DR & 0xff);
        devices[1].receive_buffer[devices[1].write_pos] = received_byte;
        devices[1].write_pos ++;
        if(RECEIVE_BUFFER_SIZE_BYTES == devices[1].write_pos)
        {
            devices[1]. write_pos = 0;
        }
    }
}

bool hal_uart_init(uint_fast8_t device)
{
    devices[device].read_pos = 0;
    devices[device].write_pos = 0;

    switch(device)
    {
    case 0 :
        // enable clock for GPIO Port
        RCC->AHB1ENR |= UART_0_RX_GPIO_PORT_RCC;
        RCC->AHB1ENR |= UART_0_TX_GPIO_PORT_RCC;
        // enable clock for interface
        RCC->APB1ENR |= UART_0_APB1ENR;
        RCC->APB2ENR |= UART_0_APB2ENR;
        // configure Pins
        // RX
        UART_0_RX_GPIO_PORT->MODER   |=  UART_0_RX_GPIO_MODER_1;
        UART_0_RX_GPIO_PORT->MODER   &= ~UART_0_RX_GPIO_MODER_0;
        UART_0_RX_GPIO_PORT->OTYPER  |=  UART_0_RX_GPIO_OTYPER_1;
        UART_0_RX_GPIO_PORT->OTYPER  &= ~UART_0_RX_GPIO_OTYPER_0;
        UART_0_RX_GPIO_PORT->OSPEEDR |=  UART_0_RX_GPIO_OSPEEDR_1;
        UART_0_RX_GPIO_PORT->OSPEEDR &= ~UART_0_RX_GPIO_OSPEEDR_0;
        UART_0_RX_GPIO_PORT->PUPDR   |=  UART_0_RX_GPIO_PUPD_1;
        UART_0_RX_GPIO_PORT->PUPDR   &= ~UART_0_RX_GPIO_PUPD_0;
        UART_0_RX_GPIO_PORT->AFR[0]  |=  UART_0_RX_GPIO_AFR_0_1;
        UART_0_RX_GPIO_PORT->AFR[0]  &= ~UART_0_RX_GPIO_AFR_0_0;
        UART_0_RX_GPIO_PORT->AFR[1]  |=  UART_0_RX_GPIO_AFR_1_1;
        UART_0_RX_GPIO_PORT->AFR[1]  &= ~UART_0_RX_GPIO_AFR_1_0;
        // TX
        UART_0_TX_GPIO_PORT->MODER   |=  UART_0_TX_GPIO_MODER_1;
        UART_0_TX_GPIO_PORT->MODER   &= ~UART_0_TX_GPIO_MODER_0;
        UART_0_TX_GPIO_PORT->OTYPER  |=  UART_0_TX_GPIO_OTYPER_1;
        UART_0_TX_GPIO_PORT->OTYPER  &= ~UART_0_TX_GPIO_OTYPER_0;
        UART_0_TX_GPIO_PORT->OSPEEDR |=  UART_0_TX_GPIO_OSPEEDR_1;
        UART_0_TX_GPIO_PORT->OSPEEDR &= ~UART_0_TX_GPIO_OSPEEDR_0;
        UART_0_TX_GPIO_PORT->PUPDR   |=  UART_0_TX_GPIO_PUPD_1;
        UART_0_TX_GPIO_PORT->PUPDR   &= ~UART_0_TX_GPIO_PUPD_0;
        UART_0_TX_GPIO_PORT->AFR[0]  |=  UART_0_TX_GPIO_AFR_0_1;
        UART_0_TX_GPIO_PORT->AFR[0]  &= ~UART_0_TX_GPIO_AFR_0_0;
        UART_0_TX_GPIO_PORT->AFR[1]  |=  UART_0_TX_GPIO_AFR_1_1;
        UART_0_TX_GPIO_PORT->AFR[1]  &= ~UART_0_TX_GPIO_AFR_1_0;
        // configure UART parameters
        UART_0->BRR = UART_0_BRR;  // baud rate
        // 16x oversampling,  no parity, 1 stop bit, 8 data bits, only Receive interrupts
        //UART_0->CR1 = 0x0000202c;
        UART_0->CR1 = 0x00002008;
        // not Lin, No SPI, address = 0
        UART_0->CR2 = 0x00000000;
        //  No CTS, RTS, no DMA, no Smartcard, full duplex, no IRDA
        UART_0->CR3 = 0x00000000;
        devices[device].port = UART_0;
        break;

    case 1:
        // enable clock for GPIO Port
        RCC->AHB1ENR |= UART_1_RX_GPIO_PORT_RCC;
        RCC->AHB1ENR |= UART_1_TX_GPIO_PORT_RCC;
        // enable clock for interface
        RCC->APB1ENR |= UART_1_APB1ENR;
        RCC->APB2ENR |= UART_1_APB2ENR;
        // configure Pins
        // RX
        UART_1_RX_GPIO_PORT->MODER   |=  UART_1_RX_GPIO_MODER_1;
        UART_1_RX_GPIO_PORT->MODER   &= ~UART_1_RX_GPIO_MODER_0;
        UART_1_RX_GPIO_PORT->OTYPER  |=  UART_0_RX_GPIO_OTYPER_1;
        UART_1_RX_GPIO_PORT->OTYPER  &= ~UART_0_RX_GPIO_OTYPER_0;
        UART_1_RX_GPIO_PORT->OSPEEDR |=  UART_1_RX_GPIO_OSPEEDR_1;
        UART_1_RX_GPIO_PORT->OSPEEDR &= ~UART_1_RX_GPIO_OSPEEDR_0;
        UART_1_RX_GPIO_PORT->PUPDR   |=  UART_0_RX_GPIO_PUPD_1;
        UART_1_RX_GPIO_PORT->PUPDR   &= ~UART_0_RX_GPIO_PUPD_0;
        UART_1_RX_GPIO_PORT->AFR[0]  |=  UART_1_RX_GPIO_AFR_0_1;
        UART_1_RX_GPIO_PORT->AFR[0]  &= ~UART_1_RX_GPIO_AFR_0_0;
        UART_1_RX_GPIO_PORT->AFR[1]  |=  UART_1_RX_GPIO_AFR_1_1;
        UART_1_RX_GPIO_PORT->AFR[1]  &= ~UART_1_RX_GPIO_AFR_1_0;
        // TX
        UART_1_TX_GPIO_PORT->MODER   |=  UART_1_TX_GPIO_MODER_1;
        UART_1_TX_GPIO_PORT->MODER   &= ~UART_1_TX_GPIO_MODER_0;
        UART_1_TX_GPIO_PORT->OTYPER  |=  UART_0_TX_GPIO_OTYPER_1;
        UART_1_TX_GPIO_PORT->OTYPER  &= ~UART_0_TX_GPIO_OTYPER_0;
        UART_1_TX_GPIO_PORT->OSPEEDR |=  UART_1_TX_GPIO_OSPEEDR_1;
        UART_1_TX_GPIO_PORT->OSPEEDR &= ~UART_1_TX_GPIO_OSPEEDR_0;
        UART_1_TX_GPIO_PORT->PUPDR   |=  UART_0_TX_GPIO_PUPD_1;
        UART_1_TX_GPIO_PORT->PUPDR   &= ~UART_0_TX_GPIO_PUPD_0;
        UART_1_TX_GPIO_PORT->AFR[0]  |=  UART_1_TX_GPIO_AFR_0_1;
        UART_1_TX_GPIO_PORT->AFR[0]  &= ~UART_1_TX_GPIO_AFR_0_0;
        UART_1_TX_GPIO_PORT->AFR[1]  |=  UART_1_TX_GPIO_AFR_1_1;
        UART_1_TX_GPIO_PORT->AFR[1]  &= ~UART_1_TX_GPIO_AFR_1_0;
        // configure UART parameters
        UART_1->BRR = UART_0_BRR;  // baud rate
        // 16x oversampling,  no parity, 1 stop bit, 8 data bits, only Receive interrupts
        UART_1->CR1 = 0x0000202c;
        // not Lin, No SPI, address = 0
        UART_1->CR2 = 0x00000000;
        //  No CTS, RTS, no DMA, no Smartcard, full duplex, no IRDA
        UART_1->CR3 = 0x00000000;
        devices[device].port = UART_1;
        break;

    default:
        // specified an unavailable Interface
        return false;
    }
    return true;
}

