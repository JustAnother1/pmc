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

// handling of constant Strings
#define STR(x) (x)

// SPI
#define STEPPER_SPI   0
#define EXPANSION_SPI 0
#define MAX_SPI       1

// UART
#define GCODE_UART  0
#define DEBUG_UART  1
#define MAX_UART    2

#define UART_RECEIVE_BUFFER_SIZE  512
#define UART_SEND_BUFFER_SIZE     2048

// LEDs
#define BOARD_HAS_DEBUG_LED
#define DEBUG_LED  0
#define MAX_LED    1

// Heaters
#define NUMBER_OF_HEATERS 2

// Steppers
#define NUMBER_OF_STEPPERS   4

// Timer


#define PWM_0_TIMER                3
#define PWM_0_Channel              1

#define PWM_1_TIMER                4
#define PWM_1_Channel              1

#define PWM_2_TIMER                2
#define PWM_2_Channel              2

#define STEP_TIMER                 9
#define STEP_BUFFER_TIMER          10

#define BUZZER_TIMER               5

#define STEP_ISR_FUNCTION static void step_isr(void)

#endif /* HAL_INCLUDE_HAL_CFG_H_ */
