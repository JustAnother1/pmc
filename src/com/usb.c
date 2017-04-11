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
#include "usb.h"
#include "hal_usb_device_cdc.h"
#include "protocol.h"
#include "com.h"

#define RECEIVE_BUFFER_SIZE  512

static void cb_Init(void);
static void cb_DeInit(void);
static void cb_receive(uint8_t* buffer, uint16_t length);
static uint_fast16_t get_number_of_available_Bytes(void);
static bool crc_is_valid(void);
static uint_fast8_t get_byte_at_offset(uint_fast16_t offset);
static void forget_bytes(uint_fast16_t how_many);

static bool is_connected = false;
static bool has_a_frame = false;
static uint_fast16_t length;
static uint_fast16_t read_pos = 0;
static uint_fast16_t write_pos = 0;
static uint_fast16_t checked_bytes = 0;
static uint8_t receive_buffer[RECEIVE_BUFFER_SIZE];

cdc_call_back_api_typ  client_cb =
{
    &cb_Init,
    &cb_DeInit,
    &cb_receive
};

static void cb_Init(void)
{
    is_connected = true;
}

static void cb_DeInit(void)
{
    is_connected = false;
}

static void cb_receive(uint8_t* buffer, uint16_t length)
{
    uint_fast16_t i;
    // TODO handle an already full receive buffer,..
    for(i = 0; i < length; i++)
    {
        receive_buffer[write_pos] = buffer[i];
        write_pos++;
    }
}

static uint_fast16_t get_number_of_available_Bytes(void)
{
    if(read_pos == write_pos)
    {
        return 0;
    }
    else if(read_pos < write_pos)
    {
        return write_pos - read_pos;
    }
    else
    {
        return write_pos + RECEIVE_BUFFER_SIZE - read_pos;
    }
}

static bool crc_is_valid(void)
{
    // CRC valid ?
    uint_fast8_t received_crc = get_byte_at_offset(length + 2);
    uint_fast8_t claculated_crc = com_crc_a_byte(length, 0);
    uint_fast16_t off;
    for(off = 0; off < length; off++)
    {
        claculated_crc = com_crc_a_byte(get_byte_at_offset(off + 2), claculated_crc);
    }

    if(claculated_crc != received_crc)
    {
        com_send_bad_crc(received_crc, claculated_crc);
        return false;
    }
    else
    {
        return true;
    }
}

static uint_fast8_t get_byte_at_offset(uint_fast16_t offset)
{
    uint_fast16_t idx = read_pos + offset;
    if(idx > RECEIVE_BUFFER_SIZE)
    {
        idx = idx - RECEIVE_BUFFER_SIZE;
    }
    return receive_buffer[idx];
}

static void forget_bytes(uint_fast16_t how_many)
{
    uint_fast16_t idx = read_pos + how_many;
    if(idx > RECEIVE_BUFFER_SIZE)
    {
        idx = idx - RECEIVE_BUFFER_SIZE;
    }
    read_pos = idx;
}

// public API:

bool start_usb(void)
{
    return hal_usb_device_cdc_init(&client_cb);
}

bool usb_has_next_frame(void) // this is usb_tick()
{
    if(false == is_connected)
    {
        return false;
    }
    else
    {
        uint_fast16_t bytes_available;
        if(true == has_a_frame)
        {
            return true;
        }
        // else :
        // check the Frame:
        // Enough Bytes
        bytes_available = get_number_of_available_Bytes();
        if(checked_bytes == bytes_available)
        {
            // we did a check on this number of Bytes and had no Frame
            return false;
        }
        else
        {
            // we will now check this amount of Bytes received.
            checked_bytes = bytes_available;
        }
        if(MIN_BYTES_HOST_FRAME > bytes_available)
        {
            return false;
        }
        // starts with a sync byte
        if(HOST_SYNC_REQUEST != receive_buffer[read_pos])
        {
            read_pos++;
            if(RECEIVE_BUFFER_SIZE == read_pos)
            {
                read_pos = 0;
            }
            checked_bytes = 0;
            return false;
        }
        // length OK and also enough bytes for Parameter
        length = get_byte_at_offset(1);
        if(length + 3 > bytes_available)
        {
            return false;
        }
        // CRC valid ?
        if(false == crc_is_valid())
        {
            usb_forget_frame();
            return false;
        }
        has_a_frame = true;
        return has_a_frame;
    }
}

uint_fast8_t usb_get_parameter_byte(uint_fast8_t index)
{
    if(false == is_connected)
    {
        return 0;
    }
    else
    {
        return get_byte_at_offset(REQUEST_FRAME_START_OF_PARAMETER + index);
    }
}

uint_fast8_t usb_get_order(void)
{
    if(false == is_connected)
    {
        return 0;
    }
    else
    {
        return get_byte_at_offset(REQUEST_FRAME_POS_OF_ORDER_CODE);
    }
}

uint_fast8_t usb_get_parameter_length(void)
{
    if(false == is_connected)
    {
        return 0;
    }
    else
    {
        return length - REQUEST_NUMBER_OF_NON_PARAMETER_BYTES_IN_LENGTH;
    }
}

uint_fast8_t usb_get_control(void)
{
    if(false == is_connected)
    {
        return 0;
    }
    else
    {
        return get_byte_at_offset(REQUEST_FRAME_POS_OF_CONTROL);
    }
}

void usb_send_frame(uint8_t * frame, uint_fast16_t length)
{
    if(false == is_connected)
    {
        return;
    }
    else
    {
        hal_usb_device_cdc_send_data(frame, length);
    }
}

void usb_forget_frame(void)
{
    if(false == is_connected)
    {
        return;
    }
    else
    {
        forget_bytes(MIN_BYTES_HOST_FRAME + usb_get_parameter_length());
        checked_bytes = 0;
        has_a_frame = false;
    }
}

// end of File
