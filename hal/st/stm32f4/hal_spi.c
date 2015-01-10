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

#include "hal_spi.h"
#include <stdbool.h>

// 20 bit per stepper in Bytes (1 Stepper = 3 Bytes; 2 Stepper = 5 Bytes; 4 Stepper = 10 Bytes)
#define SPI_BUFFER_LENGTH           5

static void start_spi_transaction(uint8_t *data_to_send,
                                  uint_fast8_t idx_of_first_byte,
                                  uint_fast8_t num_bytes_to_send,
                                  uint8_t *data_received);

// The index of the byte in the current buffer that will be send next,
// will be decremented to 0 - end of message
static uint_fast8_t spi_offset;
// pointer to Bytes to send
static uint8_t *spi_send_buffer;
static uint8_t *spi_receive_buffer;
// SPI is idle
static bool spi_idle;

void hal_spi_init(void)
{
    // initialize SPI Hardware
    spi_idle = true;
}

void hal_spi_do_transaction(uint8_t *data_to_send,
                            uint_fast8_t idx_of_first_byte,
                            uint_fast8_t num_bytes_to_send,
                            uint8_t *data_received)
{
    start_spi_transaction(data_to_send,
                          idx_of_first_byte,
                          num_bytes_to_send,
                          data_received);
    /*
    while(false == spi_idle)
    {
        ; // wait until we have the data back;
    }
    */
}

static void start_spi_transaction(uint8_t *data_to_send,
                                  uint_fast8_t idx_of_first_byte,
                                  uint_fast8_t num_bytes_to_send,
                                  uint8_t *data_received)
{
    spi_send_buffer = data_to_send;
    spi_receive_buffer = data_received;
    spi_idle = false;
    spi_offset = idx_of_first_byte;
    // SPI0DAT = spi_send_buffer[idx_of_first_byte];
}

/*
INTERRUPT(spi_isr, INTERRUPT_SPI0)
{

    SPI0CN &= ~0x80; // reset Interrupt Flag
    if(0 == spi_offset)
    {
        spi_receive_buffer[0] = SPI0DAT;
        NSS = 1;
        spi_idle = TRUE;
        return;
    }
    else
    {
        spi_receive_buffer[spi_offset] = SPI0DAT;
        spi_offset--;
        SPI0DAT = spi_send_buffer[spi_offset];
        return;
    }

}
*/
