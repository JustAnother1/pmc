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

#include <stdio.h>
#include "board_cfg.h"
#include "hal_cfg.h"
#include "hal_cpu.h"
#include "hal_debug.h"
#include "hal_led.h"
#include "hal_time.h"
#include "hal_uart.h"
#include <st_common.h>
#include <st_gpio.h>
#include <st_rcc.h>
#include <st_usart.h>
#include <st_util.h>

// Baudrate is 115200 so a byte should transfer in less than one ms
#define UART_BYTE_TIMEOUT_MS      5

typedef struct {
    // receive
    uint8_t         receive_buffer[UART_RECEIVE_BUFFER_SIZE];
    uint_fast16_t   receive_read_pos;
    uint_fast16_t   receive_write_pos;
    // send
    uint8_t         send_buffer[UART_SEND_BUFFER_SIZE];
    bool            is_sending;
    uint_fast16_t   send_read_pos; // the byte that will be send next
    uint_fast16_t   send_write_pos; // free slot after data to send
    // the port
    USART_TypeDef * port;
}uart_device_typ;

void UART_0_IRQ_HANDLER(void) __attribute__ ((interrupt ("IRQ")));
void UART_1_IRQ_HANDLER(void) __attribute__ ((interrupt ("IRQ")));

static void device_IRQ_handler(uint_fast8_t device);
static bool copy_data_to_send(uint_fast8_t device, uint8_t * frame, uint_fast16_t length);
static bool hal_uart_init(uint_fast8_t device);
#ifdef DEBUG_ACTIVE
static void hal_uart_print_configuration(uint_fast8_t device);
#endif
static uint_fast8_t hal_uart_get_byte_at_offset(uint_fast8_t device, uint_fast16_t offset);
static uint_fast16_t hal_uart_get_available_bytes(uint_fast8_t device);
static void hal_uart_forget_bytes(uint_fast8_t device, uint_fast16_t how_many);
static void hal_uart_send_frame(uint_fast8_t device, uint8_t * frame, uint_fast16_t length);
static bool hal_uart_send_frame_non_blocking(uint_fast8_t device, uint8_t * frame, uint_fast16_t length);
static uint_fast16_t get_available_bytes_in_send_Buffer(uint_fast8_t device);

static volatile uart_device_typ devices[MAX_UART];
static bool gcode_initialized = false;
static bool debug_initialized = false;

// Implementation of hal_uart_api


bool hal_init_gcode_uart(void)
{
    if(true == gcode_initialized)
    {
        // initialize only once !
        return false;
    }
    gcode_initialized = true;

    return hal_uart_init(GCODE_UART);
}

#ifdef DEBUG_ACTIVE
void hal_print_configuration_gcode_uart(void)
{
    hal_uart_print_configuration(GCODE_UART);
}
#endif

uint_fast8_t hal_get_gcode_uart_byte_at_offset(uint_fast16_t offset)
{
    return hal_uart_get_byte_at_offset(GCODE_UART, offset);
}

uint_fast16_t hal_get_available_bytes_gcode_uart(void)
{
    return hal_uart_get_available_bytes(GCODE_UART);
}

void hal_forget_bytes_gcode_uart(uint_fast16_t how_many)
{
    hal_uart_forget_bytes(GCODE_UART, how_many);
}

void hal_send_frame_gcode_uart(uint8_t * frame, uint_fast16_t length)
{
    hal_uart_send_frame(GCODE_UART, frame, length);
}

bool hal_send_frame_non_blocking_gcode_uart(uint8_t * frame, uint_fast16_t length)
{
    return hal_uart_send_frame_non_blocking(GCODE_UART, frame, length);
}

bool hal_init_debug_uart(void)
{
    if(true == debug_initialized)
    {
        // initialize only once !
        return false;
    }
    debug_initialized = true;

    return hal_uart_init(DEBUG_UART);
}

#ifdef DEBUG_ACTIVE
void hal_print_configuration_debug_uart(void)
{
    hal_uart_print_configuration(DEBUG_UART);
}
#endif

uint_fast8_t hal_get_debug_uart_byte_at_offset(uint_fast16_t offset)
{
    return hal_uart_get_byte_at_offset(DEBUG_UART, offset);
}

uint_fast16_t hal_get_available_bytes_debug_uart(void)
{
    return hal_uart_get_available_bytes(DEBUG_UART);
}

void hal_forget_bytes_debug_uart(uint_fast16_t how_many)
{
    hal_uart_forget_bytes(DEBUG_UART, how_many);
}

void hal_send_frame_debug_uart(uint8_t * frame, uint_fast16_t length)
{
    hal_uart_send_frame(DEBUG_UART, frame, length);
}

bool hal_send_frame_non_blocking_debug_uart(uint8_t * frame, uint_fast16_t length)
{
    return hal_uart_send_frame_non_blocking(DEBUG_UART, frame, length);
}


// end of hal_uart_api


static uint_fast8_t hal_uart_get_byte_at_offset(uint_fast8_t device, uint_fast16_t offset)
{
    if(device < MAX_UART)
    {
        uint_fast8_t res;
        uint_fast16_t target_pos = devices[device].receive_read_pos + offset;
        if((UART_RECEIVE_BUFFER_SIZE -1) < target_pos)
        {
            target_pos = target_pos - UART_RECEIVE_BUFFER_SIZE;
        }
        res = devices[device].receive_buffer[target_pos];
        return res;
    }
    else
    {
        // invalid Interface Specified
        return 0;
    }
}

static uint_fast16_t hal_uart_get_available_bytes(uint_fast8_t device)
{
    if(device < MAX_UART)
    {
        uint_fast16_t res = 0;
        if(devices[device].receive_read_pos != devices[device].receive_write_pos)
        {
            if(devices[device].receive_write_pos > devices[device].receive_read_pos)
            {
                res = devices[device].receive_write_pos - devices[device].receive_read_pos;
            }
            else
            {
                res = UART_RECEIVE_BUFFER_SIZE - devices[device].receive_read_pos + devices[device].receive_write_pos;
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

static void hal_uart_forget_bytes(uint_fast8_t device, uint_fast16_t how_many)
{
    if(device < MAX_UART)
    {
        uint_fast16_t target_pos = devices[device].receive_read_pos + how_many;
        if((UART_RECEIVE_BUFFER_SIZE -1) < target_pos)
        {
            target_pos = target_pos - UART_RECEIVE_BUFFER_SIZE;
        }
        devices[device].receive_read_pos = target_pos;
    }
    // else invalid Interface Specified
}

static void hal_uart_send_frame(uint_fast8_t device, uint8_t * frame, uint_fast16_t length)
{
    if(device < MAX_UART)
    {
        if(1 > length)
        {
            // no data
            return;
        }
        if(NULL == frame)
        {
            // no frame -> no data
            return;
        }

        while(false == copy_data_to_send(device, frame, length))
        {
            ; // wait
        }
    }
    // else invalid Interface Specified
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

static uint_fast16_t get_available_bytes_in_send_Buffer(uint_fast8_t device)
{
    if(device < MAX_UART)
    {
        uint_fast16_t res;
        if(devices[device].send_read_pos != devices[device].send_write_pos)
        {
            if(devices[device].send_write_pos > devices[device].send_read_pos)
            {
                res = (UART_SEND_BUFFER_SIZE -1) - devices[device].send_read_pos + devices[device].send_write_pos;
            }
            else
            {
                res = (devices[device].send_write_pos - devices[device].send_read_pos) -1;
            }
        }
        else
        {
            // Buffer is empty
            res = UART_SEND_BUFFER_SIZE -1;
        }
        return res;
    }
    else
    {
        // invalid Interface Specified
        return 0;
    }
}


static bool copy_data_to_send(uint_fast8_t device, uint8_t * frame, uint_fast16_t length)
{
    uint_fast16_t i;
    if(NULL == frame)
    {
        return false;
    }
    if(0 == length)
    {
        // another job well done !
        return true;
    }
    if(!(device < MAX_UART))
    {
        return false;
    }
    if(length > get_available_bytes_in_send_Buffer(device))
    {
        // not enough free bytes in the buffer
        return false;
    }
    for(i = 0; i < length; i++)
    {
        devices[device].send_buffer[devices[device].send_write_pos] = frame[i];
        devices[device].send_write_pos++;
        if(devices[device].send_write_pos < UART_SEND_BUFFER_SIZE)
        {
            // OK
        }
        else
        {
            // wrap around
            devices[device].send_write_pos = 0;
        }
    }
    return true;
}

static bool hal_uart_send_frame_non_blocking(uint_fast8_t device, uint8_t * frame, uint_fast16_t length)
{
    if(1 > length)
    {
        // send out 0 Bytes !
        // another Job well done ;-)
        return true;
    }

    if(!(device < MAX_UART))
    {
        // invalid Interface Specified or
        // no data
        hal_cpu_report_issue(9);
        return false;
    }

    // can we copy the data ?
    if(false == copy_data_to_send(device, frame, length))
    {
        // Send buffer is full
        hal_cpu_report_issue(10);
        return false;
    }

    // are we already sending or do we need to start sending
    if(true == devices[device].is_sending)
    {
        // nothing to do here
    }
    else
    {
        // start sending now
        devices[device].is_sending = true;
        devices[device].port->DR = (uint16_t)devices[device].send_buffer[devices[device].send_read_pos];
        devices[device].send_read_pos++;
        if(devices[device].send_read_pos < UART_SEND_BUFFER_SIZE)
        {
            // ok
        }
        else
        {
            // wrap around
            devices[device].send_read_pos = 0;
        }
        // The next line activates the Interrupt. The Interrupt may become
        // active immediately. It therefore must be the last line !
        devices[device].port->CR1 |= USART_CR1_TXEIE;
    }
    return true;
}

static void device_IRQ_handler(uint_fast8_t device)
{
    if(USART_SR_RXNE == (USART_SR_RXNE & devices[device].port->SR))
    {
        // we received a byte
        uint32_t reg = devices[device].port->DR;
        uint8_t received_byte = (reg & 0xff);
        devices[device].port->SR &= ~USART_SR_RXNE;
        devices[device].receive_buffer[devices[device].receive_write_pos] = received_byte;
        devices[device].receive_write_pos ++;
        if(UART_RECEIVE_BUFFER_SIZE == devices[device]. receive_write_pos)
        {
            devices[device].receive_write_pos = 0;
        }
    }
    if(USART_SR_TXE == (USART_SR_TXE & devices[device].port->SR))
    {
        // we can send a byte
        if(true == devices[device].is_sending)
        {
            devices[device].port->DR = (uint16_t)devices[device].send_buffer[devices[device].send_read_pos];
            devices[device].send_read_pos++;
            if(devices[device].send_read_pos < UART_SEND_BUFFER_SIZE)
            {
                // ok
            }
            else
            {
                // wrap around
                devices[device].send_read_pos = 0;
            }
            if(devices[device].send_read_pos == devices[device].send_write_pos)
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
    hal_set_isr1_led(true);
    device_IRQ_handler(0);
    hal_set_isr1_led(false);
}

void UART_1_IRQ_HANDLER(void)
{
    hal_set_isr1_led(true);
    device_IRQ_handler(1);
    hal_set_isr1_led(false);
}

#ifdef DEBUG_ACTIVE
static void hal_uart_print_configuration(uint_fast8_t device)
{
    if(device < MAX_UART)
    {
        debug_line(STR("Configuration of UART_%d :"), device);
        // Clock
        debug_line(STR("RCC->AHB1ENR  = 0x%08x"), RCC->AHB1ENR);
        debug_line(STR("RCC->APB1ENR  = 0x%08x"), RCC->APB1ENR);
        debug_line(STR("RCC->APB2ENR  = 0x%08x"), RCC->APB2ENR);
        // Uart
        debug_line(STR("UART->BRR     = 0x%08x"), devices[device].port->BRR);
        debug_line(STR("UART->CR1     = 0x%08x"), devices[device].port->CR1);
        debug_line(STR("UART->CR2     = 0x%08x"), devices[device].port->CR2);
        debug_line(STR("UART->CR3     = 0x%08x"), devices[device].port->CR3);
        debug_line(STR("UART->DR      = 0x%08x"), devices[device].port->DR);
        debug_line(STR("UART->GTPR    = 0x%08x"), devices[device].port->GTPR);
        debug_line(STR("UART->SR      = 0x%08x"), devices[device].port->SR);
        // GPIO
        switch(device)
        {
        case 0 :
            debug_line(STR("RX Pin:"));
            print_gpio_configuration(UART_0_RX_GPIO_PORT);
            debug_line(STR("TX Pin:"));
            print_gpio_configuration(UART_0_TX_GPIO_PORT);
            break;

        case 1 :
            debug_line(STR("RX Pin:"));
            print_gpio_configuration(UART_1_RX_GPIO_PORT);
            debug_line(STR("TX Pin:"));
            print_gpio_configuration(UART_1_TX_GPIO_PORT);
            break;

        default:
            break;
        }
    }
    // else invalid Interface Specified
}
#endif

static bool hal_uart_init(uint_fast8_t device)
{
    if(device < MAX_UART)
    {
        devices[device].receive_read_pos = 0;
        devices[device].receive_write_pos = 0;
        devices[device].is_sending = false;
        devices[device].send_write_pos = 0;
        devices[device].send_read_pos = 0;

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

