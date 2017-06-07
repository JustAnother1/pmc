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

void caclculate_basic_move_chunk(uint_fast8_t num_slots);
uint_fast16_t get_reload_primary_axis(void);
void get_steps_for_this_phase(float factor);
void calculate_step_chunk(uint_fast8_t num_slots);
uint_fast8_t get_number_of_free_slots(void);
void finished_cur_slot(void);
void make_the_needed_steps(uint_fast16_t reload_time);
void auto_activate_usedAxis(void);
void increment_write_pos(void);
bool check_end_stops(void);
uint32_t toggle_bit(uint_fast8_t bit, uint32_t value);
void step_isr(void);


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
    assert_int_equal(STEP_BUFFER_SIZE -1, get_number_of_free_slots());
    // one element in the buffer
    write_pos = 1;
    read_pos = 0;
    assert_int_equal(STEP_BUFFER_SIZE - 2, get_number_of_free_slots());
    // Data at element 3 and 4
    write_pos = 5;
    read_pos = 3;
    assert_int_equal(STEP_BUFFER_SIZE - 3, get_number_of_free_slots());
    // already used but empty buffer
    write_pos = 5;
    read_pos = 5;
    assert_int_equal(STEP_BUFFER_SIZE -1, get_number_of_free_slots());
    // wrap around
    write_pos = 3;
    read_pos = 5;
    assert_int_equal(1, get_number_of_free_slots());
}

// static void finished_cur_slot(void);
static void test_finished_cur_slot(void **state)
{
    busy = true;
    cur_slot_type = SLOT_TYPE_DELAY;
    finished_cur_slot();
    assert_false(busy);
    assert_int_equal(SLOT_TYPE_EMPTY, cur_slot_type);
}

// static void caclculate_basic_move_chunk(uint_fast8_t num_slots);
// static uint_fast16_t get_reload_primary_axis(void);
// static void get_steps_for_this_phase(float factor);
// static void calculate_step_chunk(uint_fast8_t num_slots);
// static void make_the_needed_steps(uint_fast16_t reload_time);
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
    step_timer_running = false;
    buffer_timer_running = false;
    busy = false;
    step_print_state();
}

static void test_step_print_state_busy(void **state)
{
    step_timer_running = false;
    buffer_timer_running = false;
    busy = true;
    step_print_state();
}

static void test_step_print_state_buffer_timer(void **state)
{
    step_timer_running = false;
    buffer_timer_running = true;
    busy = false;
    step_print_state();
}

static void test_step_print_state_step_timer(void **state)
{
    step_timer_running = true;
    buffer_timer_running = false;
    busy = false;
    step_print_state();
}

// void step_end_stop_hit_on(uint_fast8_t stepper_number, bool max)
static void test_step_end_stop_hit_on(void **state)
{
    step_end_stop_hit_on(0, false);
}

// void step_enable_motor(uint_fast8_t stepper_number, uint_fast8_t on_off)
static void test_step_enable_motor(void **state)
{
    step_enable_motor(0, false);
}

// void step_disable_all_motors(void)
static void test_step_disable_all_motors(void **state)
{
    step_disable_all_motors();
}

// bool step_is_homing(void)
static void test_step_is_homing(void **state)
{
    is_a_homing_move = false;
    assert_false(step_is_homing());
    is_a_homing_move = true;
    assert_true(step_is_homing());
}

// bool step_is_busy(void)
static void test_step_is_busy(void **state)
{
    busy = false;
    assert_false(step_is_busy());
    busy = true;
    assert_true(step_is_busy());
}

// bool step_add_delay(uint_fast16_t ms)
static void test_step_add_delay(void **state)
{
    busy = true;
    assert_false(step_add_delay(5));
    busy = false;
    assert_true(step_add_delay(5));
}

// bool step_add_basic_linear_move(uint_fast8_t *move_data)
static void test_step_add_basic_linear_move(void **state)
{
    busy = true;
    assert_false(step_add_basic_linear_move(NULL));
}

// static void auto_activate_usedAxis(void)
static void test_auto_activate_usedAxis(void **state)
{
    active_axes_map = 0;
    auto_activate_usedAxis();
    active_axes_map = 0xff;
    auto_activate_usedAxis();
}

// static void get_steps_for_this_phase(float factor)
static void test_get_steps_for_this_phase(void **state)
{
    get_steps_for_this_phase(1.0);
}

// static void make_the_needed_steps(uint_fast16_t reload_time)
static void test_make_the_needed_steps(void **state)
{
    make_the_needed_steps(300);
}

// static uint_fast16_t get_reload_primary_axis(void)
static void test_get_reload_primary_axis(void **state)
{
    phase_of_move = 42;
    assert_int_equal(0xfffd, get_reload_primary_axis());
}

// static void caclculate_basic_move_chunk(uint_fast8_t num_slots)
static void test_caclculate_basic_move_chunk(void **state)
{
    caclculate_basic_move_chunk(5);
}

// static void calculate_step_chunk(uint_fast8_t num_slots)
static void test_calculate_step_chunk(void **state)
{
    calculate_step_chunk(5);
}

// static void increment_write_pos(void)
static void test_increment_write_pos(void **state)
{
    write_pos = 0;
    increment_write_pos();
    assert_int_equal(1, write_pos);

    write_pos = STEP_BUFFER_SIZE -1;
    increment_write_pos();
    assert_int_equal(0, write_pos);
}

// static void refill_step_buffer(void)
static void test_refill_step_buffer(void **state)
{
    refill_step_buffer();
}

// static void step_isr(void)
static void test_step_isr(void **state)
{
    step_isr();
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
            cmocka_unit_test(test_step_print_state_busy),
            cmocka_unit_test(test_step_print_state_buffer_timer),
            cmocka_unit_test(test_step_print_state_step_timer),
            cmocka_unit_test(test_step_end_stop_hit_on),
            cmocka_unit_test(test_step_enable_motor),
            cmocka_unit_test(test_step_disable_all_motors),
            cmocka_unit_test(test_step_is_homing),
            cmocka_unit_test(test_step_is_busy),
            cmocka_unit_test(test_step_add_delay),
            cmocka_unit_test(test_step_add_basic_linear_move),
            cmocka_unit_test(test_auto_activate_usedAxis),
            cmocka_unit_test(test_get_steps_for_this_phase),
            cmocka_unit_test(test_make_the_needed_steps),
            cmocka_unit_test(test_get_reload_primary_axis),
            cmocka_unit_test(test_caclculate_basic_move_chunk),
            cmocka_unit_test(test_calculate_step_chunk),
            cmocka_unit_test(test_increment_write_pos),
            cmocka_unit_test(test_refill_step_buffer),
            cmocka_unit_test(test_step_isr),
#ifdef USE_STEP_DIR
            cmocka_unit_test(test_toggle_bit),
#else
#endif
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}

