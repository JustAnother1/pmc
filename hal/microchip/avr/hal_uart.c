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
#include <avr/io.h>
#include <avr/interrupt.h>
#include "board_cfg.h"
#include "hal_cfg.h"
#include "hal_cpu.h"
#include "hal_debug.h"
#include "hal_led.h"
#include "hal_time.h"
#include "hal_uart.h"

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
}uart_device_typ;

static bool copy_data_to_send(uint_fast8_t device, uint8_t * frame, uint_fast16_t length);
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

    devices[GCODE_UART].receive_read_pos = 0;
    devices[GCODE_UART].receive_write_pos = 0;
    devices[GCODE_UART].is_sending = false;
    devices[GCODE_UART].send_write_pos = 0;
    devices[GCODE_UART].send_read_pos = 0;
    cli();
    // cancel currently ongoing reception and sending
    if(GCODE_USART_U2X)
    {
        GCODE_USART_UCSRA = 0x62;
    }
    else
    {
        GCODE_USART_UCSRA = 0x60;
    }
    GCODE_USART_UBRRH = GCODE_USART_BAUD_RATE_HIGH;
    GCODE_USART_UBRRL = GCODE_USART_BAUD_RATE_LOW;
    GCODE_USART_UCSRB = 0x98; // Enable Receive Interrupt, Enable Receiver, Enable Sender, 8bit
    GCODE_USART_UCSRC = 0x06; // Async, Parity none, 1 Stop Bit, 8 data bits
    sei();
    return true;
}

bool hal_init_debug_uart(void)
{
    if(true == debug_initialized)
    {
        // initialize only once !
        return false;
    }
    debug_initialized = true;

    devices[DEBUG_UART].receive_read_pos = 0;
    devices[DEBUG_UART].receive_write_pos = 0;
    devices[DEBUG_UART].is_sending = false;
    devices[DEBUG_UART].send_write_pos = 0;
    devices[DEBUG_UART].send_read_pos = 0;
    cli();
    // cancel currently ongoing reception and sending
    if(DEBUG_USART_U2X)
    {
        DEBUG_USART_UCSRA = 0x62;
    }
    else
    {
        DEBUG_USART_UCSRA = 0x60;
    }
    DEBUG_USART_UBRRH = DEBUG_USART_BAUD_RATE_HIGH;
    DEBUG_USART_UBRRL = DEBUG_USART_BAUD_RATE_LOW;
    DEBUG_USART_UCSRB = 0x98; // Enable Receive Interrupt, Enable Receiver, Enable Sender, 8bit
    DEBUG_USART_UCSRC = 0x06; // Async, Parity none, 1 Stop Bit, 8 data bits
    sei();
    return true;
}

#ifdef DEBUG_ACTIVE
void hal_print_configuration_gcode_uart(void)
{
    debug_line(STR("Configuration of USART_%d :"), GCODE_USART_NUMBER);
    debug_line(STR("USART%d->UCSRA  = 0x%02x"), GCODE_USART_UCSRA);
    debug_line(STR("USART%d->UCSRB  = 0x%02x"), GCODE_USART_UCSRB);
    debug_line(STR("USART%d->UCSRC  = 0x%02x"), GCODE_USART_UCSRC);
    debug_line(STR("USART%d->UBRRH  = 0x%02x"), GCODE_USART_UBRRH);
    debug_line(STR("USART%d->UBRRL  = 0x%02x"), GCODE_USART_UBRRL);
}

void hal_print_configuration_debug_uart(void)
{
    debug_line(STR("Configuration of USART_%d :"), DEBUG_USART_NUMBER);
    debug_line(STR("USART%d->UCSRA  = 0x%02x"), DEBUG_USART_UCSRA);
    debug_line(STR("USART%d->UCSRB  = 0x%02x"), DEBUG_USART_UCSRB);
    debug_line(STR("USART%d->UCSRC  = 0x%02x"), DEBUG_USART_UCSRC);
    debug_line(STR("USART%d->UBRRH  = 0x%02x"), DEBUG_USART_UBRRH);
    debug_line(STR("USART%d->UBRRL  = 0x%02x"), DEBUG_USART_UBRRL);
}

#endif

uint_fast8_t hal_get_gcode_uart_byte_at_offset(uint_fast16_t offset)
{
    uint_fast8_t res;
    uint_fast16_t target_pos = devices[GCODE_UART].receive_read_pos + offset;
    if((UART_RECEIVE_BUFFER_SIZE -1) < target_pos)
    {
        target_pos = target_pos - UART_RECEIVE_BUFFER_SIZE;
    }
    res = devices[GCODE_UART].receive_buffer[target_pos];
    return res;
}

uint_fast8_t hal_get_debug_uart_byte_at_offset(uint_fast16_t offset)
{
    uint_fast8_t res;
    uint_fast16_t target_pos = devices[DEBUG_UART].receive_read_pos + offset;
    if((UART_RECEIVE_BUFFER_SIZE -1) < target_pos)
    {
        target_pos = target_pos - UART_RECEIVE_BUFFER_SIZE;
    }
    res = devices[DEBUG_UART].receive_buffer[target_pos];
    return res;
}


uint_fast16_t hal_get_available_bytes_gcode_uart(void)
{
    uint_fast16_t res = 0;
    if(devices[GCODE_UART].receive_read_pos != devices[GCODE_UART].receive_write_pos)
    {
        if(devices[GCODE_UART].receive_write_pos > devices[GCODE_UART].receive_read_pos)
        {
            res = devices[GCODE_UART].receive_write_pos - devices[GCODE_UART].receive_read_pos;
        }
        else
        {
            res = UART_RECEIVE_BUFFER_SIZE - devices[GCODE_UART].receive_read_pos + devices[GCODE_UART].receive_write_pos;
        }
    }
    // else res = 0;
    return res;
}

uint_fast16_t hal_get_available_bytes_debug_uart(void)
{
    uint_fast16_t res = 0;
    if(devices[DEBUG_UART].receive_read_pos != devices[DEBUG_UART].receive_write_pos)
    {
        if(devices[DEBUG_UART].receive_write_pos > devices[DEBUG_UART].receive_read_pos)
        {
            res = devices[DEBUG_UART].receive_write_pos - devices[DEBUG_UART].receive_read_pos;
        }
        else
        {
            res = UART_RECEIVE_BUFFER_SIZE - devices[DEBUG_UART].receive_read_pos + devices[DEBUG_UART].receive_write_pos;
        }
    }
    // else res = 0;
    return res;
}

void hal_forget_bytes_gcode_uart(uint_fast16_t how_many)
{
    uint_fast16_t target_pos = devices[GCODE_UART].receive_read_pos + how_many;
    if((UART_RECEIVE_BUFFER_SIZE -1) < target_pos)
    {
        target_pos = target_pos - UART_RECEIVE_BUFFER_SIZE;
    }
    devices[GCODE_UART].receive_read_pos = target_pos;
}

void hal_forget_bytes_debug_uart(uint_fast16_t how_many)
{
    uint_fast16_t target_pos = devices[DEBUG_UART].receive_read_pos + how_many;
    if((UART_RECEIVE_BUFFER_SIZE -1) < target_pos)
    {
        target_pos = target_pos - UART_RECEIVE_BUFFER_SIZE;
    }
    devices[DEBUG_UART].receive_read_pos = target_pos;
}

void hal_send_frame_gcode_uart(uint8_t * frame, uint_fast16_t length)
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

    while(false == copy_data_to_send(GCODE_UART, frame, length))
    {
        ; // wait
    }
}

void hal_send_frame_debug_uart(uint8_t * frame, uint_fast16_t length)
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

    while(false == copy_data_to_send(DEBUG_UART, frame, length))
    {
        ; // wait
    }
}

bool hal_send_frame_non_blocking_gcode_uart(uint8_t * frame, uint_fast16_t length)
{
    if(1 > length)
    {
        // send out 0 Bytes !
        // another Job well done ;-)
        return true;
    }

    // can we copy the data ?
    if(false == copy_data_to_send(GCODE_UART, frame, length))
    {
        // Send buffer is full
        hal_cpu_report_issue(10);
        return false;
    }

    // are we already sending or do we need to start sending
    if(true == devices[GCODE_UART].is_sending)
    {
        // nothing to do here
    }
    else
    {
        // start sending now
        devices[GCODE_UART].is_sending = true;
        GCODE_USART_UDR = (uint16_t)devices[GCODE_UART].send_buffer[devices[GCODE_UART].send_read_pos];
        devices[GCODE_UART].send_read_pos++;
        if(devices[GCODE_UART].send_read_pos < UART_SEND_BUFFER_SIZE)
        {
            // ok
        }
        else
        {
            // wrap around
            devices[GCODE_UART].send_read_pos = 0;
        }
        // The next line activates the Interrupt. The Interrupt may become
        // active immediately. It therefore must be the last line !
        GCODE_USART_UCSRB = GCODE_USART_UCSRB | 0x60; // enable TC Complete and Data Register Empty Interrupt
    }
    return true;
}

bool hal_send_frame_non_blocking_debug_uart(uint8_t * frame, uint_fast16_t length)
{
    if(1 > length)
    {
        // send out 0 Bytes !
        // another Job well done ;-)
        return true;
    }

    // can we copy the data ?
    if(false == copy_data_to_send(DEBUG_UART, frame, length))
    {
        // Send buffer is full
        hal_cpu_report_issue(10);
        return false;
    }

    // are we already sending or do we need to start sending
    if(true == devices[DEBUG_UART].is_sending)
    {
        // nothing to do here
    }
    else
    {
        // start sending now
        devices[DEBUG_UART].is_sending = true;
        DEBUG_USART_UDR = (uint16_t)devices[DEBUG_UART].send_buffer[devices[DEBUG_UART].send_read_pos];
        devices[DEBUG_UART].send_read_pos++;
        if(devices[DEBUG_UART].send_read_pos < UART_SEND_BUFFER_SIZE)
        {
            // ok
        }
        else
        {
            // wrap around
            devices[DEBUG_UART].send_read_pos = 0;
        }
        // The next line activates the Interrupt. The Interrupt may become
        // active immediately. It therefore must be the last line !
        DEBUG_USART_UCSRB = DEBUG_USART_UCSRB | 0x60; // enable TC Complete and Data Register Empty Interrupt
    }
    return true;
}


// end of hal_uart_api

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

// ISR GCODE

ISR(GCODE_RECEIVE_ISR,ISR_BLOCK)
{
    hal_set_isr1_led(true);
    // we received a byte
    uint8_t received_byte = GCODE_USART_UDR;
    devices[GCODE_UART].receive_buffer[devices[GCODE_UART].receive_write_pos] = received_byte;
    devices[GCODE_UART].receive_write_pos ++;
    if(UART_RECEIVE_BUFFER_SIZE == devices[GCODE_UART]. receive_write_pos)
    {
        devices[GCODE_UART].receive_write_pos = 0;
    }
    hal_set_isr1_led(false);
}

ISR(GCODE_TRANSMITT_COMPLETE_ISR,ISR_BLOCK)
{
    hal_set_isr1_led(true);
    if(true == devices[GCODE_UART].is_sending)
    {
        // nothing to do -> we send in Data Register Empty ISR
    }
    else
    {
        GCODE_USART_UCSRB = GCODE_USART_UCSRB & ~0x60; // disable TC Complete and Data Register Empty Interrupt
    }
    hal_set_isr1_led(false);
}

ISR(GCODE_DATA_REGISTER_EMPTY_ISR,ISR_BLOCK)
{
    hal_set_isr1_led(true);
    // we can send a byte
    if(true == devices[GCODE_UART].is_sending)
    {
        GCODE_USART_UDR = (uint16_t)devices[GCODE_UART].send_buffer[devices[GCODE_UART].send_read_pos];
        devices[GCODE_UART].send_read_pos++;
        if(devices[GCODE_UART].send_read_pos < UART_SEND_BUFFER_SIZE)
        {
            // ok
        }
        else
        {
            // wrap around
            devices[GCODE_UART].send_read_pos = 0;
        }
        if(devices[GCODE_UART].send_read_pos == devices[GCODE_UART].send_write_pos)
        {
            devices[GCODE_UART].is_sending = false;
            // Disabling TX Complete ISR here would stop sending immediately. That would kill the last byte.
            GCODE_USART_UCSRB = GCODE_USART_UCSRB & ~0x20; // disable Data Register Empty Interrupt
        }
    }
    else
    {
        // should not happen but for safety
        GCODE_USART_UCSRB = GCODE_USART_UCSRB & ~0x20; // disable Data Register Empty Interrupt
    }
    hal_set_isr1_led(false);
}

// ISR DEBUG

ISR(DEBUG_RECEIVE_ISR,ISR_BLOCK)
{
    hal_set_isr1_led(true);
    // we received a byte
    uint8_t received_byte = DEBUG_USART_UDR;
    devices[DEBUG_UART].receive_buffer[devices[DEBUG_UART].receive_write_pos] = received_byte;
    devices[DEBUG_UART].receive_write_pos ++;
    if(UART_RECEIVE_BUFFER_SIZE == devices[DEBUG_UART]. receive_write_pos)
    {
        devices[DEBUG_UART].receive_write_pos = 0;
    }
    hal_set_isr1_led(false);
}

ISR(DEBUG_TRANSMITT_COMPLETE_ISR,ISR_BLOCK)
{
    hal_set_isr1_led(true);
    if(true == devices[DEBUG_UART].is_sending)
    {
        // nothing to do -> we send in Data Register Empty ISR
    }
    else
    {
        DEBUG_USART_UCSRB = DEBUG_USART_UCSRB & ~0x60; // disable TC Complete and Data Register Empty Interrupt
    }
    hal_set_isr1_led(false);
}

ISR(DEBUG_DATA_REGISTER_EMPTY_ISR,ISR_BLOCK)
{
    hal_set_isr1_led(true);
    // we can send a byte
    if(true == devices[DEBUG_UART].is_sending)
    {
        DEBUG_USART_UDR = (uint16_t)devices[DEBUG_UART].send_buffer[devices[DEBUG_UART].send_read_pos];
        devices[DEBUG_UART].send_read_pos++;
        if(devices[DEBUG_UART].send_read_pos < UART_SEND_BUFFER_SIZE)
        {
            // ok
        }
        else
        {
            // wrap around
            devices[DEBUG_UART].send_read_pos = 0;
        }
        if(devices[DEBUG_UART].send_read_pos == devices[DEBUG_UART].send_write_pos)
        {
            devices[DEBUG_UART].is_sending = false;
            // Disabling TX Complete ISR here would stop sending immediately. That would kill the last byte.
            DEBUG_USART_UCSRB = DEBUG_USART_UCSRB & ~0x20; // disable Data Register Empty Interrupt
        }
    }
    else
    {
        // should not happen but for safety
        DEBUG_USART_UCSRB = DEBUG_USART_UCSRB & ~0x20; // disable Data Register Empty Interrupt
    }
    hal_set_isr1_led(false);
}
