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

#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <stdio.h>
#include <ctype.h>
#include <stdint.h>
#include "cmocka.h"
#include "step.h"


// Step Timer
extern volatile bool step_timer_running;
extern volatile uint_fast8_t read_pos;
extern volatile uint_fast8_t write_pos;

// step buffer timer
extern volatile bool buffer_timer_running;

// step queue
extern volatile bool busy;
extern volatile bool reached_tag;
extern volatile uint_fast8_t cur_slot_type;
// to handle delays:
extern volatile uint_fast16_t delay_ms;
// to handle Basic Linear Move:
extern volatile uint_fast16_t active_axes_map;
extern volatile uint_fast8_t primary_axis;
extern bool is_a_homing_move;
extern uint_fast8_t direction_for_move;
extern volatile uint_fast8_t start_speed;
extern volatile float start_speed_ticks;
extern volatile uint_fast8_t nominal_speed;
extern volatile float nominal_speed_ticks;
extern volatile uint_fast8_t end_speed;
extern volatile float end_speed_ticks;
extern volatile uint_fast16_t acceleration_steps;
extern volatile uint_fast16_t decelleration_steps;
extern volatile uint_fast16_t steps_on_axis[MAX_NUMBER];
extern volatile uint_fast8_t phase_of_move;
extern volatile uint_fast16_t steps_in_this_phase_on_axis[MAX_NUMBER];
extern volatile uint_fast16_t steps_already_made[MAX_NUMBER];
extern volatile uint_fast32_t curTime;
extern volatile float speed_increse_acc_tick;
extern volatile float speed_decrese_decel_tick;

// enable / disable Stepper
extern uint_fast8_t available_steppers;
extern bool enabled[MAX_NUMBER];

#ifdef USE_STEP_DIR
extern volatile uint32_t next_step[STEP_BUFFER_SIZE];
extern volatile uint32_t cur_step;
extern volatile uint_fast8_t last_direction_axis;
#else
extern volatile uint_fast8_t move_on_axis[STEP_BUFFER_SIZE];
extern volatile uint_fast8_t next_direction[STEP_BUFFER_SIZE];
#endif
extern volatile uint_fast16_t next_reload[STEP_BUFFER_SIZE];


// void debug_init(void);
static void test_step_init(void **state)
{
    step_init(0);
    assert_int_equal(0, available_steppers);
    step_init(8);
    assert_int_equal(8, available_steppers);
}

// static uint_fast8_t get_number_of_free_slots(void);
static void test_get_number_of_free_slots(void **state)
{
    // empty buffer
    write_pos = 0;
    read_pos = 0;
    assert_int_equal(STEP_BUFFER_SIZE, get_number_of_free_slots());
    // one element in the buffer
    write_pos = 1;
    read_pos = 0;
    assert_int_equal(STEP_BUFFER_SIZE - 1, get_number_of_free_slots());
    // Data at element 3 and 4
    write_pos = 5;
    read_pos = 3;
    assert_int_equal(STEP_BUFFER_SIZE - 2, get_number_of_free_slots());
    // already used but empty buffer
    write_pos = 5;
    read_pos = 5;
    assert_int_equal(STEP_BUFFER_SIZE, get_number_of_free_slots());
    // wrap around
    write_pos = 3;
    read_pos = 5;
    assert_int_equal(2, get_number_of_free_slots());
}

// static void finished_cur_slot(void);
static void test_finished_cur_slot(void **state)
{
    busy = true;
    reached_tag = false;
    cur_slot_type = SLOT_TYPE_DELAY;
    finished_cur_slot();
    assert_false(busy);
    assert_true(reached_tag);
    assert_int_equal(SLOT_TYPE_EMPTY, cur_slot_type);
}

// static void caclculate_basic_move_chunk(uint_fast8_t num_slots);
// static uint_fast16_t get_reload_primary_axis(void);
// static void get_steps_for_this_phase(float factor);
// static void calculate_step_chunk(uint_fast8_t num_slots);


// static void make_the_needed_steps(uint_fast16_t reload_time);
// static void do_step_on_axis(uint_fast8_t i, uint_fast16_t reload_time);
// static void auto_activate_usedAxis(void);
// static void step_isr(void);
// static void refill_step_buffer(void);

// void step_init(uint_fast8_t num_stepper);
// // returns TRUE wile execution a Move.
// // returns FALSE when ready for the next move
// bool step_is_busy(void);
// bool step_add_basic_linear_move(uint_fast8_t *move_data);
// bool step_add_delay(uint_fast16_t ms);
// void step_disable_all_motors(void);
// void step_enable_motor(uint_fast8_t stepper_number, uint_fast8_t on_off);

// void step_print_state(void);
static void test_step_print_state(void **state)
{
    step_print_state();
}

#ifdef USE_STEP_DIR
// static uint32_t toggle_bit(uint_fast8_t bit, uint32_t value);
static void test_toggle_bit(void **state)
{
    assert_int_equal(0, toggle_bit(0, 1));
    assert_int_equal(1, toggle_bit(0, 0));
    assert_int_equal(3, toggle_bit(1, 1));
}
#else
#endif

int main(void)
{
    const struct CMUnitTest tests[] = {
            cmocka_unit_test(test_step_init),
            cmocka_unit_test(test_get_number_of_free_slots),
            cmocka_unit_test(test_finished_cur_slot),
            cmocka_unit_test(test_step_print_state),
#ifdef USE_STEP_DIR
            cmocka_unit_test(test_toggle_bit),
#else
#endif
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}

