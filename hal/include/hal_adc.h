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

#ifndef HAL_INCLUDE_HAL_ADC_H_
#define HAL_INCLUDE_HAL_ADC_H_

#include <inttypes.h>

void hal_adc_init(void);
uint_fast8_t hal_adc_get_amount(void);
/** retrieves the currently measured analog value as temperature.
 * @param device number of temperature sensor
 * @return temperature in 0.1° steps (250 = 25°; 123 = 12.3°)
 */
uint_fast16_t hal_adc_get_value(uint_fast8_t device);
uint_fast8_t hal_adc_get_name(uint_fast8_t device, uint8_t *position);
void hal_print_configuration_adc(void);

#endif /* HAL_INCLUDE_HAL_ADC_H_ */
