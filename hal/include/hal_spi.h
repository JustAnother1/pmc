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

#ifndef HAL_SPI_H_
#define HAL_SPI_H_

#include <inttypes.h>
#include "hal_cfg.h"

void hal_spi_init(uint_fast8_t device);
void hal_spi_print_configuration(uint_fast8_t device);
void hal_spi_do_transaction(uint_fast8_t device,
                            uint8_t      *data_to_send,
                            uint_fast8_t idx_of_first_byte,
                            uint_fast8_t num_bytes_to_send,
                            uint8_t      *data_received);
#endif /* HAL_SPI_H_ */
