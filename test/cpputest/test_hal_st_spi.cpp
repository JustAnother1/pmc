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

#include "CppUTest/TestHarness.h"
#include "CppUTest/TestOutput.h"
#include "CppUTest/TestTestingFixture.h"
#include "CppUTest/PlatformSpecificFunctions.h"

#include "hal_spi.h"
#include "hal_cfg.h"
#include "st_spi.h"

typedef struct {
    volatile uint_fast8_t send_pos;
    volatile uint_fast8_t rec_pos;
    volatile uint_fast8_t length;
    // pointer to Bytes to send
    volatile uint8_t *send_buffer;
    volatile uint8_t *receive_buffer;
    // SPI is idle
    volatile bool idle;
    volatile bool successfully_received;
    volatile SPI_TypeDef * bus;
}spi_device_typ;

extern spi_device_typ spi_devices[MAX_SPI];
extern bool stepper_initialized;
extern bool expansion_initialized;

TEST_GROUP(HalStSpiTestGroup)
{
       void setup()
       {
           SPI2_BASE.CR1 = 23;
           spi_devices[0].bus = &SPI1_BASE;
           spi_devices[1].bus = &SPI2_BASE;
       }

       void teardown()
       {
          // Uninit stuff
       }
};

TEST(HalStSpiTestGroup, hal_print_stepper_spi_configuration)
{
    hal_print_stepper_spi_configuration();
}


TEST(HalStSpiTestGroup, hal_print_expansion_spi_configuration)
{
    hal_print_expansion_spi_configuration();
}

/*
// SPI Bus that is connected to the Trinamic Stepper controllers

void hal_init_stepper_spi(void);
void hal_print_stepper_spi_configuration(void);
bool hal_do_stepper_spi_transaction(uint8_t*     data_to_send,
                                    uint_fast8_t num_bytes_to_send,
                                    uint8_t*     data_received);
void hal_start_stepper_spi_transaction(uint8_t*     data_to_send,
                                       uint_fast8_t num_bytes_to_send,
                                       uint8_t*     data_received);
bool hal_stepper_spi_is_idle(void);


// the "other" SPI

void hal_init_expansion_spi(void);
void hal_print_expansion_spi_configuration(void);
bool hal_do_expansion_spi_transaction(uint8_t*     data_to_send,
                                      uint_fast8_t num_bytes_to_send,
                                      uint8_t*     data_received);
void hal_start_expansion_spi_transaction(uint8_t*     data_to_send,
                                         uint_fast8_t num_bytes_to_send,
                                         uint8_t*     data_received);
bool hal_expansion_spi_is_idle(void);
*/
