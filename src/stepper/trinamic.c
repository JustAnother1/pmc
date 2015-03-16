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
#include "trinamic.h"
#include "hal_cfg.h"
#include "hal_spi.h"
#include "hal_cpu.h"

// 20 bit per stepper in Bytes
//(1 Stepper =  3 Bytes; ( 4 bits unused)
// 2 Stepper =  5 Bytes;
// 4 Stepper = 10 Bytes
// 6 Stepper = 15 Bytes
// 8 Stepper = 20 Bytes)
#define SPI_BUFFER_LENGTH           20

typedef struct {
    uint_fast8_t step_mode;
    uint_fast8_t drvctrl_microsteps;
    uint_fast8_t chop_toff;
}lstepper_configuration_typ;


// configuration buffer
static lstepper_configuration_typ stepper_conf[8];
static uint8_t drvctrl_data[SPI_BUFFER_LENGTH];
static uint8_t chopconf_data[SPI_BUFFER_LENGTH];
static uint8_t smarten_data[SPI_BUFFER_LENGTH];
static uint8_t sgcsconf_data[SPI_BUFFER_LENGTH];
static uint8_t drvconf_data[SPI_BUFFER_LENGTH];
static uint_fast8_t num_bytes_used;

#ifdef USE_STEP_DIR

void periodic_status_check(void);

#else

static uint8_t next_step_buffer[3];

// DRVCTRL in SPI Mode:
//
// Bit     |Range  |Meaning
//---------+-------+------------------
// 19 + 18 |0      |
// 17      |0/1    |Polarity A
// 9-16    |0..255 |Current in coil A
// 8       |0/1    |Polarity B
// 0..7    |0..255 |Current in coil B
static const uint8_t DRVCONTROL_Buffer_1[32][3] = {
        // !!! last hex char must be 0 !!!
        //  0,    1,    2
        {0x1f, 0x00, 0x00}, // 0
        {0x1e, 0x63, 0x00}, // 1
        {0x1c, 0xa5, 0xf0}, // 2
        {0x19, 0xc8, 0x90}, // 3
        {0x15, 0xea, 0xf0}, // 4
        {0x11, 0x2c, 0xe0}, // 5
        {0x0b, 0xee, 0x50}, // 6
        {0x06, 0x0f, 0x30}, // 7
        {0x20, 0x0f, 0x80}, // 8
        {0x26, 0x0f, 0x30}, // 9
        {0x2b, 0xee, 0x50}, // 10
        {0x31, 0x2c, 0xe0}, // 11
        {0x35, 0xea, 0xf0}, // 12
        {0x39, 0xc8, 0x90}, // 13
        {0x3c, 0xa5, 0xf0}, // 14
        {0x3e, 0x63, 0x00}, // 15
        {0x3f, 0x00, 0x00}, // 16
        {0x3e, 0x73, 0x00}, // 17
        {0x3c, 0xb5, 0xf0}, // 18
        {0x39, 0xd8, 0x90}, // 19
        {0x35, 0xfa, 0xf0}, // 20
        {0x31, 0x3c, 0xe0}, // 21
        {0x2b, 0xf5, 0xe0}, // 22
        {0x26, 0x1f, 0x30}, // 23
        {0x00, 0x1f, 0x80}, // 24
        {0x06, 0x1f, 0x30}, // 25
        {0x0b, 0xfe, 0x50}, // 26
        {0x11, 0x3c, 0xe0}, // 27
        {0x15, 0xfa, 0xf0}, // 28
        {0x19, 0xd8, 0x90}, // 29
        {0x1c, 0xb5, 0xf0}, // 30
        {0x1e, 0x73, 0x00}  // 31
};
static const uint8_t DRVCONTROL_Buffer_2[32][3] = {
        // !!! first hex char must be 0 !!!
        //  0,    1,    2
        {0x01, 0xf0, 0x00}, // 0
        {0x01, 0xe6, 0x30}, // 1
        {0x01, 0xca, 0x5f}, // 2
        {0x01, 0x9c, 0x89}, // 3
        {0x01, 0x5e, 0xaf}, // 4
        {0x01, 0x12, 0xce}, // 5
        {0x00, 0xbe, 0xe5}, // 6
        {0x00, 0x60, 0xf3}, // 7
        {0x02, 0x00, 0xf8}, // 8
        {0x02, 0x60, 0xf3}, // 9
        {0x02, 0xbe, 0xe5}, // 10
        {0x03, 0x12, 0xce}, // 11
        {0x03, 0x5e, 0xaf}, // 12
        {0x03, 0x9c, 0x89}, // 13
        {0x03, 0xca, 0x5f}, // 14
        {0x03, 0xe6, 0x30}, // 15
        {0x03, 0xf0, 0x00}, // 16
        {0x03, 0xe7, 0x30}, // 17
        {0x03, 0xcb, 0x5f}, // 18
        {0x03, 0x9d, 0x89}, // 19
        {0x03, 0x5f, 0xaf}, // 20
        {0x03, 0x13, 0xce}, // 21
        {0x02, 0xbf, 0x5e}, // 22
        {0x02, 0x61, 0xf3}, // 23
        {0x00, 0x01, 0xf8}, // 24
        {0x00, 0x61, 0xf3}, // 25
        {0x00, 0xbf, 0xe5}, // 26
        {0x01, 0x13, 0xce}, // 27
        {0x01, 0x5f, 0xaf}, // 28
        {0x01, 0x9d, 0x89}, // 29
        {0x01, 0xcb, 0x5f}, // 30
        {0x01, 0xe7, 0x30}  // 31
};

uint8_t* get_next_step(uint_fast8_t stepper_num, bool direction_is_increasing);

#endif

uint_fast8_t cur_step[8];
static uint8_t spi_receive_buffer[SPI_BUFFER_LENGTH];

uint_fast8_t trinamic_detect_number_of_steppers(void)
{
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
}

void trinamic_enable_stepper(uint_fast8_t stepper_num)
{
    /*
    switch(stepper_num)
    {
    case 0: chopconf_data[2]  |= (0x0f & stepper_conf[stepper_num].chop_toff); break;
    case 1: chopconf_data[4]  |= (0xf0 & stepper_conf[stepper_num].chop_toff); break;
    case 2: chopconf_data[7]  |= (0x0f & stepper_conf[stepper_num].chop_toff); break;
    case 3: chopconf_data[9]  |= (0xf0 & stepper_conf[stepper_num].chop_toff); break;
    case 4: chopconf_data[12] |= (0x0f & stepper_conf[stepper_num].chop_toff); break;
    case 5: chopconf_data[14] |= (0xf0 & stepper_conf[stepper_num].chop_toff); break;
    case 6: chopconf_data[17] |= (0x0f & stepper_conf[stepper_num].chop_toff); break;
    case 7: chopconf_data[19] |= (0xf0 & stepper_conf[stepper_num].chop_toff); break;
    default: return;
    }
    hal_spi_do_transaction(STEPPER_SPI, &chopconf_data[0], num_bytes_used, &spi_receive_buffer[0]);
    */
}

void trinamic_disable_stepper(uint_fast8_t stepper_num)
{
    /*
    switch(stepper_num)
    {
    case 0: chopconf_data[2]  &= 0x0f; break;
    case 1: chopconf_data[4]  &= 0xf0; break;
    case 2: chopconf_data[7]  &= 0x0f; break;
    case 3: chopconf_data[9]  &= 0xf0; break;
    case 4: chopconf_data[12] &= 0x0f; break;
    case 5: chopconf_data[14] &= 0xf0; break;
    case 6: chopconf_data[17] &= 0x0f; break;
    case 7: chopconf_data[19] &= 0xf0; break;
    default: return;
    }
    hal_spi_do_transaction(STEPPER_SPI, &chopconf_data[0], num_bytes_used, &spi_receive_buffer[0]);
    */
}

void trinamic_configure_steppers(uint_fast8_t num_steppers)
{
    int i;
    num_bytes_used =((num_steppers+1)/2)*5;

    if(0 == num_steppers)
    {
        // TODO Event?
        return;
    }
    // TODO get complete stepper configuration
    // TODO get configuration of Which Microstep Level to use (Full Steps, half steps, .., 1/256 Steps)
    // until then,...
    for(i = 0; i < 8; i++)
    {
#ifdef USE_STEP_DIR
        stepper_conf[i].drvctrl_microsteps = MICROSTEP_1_1_STEP;
#else
        stepper_conf[i].step_mode = STEP_MODE_FULL_WAVE;
#endif
        stepper_conf[i].chop_toff = 0xff;
    }

    for(i = 0; i < 8; i++)
    {
        cur_step[i] = 0;
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

    // stepper disabled -> 9a9f0
    // stepper enabled  -> 9a9f2
    // stepper are initially disabled !
    for(i= 0; i < num_bytes_used; )
    {
        chopconf_data[i++] = 0x9a;
        chopconf_data[i++] = 0x9f;
        chopconf_data[i++] = 0x09;
        chopconf_data[i++] = 0xa9;
        chopconf_data[i++] = 0xf0;
    }
    hal_spi_do_transaction(STEPPER_SPI, &chopconf_data[0], num_bytes_used, &spi_receive_buffer[0]);

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
    // d001f
    for(i= 0; i < num_bytes_used; )
    {
        sgcsconf_data[i++] = 0xd3;
        sgcsconf_data[i++] = 0xf1;
        sgcsconf_data[i++] = 0xfd;
        sgcsconf_data[i++] = 0x3f;
        sgcsconf_data[i++] = 0x1f;
        /*
        sgcsconf_data[i++] = 0xc0;
        sgcsconf_data[i++] = 0x41;
        sgcsconf_data[i++] = 0xfc;
        sgcsconf_data[i++] = 0x04;
        sgcsconf_data[i++] = 0x1f;
        */
    }
    hal_spi_do_transaction(STEPPER_SPI, &sgcsconf_data[0], num_bytes_used, &spi_receive_buffer[0]);

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
#ifdef USE_STEP_DIR
    // -> ef460
    // ef060
    for(i= 0; i < num_bytes_used; )
    {
        drvconf_data[i++] = 0xef;
        drvconf_data[i++] = 0x06;
        drvconf_data[i++] = 0x0e;
        drvconf_data[i++] = 0xf0;
        drvconf_data[i++] = 0x60;
    }
#else
    // -> ef4e0
    // ef0e0
    for(i= 0; i < num_bytes_used; )
    {
        drvconf_data[i++] = 0xef;
        drvconf_data[i++] = 0x0e;
        drvconf_data[i++] = 0x0e;
        drvconf_data[i++] = 0xf0;
        drvconf_data[i++] = 0xe0;
    }
#endif
    hal_spi_do_transaction(STEPPER_SPI, &drvconf_data[0], num_bytes_used, &spi_receive_buffer[0]);

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
    // new : a2a63
    for(i= 0; i < num_bytes_used; )
    {
        smarten_data[i++] = 0xa0;
        smarten_data[i++] = 0x00;
        smarten_data[i++] = 0x0a;
        smarten_data[i++] = 0x00;
        smarten_data[i++] = 0x00;
        /*
        smarten_data[i++] = 0xa2;
        smarten_data[i++] = 0xa6;
        smarten_data[i++] = 0x3a;
        smarten_data[i++] = 0x2a;
        smarten_data[i++] = 0x63;
        */
    }
    hal_spi_do_transaction(STEPPER_SPI, &smarten_data[0], num_bytes_used, &spi_receive_buffer[0]);

#ifdef USE_STEP_DIR
    // DRVCTRL in STEP/DIR Mode:
    //
    // Bit     |Range  |Meaning
    //---------+-------+------------------
    // -> 00108
    int cur_stepper = 0;
    for(i= 0; i < num_bytes_used; )
    {
        drvctrl_data[i++] = 0x00;
        drvctrl_data[i++] = 0x10;
        drvctrl_data[i++] = 0x00 | (stepper_conf[cur_stepper].drvctrl_microsteps << 8);
        cur_stepper++;
        drvctrl_data[i++] = 0x01;
        drvctrl_data[i++] = 0x00 | stepper_conf[cur_stepper].drvctrl_microsteps;
    }

    hal_cpu_add_ms_tick_function(&periodic_status_check);

#else
    // TODO 1f000
    for(i= 0; i < num_bytes_used; )
    {
        drvctrl_data[i++] = 0x1f;
        drvctrl_data[i++] = 0x00;
        drvctrl_data[i++] = 0x01;
        drvctrl_data[i++] = 0xf0;
        drvctrl_data[i++] = 0x00;
    }
#endif
    hal_spi_do_transaction(STEPPER_SPI, &drvctrl_data[0], num_bytes_used, &spi_receive_buffer[0]);


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

    // stepper disabled -> 9a9f0
    // stepper enabled  -> 9a9f2
    // stepper are initially disabled !
    for(i= 0; i < num_bytes_used; )
    {
        chopconf_data[i++] = 0x9a;
        chopconf_data[i++] = 0x9f;
        chopconf_data[i++] = 0xf9;
        chopconf_data[i++] = 0xa9;
        chopconf_data[i++] = 0xff;
    }
    hal_spi_do_transaction(STEPPER_SPI, &chopconf_data[0], num_bytes_used, &spi_receive_buffer[0]);

}

#ifdef USE_STEP_DIR

void periodic_status_check(void)
{
    hal_spi_start_spi_transaction(STEPPER_SPI, &smarten_data[0], num_bytes_used, &spi_receive_buffer[0]);
    // TODO check reply
}

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
static const uint8_t STEP_MODE_WAVE_TABLE[4][3] = {
        // !!! last hex char must be 0 !!!
        //  Byte Index      | current    | Microstep
        //  0,    1,    2   |     A    B | 1/1   1/2
        //------------------+------------+----------
        {0x1f, 0x00, 0x00}, // +248    0 |  0     0
        {0x00, 0x0f, 0x80}, //    0 +248 |  1     2
        {0x3f, 0x00, 0x00}, // -248    0 |  2     4
        {0x00, 0x1f, 0x80}, //    0 -248 |  3     6
};



// DRVCTRL in SPI Mode:
//
// Bit     |Range  |Meaning
//---------+-------+------------------
// 19 + 18 |0      |
// 17      |0/1    |Polarity A
// 9-16    |0..255 |Current in coil A
// 8       |0/1    |Polarity B
// 0..7    |0..255 |Current in coil B
static const uint8_t STEP_MODE_FULL_HALF[8][3] = {
        // !!! last hex char must be 0 !!!
        //  Byte Index      | current    | Microstep
        //  0,    1,    2   |     A    B | 1/1   1/2
        //------------------+------------+----------
        {0x1f, 0x00, 0x00}, // +248    0 |  0     0
        {0x1f, 0x0f, 0x80}, // +248 +248 |        1
        {0x00, 0x0f, 0x80}, //    0 +248 |  1     2
        {0x3f, 0x0f, 0x80}, // -248 +248 |        3
        {0x3f, 0x00, 0x00}, // -248    0 |  2     4
        {0x3f, 0x1f, 0x80}, // -248 -248 |        5
        {0x00, 0x1f, 0x80}, //    0 -248 |  3     6
        {0x1f, 0x1f, 0x80}, // +248 -248 |        7
};



// step is in the first 2 bytes and the 4 higher bits of the 3rd byte.
// The 4 low bits of the 3rd byte are always 0 !
uint8_t* get_next_step(uint_fast8_t stepper_num, bool direction_is_increasing)
{
    switch (stepper_conf[stepper_num].step_mode)
    {
    case STEP_MODE_FULL_WAVE:
        if(true == direction_is_increasing)
        {
            cur_step[stepper_num]++;
            if(4 == cur_step[stepper_num])
            {
                cur_step[stepper_num] = 0;
            }
        }
        else
        {
            if(0 == cur_step[stepper_num])
            {
                cur_step[stepper_num] = 3;
            }
            else
            {
                cur_step[stepper_num]--;
            }
        }
        next_step_buffer[0] = STEP_MODE_WAVE_TABLE[cur_step[stepper_num]][0];
        next_step_buffer[1] = STEP_MODE_WAVE_TABLE[cur_step[stepper_num]][1];
        next_step_buffer[2] = STEP_MODE_WAVE_TABLE[cur_step[stepper_num]][2];
        break;

    case STEP_MODE_FULL_TWO_COILS_ACTIVE:
        if(true == direction_is_increasing)
        {
            cur_step[stepper_num] += 2;
            if(8 == cur_step[stepper_num])
            {
                cur_step[stepper_num] = 0;
            }
        }
        else
        {
            if(0 == cur_step[stepper_num])
            {
                cur_step[stepper_num] = 6;
            }
            else
            {
                cur_step[stepper_num] -= 2;
            }
        }
        next_step_buffer[0] = STEP_MODE_FULL_HALF[cur_step[stepper_num]][0];
        next_step_buffer[1] = STEP_MODE_FULL_HALF[cur_step[stepper_num]][1];
        next_step_buffer[2] = STEP_MODE_FULL_HALF[cur_step[stepper_num]][2];
        break;

    case STEP_MODE_HALF_STEP:
        if(true == direction_is_increasing)
        {
            cur_step[stepper_num]++;
            if(8 == cur_step[stepper_num])
            {
                cur_step[stepper_num] = 0;
            }
        }
        else
        {
            if(0 == cur_step[stepper_num])
            {
                cur_step[stepper_num] = 7;
            }
            else
            {
                cur_step[stepper_num]--;
            }
        }
        next_step_buffer[0] = STEP_MODE_FULL_HALF[cur_step[stepper_num]][0];
        next_step_buffer[1] = STEP_MODE_FULL_HALF[cur_step[stepper_num]][1];
        next_step_buffer[2] = STEP_MODE_FULL_HALF[cur_step[stepper_num]][2];
        break;

    default:
        // TODO event
        next_step_buffer[0] = 0;
        next_step_buffer[1] = 0;
        next_step_buffer[2] = 0;
        break;
    }
    return &next_step_buffer[0];
}


void trinamic_make_step_using_SPI(uint_fast8_t stepper_num, bool direction_is_increasing)
{
    int offset = 0;
    bool shift_nibble = false;
    uint8_t * next_step;
    // step is in the first 2 bytes and the 4 higher bits of the 3rd byte.
    // The 4 low bits of the 3rd byte are always 0 !
    next_step = get_next_step(stepper_num, direction_is_increasing);

    switch(stepper_num)
    {
    case 0: shift_nibble = false; offset = 0;  break;
    case 1: shift_nibble = true;  offset = 2;  break;
    case 2: shift_nibble = false; offset = 5;  break;
    case 3: shift_nibble = true;  offset = 7;  break;
    case 4: shift_nibble = false; offset = 10; break;
    case 5: shift_nibble = true;  offset = 12; break;
    case 6: shift_nibble = false; offset = 15; break;
    case 7: shift_nibble = true;  offset = 17; break;
    default: return;
    }

    if(false == shift_nibble)
    {
        drvctrl_data[offset + 0] = next_step[0];
        drvctrl_data[offset + 1] = next_step[1];
        drvctrl_data[offset + 2] = (drvctrl_data[offset + 2] & 0x0f) | (0xf0 & next_step[2]);
    }
    else
    {
        drvctrl_data[offset + 0] = (drvctrl_data[offset + 0] & 0xf0) | (0x0f & (next_step[0]>>4));
        drvctrl_data[offset + 1] = (0xf0 & (next_step[0]<<4)) | (0x0f & (next_step[1]>>4));
        drvctrl_data[offset + 2] = (0xf0 & (next_step[1]<<4)) | (0x0f & (next_step[2]>>4));
    }

    hal_spi_start_spi_transaction(STEPPER_SPI,
                                  &drvctrl_data[0],
                                  num_bytes_used,
                                  &spi_receive_buffer[0]);
}
#endif

