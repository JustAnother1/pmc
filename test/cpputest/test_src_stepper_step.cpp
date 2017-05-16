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

extern "C"
{
#include "step.h"
#include "hal_stepper_port.h"

extern uint_fast8_t direction_for_move;  // bitmap
extern volatile uint_fast16_t active_axes_map;
extern uint_fast8_t available_steppers;
extern bool enabled[MAX_NUMBER];
extern bool is_a_homing_move;
extern volatile bool busy;
extern volatile bool step_timer_running;
extern volatile bool buffer_timer_running;
extern uint_fast8_t available_steppers;
extern volatile uint_fast16_t steps_on_axis[MAX_NUMBER];
extern volatile uint_fast16_t steps_in_this_phase_on_axis[MAX_NUMBER];
extern volatile uint_fast8_t last_direction_axis;
extern volatile uint_fast8_t phase_of_move;
extern uint_fast16_t speed_reloads[256];
extern volatile uint_fast8_t nominal_speed;
extern volatile uint_fast8_t cur_slot_type;
extern volatile uint_fast8_t read_pos;
extern volatile uint_fast8_t write_pos;
extern volatile float error_on_axis[MAX_NUMBER];
extern volatile uint_fast32_t curTime;
extern volatile uint_fast8_t primary_axis;
extern volatile uint_fast16_t steps_already_made[MAX_NUMBER];
extern volatile uint32_t next_step[STEP_BUFFER_SIZE];
extern volatile uint_fast16_t delay_ms;
extern volatile uint_fast16_t decelleration_steps;


void step_isr_function(void);
uint_fast8_t get_number_of_free_slots(void);
void finished_cur_slot(void);
void increment_write_pos(void);
void calculate_step_chunk(uint_fast8_t num_slots);
void caclculate_basic_move_chunk(uint_fast8_t num_slots);
uint_fast16_t get_reload_primary_axis(void);
uint32_t toggle_bit(uint_fast8_t bit, uint32_t value);
void make_the_needed_steps(uint_fast16_t reload_time);
void get_steps_for_this_phase(float factor);
void auto_activate_usedAxis(void);
bool check_end_stops(void);

}

TEST_GROUP(SrcStepperStepTestGroup)
{

};

TEST(SrcStepperStepTestGroup, step_print_state)
{
    busy = false;
    buffer_timer_running = false;
    step_timer_running = false;
    step_print_state();
    step_timer_running = true;
    step_print_state();
    buffer_timer_running = true;
    step_print_state();
    busy = true;
    step_print_state();
}

TEST(SrcStepperStepTestGroup, step_end_stop_hit_on)
{
    direction_for_move = 0;
    active_axes_map = 0;
    step_end_stop_hit_on(0, false);

    active_axes_map = 1;
    direction_for_move = 1;
    step_end_stop_hit_on(0, true);
}

TEST(SrcStepperStepTestGroup, step_is_moving_towards)
{
    direction_for_move = 0;
    active_axes_map = 0;
    CHECK_FALSE(step_is_moving_towards(0, false));

    CHECK_FALSE(step_is_moving_towards(0, true));

    active_axes_map = 1;
    direction_for_move = 0;
    CHECK_FALSE(step_is_moving_towards(0, true));

    CHECK(step_is_moving_towards(0, false));

    active_axes_map = 1;
    direction_for_move = 1;
    CHECK(step_is_moving_towards(0, true));

    CHECK_FALSE(step_is_moving_towards(0, false));
}

TEST(SrcStepperStepTestGroup, step_enable_motor)
{
    for(int i = 0; i < MAX_NUMBER; i++)
    {
        enabled[i] = false;
    }
    available_steppers = 5;
    step_enable_motor(0, 0);
    step_enable_motor(6, 0);
    CHECK_FALSE(enabled[0]);
    CHECK_FALSE(enabled[6]);
    enabled[2] = true;
    step_enable_motor(2, 0);
    CHECK_FALSE(enabled[2]);
    step_enable_motor(2, 1);
    CHECK(enabled[2]);
    step_enable_motor(2, 1);
    CHECK(enabled[2]);
    step_enable_motor(3, 3);
    CHECK_FALSE(enabled[3]);
}

TEST(SrcStepperStepTestGroup, step_disable_all_motors)
{
    for(int i = 0; i < MAX_NUMBER; i++)
    {
        enabled[i] = true;
    }
    available_steppers = 5;
    enabled[3] = false;
    step_disable_all_motors();
    CHECK_FALSE(enabled[0]);
    CHECK_FALSE(enabled[1]);
    CHECK_FALSE(enabled[2]);
    CHECK_FALSE(enabled[3]);
    CHECK_FALSE(enabled[4]);
    CHECK(enabled[5]);
    CHECK(enabled[6]);
    CHECK(enabled[7]);
}

TEST(SrcStepperStepTestGroup, step_is_homing)
{
    is_a_homing_move = true;
    CHECK(step_is_homing());
}

TEST(SrcStepperStepTestGroup, step_is_busy)
{
    busy = true;
    CHECK(step_is_busy());
}

TEST(SrcStepperStepTestGroup, step_add_delay)
{
    busy = true;
    CHECK_FALSE(step_add_delay(10));
    busy = false;
    CHECK(step_add_delay(10));
    CHECK(busy);
}

TEST(SrcStepperStepTestGroup, step_add_basic_linear_move)
{
    uint_fast8_t move_data[14] = {0};
    move_data[0] = 0x03;
    busy = true;
    CHECK_FALSE(step_add_basic_linear_move(move_data));

    busy = false;
    move_data[1] = 0x03; // 1 byte axis selection (0..7), 2 active axis: 0, 1
    move_data[2] = 0x02; // 1 byte step counts, Direction on 0 = 0, direction on 1 = 1;
    move_data[3] = 0x01; // primary axis is 1, not homing
    move_data[4] = 13;   // nominal speed
    move_data[5] = 5;    // end speed
    move_data[6] = 10;   // acceleration steps
    move_data[7] = 9;    // deceleration steps
    move_data[8] = 20;   // steps on axis 0
    move_data[9] = 200;  // steps on axis 1
    CHECK(step_add_basic_linear_move(move_data));

    busy = false;
    move_data[1] = 0x03; // 1 byte axis selection (0..7), 2 active axis: 0, 1
    move_data[2] = 0x02; // 1 byte step counts, Direction on 0 = 0, direction on 1 = 1;
    move_data[3] = 0x11; // primary axis is 1, homing
    move_data[4] = 13;   // nominal speed
    move_data[5] = 5;    // end speed
    move_data[6] = 10;   // acceleration steps
    move_data[7] = 9;    // deceleration steps
    move_data[8] = 20;   // steps on axis 0
    move_data[9] = 200;  // steps on axis 1
    CHECK(step_add_basic_linear_move(move_data));

    busy = false;
    move_data[ 1] = 0x03; // 1 byte axis selection (0..7), 2 active axis: 0, 1
    move_data[ 2] = 0x82; // 2 byte step counts, Direction on 0 = 0, direction on 1 = 1;
    move_data[ 3] = 0x01; // primary axis is 1, not homing
    move_data[ 4] = 13;   // nominal speed
    move_data[ 5] = 5;    // end speed
    move_data[ 6] = 0x02; // acceleration steps
    move_data[ 7] = 0x34; // acceleration steps
    move_data[ 8] = 0x01; // deceleration steps
    move_data[ 9] = 0x23; // deceleration steps
    move_data[10] = 0x54; // steps on axis 0
    move_data[11] = 0x32; // steps on axis 0
    move_data[12] = 0x98; // steps on axis 1
    move_data[13] = 0x76; // steps on axis 1
    CHECK(step_add_basic_linear_move(move_data));

    busy = false;
    move_data[ 1] = 0x03; // 1 byte axis selection (0..7), 2 active axis: 0, 1
    move_data[ 2] = 0x82; // 2 byte step counts, Direction on 0 = 0, direction on 1 = 1;
    move_data[ 3] = 0x11; // primary axis is 1, homing
    move_data[ 4] = 13;   // nominal speed
    move_data[ 5] = 5;    // end speed
    move_data[ 6] = 0x02; // acceleration steps
    move_data[ 7] = 0x34; // acceleration steps
    move_data[ 8] = 0x01; // deceleration steps
    move_data[ 9] = 0x23; // deceleration steps
    move_data[10] = 0x54; // steps on axis 0
    move_data[11] = 0x32; // steps on axis 0
    move_data[12] = 0x98; // steps on axis 1
    move_data[13] = 0x76; // steps on axis 1
    CHECK(step_add_basic_linear_move(move_data));

}

TEST(SrcStepperStepTestGroup, step_init)
{
    step_init(MAX_NUMBER + 1);
    CHECK_EQUAL(MAX_NUMBER, available_steppers);

    step_init(3);
    CHECK_EQUAL(3, available_steppers);
}

TEST(SrcStepperStepTestGroup, check_end_stops)
{
    direction_for_move = 0;
    active_axes_map = 0;
    CHECK_FALSE(check_end_stops());

    direction_for_move = 0;
    active_axes_map = 1;
    CHECK(check_end_stops());

    direction_for_move = 1;
    active_axes_map = 1;
    CHECK(check_end_stops());
}

TEST(SrcStepperStepTestGroup, auto_activate_usedAxis)
{
    active_axes_map = 0;
    auto_activate_usedAxis();
    active_axes_map = 1;
    auto_activate_usedAxis();
    active_axes_map = 2;
    auto_activate_usedAxis();
    active_axes_map = 4;
    auto_activate_usedAxis();
    active_axes_map = 8;
    auto_activate_usedAxis();
    active_axes_map = 16;
    auto_activate_usedAxis();
    active_axes_map = 32;
    auto_activate_usedAxis();
    active_axes_map = 64;
    auto_activate_usedAxis();
    active_axes_map = 128;
    auto_activate_usedAxis();
}

TEST(SrcStepperStepTestGroup, get_steps_for_this_phase)
{
    active_axes_map = 3;
    for(int i = 0; i < MAX_NUMBER; i++)
    {
        steps_on_axis[i] = 30;
    }
    get_steps_for_this_phase(0.3);
    CHECK_EQUAL(9, steps_in_this_phase_on_axis[0]);
    CHECK_EQUAL(9, steps_in_this_phase_on_axis[1]);
    CHECK_EQUAL(0, steps_in_this_phase_on_axis[2]);
    CHECK_EQUAL(0, steps_in_this_phase_on_axis[3]);
    CHECK_EQUAL(0, steps_in_this_phase_on_axis[4]);
    CHECK_EQUAL(0, steps_in_this_phase_on_axis[5]);
    CHECK_EQUAL(0, steps_in_this_phase_on_axis[6]);
    CHECK_EQUAL(0, steps_in_this_phase_on_axis[7]);

    active_axes_map = 3;
    for(int i = 0; i < MAX_NUMBER; i++)
    {
        steps_on_axis[i] = 30;
    }
    get_steps_for_this_phase(1.0);
    CHECK_EQUAL(30, steps_in_this_phase_on_axis[0]);
    CHECK_EQUAL(30, steps_in_this_phase_on_axis[1]);
    CHECK_EQUAL(0, steps_in_this_phase_on_axis[2]);
    CHECK_EQUAL(0, steps_in_this_phase_on_axis[3]);
    CHECK_EQUAL(0, steps_in_this_phase_on_axis[4]);
    CHECK_EQUAL(0, steps_in_this_phase_on_axis[5]);
    CHECK_EQUAL(0, steps_in_this_phase_on_axis[6]);
    CHECK_EQUAL(0, steps_in_this_phase_on_axis[7]);
}

TEST(SrcStepperStepTestGroup, make_the_needed_steps)
{
    direction_for_move = 0;
    last_direction_axis = 0;
    active_axes_map = 0;
    for(int i = 0; i < MAX_NUMBER; i++)
    {
        error_on_axis[i] = 0;
    }
    make_the_needed_steps(0);

    direction_for_move = 3;
    last_direction_axis = 0;
    active_axes_map = 0;
    for(int i = 0; i < MAX_NUMBER; i++)
    {
        error_on_axis[i] = 0;
    }
    make_the_needed_steps(0);

    direction_for_move = 0;
    last_direction_axis = 0;
    active_axes_map = 0;
    for(int i = 0; i < MAX_NUMBER; i++)
    {
        error_on_axis[i] = 0;
    }
    error_on_axis[2] = 1.3;
    make_the_needed_steps(0);
}

TEST(SrcStepperStepTestGroup, toggle_bit)
{
    CHECK_EQUAL(0, toggle_bit(0,1));
    CHECK_EQUAL(1, toggle_bit(0,0));
    CHECK_EQUAL(2, toggle_bit(1,0));
}

TEST(SrcStepperStepTestGroup, get_reload_primary_axis)
{
    phase_of_move = 120;
    CHECK_EQUAL(0xfffd, get_reload_primary_axis());

    phase_of_move = MOVE_PHASE_ACCELLERATE;
    curTime = 0;
    nominal_speed = 255;
#ifdef STEP_TIMER_FREQ_12MHZ
    CHECK_EQUAL(300, get_reload_primary_axis());
#else
    #ifdef STEP_TIMER_FREQ_16MHZ
        CHECK_EQUAL(400, get_reload_primary_axis());
    #else
        FAIL("Step Timer Frequency not defined!")
    #endif
#endif

    phase_of_move = MOVE_PHASE_CONSTANT_SPEED;
    nominal_speed = 255;
#ifdef STEP_TIMER_FREQ_12MHZ
    CHECK_EQUAL(300, get_reload_primary_axis());
#else
    #ifdef STEP_TIMER_FREQ_16MHZ
        CHECK_EQUAL(400, get_reload_primary_axis());
    #else
        FAIL("Step Timer Frequency not defined!")
    #endif
#endif

    phase_of_move = MOVE_PHASE_DECELERATE;
    curTime = 0;
    nominal_speed = 255;
#ifdef STEP_TIMER_FREQ_12MHZ
    CHECK_EQUAL(300, get_reload_primary_axis());
#else
    #ifdef STEP_TIMER_FREQ_16MHZ
        CHECK_EQUAL(0, get_reload_primary_axis());
    #else
        FAIL("Step Timer Frequency not defined!")
    #endif
#endif
}

TEST(SrcStepperStepTestGroup, caclculate_basic_move_chunk)
{
    caclculate_basic_move_chunk(0);

    primary_axis = 0;
    steps_already_made[0] = 0;
    steps_in_this_phase_on_axis[0] = 1;
    caclculate_basic_move_chunk(1);

    primary_axis = 0;
    steps_already_made[0] = 1;
    steps_on_axis[primary_axis] = 5;
    decelleration_steps = 0;
    steps_in_this_phase_on_axis[0] = 1;
    phase_of_move = MOVE_PHASE_ACCELLERATE;
    caclculate_basic_move_chunk(1);

    primary_axis = 0;
    steps_already_made[0] = 1;
    steps_in_this_phase_on_axis[0] = 1;
    steps_on_axis[primary_axis] = 0;
    decelleration_steps = 5;
    phase_of_move = MOVE_PHASE_ACCELLERATE;
    caclculate_basic_move_chunk(1);

    primary_axis = 0;
    steps_already_made[0] = 1;
    steps_in_this_phase_on_axis[0] = 1;
    steps_on_axis[primary_axis] = 0;
    decelleration_steps = 0;
    phase_of_move = MOVE_PHASE_ACCELLERATE;
    caclculate_basic_move_chunk(1);

    primary_axis = 0;
    steps_already_made[0] = 1;
    steps_in_this_phase_on_axis[0] = 1;
    phase_of_move = MOVE_PHASE_CONSTANT_SPEED;
    caclculate_basic_move_chunk(1);

    primary_axis = 0;
    steps_already_made[0] = 1;
    steps_in_this_phase_on_axis[0] = 1;
    phase_of_move = MOVE_PHASE_DECELERATE;
    caclculate_basic_move_chunk(1);

    primary_axis = 0;
    steps_already_made[0] = 1;
    steps_in_this_phase_on_axis[0] = 1;
    phase_of_move = 120;
    caclculate_basic_move_chunk(1);
}

TEST(SrcStepperStepTestGroup, calculate_step_chunk)
{
    cur_slot_type = SLOT_TYPE_BASIC_LINEAR_MOVE;
    calculate_step_chunk(0);

    busy = true;
    delay_ms = 0;
    cur_slot_type = SLOT_TYPE_DELAY;
    calculate_step_chunk(10);
    CHECK_FALSE(busy);

    busy = true;
    delay_ms = 5;
    cur_slot_type = SLOT_TYPE_DELAY;
    calculate_step_chunk(10);
    CHECK_FALSE(busy);

    cur_slot_type = SLOT_TYPE_EMPTY;
    calculate_step_chunk(0);

    cur_slot_type = 120;
    calculate_step_chunk(0);
}

TEST(SrcStepperStepTestGroup, increment_write_pos)
{
    write_pos = 0;
    increment_write_pos();
    CHECK_EQUAL(1, write_pos);

    write_pos = STEP_BUFFER_SIZE -1;
    increment_write_pos();
    CHECK_EQUAL(0, write_pos);
}

TEST(SrcStepperStepTestGroup, finished_cur_slot)
{
    busy = true;
    cur_slot_type = SLOT_TYPE_BASIC_LINEAR_MOVE;
    finished_cur_slot();
    CHECK_EQUAL(SLOT_TYPE_EMPTY, cur_slot_type);
    CHECK_FALSE(busy);
}

TEST(SrcStepperStepTestGroup, get_number_of_free_slots)
{
    read_pos = 5;
    write_pos = 4;
    CHECK_EQUAL(0, get_number_of_free_slots());

    read_pos = 4;
    write_pos = 5;
    CHECK_EQUAL(STEP_BUFFER_SIZE -2, get_number_of_free_slots());
}

TEST(SrcStepperStepTestGroup, refill_step_buffer)
{
    read_pos = 5;
    write_pos = 4;
    step_timer_running = true;
    refill_step_buffer();

    read_pos = 5;
    write_pos = 5;
    step_timer_running = false;
    refill_step_buffer();
}

TEST(SrcStepperStepTestGroup, step_isr_function)
{
    hal_stepper_port_init();
    read_pos = 5;
    write_pos = 5;
    step_isr_function();
    CHECK_EQUAL(0, hal_stepper_get_Output());

    hal_stepper_port_init();
    read_pos = 0;
    write_pos = 1;
    next_step[0] = 0x12345678;
    step_isr_function();
    CHECK_EQUAL(1, read_pos);
    CHECK_EQUAL(0x12345678, hal_stepper_get_Output());

    hal_stepper_port_init();
    read_pos = STEP_BUFFER_SIZE -1;
    write_pos = 1;
    next_step[read_pos] = 0x87654321;
    step_isr_function();
    CHECK_EQUAL(0, read_pos);
    CHECK_EQUAL(0x87654321, hal_stepper_get_Output());
}

