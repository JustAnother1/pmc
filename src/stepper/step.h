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

#ifndef STEP_H_
#define STEP_H_

#include <inttypes.h>
#include <stdbool.h>

// max allowed number of steppers
#define MAX_NUMBER 8
// bit offset of direction signal in stepper port
#define DIRECTION_OFFSET 8

#define STEP_CHUNK_SIZE             10
#define STEP_BUFFER_SIZE            (10 * STEP_CHUNK_SIZE) + 1

#define SLOT_TYPE_EMPTY             0
#define SLOT_TYPE_DELAY             1
#define SLOT_TYPE_BASIC_LINEAR_MOVE 2

#define MOVE_PHASE_ACCELLERATE      0
#define MOVE_PHASE_CONSTANT_SPEED   1
#define MOVE_PHASE_DECELERATE       2
#ifdef STEP_TIMER_FREQ_12MHZ
// Timer runs on 12 MHz clock.
#define TICKS_PER_SECOND            (12L*1000L*1000L)
#define STEP_TIME_ONE_MS            (TICKS_PER_SECOND/1000)
#define REFILL_BUFFER_RELOAD        3000
#endif
#ifdef STEP_TIMER_FREQ_16MHZ
// Timer runs on 16 MHz clock.
#define TICKS_PER_SECOND            (16L*1000L*1000L)
#define STEP_TIME_ONE_MS            (TICKS_PER_SECOND/1000)
#define REFILL_BUFFER_RELOAD        4000
#endif

void step_init(uint_fast8_t num_stepper);
// returns TRUE wile execution a Move.
// returns FALSE when ready for the next move
bool step_is_busy(void);
bool step_is_homing(void);

bool step_add_basic_linear_move(uint_fast8_t *move_data);
bool step_add_delay(uint_fast16_t ms);
void step_disable_all_motors(void);
void step_enable_motor(uint_fast8_t stepper_number, uint_fast8_t on_off);
void step_end_stop_hit_on(uint_fast8_t stepper_number, bool max);
void refill_step_buffer(void);
bool step_is_moving_towards(uint_fast8_t stepper_number, bool max);


#ifdef DEBUG_ACTIVE
void step_print_state(void);
#endif

#endif /* STEP_H_ */
