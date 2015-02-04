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

#define STEP_CHUNK_SIZE             10

#define SLOT_TYPE_EMPTY             0
#define SLOT_TYPE_DELAY             1
#define SLOT_TYPE_BASIC_LINEAR_MOVE 2

#define MOVE_PHASE_ACCELLERATE      0
#define MOVE_PHASE_CONSTANT_SPEED   1
#define MOVE_PHASE_DECELLERATE      2
// Timer runs on 12 MHz clock.
#define STEP_TIME_ONE_MS            12000


static void calculate_conmstant_speed_reloads(void);
static void calculate_chunk_constant_speed(void);
static void calculate_chunk_accellerate(void);
static void calculate_chunk_decellerate(void);
static void get_steps_for_this_phase(float factor);
static void calculate_step_chunk(uint_fast8_t num_slots);

static uint_fast8_t step_pos = 0;
static uint_fast8_t stop_pos = 0;

#ifdef USE_STEP_DIR
static uint_fast8_t next_step[256];
static uint_fast8_t next_direction[256];
#else
#endif

static bool is_a_move[256];

static uint_fast16_t next_reload[256];

static bool reached_tag = false;
static uint_fast8_t cur_slot_type = SLOT_TYPE_EMPTY;
static bool busy = false;
// to handle delays:
static uint_fast16_t delay_ms = 0;
// to handle Basic Linear Move:
static uint_fast16_t steps_on_axis[8];
static uint_fast8_t inverted_axis = 0; // each inverted Axis has a 1 in this bitmap
static uint_fast8_t direction_for_move;
static bool is_a_homing_move;
static uint_fast8_t phase_of_move;
static uint_fast8_t active_axes_map;
static uint_fast8_t primary_axis;
static uint_fast8_t start_speed;
static uint_fast8_t nominal_speed;
static uint_fast8_t end_speed;
static uint_fast16_t acceleration_steps;
static uint_fast16_t decelleration_steps;
static uint_fast16_t steps_in_this_phase_on_axis[8];
static uint_fast16_t reload_for_axis[8];
static uint_fast16_t next_move_on_axis_in[8];

bool enabled[MAX_NUMBER];

// 16bit Timer running at 12MHz
static uint_fast16_t speed_reloads[256] ={
           0,/*TODO?*/0, 38250, 25500, 19125, 15300, 12750, 10929,
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
 step_isr is a high priority isr. the buffer_isr is low priority. step_isr will
 happen more often than step_buffer_isr.

 The step_tick() function checks the queued commands. If there is something in
 the Queue step_tick() will take it out. If that has been a move then it will
 try to add that move to the queue inside this file.

 If it was a queued command but not a move then this command is either executed
 directly if there is no movement ongoing. If a move is happening then a tag is
 set at this position in the queue. And the non move command is put in the
 nonmove queue. Once the movement before the tag has been executed a flag is set
 to signal that the marked position has been reached. step_tick() checks that
 flag. once it has been set the queued commands in the nonmove queue are executed.

 if step_tick has put something into the internal queue then it will also start
 the step_buffer_timer and the step timer. That causes step_buffer_isr and the
 step_isr to execute.

 step_buffer_isr will take the move data and calculate the times when the  steps
 need to happen. It will store the correct setting of the steps pins in
 nextSteps[], the correct setting of the Direction Pins into nextDirection[] and
 the time to the next step in nextReload[]. It will try to fill that buffer.

 Once the next*[] have been filled the step_isr will execute the steps, by
 writing the calculated values out to the port pins.

 */
static void step_isr(void) // 16bit Timer at 12MHz Tick Rate High priority !
{
    if(step_pos == stop_pos)
    {
        // we are done nothing more to do
        // -> disable Interrupt Stop Timer
        // TODO
    }
    else
    {
#ifdef USE_STEP_DIR
        // STEP_PORT = next_step[step_pos];
        // DIR_PORT = next_direction[step_pos];
#else
        if(true == is_a_move[step_pos])
        {
            trinamic_make_step_using_SPI();
        }
        // else is a Delay -> we are done
#endif
        // TMR4RLL = next_reload[stepPos];
        step_pos++;
    }
}


/*
       +-----+
 255 - |     |<- already used data - free
       +-----+
       | ... |<- already used data - free
       +-----+
       |     |<- stop_pos
       +-----+
       | ... |<- valid data
       +-----+
       |     |<- step_pos
       +-----+
       | ... |<- already used data - free
       +-----+
   0 - |     |<- already used data - free
       +-----+


 */
static void refill_step_buffer(void)
{
    uint_fast8_t free_slots;
    if(stop_pos > step_pos)
    {
        free_slots = 255 - (stop_pos - step_pos);
    }
    else if(stop_pos == step_pos)
    {
        free_slots = 255;
    }
    else // stop_pos < step_pos
    {
        free_slots = 255 - stop_pos + step_pos;
    }
    if(free_slots > STEP_CHUNK_SIZE)
    {
        // calculate next chunk
        calculate_step_chunk(STEP_CHUNK_SIZE);
        // TODO check if we need to restart the step timer
        // TODO stop this timer if we have nothing to do
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
            uint_fast16_t steps_to_copy = steps_on_axis[i] * factor;
            steps_in_this_phase_on_axis[i] = steps_to_copy;
            steps_on_axis[i] = steps_on_axis[i] - steps_to_copy;
        }
    }
}

static void calculate_conmstant_speed_reloads(void)
{
    uint_fast8_t i;
    uint_fast32_t time_of_phase = steps_on_axis[primary_axis] * speed_reloads[nominal_speed];

    for(i = 0; i < 8; i++)
    {
        if(0 == (active_axes_map & (0x01 << i)))
        {
            // Axis is not active
            // -> skip this axis
            reload_for_axis[i] = 0xffff;
            next_move_on_axis_in[i] = 0xffff;
        }
        else
        {
            reload_for_axis[i] = time_of_phase/steps_on_axis[i];
            next_move_on_axis_in[i] = reload_for_axis[i];
        }
    }
}

static void calculate_chunk_constant_speed(void)
{
    uint_fast8_t i;
    for(i = 0; i < 8; i++)
    {
        if(0 == (active_axes_map & (0x01 << i)))
        {
            // Axis is not active
            // -> skip this axis
        }
        else
        {
            uint_fast16_t cur_reload = speed_reloads[nominal_speed];
            if(cur_reload >= next_move_on_axis_in[i])
            {
                // make a step
#ifdef USE_STEP_DIR
            // TODO STEP_PORT = next_step[step_pos];
#else
                is_a_move[stop_pos] = true;
#endif
                next_reload[stop_pos] = cur_reload;
                stop_pos ++;
                next_move_on_axis_in[i] = next_move_on_axis_in[i] - cur_reload;
            }
            // else wait
            next_move_on_axis_in[i] = next_move_on_axis_in[i] + reload_for_axis[i];
        }
    }
    // check if we reached the end of the phase
    steps_in_this_phase_on_axis[primary_axis]--;
    if(0 == steps_in_this_phase_on_axis[primary_axis])
    {
        // end of this phase
        if(0 < decelleration_steps)
        {
            phase_of_move = MOVE_PHASE_DECELLERATE;
        }
        else
        {
            // no deceleration Phase, This move is done
            cur_slot_type = SLOT_TYPE_EMPTY;
            busy = false;
            return;
        }
    }
}

static void calculate_chunk_accellerate(void)
{
 // TODO
}

static void calculate_chunk_decellerate(void)
{
 // TODO
}

static void calculate_step_chunk(uint_fast8_t num_slots)
{
    if(SLOT_TYPE_BASIC_LINEAR_MOVE == cur_slot_type)
    {
        uint_fast8_t i;
        for(i = 0; i < num_slots; i++)
        {
            if(MOVE_PHASE_ACCELLERATE == phase_of_move)
            {
                calculate_chunk_accellerate();
            }
            else if(MOVE_PHASE_CONSTANT_SPEED == phase_of_move)
            {
                calculate_chunk_constant_speed();
            }
            else if(MOVE_PHASE_DECELLERATE == phase_of_move)
            {
                calculate_chunk_decellerate();
            }
            else
            {
                // invalid Phase
                // TODO Event
            }


            // TODO
#ifdef USE_STEP_DIR
            // TODO DIR_PORT[stop_pos] = direction_for_move;
#endif
            stop_pos ++;
        }
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
                busy = false;
                cur_slot_type = SLOT_TYPE_EMPTY;
                return;
            }
#ifdef USE_STEP_DIR
            // TODO STEP_PORT = nextStep[stepPos];
            // TODO DIR_PORT = nextDirection[stepPos];
#else
            is_a_move[stop_pos] = false;
#endif
            next_reload[stop_pos] = STEP_TIME_ONE_MS;
            stop_pos ++;
        }
    }
    else if(SLOT_TYPE_EMPTY == cur_slot_type)
    {
        // TODO Do nothing ?
    }
    // else invalid Slot Type -> TODO Event
}

// public functions

void step_init(void)
{
    uint_fast8_t i;
    for(i = 0; i < 8; i++)
    {
        steps_on_axis[i] = 0;
        steps_in_this_phase_on_axis[i] = 0;
        reload_for_axis[i] = 0;
    }
    start_speed = 0;

    hal_spi_init(STEPPER_SPI);

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
    if(0x80 == (0x80 & *(move_data + 1)))
    {
        // two bytes per axis
        // active Axes
        active_axes_map = *(move_data + 2);
        // Directions of Axis are in that byte. The XOR takes care of the inverted axes
        direction_for_move = inverted_axis ^ *(move_data + 4);
        // primary Axis
        primary_axis = (0x0f & *(move_data + 5));
        // Homing ?
        if(0x10 == (0x10 & *(move_data + 5)))
        {
            is_a_homing_move = true;
        }
        else
        {
            is_a_homing_move = false;
        }
        nominal_speed = *(move_data + 6);
        end_speed = *(move_data + 7);
        // 1 or 2 Bytes for Steps?
        if(0x80 == (0x80 & *(move_data + 3)))
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
        active_axes_map = (0x7f & *(move_data + 1));
        // Directions of Axis are in that byte. The XOR takes care of the inverted axes
        direction_for_move = inverted_axis ^ (0x7f & *(move_data + 2));
        // primary Axis
        primary_axis = (0x0f & *(move_data + 3));
        // Homing ?
        if(0x10 == (0x10 & *(move_data + 3)))
        {
            is_a_homing_move = true;
        }
        else
        {
            is_a_homing_move = false;
        }
        nominal_speed = *(move_data + 4);
        end_speed = *(move_data + 5);
        // 1 or 2 Bytes for Steps?
        if(0x80 == (0x80 & *(move_data + 2)))
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
    // acceleration Steps
    if(true == eight_Bit_Steps)
    {
        acceleration_steps = *(move_data + offset);
        offset++;
    }
    else
    {
        acceleration_steps = *(move_data + offset) * 256 + *(move_data + offset + 1);
        offset = offset + 2;
    }
    // Deceleration Steps
    if(true == eight_Bit_Steps)
    {
        decelleration_steps = *(move_data + offset);
        offset++;
    }
    else
    {
        decelleration_steps = *(move_data + offset) * 256 + *(move_data + offset + 1);
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
                steps_on_axis[i] = *(move_data + offset);
                offset++;
            }
            else
            {
                steps_on_axis[i] = *(move_data + offset) * 256 + *(move_data + offset + 1);
                offset = offset + 2;
            }
        }
    }
    // TODO test if length of packet matches

    if(0 < acceleration_steps)
    {
        phase_of_move = MOVE_PHASE_ACCELLERATE;
        get_steps_for_this_phase(acceleration_steps /steps_on_axis[primary_axis]);
    }
    else
    {
        // we already know that accelerationSteps is 0 !
        uint_fast16_t constant_speed_steps= steps_on_axis[primary_axis] - decelleration_steps;
        if(0 < constant_speed_steps)
        {
            phase_of_move = MOVE_PHASE_CONSTANT_SPEED;
            get_steps_for_this_phase(constant_speed_steps / steps_on_axis[primary_axis]);
            calculate_conmstant_speed_reloads();
        }
        else
        {
            // this move is decelerating only
            phase_of_move = MOVE_PHASE_DECELLERATE;
        }
    }
    cur_slot_type = SLOT_TYPE_BASIC_LINEAR_MOVE;
    busy = true;
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
    reached_tag = true;
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
