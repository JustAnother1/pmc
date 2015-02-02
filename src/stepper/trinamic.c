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
#include <stdbool.h>
#include "hal_spi.h"

// 20 bit per stepper in Bytes
//(1 Stepper =  3 Bytes; ( 4 bits unused)
// 2 Stepper =  5 Bytes;
// 4 Stepper = 10 Bytes
// 6 Stepper = 15 Bytes
// 8 Stepper = 20 Bytes)
#define SPI_BUFFER_LENGTH           20

#ifdef USE_STEP_DIR
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
static uint8_t spi_receive_buffer[SPI_BUFFER_LENGTH];

uint_fast8_t step_detect_number_of_steppers(void)
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

void step_configure_steppers(uint_fast8_t num_steppers)
{
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
}

#ifdef USE_STEP_DIR
#else
void make_step_using_SPI(void)
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
#endif

