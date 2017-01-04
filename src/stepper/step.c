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

#include "error.h"
#include "hal_cfg.h"
#include "hal_debug.h"
#include "hal_led.h"
#include "hal_spi.h"
#include "hal_stepper_port.h"
#include "hal_time.h"
#include "step.h"
#include "trinamic.h"


static void caclculate_basic_move_chunk(uint_fast8_t num_slots);
static uint_fast16_t get_reload_primary_axis(void);
static void get_steps_for_this_phase(float factor);
static void calculate_step_chunk(uint_fast8_t num_slots);
static uint_fast8_t get_number_of_free_slots(void);
static void finished_cur_slot(void);
static void make_the_needed_steps(uint_fast16_t reload_time);
static void do_step_on_axis(uint_fast8_t i, uint_fast16_t reload_time);
static void auto_activate_usedAxis(void);
static void step_isr(void);
static void refill_step_buffer(void);
#ifdef USE_STEP_DIR
static uint32_t toggle_bit(uint_fast8_t bit, uint32_t value);
#else
#endif

// Step Timer
static volatile bool step_timer_running;
static volatile uint_fast8_t read_pos = 0;
static volatile uint_fast8_t write_pos = 0;

// step buffer timer
static volatile bool buffer_timer_running;

// step queue
static volatile bool busy;
static volatile bool reached_tag;
static volatile uint_fast8_t cur_slot_type;
// to handle delays:
static volatile uint_fast16_t delay_ms;
// to handle Basic Linear Move:
static volatile uint_fast16_t active_axes_map;
static volatile uint_fast8_t primary_axis;
static bool is_a_homing_move;
static uint_fast8_t direction_for_move;
static volatile uint_fast8_t start_speed;
static volatile float start_speed_ticks;
static volatile uint_fast8_t nominal_speed;
static volatile float nominal_speed_ticks;
static volatile uint_fast8_t end_speed;
static volatile float end_speed_ticks;
static volatile uint_fast16_t acceleration_steps;
static volatile uint_fast16_t decelleration_steps;
static volatile uint_fast16_t steps_on_axis[MAX_NUMBER];
static volatile uint_fast8_t phase_of_move;
static volatile uint_fast16_t steps_in_this_phase_on_axis[MAX_NUMBER];
static volatile uint_fast16_t steps_already_made[MAX_NUMBER];
static volatile uint_fast32_t curTime;
static volatile float speed_increse_acc_tick;
static volatile float speed_decrese_decel_tick;



// enable / disable Stepper
static uint_fast8_t available_steppers;
static bool enabled[MAX_NUMBER];



#ifdef USE_STEP_DIR
static volatile uint32_t next_step[STEP_BUFFER_SIZE];
static volatile uint32_t cur_step;
static volatile uint_fast8_t last_direction_axis;
#else
static volatile uint_fast8_t move_on_axis[STEP_BUFFER_SIZE];
static volatile uint_fast8_t next_direction[STEP_BUFFER_SIZE];
#endif
static volatile uint_fast16_t next_reload[STEP_BUFFER_SIZE];

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
 * How this works:
 *
 * We use two Interrupt service routines. Both are connected to a timer. The
 * step_isr() is high priority. the refill_step_buffer() is low priority.
 * step_isr() will happen more often than refill_step_buffer().
 *
 * The cmd_queue_tick() function checks the queued commands. If there is
 * something in the Queue cmd_queue_tick() will take it out. If that has been a
 * move then it will try to add that move to the queue(max length = 1) inside
 * this file.
 *
 * If it was a queued command but not a move then this command is either
 * executed directly if there is no movement ongoing. Or if a move is happening
 * then a tag is set at this position in the queue. And the non move command is
 * put in the nonmove queue. Once the movement before the tag has been executed
 * a flag is set to signal that the marked position has been reached.
 * cmd_queue_tick() checks that flag. once it has been set the queued commands
 * in the nonmove queue are executed.
 *
 * If cmd_queue_tick() has put something into the internal queue then this will
 * also start the refill_step_buffer()'s timer and the step_isr()'s timer. That
 * causes refill_step_buffer() and the step_isr() to execute.
 *
 * refill_step_buffer() will take the move data and calculate the times when the
 * steps need to happen. It will store the calculated values in the
 * variables[STEP_BUFFER_SIZE]. It will try to fill that buffer.
 *
 * Once the variables[STEP_BUFFER_SIZE] have been filled the step_isr() will
 * execute the steps, by writing the calculated values out to the port pins. It
 * will sleep between the Steps for the times specified.
 */
static void step_isr(void) // 16bit Timer at 12MHz Tick Rate High priority !
{
    // debug_line("Step ISR");
    if(read_pos == write_pos)
    {
        // we are done nothing more to do
        // -> disable Interrupt Stop Timer
        hal_time_stop_timer(STEP_TIMER);
        step_timer_running = false;
    }
    else
    {
#ifdef USE_STEP_DIR
        hal_stepper_set_Output(next_step[read_pos]);
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

            if(0 != (move_on_axis[read_pos] & mask))
            {
                // step on this axis
                if(0 != (next_direction[read_pos] & mask))
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
        hal_time_set_timer_reload(STEP_TIMER, next_reload[read_pos]);
        read_pos++;
        if(read_pos == STEP_BUFFER_SIZE)
        {
            read_pos = 0;
        }
    }
}

// TODO deal with more than one axis

static uint_fast8_t get_number_of_free_slots(void)
{
    uint_fast8_t free_slots;
    if(write_pos > read_pos)
    {
        free_slots = STEP_BUFFER_SIZE - (write_pos - read_pos);
    }
    else if(write_pos == read_pos)
    {
        free_slots = STEP_BUFFER_SIZE;
    }
    else // write_pos < read_pos
    {
        free_slots = read_pos - write_pos;
    }
    return free_slots;
}

static void finished_cur_slot(void)
{
    busy = false;
    reached_tag = true; // TODO check
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
    // debug_line("Buffer ISR");
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
        if(read_pos != write_pos)
        {
            if(false == hal_time_start_timer(STEP_TIMER,
                                             TICKS_PER_SECOND,
                                             5,
                                             &step_isr))
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
            if(false == hal_time_start_timer(STEP_BUFFER_TIMER,
                                             TICKS_PER_SECOND,
                                             REFILL_BUFFER_RELOAD,
                                             &refill_step_buffer))
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
        if(SLOT_TYPE_EMPTY == cur_slot_type)
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
            next_step[write_pos] = cur_step;
#else
            move_on_axis[write_pos] = 0;
            next_direction[write_pos] = 0;
#endif
            next_reload[write_pos] = STEP_TIME_ONE_MS;
            write_pos ++;
            if(write_pos == STEP_BUFFER_SIZE)
            {
                write_pos = 0;
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
    unsigned int i;
    for(i = 0; i < num_slots; i++)
    {
        // check if we have more steps in this move / this phase
        if(steps_already_made[primary_axis] < steps_in_this_phase_on_axis[primary_axis])
        {
            // more steps to do in this phase
            // Calculate reload time for primary axis
            uint_fast16_t reload_time = get_reload_primary_axis();
            // decide which axis will have a step this time
            make_the_needed_steps(reload_time);
        }
        else
        {
            int n;
            for(n = 0; n < MAX_NUMBER; n++)
            {
                steps_already_made[n] = 0;
            }
            // switch phase / end move
            switch(phase_of_move)
            {
            case MOVE_PHASE_ACCELLERATE:
            {
                uint_fast16_t constant_speed_steps = steps_on_axis[primary_axis] - decelleration_steps;
                if(0 < constant_speed_steps)
                {
                    phase_of_move = MOVE_PHASE_CONSTANT_SPEED;
                    get_steps_for_this_phase((float)constant_speed_steps / (float)steps_on_axis[primary_axis]);
                }
                else if(0 < decelleration_steps)
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
    uint_fast16_t curReload;
    float curSpeed;
    switch(phase_of_move)
    {
    case MOVE_PHASE_ACCELLERATE:
        curSpeed = start_speed_ticks + (speed_increse_acc_tick * curTime);
        curReload = (uint_fast16_t)(((1/curSpeed) * (float)TICKS_PER_SECOND) + 0.5);
        if(curReload < speed_reloads[nominal_speed])
        {
            curReload = speed_reloads[nominal_speed];
        }
        curTime = curTime + curReload;
        return curReload;

    case MOVE_PHASE_CONSTANT_SPEED:
        return speed_reloads[nominal_speed];

    case MOVE_PHASE_DECELERATE :
        curSpeed = nominal_speed_ticks - (speed_decrese_decel_tick * curTime);
        curReload = (uint_fast16_t)(((1/curSpeed) * (float)TICKS_PER_SECOND) + 0.5);
        if(curReload > speed_reloads[end_speed])
        {
            curReload = speed_reloads[end_speed];
        }
        curTime = curTime + curReload;
        return curReload;

    }
    // default:
    // should never happen:
    return 0xfffd;
}


static void make_the_needed_steps(uint_fast16_t reload_time)
{
    uint_fast8_t i;
    for(i = 0; i < MAX_NUMBER; i++)
    {
        if(0 != steps_in_this_phase_on_axis[i])
        {
            if(i == primary_axis)
            {
                // we step on the primary axis every time
                do_step_on_axis(i, reload_time);
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

#ifdef USE_STEP_DIR
static uint32_t toggle_bit(uint_fast8_t bit, uint32_t value)
{
    uint32_t mask = 1<<bit;
    if(0 == (value & mask))
    {
        value |= mask;
    }
    else
    {
        value &= ~mask;
    }
    return value;
}
#else
#endif

static void do_step_on_axis(uint_fast8_t i, uint_fast16_t reload_time)
{
#ifdef USE_STEP_DIR
    bool cur_step_has_direction_change = false;
    uint32_t last_step = cur_step;
    // direction
    if( (direction_for_move & (1 << i))
            !=
        (last_direction_axis & (1 << i)) )
    {
        // change of direction on this axis
        cur_step_has_direction_change = true;
        cur_step = toggle_bit(i + 8, cur_step);
        last_direction_axis = toggle_bit(i, last_direction_axis);
    }
    // step
    cur_step = toggle_bit(i, cur_step);
    if(true == cur_step_has_direction_change)
    {
        uint_fast16_t half_reload = reload_time /2;
        reload_time = reload_time - half_reload;
        next_step[write_pos] = (0x00ff & last_step) | (0xff00 & cur_step);
        next_reload[write_pos] = half_reload;
        write_pos ++;
        if(write_pos == STEP_BUFFER_SIZE)
        {
            write_pos = 0;
        }
    }
    next_step[write_pos] = cur_step;
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
    move_on_axis[write_pos] |= axis_mask;
    next_direction[write_pos] = 0;
#endif
    steps_already_made[i]++;
    // write values out
    next_reload[write_pos] = reload_time;
    write_pos ++;
    if(write_pos == STEP_BUFFER_SIZE)
    {
        write_pos = 0;
    }
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
    int i;
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
    if(num_stepper < MAX_NUMBER)
    {
        available_steppers = num_stepper;
    }
    else
    {
        available_steppers = MAX_NUMBER;
    }
    buffer_timer_running = false;
    step_timer_running = false;
    for(i = 0; i < STEP_BUFFER_SIZE; i++)
    {
        next_reload[i] = 0xfffe;
#ifdef USE_STEP_DIR
        next_step[i] = 0;
    }
    cur_step = 0;
    last_direction_axis = 0;
    hal_stepper_port_init();
#else
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
    debug_line("Adding Basic Linear Move!");
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
    nominal_speed_ticks = (float)TICKS_PER_SECOND/(float)speed_reloads[nominal_speed];

    if(0 < acceleration_steps)
    {
        phase_of_move = MOVE_PHASE_ACCELLERATE;
        get_steps_for_this_phase((float)acceleration_steps / (float)steps_on_axis[primary_axis]);
        start_speed_ticks = (float)((float)TICKS_PER_SECOND/(float)speed_reloads[start_speed]);
        // curTime = 0.5 * speed_reloads[start_speed];
    }
    else
    {
        // we already know that accelerationSteps is 0 !
        uint_fast16_t constant_speed_steps= steps_on_axis[primary_axis] - decelleration_steps;
        if(0 < constant_speed_steps)
        {
            phase_of_move = MOVE_PHASE_CONSTANT_SPEED;
            get_steps_for_this_phase((float)constant_speed_steps / (float)steps_on_axis[primary_axis]);
        }
        else
        {
            // this move is decelerating only
            phase_of_move = MOVE_PHASE_DECELERATE ;
            get_steps_for_this_phase(1);
        }
    }

    if(0 < acceleration_steps)
    {
        float average_speed = (nominal_speed_ticks + start_speed_ticks) / 2;
        float speed_increase = nominal_speed_ticks - start_speed_ticks;
        float time_ticks = (float)acceleration_steps/average_speed * TICKS_PER_SECOND;
        speed_increse_acc_tick = speed_increase/time_ticks;
    }

    if(0 < decelleration_steps)
    {
        end_speed_ticks = (float)((float)TICKS_PER_SECOND/(float)speed_reloads[end_speed]);
        float average_speed = (nominal_speed_ticks + end_speed_ticks) / 2;
        float speed_decrease = nominal_speed_ticks - end_speed_ticks;
        float time_ticks = (float)decelleration_steps/average_speed * TICKS_PER_SECOND;
        speed_decrese_decel_tick = speed_decrease/time_ticks;
    }

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

#ifdef DEBUG_ACTIVE

void step_print_state(void)
{
    debug_line("Step State:");
    debug_line("available steppers : %d", available_steppers);
    debug_msg("Step Timer : ");
    if(true == step_timer_running)
    {
        debug_line("running");
    }
    else
    {
        debug_line("not running");
    }
    debug_line("Step Position : %d", read_pos);
    debug_line("Stop Position : %d", write_pos);

    debug_msg("Buffer Timer : ");
    if(true == buffer_timer_running)
    {
        debug_line("running");
    }
    else
    {
        debug_line("not running");
    }

    if(true == busy)
    {
        debug_line("busy");
    }
    else
    {
        debug_line("not busy");
    }
    debug_line("free slots : %d", get_number_of_free_slots());
}

#endif

// end of File
