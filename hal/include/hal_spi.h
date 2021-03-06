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
#include <stdbool.h>

// SPI Bus that is connected to the Trinamic Stepper controllers

void hal_init_stepper_spi(void);
#ifdef DEBUG_ACTIVE
void hal_print_stepper_spi_configuration(void);
#endif
bool hal_do_stepper_spi_transaction(uint8_t*     data_to_send,
                                    uint_fast8_t num_bytes_to_send,
                                    uint8_t*     data_received);
void hal_start_stepper_spi_transaction(uint8_t*     data_to_send,
                                       uint_fast8_t num_bytes_to_send,
                                       uint8_t*     data_received);
bool hal_stepper_spi_is_idle(void);
void hal_reset_stepper_spi_transaction(void);


// the "other" SPI

void hal_init_expansion_spi(void);
#ifdef DEBUG_ACTIVE
void hal_print_expansion_spi_configuration(void);
#endif
bool hal_do_expansion_spi_transaction(uint8_t*     data_to_send,
                                      uint_fast8_t num_bytes_to_send,
                                      uint8_t*     data_received);
void hal_start_expansion_spi_transaction(uint8_t*     data_to_send,
                                         uint_fast8_t num_bytes_to_send,
                                         uint8_t*     data_received);
bool hal_expansion_spi_is_idle(void);
void hal_reset_expansion_spi_transaction(void);
#endif /* HAL_SPI_H_ */
