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

#define HAS_CCM  1

// SPI
#define STEPPER_SPI   0
#define EXPANSION_SPI 1
#define MAX_SPI       2

// UART
#define GCODE_UART  0
#define DEBUG_UART  1
#define MAX_UART    2

#define UART_RECEIVE_BUFFER_SIZE  512
#define UART_SEND_BUFFER_SIZE     2048


// Heaters
#define NUMBER_OF_HEATERS 5

// LEDs
#define DEBUG_LED  0
#define ISR_1_LED  1
#define ERROR_LED  2
#define MAX_LED    3

#define BOARD_HAS_DEBUG_LED    1
#define BOARD_HAS_ERROR_LED    1
#define BOARD_HAS_ISR_1_LED    1

// Timer

#define PWM_2_TIMER                1
#define PWM_2_Channel              1

#define PWM_0_TIMER                1
#define PWM_0_Channel              2

#define PWM_5_TIMER                1
#define PWM_5_Channel              4

#define PWM_4_TIMER                2
#define PWM_4_Channel              3

#define PWM_1_TIMER                2
#define PWM_1_Channel              4

#define PWM_6_TIMER                3
#define PWM_6_Channel              3

#define PWM_7_TIMER                3
#define PWM_7_Channel              4

#define PWM_3_TIMER                4
#define PWM_3_Channel              3

#define STEP_TIMER                 6
#define STEP_BUFFER_TIMER          7

#define PWM_8_TIMER                9
#define PWM_8_Channel              1

#define PWM_9_TIMER                9
#define PWM_9_Channel              2

#define BUZZER_TIMER               11

#define STEP_ISR_FUNCTION static void step_isr(void)

#endif /* HAL_INCLUDE_HAL_CFG_H_ */
