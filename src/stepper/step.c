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
#ifdef HAS_SPI
#include "hal_spi.h"
#endif

#define STEP_CHUNK_SIZE             10

#define SLOT_TYPE_EMPTY             0
#define SLOT_TYPE_DELAY             1
#define SLOT_TYPE_BASIC_LINEAR_MOVE 2

#define MOVE_PHASE_ACCELLERATE      0
#define MOVE_PHASE_CONSTANT_SPEED   1
#define MOVE_PHASE_DECELLERATE      2
// Timer runs on 12 MHz clock.
#define STEP_TIME_ONE_MS            12000
#ifdef HAS_SPI
// 20 bit per stepper in Bytes
//(1 Stepper =  3 Bytes; ( 4 bits unused)
// 2 Stepper =  5 Bytes;
// 4 Stepper = 10 Bytes
// 6 Stepper = 15 Bytes
// 8 Stepper = 20 Bytes)
#define SPI_BUFFER_LENGTH           20
#endif

static void calculate_conmstant_speed_reloads(void);
static void calculate_chunk_constant_speed(void);
static void calculate_chunk_accellerate(void);
static void calculate_chunk_decellerate(void);
static void get_steps_for_this_phase(float factor);
static void calculate_step_chunk(uint_fast8_t num_slots);

#if !defined(USE_STEP_DIR) && !defined(HAS_SPI)
#error "Can not do steps! Need either SPI or STEP + DIR or both !"
#endif

static uint_fast8_t step_pos = 0;
static uint_fast8_t stop_pos = 0;

#ifdef HAS_SPI
static uint8_t spi_receive_buffer[SPI_BUFFER_LENGTH];

#ifdef USE_STEP_DIR
static uint_fast8_t next_step[256];
static uint_fast8_t next_direction[256];
#else
// DRVCTRL in SPI Mode:
//
// Bit     |Range  |Meaning
//---------+-------+------------------
// 19 + 18 |0      |
// 17      |0/1    |Polarity A
// 9-16    |0..255 |Current in coil A
// 8       |0/1    |Polarity B
// 0..7    |0..255 |Current in coil B
static uint8_t DRVCONTROL_Buffer[32][3] = {
        //  0,    1,    2
        {0x00, 0xf0, 0x01}, // 0
        {0x30, 0xe6, 0x01}, // 1
        {0x5f, 0xca, 0x01}, // 2
        {0x89, 0x9c, 0x01}, // 3
        {0xaf, 0x5e, 0x01}, // 4
        {0xce, 0x12, 0x01}, // 5
        {0xe5, 0xbe, 0x00}, // 6
        {0xf3, 0x60, 0x00}, // 7
        {0xf8, 0x00, 0x02}, // 8
        {0xf3, 0x60, 0x02}, // 9
        {0xe5, 0xbe, 0x02}, // 10
        {0xce, 0x12, 0x03}, // 11
        {0xaf, 0x5e, 0x03}, // 12
        {0x89, 0x9c, 0x03}, // 13
        {0x5f, 0xca, 0x03}, // 14
        {0x30, 0xe6, 0x03}, // 15
        {0x00, 0xf0, 0x03}, // 16
        {0x30, 0xe7, 0x03}, // 17
        {0x5f, 0xcb, 0x03}, // 18
        {0x89, 0x9d, 0x03}, // 19
        {0xaf, 0x5f, 0x03}, // 20
        {0xce, 0x13, 0x03}, // 21
        {0x5e, 0xbf, 0x02}, // 22
        {0xf3, 0x61, 0x02}, // 23
        {0xf8, 0x01, 0x00}, // 24
        {0xf3, 0x61, 0x00}, // 25
        {0xe5, 0xbf, 0x00}, // 26
        {0xce, 0x13, 0x01}, // 27
        {0xaf, 0x5f, 0x01}, // 28
        {0x89, 0x9d, 0x01}, // 29
        {0x5f, 0xcb, 0x01}, // 30
        {0x30, 0xe7, 0x01}, // 31
};
#endif
uint_fast8_t cur_step = 0;
bool direction_is_increasing = true;
bool is_a_move[256];
#endif

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


// 16bit Timer running at 12MHz
static uint_fast16_t speed_reloads[256] ={
           0,/*?*/0, 38250, 25500, 19125, 15300, 12750, 10929,
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

#ifdef HAS_SPI
    hal_spi_init(STEPPER_SPI);
#endif
}

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
void step_isr(void) // 16bit Timer at 12MHz Tick Rate High priority !
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
            hal_spi_do_transaction(STEPPER_SPI,
                                   &DRVCONTROL_Buffer[cur_step][0],
                                   3,
                                   &spi_receive_buffer[0]);
            if(true == direction_is_increasing)
            {
                cur_step++;
                if(32 == cur_step)
                {
                    cur_step = 0;
                }
            }
            else
            {
                if(0 == cur_step)
                {
                    cur_step = 32;
                }
                else
                {
                    cur_step--;
                }
            }
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

uint_fast8_t step_detect_number_of_steppers(void)
{
#ifdef HAS_SPI
    uint8_t detect_data[SPI_BUFFER_LENGTH];
    uint_fast8_t i;
    uint_fast8_t count = 0;
    for(i = 0; i < SPI_BUFFER_LENGTH; i++)
    {
        // there is no save(invalid) value for detection.
        // Each value send does some configuration in the stepper driver.
        // The value send will also be send back if the SPI Loop is closed.
        // 0x00000 was chosen for these reasons:
        // 0xfffff will be send back if the loop is open (No SPI Device connected)
        // 0x00000 will be send back for each missing stepper
        // any other value is the current status of a stepper.
        detect_data[i] = 0x0;
    }

    hal_spi_do_transaction(STEPPER_SPI, // device
                           &detect_data[0], // data to send
                           SPI_BUFFER_LENGTH, // number of bytes to send
                           &spi_receive_buffer[0] // where to put the response
                           );
    for(i = 0; i < ((SPI_BUFFER_LENGTH/5)* 2); i++)
    {
        // 20 bits = 2.5 byte per stepper,..
        //  first Stepper is in bytes 0, 1, 2
        // second stepper is in bytes 2, 3, 4
        //  third stepper is in bytes 5, 6, 7
        // fourth stepper is in bytes 7, 8, 9
        // ...
        int start = (i * 25)/10;
        if( (   (0xff == spi_receive_buffer[start + i + 0])
             && (0xff == spi_receive_buffer[start + i + 1])
             && (0xf0 == (0xf0 & spi_receive_buffer[start + i + 2])) )
                ||
            (   (0 == spi_receive_buffer[start + i + 0])
             && (0 == spi_receive_buffer[start + i + 1])
             && (0 == (0xf0 & spi_receive_buffer[start + i + 2])) ) )
        {
            // stepper not available
        }
        else
        {
            count ++;
        }
    }
    return count;
#else
    return 8;
#endif
}

void step_configure_steppers(uint_fast8_t num_steppers)
{
#ifdef HAS_SPI
    uint8_t cfg_data[SPI_BUFFER_LENGTH];
    uint_fast8_t num_bytes =((num_steppers+1)/2)*5;

    if(0 == num_steppers)
    {
        // TODO Event?
        return;
    }

    // CHOPCONF
    //
    // Bit     |Range  |Meaning
    //---------+-------+------------------
    // 19      |1      |
    // 18 + 17 |0      |
    // 16 + 15 |0.3    |Blanking time System clock periods)(0 = 16 1=24 2=36 3=54)
    // 14      |0/1    | Chopper Mode (0=spread cycle; 1=constant toff with fast decay)
    // 13      |0/1    | random Toff (0=fixed 1=random(-12 .. +3 clocks)
    // 11 + 12 |0..3   | spread cycle :hysteresis decrement period (0=16 1=32 2=48 3=64 clocks)
    //         |       | random Toff  : 0x: current comparator can terminate the fast decay 1x: only timer terminated fast decay;  low bit is MSB of fast decay time
    // 7..10   |0..15  | spread cycle : Hysteresis 0..15 -3 -> -3..12
    //         |       | random Toff  : Hysteresis 0..15 -3 -> -3..12
    // 4..6    |0..7   | spread cycle : Hysteresis start offset 0..7 + 1 -> 1..8 Hysteresis + offset must be < 16
    //         |       | random Toff  : lsbs of fast decay time 0..15 *32 clocks.
    // 0..3    |0..15  | Toff ( 0 = free rotate; 1 = needs Blanking time of minimum 24 clocks 2..15: duration of slow decay = 12 + (32 * toff) clocks)

    // -> 9a9f2
    for(int i= 0; i < num_bytes; )
    {
        cfg_data[i++] = 0x9a;
        cfg_data[i++] = 0x9f;
        cfg_data[i++] = 0x29;
        cfg_data[i++] = 0xa9;
        cfg_data[i++] = 0xf2;
    }
    hal_spi_do_transaction(STEPPER_SPI, &cfg_data[0], num_bytes, &spi_receive_buffer[0]);

    // SGCSCONF
    //
    // Bit     |Range  |Meaning
    //---------+-------+------------------
    // 19 + 18 |1      |
    // 17      |0      |
    // 16      |0/1    |SFILT 0=no filtering; 1 = Filter active (increases precision, reduces speed)
    // 15      |0      |
    // 8..14   |0..127 |SGT 0..127 -64 -> -64 .. +63 (+63 = lowest sensitivity; -64 = highest sensitivity)
    // 5..7    |0      |
    // 0..4    |0..31  | CS current scale 0..31 -> 1/32 .. 32/32

    // -> c041f
    for(int i= 0; i < num_bytes; )
    {
        cfg_data[i++] = 0xc0;
        cfg_data[i++] = 0x41;
        cfg_data[i++] = 0xfc;
        cfg_data[i++] = 0x04;
        cfg_data[i++] = 0x1f;
    }
    hal_spi_do_transaction(STEPPER_SPI, &cfg_data[0], num_bytes, &spi_receive_buffer[0]);


    // DRVCONF
    //
    // Bit     |Range  |Meaning
    //---------+-------+------------------
    // 17..19  |1      |
    // 16      |0/1    | Test mode (0=normal operation; 1= test)
    // 14 + 15 |0..3   | slope control high (0=min 3=max)
    // 12 + 13 |0..3   | slope control low (0=min 3=max)
    // 11      |0      |
    // 10      |0/1    |short to gnd detection(0=enabled 1=disabled)
    // 8 + 9   |0..3   |short to gnd detection timer (0=3.2us 1=1.6us 2=1.2us 3=0.8us)
    // 7       |0/1    |Step/Dir interface (0=enabled 1=disabled)
    // 6       |0/1    |Sense Resistor full scale(0=305mV 1=165mV)
    // 4 + 5   |0..3   |Response frame format (0=microstep position; 1=SG; 2=SG+cool Step; 3=do not use)
    // 0..3    |0      |

    // -> ef4e0
    for(int i= 0; i < num_bytes; )
    {
        cfg_data[i++] = 0xef;
        cfg_data[i++] = 0x4e;
        cfg_data[i++] = 0x0e;
        cfg_data[i++] = 0xf4;
        cfg_data[i++] = 0xe0;
    }
    hal_spi_do_transaction(STEPPER_SPI, &cfg_data[0], num_bytes, &spi_receive_buffer[0]);


    // SMARTEN
    //
    // Bit     |Range  |Meaning
    //---------+-------+------------------
    // 19      |1      |
    // 18      |0      |
    // 17      |1      |
    // 16      |0      |
    // 15      |0/1    |SEIMIN Minimum Motor current 0=1/2 CS 1= 1/4 CS
    // 14 + 13 |0..3   |SEDN Number of measurements >= upper threshold to decrement current (0 = 32 1=8 2=2 3=1)
    // 16      |0      |
    // 8..11   |0..15  |SEMAX upper threshold = (SEMIN + SEMAX + 1)*32
    // 7       |0      |
    // 5 + 6   |0.. 3  |SEUP Number of measurements <= lower threshold to increment current(0=1 1=2 2=4 3=8)
    // 4       |0      |
    // 0..3    |0..15  |SEMIN lower threshold

    // -> a0000
    for(int i= 0; i < num_bytes; )
    {
        cfg_data[i++] = 0xa0;
        cfg_data[i++] = 0x00;
        cfg_data[i++] = 0x0a;
        cfg_data[i++] = 0x00;
        cfg_data[i++] = 0x00;
    }
    hal_spi_do_transaction(STEPPER_SPI, &cfg_data[0], num_bytes, &spi_receive_buffer[0]);

#ifdef USE_STEP_DIR

#else

#endif
#endif
}

// end of File
