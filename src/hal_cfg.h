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

#ifndef HAL_INCLUDE_HAL_CFG_H_
#define HAL_INCLUDE_HAL_CFG_H_

// SPI
#define STEPPER_SPI   0
#define EXPANSION_SPI 1
#define MAX_SPI       2

// UART
#define GCODE_UART  0
#define DEBUG_UART  1
#define MAX_UART    2

// Heaters
#define NUMBER_OF_HEATERS 6

// LEDs
#define DEBUG_LED  0
#define ERROR_LED  1
#define MAX_LED    2


// Timer
// Step creation
#define STEP_TIMER        0
#define STEP_BUFFER_TIMER 1

#endif /* HAL_INCLUDE_HAL_CFG_H_ */
