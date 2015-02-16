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

#include "step.h"
#include "error.h"
#include "hal_cfg.h"
#include "hal_spi.h"
#include "trinamic.h"
#include "hal_time.h"
#include "hal_cfg.h"
#include "hal_stepper_port.h"


#define STEP_CHUNK_SIZE             10
#define STEP_BUFFER_SIZE            10 * STEP_CHUNK_SIZE

#define SLOT_TYPE_EMPTY             0
#define SLOT_TYPE_DELAY             1
#define SLOT_TYPE_BASIC_LINEAR_MOVE 2

#define MOVE_PHASE_ACCELLERATE      0
#define MOVE_PHASE_CONSTANT_SPEED   1
#define MOVE_PHASE_DECELERATE       2
// Timer runs on 12 MHz clock.
#define TICKS_PER_SECOND            12000000
#define STEP_TIME_ONE_MS            (TICKS_PER_SECOND/1000)


// Timer:
static void step_isr(void);
static void refill_step_buffer(void);

static void caclculate_basic_move_chunk(uint_fast8_t num_slots);
static uint_fast16_t get_reload_primary_axis(void);
static void get_steps_for_this_phase(float factor);
static void calculate_step_chunk(uint_fast8_t num_slots);
static uint_fast8_t get_number_of_free_slots(void);
static void finished_cur_slot(void);
static void make_the_needed_steps(void);
static void do_step_on_axis(uint_fast8_t i);
static void auto_activate_usedAxis(void);

// Step Timer
static bool step_timer_running;
static uint_fast8_t step_pos = 0;
static uint_fast8_t stop_pos = 0;

// step buffer timer
static bool buffer_timer_running;

// step queue
static bool busy;
static bool reached_tag;
static uint_fast8_t cur_slot_type;
// to handle delays:
static uint_fast16_t delay_ms;
// to handle Basic Linear Move:
static uint_fast16_t active_axes_map;
static uint_fast8_t primary_axis;
static bool is_a_homing_move;
static uint_fast8_t direction_for_move;
static uint_fast8_t start_speed;
static uint_fast8_t nominal_speed;
static uint_fast8_t end_speed;
static uint_fast16_t acceleration_steps;
static uint_fast16_t decelleration_steps;
static uint_fast16_t steps_on_axis[MAX_NUMBER];
static uint_fast8_t phase_of_move;
static uint_fast16_t steps_in_this_phase_on_axis[MAX_NUMBER];
static uint_fast16_t steps_already_made[MAX_NUMBER];
static uint_fast32_t curTime;
static float start_speed_ticks;
static float nominal_speed_ticks;
static float curSpeed;
static float speed_increse_acc_tick;
static float speed_decrese_decel_tick;
static float average_speed_acceleration;
static float average_speed_deceleration;


// enable / disable Stepper
static uint_fast8_t available_steppers;
static bool enabled[MAX_NUMBER];



#ifdef USE_STEP_DIR
static uint32_t next_step[STEP_BUFFER_SIZE];
static uint32_t cur_step;
#else
static uint_fast8_t move_on_axis[STEP_BUFFER_SIZE];
static uint_fast8_t next_direction[STEP_BUFFER_SIZE];
#endif
static uint_fast16_t next_reload[STEP_BUFFER_SIZE];

/*
static uint_fast8_t start_speed;

static uint_fast16_t next_move_on_axis_in[MAX_NUMBER];
*/



// 16bit Timer running at 12MHz
static uint_fast16_t speed_reloads[256] ={
        65535,  65535, 38250, 25500, 19125, 15300, 12750, 10929,
        9563,  8500,  7650,  6955,  6373,  5885,  5464,  5100,
        4781,  4500,  4250,  4026,  3825,  3643,  3477,  3326,
        3188,  3060,  2942,  2833,  2732,  2638,  2550,  2468,
        2391,  2318,  2250,  2186,  2125,  2068,  2013,  1962,
        1913,  1866,  1821,  1779,  1739,  1700,  1663,  1628,
        1594,  1561,  1530,  1500,  1471,  1443,  1417,  1391,
        1366,  1342,  1319,  1297,  1275,  1254,  1234,  1214,

        1195,  1177,  1159,  1142,  1125,  1109,  1093,  1077,
        1063,  1048,  1034,  1020,  1007,   994,   981,   968,
         956,   944,   933,   922,   911,   900,   890,   879,
         869,   860,   850,   841,   832,   823,   814,   805,
         797,   789,   781,   773,   765,   757,   750,   743,
         736,   729,   722,   715,   708,   702,   695,   689,
         683,   677,   671,   665,   659,   654,   648,   643,
         638,   632,   627,   622,   617,   612,   607,   602,

         598,   593,   588,   584,   580,   575,   571,   567,
         563,   558,   554,   550,   546,   543,   539,   535,
         531,   528,   524,   520,   517,   513,   510,   507,
         503,   500,   497,   494,   490,   487,   484,   481,
         478,   475,   472,   469,   466,   464,   461,   458,
         455,   453,   450,   447,   445,   442,   440,   437,
         435,   432,   430,   427,   425,   423,   420,   418,
         416,   414,   411,   409,   407,   405,   403,   401,

         398,   396,   394,   392,   390,   388,   386,   384,
         383,   381,   379,   377,   375,   373,   371,   370,
         368,   366,   364,   363,   361,   359,   357,   356,
         354,   353,   351,   349,   348,   346,   345,   343,
         342,   340,   338,   337,   336,   334,   333,   331,
         330,   328,   327,   326,   324,   323,   321,   320,
         319,   317,   316,   315,   314,   312,   311,   310,
         308,   307,   306,   305,   304,   302,   301,   300
};


/*

 How this works:

 We use two Interrupt service routines. Both are connected to a timer. The
 step_isr() is high priority. the refill_step_buffer() is low priority.
 step_isr() will happen more often than refill_step_buffer().

 The cmd_queue_tick() function checks the queued commands. If there is something
 in the Queue cmd_queue_tick() will take it out. If that has been a move then it
 will try to add that move to the queue(max length = 1) inside this file.

 If it was a queued command but not a move then this command is either executed
 directly if there is no movement ongoing. Or if a move is happening then a tag
 is set at this position in the queue. And the non move command is put in the
 nonmove queue. Once the movement before the tag has been executed a flag is
 set to signal that the marked position has been reached. cmd_queue_tick()
 checks that flag. once it has been set the queued commands in the nonmove queue
 are executed.

 If cmd_queue_tick() has put something into the internal queue then this will
 also start the refill_step_buffer()'s timer and the step_isr()'s timer. That
 causes refill_step_buffer() and the step_isr() to execute.

 refill_step_buffer() will take the move data and calculate the times when the
 steps need to happen. It will store the calculated values in the
 variables[STEP_BUFFER_SIZE]. It will try to fill that buffer.

 Once the variables[STEP_BUFFER_SIZE] have been filled the step_isr() will
 execute the steps, by writing the calculated values out to the port pins. It
 will sleep between the Steps for the times specified.

 */
static void step_isr(void) // 16bit Timer at 12MHz Tick Rate High priority !
{
    if(step_pos == stop_pos)
    {
        //TODO check if the queue is really empty
        // we are done nothing more to do
        // -> disable Interrupt Stop Timer
        hal_time_stop_timer(STEP_TIMER);
        step_timer_running = false;
    }
    else
    {
#ifdef USE_STEP_DIR
        hal_stepper_set_Output(next_step[step_pos]);
#else
        int i;
        uint_fast8_t mask;
        for(i = 0; i < 8; i++)
        {
            switch(i)
            {
            case 0: mask = 0x01; break;
            case 1: mask = 0x02; break;
            case 2: mask = 0x04; break;
            case 3: mask = 0x08; break;

            case 4: mask = 0x10; break;
            case 5: mask = 0x20; break;
            case 6: mask = 0x40; break;
            case 7: mask = 0x80; break;
            }

            if(0 != (move_on_axis[step_pos] & mask))
            {
                // step on this axis
                if(0 != (next_direction[step_pos] & mask))
                {
                    trinamic_make_step_using_SPI(i, true);
                }
                else
                {
                    trinamic_make_step_using_SPI(i, false);
                }
            }
            // else no step on this axis
            // A delay is just a move that has no steps on all axis.
        }
#endif
        hal_time_set_timer_reload(STEP_TIMER, next_reload[step_pos]);
        step_pos++;
        if(step_pos == STEP_BUFFER_SIZE)
        {
            step_pos = 0;
        }
    }
}


// TODO deal with direction
// TODO deal with more than one axis

static uint_fast8_t get_number_of_free_slots(void)
{
    // The slot stop_pos can not be used ! -> no data in buffer stop_pos = step_pos
    uint_fast8_t free_slots;
    if(stop_pos > step_pos)
    {
        free_slots = STEP_BUFFER_SIZE - (stop_pos - step_pos) - 1;
    }
    else if(stop_pos == step_pos)
    {
        free_slots = STEP_BUFFER_SIZE;
    }
    else // stop_pos < step_pos
    {
        free_slots = step_pos - (stop_pos + 1);
    }
    return free_slots;
}

static void finished_cur_slot(void)
{
    busy = false;
    reached_tag = true;
    cur_slot_type = SLOT_TYPE_EMPTY;
}

/*
                    +-----+
 STEP_BUFFER_SIZE - |     |<- already used data - free
                    +-----+
                    | ... |<- already used data - free
                    +-----+
                    |     |<- stop_pos - free - will be filled next
                    +-----+
                    | ... |<- valid data
                    +-----+
                    |     |<- step_pos  - data that will now be used
                    +-----+
                2 - | ... |<- already used data - free
                    +-----+
                1 - | ... |<- already used data - free
                    +-----+
                0 - |     |<- already used data - free
                    +-----+


 */
static void refill_step_buffer(void)
{
    uint_fast8_t free_slots = get_number_of_free_slots();
    if(free_slots > STEP_CHUNK_SIZE)
    {
        // calculate next chunk
        calculate_step_chunk(STEP_CHUNK_SIZE);
    }

    // check if we need to restart the step timer
    if(false == step_timer_running)
    {
        // if we have some steps then start the timer
        if(step_pos != stop_pos)
        {
            if(false == hal_time_start_timer(STEP_TIMER, 0, step_isr))
            {
                error_fatal_error("Failed to start Timer !");
            }
            step_timer_running = true;
        }
        // else nothing to do for the step timer -> no need to start it
    }

    // start this timer if not already running -> first move after pause in movement
    if(false == buffer_timer_running)
    {
        // but only if we have something to do
        if(SLOT_TYPE_EMPTY != cur_slot_type)
        {
            if(false == hal_time_start_timer(STEP_BUFFER_TIMER, 0, refill_step_buffer))
            {
                error_fatal_error("Failed to start Timer !");
            }
            buffer_timer_running = true;
        }
        // else nothing more to do -> no need to start this timer.
    }
    else
    {
        // stop this timer if we have nothing to do
        if(SLOT_TYPE_EMPTY != cur_slot_type)
        {
            hal_time_stop_timer(STEP_BUFFER_TIMER);
            buffer_timer_running = false;
        }
    }
}

static void calculate_step_chunk(uint_fast8_t num_slots)
{
    if(SLOT_TYPE_BASIC_LINEAR_MOVE == cur_slot_type)
    {
        caclculate_basic_move_chunk(num_slots);
    }
    else if(SLOT_TYPE_DELAY == cur_slot_type)
    {
        uint_fast8_t i;
        for(i = 0; i < num_slots; i++)
        {
            if(0 < delay_ms)
            {
                delay_ms--;
            }
            else
            {
                // Finished to add the delay
                finished_cur_slot();
                return;
            }
#ifdef USE_STEP_DIR
            next_step[stop_pos] = cur_step;
#else
            move_on_axis[stop_pos] = 0;
            next_direction[stop_pos] = 0;
#endif
            next_reload[stop_pos] = STEP_TIME_ONE_MS;
            stop_pos ++;
            if(stop_pos == STEP_BUFFER_SIZE)
            {
                stop_pos = 0;
            }
        }
    }
    else if(SLOT_TYPE_EMPTY == cur_slot_type)
    {
        // do nothing ! - should not happen
    }
    // else invalid Slot Type -> TODO Event
}

static void caclculate_basic_move_chunk(uint_fast8_t num_slots)
{
    int i;
    for(i = 0; i < num_slots; i++)
    {
        // check if we have more steps int this move / this phase
        if(steps_already_made[primary_axis] < steps_in_this_phase_on_axis[primary_axis])
        {
            // more steps to do in this phase
            // Calculate reload time for primary axis
            uint_fast16_t reload_time = get_reload_primary_axis();
            // decide which axis will have a step this time
            make_the_needed_steps();
            // write values out
            next_reload[stop_pos] = reload_time;
            stop_pos ++;
            if(stop_pos == STEP_BUFFER_SIZE)
            {
                stop_pos = 0;
            }
        }
        else
        {
            // switch phase / end move
            switch(phase_of_move)
            {
            case MOVE_PHASE_ACCELLERATE:
            {
                uint_fast16_t constant_speed_steps = steps_on_axis[primary_axis] - decelleration_steps;
                if(0 < constant_speed_steps)
                {
                    phase_of_move = MOVE_PHASE_CONSTANT_SPEED;
                    get_steps_for_this_phase(constant_speed_steps / steps_on_axis[primary_axis]);
                }
                else if(0 < decelleration_steps)
                {
                    // switch to decelerate
                    phase_of_move = MOVE_PHASE_DECELERATE ;
                    get_steps_for_this_phase(1);
                }
                else
                {
                    // no deceleration phase
                    start_speed = nominal_speed;
                    finished_cur_slot();
                }
            }
                break;

            case MOVE_PHASE_CONSTANT_SPEED:
                if(0 < decelleration_steps)
                {
                    // switch to decelerate
                    phase_of_move = MOVE_PHASE_DECELERATE ;
                    curTime = 0;
                    get_steps_for_this_phase(1);
                }
                else
                {
                    // no deceleration phase
                    start_speed = nominal_speed;
                    finished_cur_slot();
                }
                break;

            case MOVE_PHASE_DECELERATE :
                start_speed = end_speed;
                finished_cur_slot();
                break;
            }
        }
    }
}

static uint_fast16_t get_reload_primary_axis(void)
{
    switch(phase_of_move)
    {
    case MOVE_PHASE_ACCELLERATE:
    {
        uint_fast16_t curReload;
        curSpeed = start_speed_ticks + (speed_increse_acc_tick * curTime);
        curReload = (uint_fast16_t)((1/curSpeed) + 0.5);
        curTime = curTime + curReload;
        return curReload;
    }

    case MOVE_PHASE_CONSTANT_SPEED:
        return speed_reloads[nominal_speed];

    case MOVE_PHASE_DECELERATE :
    {
        uint_fast16_t curReload;
        curSpeed = nominal_speed_ticks - (speed_decrese_decel_tick * curTime);
        curReload = (uint_fast16_t)((1/curSpeed) + 0.5);
        curTime = curTime + curReload;
        return curReload;
    }

    }
    // should never happen:
    return 1;
}

static void make_the_needed_steps(void)
{
    uint_fast8_t i;
    for(i = 0; i < MAX_NUMBER; i++)
    {
        if(0 != steps_in_this_phase_on_axis[i])
        {
            if(i == primary_axis)
            {
                // we step on the primary axis every time
                do_step_on_axis(i);
            }
            else
            {
                // non primary axis
                // TODO
            }
        }
        // no steps on this axis
    }
}

static void do_step_on_axis(uint_fast8_t i)
{
#ifdef USE_STEP_DIR
    uint32_t axis_mask = 0;
    switch(i)
    {

    }
    if(0 == (axis_mask & cur_step))
    {
        cur_step |= axis_mask;
    }
    else
    {
        cur_step &= ~axis_mask;
    }
    next_step[stop_pos] = cur_step;
#else
    uint_fast8_t axis_mask = 0;
    switch(i)
    {
    case 0: axis_mask = 0x01; break;
    case 1: axis_mask = 0x02; break;
    case 2: axis_mask = 0x04; break;
    case 3: axis_mask = 0x08; break;

    case 4: axis_mask = 0x10; break;
    case 5: axis_mask = 0x20; break;
    case 6: axis_mask = 0x40; break;
    case 7: axis_mask = 0x80; break;
    }
    move_on_axis[stop_pos] |= axis_mask;
    next_direction[stop_pos] = 0;
#endif
    steps_already_made[i]++;
}

static void get_steps_for_this_phase(float factor)
{
    uint_fast8_t i;
    for(i = 0; i < 8; i++)
    {
        if(0 == (active_axes_map & (0x01 << i)))
        {
            // Axis is not active
            // -> skip this axis
            steps_in_this_phase_on_axis[i] = 0;
        }
        else
        {
            if(1.0 == factor)
            {
                steps_in_this_phase_on_axis[i] = steps_on_axis[i];
                steps_on_axis[i] = 0;
            }
            else
            {
                uint_fast16_t steps_to_copy = steps_on_axis[i] * factor;
                steps_in_this_phase_on_axis[i] = steps_to_copy;
                steps_on_axis[i] = steps_on_axis[i] - steps_to_copy;
            }
        }
    }
}

// public functions

void step_init(uint_fast8_t num_stepper)
{
    uint_fast8_t i;
    for(i = 0; i < MAX_NUMBER; i++)
    {
        steps_on_axis[i] = 0;
        steps_in_this_phase_on_axis[i] = 0;
        steps_already_made[i] = 0;
        enabled[i] = false;
    }
    start_speed = 0;
    reached_tag = true;
    cur_slot_type = SLOT_TYPE_EMPTY;
    busy = false;
    delay_ms = 0;
    available_steppers = num_stepper;
    buffer_timer_running = false;
    step_timer_running = false;
#ifdef USE_STEP_DIR
    hal_stepper_port_init();
    for(i = 0; i < MAX_NUMBER; i++)
    {
        next_step[i] = 0;
    }
    cur_step = 0;
#else
    for(i = 0; i < MAX_NUMBER; i++)
    {
        move_on_axis[i] = 0;
        next_direction[i] = 0;
    }
#endif
}

static void auto_activate_usedAxis(void)
{
    if(0 != (0x01 & active_axes_map))
    {
        step_enable_motor(0, 1);
    }
    if(0 != (0x02 & active_axes_map))
    {
        step_enable_motor(1, 1);
    }
    if(0 != (0x04 & active_axes_map))
    {
        step_enable_motor(2, 1);
    }
    if(0 != (0x08 & active_axes_map))
    {
        step_enable_motor(3, 1);
    }
    if(0 != (0x10 & active_axes_map))
    {
        step_enable_motor(4, 1);
    }
    if(0 != (0x20 & active_axes_map))
    {
        step_enable_motor(5, 1);
    }
    if(0 != (0x40 & active_axes_map))
    {
        step_enable_motor(6, 1);
    }
    if(0 != (0x80 & active_axes_map))
    {
        step_enable_motor(7, 1);
    }
}

bool step_add_basic_linear_move(uint_fast8_t *move_data)
{
    bool eight_Bit_Steps;
    uint_fast8_t offset;
    uint_fast8_t i;
    if(true == busy)
    {
        return false;
    }
    active_axes_map = 0;
    primary_axis = 0;
    if(0x80 == (0x80 & move_data[1]))
    {
        // two bytes per axis
        // active Axes
        active_axes_map = move_data[2];
        // Directions of Axis are in that byte.
        direction_for_move = move_data[4];
        // primary Axis
        primary_axis = (0x0f & move_data[5]);
        // Homing ?
        if(0x10 == (0x10 & move_data[5]))
        {
            is_a_homing_move = true;
        }
        else
        {
            is_a_homing_move = false;
        }
        nominal_speed = move_data[6];
        end_speed = move_data[7];
        // 1 or 2 Bytes for Steps?
        if(0x80 == (0x80 & move_data[3]))
        {
            // bytes for Steps = 2;
            eight_Bit_Steps = false;
        }
        else
        {
            // bytes for Steps = 1;
            eight_Bit_Steps = true;
        }
        offset = 8;
    }
    else
    {
        // one byte per axis
        // active Axes
        active_axes_map = (0x7f & move_data[1]);
        // Directions of Axis are in that byte.
        direction_for_move = (0x7f & move_data[2]);
        // primary Axis
        primary_axis = (0x0f & move_data[3]);
        // Homing ?
        if(0x10 == (0x10 & move_data[3]))
        {
            is_a_homing_move = true;
        }
        else
        {
            is_a_homing_move = false;
        }
        nominal_speed = move_data[4];
        end_speed = move_data[5];
        // 1 or 2 Bytes for Steps?
        if(0x80 == (0x80 & move_data[2]))
        {
            // bytes for Steps = 2;
            eight_Bit_Steps = false;
        }
        else
        {
            // bytes for Steps = 1;
            eight_Bit_Steps = true;
        }
        offset = 6;
    }
    auto_activate_usedAxis();
    // acceleration Steps
    if(true == eight_Bit_Steps)
    {
        acceleration_steps = move_data[offset];
        offset++;
    }
    else
    {
        acceleration_steps = move_data[offset] * 256 + move_data[offset + 1];
        offset = offset + 2;
    }
    // Deceleration Steps
    if(true == eight_Bit_Steps)
    {
        decelleration_steps = move_data[offset];
        offset++;
    }
    else
    {
        decelleration_steps = move_data[offset] * 256 + move_data[offset + 1];
        offset = offset + 2;
    }
    for(i = 0; i < 8; i++)
    {
        if(0 == (active_axes_map & (0x01 << i)))
        {
            // Axis is not active
            // -> skip this axis
        }
        else
        {
            if(true == eight_Bit_Steps)
            {
                steps_on_axis[i] = move_data[offset];
                offset++;
            }
            else
            {
                steps_on_axis[i] = move_data[offset] * 256 + move_data[offset + 1];
                offset = offset + 2;
            }
        }
    }
    // TODO test if length of packet matches

    curTime = 0;
    nominal_speed_ticks = (float)1/(float)speed_reloads[nominal_speed];

    if(0 < acceleration_steps)
    {
        phase_of_move = MOVE_PHASE_ACCELLERATE;
        get_steps_for_this_phase(acceleration_steps /steps_on_axis[primary_axis]);
        start_speed_ticks = (float)1/(float)speed_reloads[start_speed];
        curSpeed = start_speed_ticks;
    }
    else
    {
        curSpeed = nominal_speed_ticks;
        // we already know that accelerationSteps is 0 !
        uint_fast16_t constant_speed_steps= steps_on_axis[primary_axis] - decelleration_steps;
        if(0 < constant_speed_steps)
        {
            phase_of_move = MOVE_PHASE_CONSTANT_SPEED;
            get_steps_for_this_phase(constant_speed_steps / steps_on_axis[primary_axis]);
        }
        else
        {
            // this move is decelerating only
            phase_of_move = MOVE_PHASE_DECELERATE ;
            get_steps_for_this_phase(1);
        }
    }

    average_speed_acceleration = speed_reloads[start_speed] + (speed_reloads[nominal_speed] - speed_reloads[start_speed])/2;
    average_speed_deceleration = speed_reloads[nominal_speed] + (speed_reloads[nominal_speed] - speed_reloads[end_speed])/2;
    speed_increse_acc_tick = (float)1
            /(float)(speed_reloads[end_speed] - speed_reloads[start_speed])
    /(steps_in_this_phase_on_axis[primary_axis] * average_speed_acceleration);
    speed_decrese_decel_tick = (float)1
            /(float)(speed_reloads[nominal_speed] - speed_reloads[end_speed])
    /(steps_in_this_phase_on_axis[primary_axis]/average_speed_deceleration);
    cur_slot_type = SLOT_TYPE_BASIC_LINEAR_MOVE;
    busy = true;
    refill_step_buffer();
    return true;
}

bool step_add_delay(uint_fast8_t msb,uint_fast8_t lsb)
{
    if(true == busy)
    {
        return false;
    }
    delay_ms = msb * 256 + lsb;
    cur_slot_type = SLOT_TYPE_DELAY;
    busy = true;
    refill_step_buffer();
    return true;
}

bool step_is_busy(void)
{
    return busy;
}

void step_request_tag(void)
{
    // TODO current position in the local queue gets marked
    // and once the moves until that position are executed
    // the variable reachedTag becomes true.
    // currently the Queue has only one element.
    reached_tag = false;
}

bool step_has_reached_tag(void)
{
    return reached_tag;
}

void step_disable_all_motors(void)
{
    uint_fast8_t i;
    for(i = 0 ; i < available_steppers; i++)
    {
        if(true == enabled[i])
        {
            trinamic_disable_stepper(i);
        }
        // else already disabled
    }
}

void step_enable_motor(uint_fast8_t stepper_number, uint_fast8_t on_off)
{
    if(stepper_number < available_steppers)
    {
        if(1 == on_off)
        {
            if(false == enabled[stepper_number])
            {
                trinamic_enable_stepper(stepper_number);
            }
            // already enabled
        }
        else if(0 == on_off)
        {
            if(true == enabled[stepper_number])
            {
                trinamic_disable_stepper(stepper_number);
            }
            // already disabled
        }
        // else invalid state
    }
    // else invalid stepper
}

// end of File
