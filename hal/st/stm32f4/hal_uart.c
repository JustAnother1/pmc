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

#include <stddef.h>
#include <stdlib.h>
#include "hal_uart.h"
#include "hal_time.h"
#include "rcc.h"
#include "hal_cfg.h"
#include "board_cfg.h"
#include "usart.h"
#include "gpio.h"
#include "util.h"
#include "hal_debug.h"
#include "hal_common.h"
#include "hal_led.h"

// Baudrate is 115200 so a byte should transfere in less than one ms
#define UART_BYTE_TIMEOUT_MS      5


typedef struct {
    // receive
    uint8_t       * receive_buffer;
    uint_fast16_t   read_pos;
    uint_fast16_t   write_pos;
    uint_fast16_t   size_receive_buffer;
    // send
    uint8_t       * send_buffer;
    bool            is_sending;
    uint_fast16_t   send_pos; // the byte that will be send next
    uint_fast16_t   send_end_pos; // free slot after data to send
    uint_fast16_t   size_send_buffer;
    // the port
    USART_TypeDef * port;
}uart_device_typ;

static void device_IRQ_handler(uint_fast8_t device);
static void make_sure_that_we_can_send(uint_fast8_t device);
static void copy_data_to_send(uint_fast8_t device, uint8_t * frame, uint_fast16_t length);

static volatile uart_device_typ devices[MAX_UART];


uint_fast8_t hal_uart_get_byte_at_offset(uint_fast8_t device, uint_fast16_t offset)
{
    if(device < MAX_UART)
    {
        uint_fast8_t res;
        uint_fast16_t target_pos = devices[device].read_pos + offset;
        if((devices[device].size_receive_buffer -1) < target_pos)
        {
            target_pos = target_pos - (devices[device].size_receive_buffer -1);
        }
        res = devices[device].receive_buffer[target_pos];
        return res;
    }
    else
    {
        // invalid Interface Specified
        return ' ';
    }
}

uint_fast16_t hal_uart_get_available_bytes(uint_fast8_t device)
{
    if(device < MAX_UART)
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
                res = devices[device].size_receive_buffer - devices[device].read_pos + (0 - devices[device].write_pos);
            }
        }
        // else res = 0;
        return res;
    }
    else
    {
        // invalid Interface Specified
        return 0;
    }
}

void hal_uart_forget_bytes(uint_fast8_t device, uint_fast16_t how_many)
{
    if(device < MAX_UART)
    {
        uint_fast16_t target_pos = devices[device].read_pos + how_many;
        if((devices[device].size_receive_buffer -1) < target_pos)
        {
            target_pos = target_pos - (devices[device].size_receive_buffer -1);
        }
        devices[device].read_pos = target_pos;
    }
    // else invalid Interface Specified
}

void hal_uart_send_frame(uint_fast8_t device, uint8_t * frame, uint_fast16_t length)
{
    if(device < MAX_UART)
    {
        if(1 > length)
        {
            // no data
            return;
        }
        // TODO add timeout 1ms for each byte of the send buffer size
        while(true == devices[device].is_sending)
        {
            ; // wait
        }
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
            // send one Byte
            devices[device].port->DR = (uint16_t)frame[bytesSend];
            bytesSend++;
        }
    }
    // else invalid Interface Specified
}

static void make_sure_that_we_can_send(uint_fast8_t device)
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
}

// TODO non blocking using DMA
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

static void copy_data_to_send(uint_fast8_t device, uint8_t * frame, uint_fast16_t length)
{
    // TODO simplify
    if(devices[device].send_end_pos < devices[device].send_pos)
    {
        // buffer already wrapped
        if(length < (devices[device].send_pos - devices[device].send_end_pos))
        {
            // but this still fits in
            // copy data
            uint_fast16_t i;
            for(i = 0; i < length; i++)
            {
                devices[device].send_buffer[i+ devices[device].send_end_pos] = frame[i];
            }
            devices[device].send_end_pos = devices[device].send_end_pos + length;
        }
        else
        {
            // already wrapped and not fitting -> blocking
            hal_uart_send_frame(device, frame, length);
            return;
        }
    }
    else
    {
        // not wrapped yet
        if(length < (devices[device].size_send_buffer - devices[device].send_end_pos))
        {
            // and we can put it in without wrapping
            // copy data
            uint_fast16_t i;
            for(i = 0; i < length; i++)
            {
                devices[device].send_buffer[i + devices[device].send_end_pos] = frame[i];
            }
            devices[device].send_end_pos = devices[device].send_end_pos + length;
        }
        else
        {
            uint_fast16_t new_end_position;
            new_end_position = length - (devices[device].size_send_buffer - devices[device].send_end_pos);
            if(new_end_position < devices[device].send_pos)
            {
                // it fits wrapped
                // copy data
                uint_fast16_t data_before_wrap = length - new_end_position;
                uint_fast16_t i;
                for(i = 0; i < data_before_wrap; i++)
                {
                    devices[device].send_buffer[i + devices[device].send_end_pos] = frame[i];
                }
                for(; i < length; i++)
                {
                    devices[device].send_buffer[i - data_before_wrap] = frame[i];
                }
                devices[device].send_end_pos = new_end_position;
            }
            else
            {
                // doesn't fit -> blocking !
                hal_uart_send_frame(device, frame, length);
                return;
            }
        }
    }
}

void hal_uart_send_frame_non_blocking(uint_fast8_t device, uint8_t * frame, uint_fast16_t length)
{
    if( !(device < MAX_UART) || (1 > length) )
    {
        // invalid Interface Specified or
        // no data
        return;
    }

    if(NULL == devices[device].send_buffer)
    {
        // send_buffer is generated in hal_uart_init() !
        // So this only happens if sending on a Device that has not been initialized.
        devices[device].is_sending = false; // for safety
        devices[device].port->CR1 &= ~USART_CR1_TXEIE;
        // no sendbuffer -> blocking
        hal_uart_send_frame(device, frame, length);
        return;
    }

    // can we copy the data ?
    copy_data_to_send(device, frame, length);

    // are we already sending or do we need to start sending
    if(true == devices[device].is_sending)
    {
        // nothing to do here
    }
    else
    {
        // start sending now
        make_sure_that_we_can_send(device);
        devices[device].is_sending = true;
        devices[device].port->DR = (uint16_t)devices[device].send_buffer[devices[device].send_pos];
        devices[device].send_pos++;
        if(devices[device].send_pos < devices[device].size_send_buffer)
        {
            // ok
        }
        else
        {
            // wrap around
            devices[device].send_pos = 0;
        }
        // The next line activates the Interrupt. The Interrupt may become
        // active immediately. It therefore must be the last line !
        devices[device].port->CR1 |= USART_CR1_TXEIE;
    }
}

static void device_IRQ_handler(uint_fast8_t device)
{
    if(USART_SR_RXNE == (USART_SR_RXNE & devices[device].port->SR))
    {
        uint32_t reg = devices[device].port->DR;
        uint8_t received_byte = (reg & 0xff);
        devices[device].port->SR &= ~USART_SR_RXNE;
        devices[device].receive_buffer[devices[device].write_pos] = received_byte;
        devices[device].write_pos ++;
        if(devices[device].size_receive_buffer == devices[device]. write_pos)
        {
            devices[device].write_pos = 0;
        }
    }
    if(USART_SR_TXE == (USART_SR_TXE & devices[device].port->SR))
    {
        if(true == devices[device].is_sending)
        {
            devices[device].port->DR = (uint16_t)devices[device].send_buffer[devices[device].send_pos];
            devices[device].send_pos++;
            if(devices[device].send_pos < devices[device].size_send_buffer)
            {
                // ok
            }
            else
            {
                // wrap around
                devices[device].send_pos = 0;
            }
            if(devices[device].send_pos == devices[device].send_end_pos)
            {
                devices[device].is_sending = false;
                devices[device].port->CR1 &= ~USART_CR1_TXEIE;
            }
        }
        // else nothing to do;
        devices[device].port->SR &= ~USART_SR_TXE;
    }
}

void UART_0_IRQ_HANDLER(void)
{
    device_IRQ_handler(0);
}

void UART_1_IRQ_HANDLER(void)
{
    device_IRQ_handler(1);
}

void hal_uart_print_configuration(uint_fast8_t device)
{
    if(device < MAX_UART)
    {
        debug_line("Configuration of UART_%d :", device);
        // Clock
        debug_line("RCC->AHB1ENR  = 0x%08x", RCC->AHB1ENR);
        debug_line("RCC->APB1ENR  = 0x%08x", RCC->APB1ENR);
        debug_line("RCC->APB2ENR  = 0x%08x", RCC->APB2ENR);
        // Uart
        debug_line("UART->BRR     = 0x%08x", devices[device].port->BRR);
        debug_line("UART->CR1     = 0x%08x", devices[device].port->CR1);
        debug_line("UART->CR2     = 0x%08x", devices[device].port->CR2);
        debug_line("UART->CR3     = 0x%08x", devices[device].port->CR3);
        debug_line("UART->DR      = 0x%08x", devices[device].port->DR);
        debug_line("UART->GTPR    = 0x%08x", devices[device].port->GTPR);
        debug_line("UART->SR      = 0x%08x", devices[device].port->SR);
        // GPIO
        switch(device)
        {
        case 0 :
            debug_line("RX Pin:");
            print_gpio_configuration(UART_0_RX_GPIO_PORT);
            debug_line("TX Pin:");
            print_gpio_configuration(UART_0_TX_GPIO_PORT);
            break;

        case 1 :
            debug_line("RX Pin:");
            print_gpio_configuration(UART_1_RX_GPIO_PORT);
            debug_line("TX Pin:");
            print_gpio_configuration(UART_1_TX_GPIO_PORT);
            break;

        default:
            break;
        }
    }
    // else invalid Interface Specified
}

bool hal_uart_init(uint_fast8_t device, uint_fast16_t rec_buf_size, uint_fast16_t send_buf_size)
{
    if(device < MAX_UART)
    {
        uint8_t * mal_hlp;
        devices[device].read_pos = 0;
        devices[device].write_pos = 0;
        devices[device].is_sending = false;
        // create receive Buffer
        if(1 > rec_buf_size)
        {
            // sending only on this interface ?
            devices[device].receive_buffer = NULL;
        }
        else
        {
            mal_hlp = (uint8_t *) malloc(rec_buf_size);
            if(NULL == mal_hlp)
            {
                return false;
            }
            devices[device].receive_buffer = mal_hlp;
        }
        // create send Buffer
        if(1 > send_buf_size)
        {
            // nothing to send ?
            devices[device].send_buffer = NULL;
        }
        else
        {
            mal_hlp = (uint8_t *) malloc(send_buf_size);
            if(NULL == mal_hlp)
            {
                return false;
            }
            devices[device].send_buffer = mal_hlp;
        }
        devices[device].size_receive_buffer = rec_buf_size;
        devices[device].size_send_buffer = send_buf_size;
        devices[device].send_end_pos = 0;
        devices[device].send_pos = 0;

        switch(device)
        {
        case 0 :
            // enable clock for GPIO Port
            RCC->AHB1ENR |= UART_0_RX_GPIO_PORT_RCC;
            RCC->AHB1ENR |= UART_0_TX_GPIO_PORT_RCC;
            // enable clock for interface
            RCC->APB1ENR |= UART_0_APB1ENR;
            RCC->APB2ENR |= UART_0_APB2ENR;

            // configure UART parameters
            // enable UART
            UART_0->CR1 = 0x00002000;
            // baud rate
            UART_0->BRR = UART_0_BRR;
            // 16x oversampling,  no parity, 1 stop bit, 8 data bits, only Receive interrupts
            //UART_0->CR1 = 0x0000202c;
            UART_0->CR1 = 0x00002008;
            // not Lin, No SPI, address = 0
            UART_0->CR2 = 0x00000000;
            //  No CTS, RTS, no DMA, no Smartcard, full duplex, no IRDA
            UART_0->CR3 = 0x00000000;
            // enable RX and TX
            UART_0->CR1 = 0x0000202c;
            // Enable Receive Interrupt
            NVIC_SetPriority(UART_0_IRQ_NUMBER, UART_0_IRQ_PRIORITY);
            NVIC_EnableIRQ(UART_0_IRQ_NUMBER);
            // configure Pins
            // RX
            UART_0_RX_GPIO_PORT->MODER   |=  UART_0_RX_GPIO_MODER_1;
            UART_0_RX_GPIO_PORT->MODER   &= ~UART_0_RX_GPIO_MODER_0;
            UART_0_RX_GPIO_PORT->AFR[0]  |=  UART_0_RX_GPIO_AFR_0_1;
            UART_0_RX_GPIO_PORT->AFR[0]  &= ~UART_0_RX_GPIO_AFR_0_0;
            UART_0_RX_GPIO_PORT->AFR[1]  |=  UART_0_RX_GPIO_AFR_1_1;
            UART_0_RX_GPIO_PORT->AFR[1]  &= ~UART_0_RX_GPIO_AFR_1_0;
            UART_0_RX_GPIO_PORT->OTYPER  |=  UART_0_RX_GPIO_OTYPER_1;
            UART_0_RX_GPIO_PORT->OTYPER  &= ~UART_0_RX_GPIO_OTYPER_0;
            UART_0_RX_GPIO_PORT->PUPDR   |=  UART_0_RX_GPIO_PUPD_1;
            UART_0_RX_GPIO_PORT->PUPDR   &= ~UART_0_RX_GPIO_PUPD_0;
            // TX
            UART_0_TX_GPIO_PORT->MODER   |=  UART_0_TX_GPIO_MODER_1;
            UART_0_TX_GPIO_PORT->MODER   &= ~UART_0_TX_GPIO_MODER_0;
            UART_0_TX_GPIO_PORT->AFR[0]  |=  UART_0_TX_GPIO_AFR_0_1;
            UART_0_TX_GPIO_PORT->AFR[0]  &= ~UART_0_TX_GPIO_AFR_0_0;
            UART_0_TX_GPIO_PORT->AFR[1]  |=  UART_0_TX_GPIO_AFR_1_1;
            UART_0_TX_GPIO_PORT->AFR[1]  &= ~UART_0_TX_GPIO_AFR_1_0;
            UART_0_TX_GPIO_PORT->OTYPER  |=  UART_0_TX_GPIO_OTYPER_1;
            UART_0_TX_GPIO_PORT->OTYPER  &= ~UART_0_TX_GPIO_OTYPER_0;
            UART_0_TX_GPIO_PORT->PUPDR   |=  UART_0_TX_GPIO_PUPD_1;
            UART_0_TX_GPIO_PORT->PUPDR   &= ~UART_0_TX_GPIO_PUPD_0;

            // configure Pins
            // RX
            UART_0_RX_GPIO_PORT->OSPEEDR |=  UART_0_RX_GPIO_OSPEEDR_1;
            UART_0_RX_GPIO_PORT->OSPEEDR &= ~UART_0_RX_GPIO_OSPEEDR_0;
            // TX
            UART_0_TX_GPIO_PORT->OSPEEDR |=  UART_0_TX_GPIO_OSPEEDR_1;
            UART_0_TX_GPIO_PORT->OSPEEDR &= ~UART_0_TX_GPIO_OSPEEDR_0;

            devices[device].port = UART_0;
            break;

        case 1:
            // enable clock for GPIO Port
            RCC->AHB1ENR |= UART_1_RX_GPIO_PORT_RCC;
            RCC->AHB1ENR |= UART_1_TX_GPIO_PORT_RCC;
            // enable clock for interface
            RCC->APB1ENR |= UART_1_APB1ENR;
            RCC->APB2ENR |= UART_1_APB2ENR;

            // configure UART parameters
            // enable UART
            UART_1->CR1 = 0x00002000;
            // baud rate
            UART_1->BRR = UART_0_BRR;
            // 16x oversampling,  no parity, 1 stop bit, 8 data bits, only Receive interrupts
            UART_1->CR1 = 0x00002020;
            // not Lin, No SPI, address = 0
            UART_1->CR2 = 0x00000000;
            //  No CTS, RTS, no DMA, no Smartcard, full duplex, no IRDA
            UART_1->CR3 = 0x00000000;
            // enable RX and TX
            UART_1->CR1 = 0x0000202c;
            // Enable Receive Interrupt
            NVIC_SetPriority(UART_1_IRQ_NUMBER, UART_1_IRQ_PRIORITY);
            NVIC_EnableIRQ(UART_1_IRQ_NUMBER);
            // configure Pins
            // RX
            UART_1_RX_GPIO_PORT->MODER   |=  UART_1_RX_GPIO_MODER_1;
            UART_1_RX_GPIO_PORT->MODER   &= ~UART_1_RX_GPIO_MODER_0;
            UART_1_RX_GPIO_PORT->AFR[0]  |=  UART_1_RX_GPIO_AFR_0_1;
            UART_1_RX_GPIO_PORT->AFR[0]  &= ~UART_1_RX_GPIO_AFR_0_0;
            UART_1_RX_GPIO_PORT->AFR[1]  |=  UART_1_RX_GPIO_AFR_1_1;
            UART_1_RX_GPIO_PORT->AFR[1]  &= ~UART_1_RX_GPIO_AFR_1_0;
            // TX
            UART_1_TX_GPIO_PORT->MODER   |=  UART_1_TX_GPIO_MODER_1;
            UART_1_TX_GPIO_PORT->MODER   &= ~UART_1_TX_GPIO_MODER_0;
            UART_1_TX_GPIO_PORT->AFR[0]  |=  UART_1_TX_GPIO_AFR_0_1;
            UART_1_TX_GPIO_PORT->AFR[0]  &= ~UART_1_TX_GPIO_AFR_0_0;
            UART_1_TX_GPIO_PORT->AFR[1]  |=  UART_1_TX_GPIO_AFR_1_1;
            UART_1_TX_GPIO_PORT->AFR[1]  &= ~UART_1_TX_GPIO_AFR_1_0;

            // configure Pins
            // RX
            UART_1_RX_GPIO_PORT->OTYPER  |=  UART_1_RX_GPIO_OTYPER_1;
            UART_1_RX_GPIO_PORT->OTYPER  &= ~UART_1_RX_GPIO_OTYPER_0;
            UART_1_RX_GPIO_PORT->OSPEEDR |=  UART_1_RX_GPIO_OSPEEDR_1;
            UART_1_RX_GPIO_PORT->OSPEEDR &= ~UART_1_RX_GPIO_OSPEEDR_0;
            UART_1_RX_GPIO_PORT->PUPDR   |=  UART_1_RX_GPIO_PUPD_1;
            UART_1_RX_GPIO_PORT->PUPDR   &= ~UART_1_RX_GPIO_PUPD_0;
            // TX
            UART_1_TX_GPIO_PORT->OTYPER  |=  UART_1_TX_GPIO_OTYPER_1;
            UART_1_TX_GPIO_PORT->OTYPER  &= ~UART_1_TX_GPIO_OTYPER_0;
            UART_1_TX_GPIO_PORT->OSPEEDR |=  UART_1_TX_GPIO_OSPEEDR_1;
            UART_1_TX_GPIO_PORT->OSPEEDR &= ~UART_1_TX_GPIO_OSPEEDR_0;
            UART_1_TX_GPIO_PORT->PUPDR   |=  UART_1_TX_GPIO_PUPD_1;
            UART_1_TX_GPIO_PORT->PUPDR   &= ~UART_1_TX_GPIO_PUPD_0;
            devices[device].port = UART_1;
            break;

        default:
            // specified an unavailable Interface
            return false;
        }
        return true;
    }
    else
    {
        // invalid Interface Specified
        return false;
    }
}

