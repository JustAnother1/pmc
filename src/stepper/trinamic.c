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
#include <stdio.h>
#include <stdlib.h>
#include "trinamic.h"
#include "hal_cfg.h"
#include "hal_spi.h"
#include "hal_cpu.h"
#include "hal_debug.h"

/*
 * D E F I N E S
 */

// 20 bit per stepper in Bytes
//(1 Stepper =  3 Bytes; ( 4 bits unused)
// 2 Stepper =  5 Bytes;
// 4 Stepper = 10 Bytes
// 6 Stepper = 15 Bytes
// 8 Stepper = 20 Bytes)
#define SPI_BUFFER_LENGTH           20
#define MAX_NUM_STEPPERS            8
#define NUM_CFG_REGISTERS           5

/*
 *  T Y P E S
 */

    typedef struct {
        // DRVCTRL
    #ifdef USE_STEP_DIR
        bool stepInterpolation;
        bool doubleEdgeStepPulse;
        uint_fast8_t microstepResolution;
    #else
        uint_fast8_t step_mode;
    #endif
        // CHOPCONF
        uint_fast8_t blankingTime;
        bool chopperMode;
        bool randomToff;
        uint_fast8_t hysteresisDecrementTime;
        bool fastDecayMode;
        uint_fast8_t hysteresisEnd;
        uint_fast8_t sineWaveOffset;
        uint_fast8_t hysteresisStartOffset;
        uint_fast8_t fastDecayTime;
        uint_fast8_t toff;
        // SMARTEN
        bool seIMin;
        uint_fast8_t decrementSpeed;
        uint_fast8_t seUpper;
        uint_fast8_t seUpStep;
        uint_fast8_t seLower;
        //SGCSCONF
        bool sgFilter;
        int_fast8_t sgThreshold;
        uint_fast8_t sgCS;
        // DRVCONF
        bool test;
        uint_fast8_t slopeHigh;
        uint_fast8_t slopeLow;
        uint_fast8_t shortGNDdisabled;
        uint_fast8_t shortGNDtimer;
        bool disableSTEPDIR;
        bool lowVoltageRsense;
        uint_fast8_t responseFormat;

    }lstepper_configuration_typ;

    enum cfgSetting {
        // DRVCTRL
        stepInterpolation,
        doubleEdge,
        microstepResolution,
        // CHOPCONF
        blankingTime,
        chopperMode,
        randomToff,
        hysteresisDecrementTime,
        fastDecayMode,
        hysteresisEnd,
        sineWaveOffset,
        hysteresisStartOffset,
        fastDecayTime,
        toff,
        // SMARTEN
        seIMin,
        decrementSpeed,
        seUpper,
        seUpStep,
        seLower,
        // SGCSCONF
        sgFilter,
        sgThreshold,
        sgCS,
        // DRVCONF
        test,
        slopeHigh,
        slopeLow,
        shortGNDdisabled,
        shortGNDtimer,
        disableSTEPDIR,
        lowVoltageRsense,
        responseFormat,
    };

    enum cfgRegisters {
        DRVCTRL  = 0,
        CHOPCONF = 1,
        SMARTEN  = 2,
        SGCSCONF = 3,
        DRVCONF  = 4,
    };

/*
 * S T A T I C   F U N C T I O N S
 */
static void setBit(enum cfgRegisters reg, int bit);
static void resetBit(enum cfgRegisters reg, int bit);
static void writeInt(int value,enum cfgRegisters reg, int bit, int bits);
static void setInt(int value, enum cfgSetting Setting, int stepper);
static void setBool(bool value, enum cfgSetting Setting, int stepper);

#ifdef USE_STEP_DIR
    static void periodic_status_check(void);
#else
    static uint8_t* get_next_step(uint_fast8_t stepper_num, bool direction_is_increasing);
#endif

/*
 *  S T A T I C   V A R I A B L E S
 */

static lstepper_configuration_typ stepper_conf[MAX_NUM_STEPPERS];
static uint8_t cfg_data[5][SPI_BUFFER_LENGTH];
static uint_fast8_t num_bytes_used;
static uint_fast8_t steppers_detected_on_last_detection;
static uint8_t spi_receive_buffer[SPI_BUFFER_LENGTH];

#ifdef USE_STEP_DIR

#else
static uint_fast8_t cur_step[8];
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

#endif

/*
 * P U B L I C   F U N C T I O N S
 */

void trinamic_init(void)
{
    uint_fast8_t i;
    hal_spi_init(STEPPER_SPI);
    steppers_detected_on_last_detection = 0;

    // clean configuration Buffers
    for(i = 0; i < NUM_CFG_REGISTERS ; i++)
    {
        uint_fast8_t j;
        for(j = 0; j < SPI_BUFFER_LENGTH; j++)
        {
            cfg_data[i][j] = 0;
        }
    }

    // default configuration
    // DRVCTRL  = 0x00100
    // CHOPCONF = 0x9a9f0 // stepper enabled = 0x 9a9ff
    // SMARTEN  = 0xa0000
    // SGCSCONF = 0xd3f1f
    // DRVCONF  = 0xef060
    for(i = 0; i < MAX_NUM_STEPPERS; i++)
    {
        // set Register Address bits
        // bit 19
        setBit(CHOPCONF, 19 + (i * 20));
        setBit(SMARTEN,  19 + (i * 20));
        setBit(SGCSCONF, 19 + (i * 20));
        setBit(DRVCONF,  19 + (i * 20));
        // bit 18
        setBit(SGCSCONF, 18 + (i * 20));
        setBit(DRVCONF,  18 + (i * 20));
        // bit 17
        setBit(SMARTEN,  17 + (i * 20));
        setBit(DRVCONF,  17 + (i * 20));

        // Drive Control DRVCTRL
        // Step Interpolation
        // - false = Disabled
        // - true  = Enabled
        stepper_conf[i].stepInterpolation = false;
        // Double Edge Step Pulses
        // - false = only rising edge
        // - true  = both edges trigger Steps
        stepper_conf[i].doubleEdgeStepPulse = true;
        // Micro Step Resolution MRES(4 Bit)
        // - 0 = 1/256
        // - 1 = 1/128
        // - 2 = 1/64
        // - 3 = 1/32
        // - 4 = 1/16
        // - 5 = 1/8
        // - 6 = 1/4
        // - 7 = half steps
        // - 8 = full Steps
        stepper_conf[i].microstepResolution = 8;

        // Chopper Control CHOPCONF
        // Blanking Time TBL(2 Bit)
        // - 0 = 16
        // - 1 = 24
        // - 2 = 36
        // - 3 = 54 (in System Clocks)
        stepper_conf[i].blankingTime = 3;
        // Chopper Mode (CHM)
        // - false = Spread Cycle
        // - true = Constant Toff with fast decay
        stepper_conf[i].chopperMode  = false;
        // Random Toff (RNDTF)
        // - false = Toff is fixed
        // - true  = Toff = Toff + (-12..+3) clocks
        stepper_conf[i].randomToff = true;
        // Hysteresis Decrement Time HDEC (2 Bit)
        // - 0 = 16
        // - 1 = 32
        // - 2 = 48
        // - 3 = 64 (in System clock cycles)
        stepper_conf[i].hysteresisDecrementTime = 1;
        // Fast Decay mode HDEC (1 bit)
        // - false = fast decay can be finished sooner
        // - true  = fast decay time defined by timer
        stepper_conf[i].fastDecayMode = true;
        // Hysteresis end (low value) HEND (4bit)
        // -  0 = -3
        // -  1 = -2
        // -  2 = -1
        // -  3 =  0
        // -  4 =  1
        // -  5 =  2
        // -  6 =  3
        // -  7 =  4
        // -  8 =  5
        // -  9 =  6
        // - 10 =  7
        // - 11 =  8
        // - 12 =  9
        // - 13 = 10
        // - 14 = 11
        // - 15 = 12
        stepper_conf[i].hysteresisEnd = 3;
        // Sine Wave Offset HEND (4 Bit)
        // -  0 = -3
        // -  1 = -2
        // -  2 = -1
        // -  3 =  0
        // -  4 =  1
        // -  5 =  2
        // -  6 =  3
        // -  7 =  4
        // -  8 =  5
        // -  9 =  6
        // - 10 =  7
        // - 11 =  8
        // - 12 =  9
        // - 13 = 10
        // - 14 = 11
        // - 15 = 12
        stepper_conf[i].sineWaveOffset = 3;
        // Hysteresis start Value HSTRT (3 bit) Hysteresis start offset from HEND
        // - 0 = 1
        // - 1 = 2
        // - 2 = 3
        // - 3 = 4
        // - 4 = 5
        // - 5 = 6
        // - 6 = 7
        // - 7 = 8
        // HEND + HSTRT must be <=15
        stepper_conf[i].hysteresisStartOffset = 7;
        // Fast decay time setting HSTRT (4bit)
        stepper_conf[i].fastDecayTime = 7;
        // Off Time / MOSFET disable Toff (4 Bit)
        // - 0 = Drive disable, bridges off
        // - 1 = 1 (TBL>= 24 clk)
        // - 2 = 2, .. 15 = 15
        stepper_conf[i].toff = 15;//0;

        // coolStep Control Register SMARTEN
        //Minimum coolStep current(SEIMIN)
        // - false = 1/2 Current Scale (CS)
        // - true  = 1/4 CS
        stepper_conf[i].seIMin = false;
        // Current decrement Speed (SEDN) 2bit
        // Number of times that the stallGurad2 value must be sampled equal to
        // or above the upper threshold for each decrement of the coil current.
        // - 0 = 32
        // - 1 = 8
        // - 2 = 2
        // - 3 = 1
        stepper_conf[i].decrementSpeed = 0;
        // Upper coolStep Threshold (offset from the lower threshold) 4bit
        // - 0 .. 15
        stepper_conf[i].seUpper = 0;
        // current increment size 2 bit
        // - 0 = 1
        // - 1 = 2
        // - 2 = 4
        // - 3 = 8
        stepper_conf[i].seUpStep = 0;
        // Lower coolStep threshold / disable 4bit
        // - 0 = disabled
        // - 1..15
        stepper_conf[i].seLower = 0;

        // stallGuard2 Control Register SGCSCONF
        // Filter enable
        // - false = standard mode, fastest response time
        // - true  = filtered mode, updated once each 4 full steps, highest accuracy
        stepper_conf[i].sgFilter = true;
        // threshold
        // two's compliment signed int
        // Range +63 .. -64
        // Values below -10 are not recommended
        // lower value = higher sensitivity
        stepper_conf[i].sgThreshold = 63;
        // Current scale
        // - 0 = 1/32
        // - 1 = 2/32
        // - ...
        // - 31 = 32/32
        stepper_conf[i].sgCS = 31;

        // Driver Control Register DRVCONF
        // Test Mode
        // - false = disabled - normal operation
        // - true  = enabled - test mode
        stepper_conf[i].test = false;
        // Slope Control High Side 2 bit
        // - 0 = Minimum
        // - 1 = Minimum temperature compensation mode
        // - 2 = Medium temperature compensation mode
        // - 3 = maximum
        stepper_conf[i].slopeHigh = 3;
        // Slope Control low Side 2 bit
        // - 0 = Minimum
        // - 1 = Minimum
        // - 2 = Medium
        // - 3 = maximum
        stepper_conf[i].slopeLow = 3;
        // Short to Ground protection disable
        // - false = protection enabled
        // - true  = protection disabled
        stepper_conf[i].shortGNDdisabled = false;
        // short to ground(GND) detection timer
        // - 0 = 3,2µs
        // - 1 = 1.6µs
        // - 2 = 1.2µs
        // - 3 = 0.8µs
        stepper_conf[i].shortGNDtimer = 0;
        // disable STEp/DIR interface
        // - false = STEP/DIR interface  enabled
        // - true  = STEP/DIR interface disabled
        stepper_conf[i].disableSTEPDIR = false;
        // Sense Resistor voltage based current scaling
        // - false = full scale voltage is 305mV
        // - true  = full scale voltage is 165 mV
        stepper_conf[i].lowVoltageRsense = true;
        // Response Data Format
        // - 0 = Microstep Position
        // - 1 = stallGuard2 level
        // - 2 = stallGuard2 level and coolStep current level
        // - 3 = reserved. do not use
        stepper_conf[i].responseFormat = 2;
    }
}

void trinamic_configure_steppers(uint_fast8_t num_steppers)
{
    uint_fast8_t i;

    for(i = 0; i < num_steppers; i++)
    {
        // DRVCTRL
#ifdef USE_STEP_DIR
        setBool(stepper_conf[i].stepInterpolation,   stepInterpolation,   i);
        setBool(stepper_conf[i].doubleEdgeStepPulse, doubleEdge,          i);
        setInt( stepper_conf[i].microstepResolution, microstepResolution, i);
#else
        cfg_data[DRVCTRL][(i/2)*5 + 0] = 0x1f;
        cfg_data[DRVCTRL][(i/2)*5 + 1] = 0x00;
        cfg_data[DRVCTRL][(i/2)*5 + 2] = 0x01;
        cfg_data[DRVCTRL][(i/2)*5 + 3] = 0xf0;
        cfg_data[DRVCTRL][(i/2)*5 + 4] = 0x00;

        stepper_conf[i].step_mode = STEP_MODE_FULL_WAVE;
        for(i = 0; i < 8; i++)
        {
            cur_step[i] = 0;
        }
#endif

        // CHOPCONF
        setInt( stepper_conf[i].blankingTime,        blankingTime,        i);
        setBool(stepper_conf[i].chopperMode,         chopperMode,         i);
        setBool(stepper_conf[i].randomToff,          randomToff,          i);
        if(false == stepper_conf[i].chopperMode)
        {
            setInt( stepper_conf[i].hysteresisDecrementTime, hysteresisDecrementTime, i);
            setInt( stepper_conf[i].hysteresisEnd,           hysteresisEnd,           i);
            setInt( stepper_conf[i].hysteresisStartOffset,   hysteresisStartOffset,   i);
        }
        else
        {
            setBool(stepper_conf[i].fastDecayMode,  fastDecayMode,  i);
            setInt( stepper_conf[i].sineWaveOffset, sineWaveOffset, i);
            setInt( stepper_conf[i].fastDecayTime,  fastDecayTime,  i);
        }
        setInt( stepper_conf[i].toff,           toff,           i);
        //SMARTEN
        setBool(stepper_conf[i].seIMin,         seIMin,         i);
        setInt( stepper_conf[i].decrementSpeed, decrementSpeed, i);
        setInt( stepper_conf[i].seUpper,        seUpper,        i);
        setInt( stepper_conf[i].seUpStep,       seUpStep,       i);
        setInt( stepper_conf[i].seLower,        seLower,        i);
        // SGCSCONF
        setBool(stepper_conf[i].sgFilter,       sgFilter,       i);
        setInt( stepper_conf[i].sgThreshold,    sgThreshold,    i);
        setInt( stepper_conf[i].sgCS,           sgCS,           i);
        // DRVCONF
        setBool(stepper_conf[i].test,             test,             i);
        setInt( stepper_conf[i].slopeHigh,        slopeHigh,        i);
        setInt( stepper_conf[i].slopeLow,         slopeLow,         i);
        setBool(stepper_conf[i].shortGNDdisabled, shortGNDdisabled, i);
        setInt( stepper_conf[i].shortGNDtimer,    shortGNDtimer,    i);
        setBool(stepper_conf[i].disableSTEPDIR,   disableSTEPDIR,   i);
        setBool(stepper_conf[i].lowVoltageRsense, lowVoltageRsense, i);
        setInt( stepper_conf[i].responseFormat,   responseFormat,   i);
    }

    num_bytes_used =((num_steppers+1)/2)*5; // 20 bits per Stepper

    hal_spi_do_transaction(STEPPER_SPI, &cfg_data[DRVCTRL] [20 - num_bytes_used], num_bytes_used, &spi_receive_buffer[0]);
    hal_spi_do_transaction(STEPPER_SPI, &cfg_data[CHOPCONF][20 - num_bytes_used], num_bytes_used, &spi_receive_buffer[0]);
    hal_spi_do_transaction(STEPPER_SPI, &cfg_data[SMARTEN] [20 - num_bytes_used], num_bytes_used, &spi_receive_buffer[0]);
    hal_spi_do_transaction(STEPPER_SPI, &cfg_data[SGCSCONF][20 - num_bytes_used], num_bytes_used, &spi_receive_buffer[0]);
    hal_spi_do_transaction(STEPPER_SPI, &cfg_data[DRVCONF] [20 - num_bytes_used], num_bytes_used, &spi_receive_buffer[0]);

#ifdef USE_STEP_DIR
    hal_cpu_add_ms_tick_function(&periodic_status_check);
#else
#endif
}

bool trinamic_change_setting(uint8_t* setting)
{
    switch(*setting)
    {
    case 'S':
    case 's':
        debug_line("detected %d Steppers !", steppers_detected_on_last_detection);

        debug_msg("DRVCTRL  hex: ");
        for(int i = 0; i < num_bytes_used; i++)
        {
            debug_msg("%02X ", cfg_data[DRVCTRL][i]);
        }
        debug_line(" ");

        debug_msg("CHOPCONF hex: ");
        for(int i = 0; i < num_bytes_used; i++)
        {
            debug_msg("%02X ", cfg_data[CHOPCONF][i]);
        }
        debug_line(" ");

        debug_msg("SMARTEN  hex: ");
        for(int i = 0; i < num_bytes_used; i++)
        {
            debug_msg("%02X ", cfg_data[SMARTEN][i]);
        }
        debug_line(" ");

        debug_msg("SGCSCONF hex: ");
        for(int i = 0; i < num_bytes_used; i++)
        {
            debug_msg("%02X ", cfg_data[SGCSCONF][i]);
        }
        debug_line(" ");

        debug_msg("DRVCONF  hex: ");
        for(int i = 0; i < num_bytes_used; i++)
        {
            debug_msg("%02X ", cfg_data[DRVCONF][i]);
        }
        debug_line(" ");
        break;

    case 'C':
    case 'c':
    {
        enum cfgSetting settingToChange;
        uint_fast8_t numSteppers = trinamic_detect_number_of_steppers();
        setting++;

        //  3 4 5 6 7 8 9

        switch(*setting)
        {
        // DRVCTRL
        case 'i': settingToChange = stepInterpolation; break;
        case 'e': settingToChange = doubleEdge; break;
        case 'm': settingToChange = microstepResolution; break;
        // CHOPCONF
        case 'b': settingToChange = blankingTime; break;
        case 'c': settingToChange = chopperMode; break;
        case 'r': settingToChange = randomToff; break;
        case 'd': settingToChange = hysteresisDecrementTime; break;
        case 'f': settingToChange = fastDecayMode; break;
        case 'h': settingToChange = hysteresisEnd; break;
        case 'w': settingToChange = sineWaveOffset; break;
        case 's': settingToChange = hysteresisStartOffset; break;
        case 't': settingToChange = fastDecayTime; break;
        case 'o': settingToChange = toff; break;
        // SMARTEN
        case 'n': settingToChange = seIMin; break;
        case 'p': settingToChange = decrementSpeed; break;
        case 'u': settingToChange = seUpper; break;
        case '1': settingToChange = seUpStep; break;
        case '0': settingToChange = seLower; break;
        // SGCSCONF
        case 'g': settingToChange = sgFilter; break;
        case 'l': settingToChange = sgThreshold; break;
        case '2': settingToChange = sgCS; break;
        // DRVCONF
        case 'x': settingToChange = test; break;
        case 'a': settingToChange = slopeHigh; break;
        case 'j': settingToChange = slopeLow; break;
        case 'k': settingToChange = shortGNDdisabled; break;
        case 'q': settingToChange = shortGNDtimer; break;
        case 'v': settingToChange = disableSTEPDIR; break;
        case 'y': settingToChange = lowVoltageRsense; break;
        case 'z': settingToChange = responseFormat; break;
        }

        switch(settingToChange)
        {
        // DRVCTRL
        case stepInterpolation:
        case doubleEdge:
        // CHOPCONF
        case chopperMode:
        case randomToff:
        case fastDecayMode:
        // SMARTEN
        case seIMin:
        // SGCSCONF
        case sgFilter:
        // DRVCONF
        case test:
        case shortGNDdisabled:
        case disableSTEPDIR:
        case lowVoltageRsense:
            // BOOL
            if(*(++setting) == 't')
            {
                setBool(true, settingToChange, 1); // stepper 1
            }
            break;

        // DRVCTRL
        case microstepResolution:
        // CHOPCONF
        case blankingTime:
        case hysteresisDecrementTime:
        case hysteresisEnd:
        case sineWaveOffset:
        case hysteresisStartOffset:
        case fastDecayTime:
        case toff:
        // SMARTEN
        case decrementSpeed:
        case seUpper:
        case seUpStep:
        case seLower:
        // SGCSCONF
        case sgThreshold:
        case sgCS:
        // DRVCONF
        case slopeHigh:
        case slopeLow:
        case shortGNDtimer:
        case responseFormat:
            // Int
            setInt(atoi((char *)++setting), settingToChange, 1); // stepper 1
            break;

        }

        debug_line("configuring %d Steppers !", numSteppers);
        trinamic_configure_steppers(numSteppers);
    }
        break;

    default:
        return false;
    }
    return true;
}

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
    steppers_detected_on_last_detection = count;
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

#ifdef USE_STEP_DIR

void trinamic_print_stepper_status(void)
{
    int i;
    int step = 1;
    int num = steppers_detected_on_last_detection;
    // raw data
    debug_msg("hex: ");
    for(int i = 0; i < num_bytes_used; i++)
    {
        debug_msg("%02X ", spi_receive_buffer[i]);
    }
    debug_line(" ");
    i = 0;

    /*
     * Status Format:
     *
     * RDSEL (0,1,2)
     *
     *  Bit   | RDSEL  | RDSEL | RDSEL
     * Number |  0     |  1    |  2
     * -------+--------+-------+------
     *   19   | MSTEP9 | SG9   | SG9
     *   18   | MSTEP8 | SG8   | SG8
     *   17   | MSTEP7 | SG7   | SG7
     *   16   | MSTEP6 | SG6   | SG6
     *   15   | MSTEP5 | SG5   | SG5
     *   14   | MSTEP4 | SG4   | SE4
     *   13   | MSTEP3 | SG3   | SE3
     *   12   | MSTEP2 | SG2   | SE2
     *   11   | MSTEP1 | SG1   | SE1
     *   10   | MSTEP0 | SG0   | SE0
     *    9   | not used
     *    8   | not used
     *    7   | STST = Standstill detector           (0 = moving;          1 = standstill)
     *    6   | OLB  = Open Load Coil B              (0 = load detected;   1 = open load detected) only reliable during slow movement
     *    5   | OLA  = Open Load Coil A              (0 = load detected;   1 = open load detected) only reliable during slow movement
     *    4   | S2GB = short to GND detection Coil B (0 = OK;              1 = Short to GND on high side transistors)
     *    3   | S2GA = short to GND detection Coil A (0 = OK;              1 = Short to GND on high side transistors)
     *    2   | OTPW = over Temperature warning      (0 = Temperature OK,  1 = over Temperature !)
     *    1   | OT   = over Temperature shutdown     (0 = OK;              1 = Motor has been shut down due to over temperature condition)
     *    0   | SG   = Stall Guard                   (0 = motor is moving; 1 = stall guard current threshold has been reached)
     */

    while(i <= (num_bytes_used -5))
    {
        int sg = -1;
        int se = -1;

        if(step > num)
        {
            return;
        }
        debug_line("Stepper %d:", step);
        step ++;

        sg = (spi_receive_buffer[i] & 0xf8) * 4;
        se = (spi_receive_buffer[i] & 0x07) * 4 + ((spi_receive_buffer[i +1] & 0xc0)>>6);
        debug_line("SG: %4d", sg);
        debug_line("SE: %4d", se);
        if(0 != (spi_receive_buffer[i + 1] & 0x08))
        {
            debug_line("standing still");
        }
        else
        {
            debug_line("moving");
        }
        if(0 != (spi_receive_buffer[i + 1] & 0x04))
        {
            debug_line("open Load coil B");
        }
        if(0 != (spi_receive_buffer[i + 1] & 0x02))
        {
            debug_line("open Load coil A");
        }
        if(0 != (spi_receive_buffer[i + 1] & 0x01))
        {
            debug_line("short to GND coil B");
        }
        if(0 != (spi_receive_buffer[i + 2] & 0x80))
        {
            debug_line("short to GND coil A");
        }
        if(0 != (spi_receive_buffer[i + 2] & 0x40))
        {
            debug_line("Warning: over Temperature !");
        }
        if(0 != (spi_receive_buffer[i + 2] & 0x20))
        {
            debug_line("Shutdown due to over Temperature !");
        }
        if(0 != (spi_receive_buffer[i + 2] & 0x10))
        {
            debug_line("Stall detected !");
        }
        // second stepper in this 5 byte packet
        if(step > num)
        {
            return;
        }
        debug_line("Stepper %d:", step);
        step++;

        sg = ((spi_receive_buffer[i +2] & 0x0f)<<6) + ((spi_receive_buffer[i + 3] & 0x80)>>2);
        se = ((spi_receive_buffer[i +3] & 0x7c)>>2);
        debug_line("SG: %4d", sg);
        debug_line("SE: %4d", se);
        if(0 != (spi_receive_buffer[i + 4] & 0x80))
        {
            debug_line("standing still");
        }
        else
        {
            debug_line("moving");
        }
        if(0 != (spi_receive_buffer[i + 4] & 0x40))
        {
            debug_line("open Load coil B");
        }
        if(0 != (spi_receive_buffer[i + 4] & 0x20))
        {
            debug_line("open Load coil A");
        }
        if(0 != (spi_receive_buffer[i + 4] & 0x10))
        {
            debug_line("short to GND coil B");
        }
        if(0 != (spi_receive_buffer[i + 4] & 0x08))
        {
            debug_line("short to GND coil A");
        }
        if(0 != (spi_receive_buffer[i + 4] & 0x04))
        {
            debug_line("Warning: over Temperature !");
        }
        if(0 != (spi_receive_buffer[i + 4] & 0x02))
        {
            debug_line("Shutdown due to over Temperature !");
        }
        if(0 != (spi_receive_buffer[i + 4] & 0x01))
        {
            debug_line("Stall detected !");
        }

        // done with these two steppers
        i = i+5;
    }
}

#else

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

/*
 * P R I V A T E   F U N C T I O N S
 */


// SPI:
// ----
// - Most significant Bit First
// - buffer[0], buffer[1], buffer[2]
// - first bit send is bit 19, followed by bit 18
// -> 1. Byte contains bits 19 .. 12
//    2. Byte contains bits 11 ..  4
//    3. Byte contains bits 3, 2, 1, 0 and bit 19 .. 16 of next stepper
//    4. Byte contains next stepper bits       15 ..  8
//    5. Byte contains next Stepper bits        7 ..  0

static void setBit(enum cfgRegisters reg, int bit)
{
    int byte;
    int bitShift;
    byte = 19 - (bit/8);
    bitShift = bit%8;
    cfg_data[reg][byte] |= 1<<bitShift;
}

static void resetBit(enum cfgRegisters reg, int bit)
{
    int byte;
    int bitShift;
    byte = 19 - (bit/8);
    bitShift = bit%8;
    cfg_data[reg][byte] &= ~(1<<bitShift);
}

static void writeInt(int value,enum cfgRegisters reg, int bit, int bits)
{
    int bitmask;
    int byte;
    int bitShift;
    byte = 19 - (bit/8);
    bitShift = bit%8;
    switch(bits)
    {
    case 0: return;
    case 1: bitmask = 0x01; break;
    case 2: bitmask = 0x03; break;
    case 3: bitmask = 0x07; break;
    case 4: bitmask = 0x0f; break;
    case 5: bitmask = 0x1f; break;
    case 6: bitmask = 0x3f; break;
    case 7: bitmask = 0x7f; break;
    case 8: bitmask = 0xff; break;
    default: return;
    }
    // write all used bits to 0
    cfg_data[reg][byte] &= ~(bitmask<<bitShift);
    // set the bits needed
    cfg_data[reg][byte] |= value<<bitShift;
    // check if data fit into byte
    if(bitShift + bits > 8)
    {
        // next byte is also effected
        int shift = 8 -bitShift;
        int additional_bits = (bitShift + bits) -8;
        switch(additional_bits)
        {
        case 0: return;
        case 1: bitmask = 0x01; break;
        case 2: bitmask = 0x03; break;
        case 3: bitmask = 0x07; break;
        case 4: bitmask = 0x0f; break;
        case 5: bitmask = 0x1f; break;
        case 6: bitmask = 0x3f; break;
        case 7: bitmask = 0x7f; break;
        case 8: bitmask = 0xff; break;
        default: return;
        }
        // write all used bits to 0
        cfg_data[reg][byte-1] &= ~(bitmask);
        // set the bits needed
        cfg_data[reg][byte-1] |= bitmask & (value>>shift);
    }
}

static void setInt(int value, enum cfgSetting setting, int stepper)
{
    enum cfgRegisters reg; // which register
    int bitPosition;       // Which bit in the addressed Register
    int numBits;           // the number of bits the int consumes
    switch(setting)
    {
    // DRVCTRL
    case microstepResolution:
        reg = DRVCTRL;
        bitPosition = 0;
        numBits = 4;
        break;

    // CHOPCONF
    case blankingTime:
        reg = CHOPCONF;
        bitPosition = 15;
        numBits = 2;
        break;

    case hysteresisDecrementTime:
        reg = CHOPCONF;
        bitPosition = 11;
        numBits = 2;
        break;

    case hysteresisEnd:
        reg = CHOPCONF;
        bitPosition = 7;
        numBits = 4;
        break;

    case sineWaveOffset:
        reg = CHOPCONF;
        bitPosition = 7;
        numBits = 4;
        break;

    case hysteresisStartOffset:
        reg = CHOPCONF;
        bitPosition = 4;
        numBits = 3;
        break;

    case fastDecayTime:
        reg = CHOPCONF;
        bitPosition = 3;
        // handle 4th bit
        if(value > 7)
        {
            setBit(reg, bitPosition + 7 + ((MAX_NUM_STEPPERS -(stepper +1)) * 20));
        }
        else
        {
            resetBit(reg, bitPosition + 7 + ((MAX_NUM_STEPPERS -(stepper +1)) * 20));
        }
        value = value %8;
        numBits = 3;
        break;

    case toff:
        reg = CHOPCONF;
        bitPosition = 0;
        numBits = 4;
        break;

    // SMARTEN
    case decrementSpeed:
        reg = SMARTEN;
        bitPosition = 13;
        numBits = 3;
        break;

    case seUpper:
        reg = SMARTEN;
        bitPosition = 8;
        numBits = 4;
        break;

    case seUpStep:
        reg = SMARTEN;
        bitPosition = 5;
        numBits = 2;
        break;

    case seLower:
        reg = SMARTEN;
        bitPosition = 0;
        numBits = 4;
        break;

    // SGCSCONF
    case sgThreshold:
        reg = SGCSCONF;
        bitPosition = 8;
        numBits = 7;
        break;

    case sgCS:
        reg = SGCSCONF;
        bitPosition = 0;
        numBits = 5;
        break;

    // DRVCONF
    case slopeHigh:
        reg = DRVCONF;
        bitPosition = 14;
        numBits = 2;
        break;

    case slopeLow:
        reg = DRVCONF;
        bitPosition = 12;
        numBits = 2;
        break;

    case shortGNDtimer:
        reg = DRVCONF;
        bitPosition = 8;
        numBits = 2;
        break;

    case responseFormat:
        reg = DRVCONF;
        bitPosition = 4;
        numBits = 2;
        break;

    default:
        debug_line("Tried to set unknown bool setting !");
        break;
    }
    writeInt(value, reg, bitPosition + (stepper * 20), numBits);
}

static void setBool(bool value, enum cfgSetting setting, int stepper)
{
    enum cfgRegisters reg; // which register
    int bitPosition;       // Which bit in the addressed Register

    switch(setting)
    {
    // DRVCTRL
    case stepInterpolation:
        reg = DRVCTRL;
        bitPosition = 9;
        break;

    case doubleEdge:
        reg = DRVCTRL;
        bitPosition = 8;
        break;

    // CHOPCONF
    case chopperMode:
        reg = CHOPCONF;
        bitPosition = 14;
        break;

    case randomToff:
        reg = CHOPCONF;
        bitPosition = 13;
        break;

    case fastDecayMode:
        reg = CHOPCONF;
        bitPosition = 12;
        break;

    // SMARTEN
    case seIMin:
        reg = SMARTEN;
        bitPosition = 15;
        break;

    // SGCSCONF
    case sgFilter:
        reg = SGCSCONF;
        bitPosition = 16;
        break;

    // DRVCONF
    case test:
        reg = DRVCONF;
        bitPosition = 16;
        break;

    case shortGNDdisabled:
        reg = DRVCONF;
        bitPosition = 10;
        break;

    case disableSTEPDIR:
        reg = DRVCONF;
        bitPosition = 7;
        break;

    case lowVoltageRsense:
        reg = DRVCONF;
        bitPosition = 6;
        break;

    default:
        debug_line("Tried to set unknown bool setting !");
        return;
    }
    if(true == value)
    {
        setBit(  reg, bitPosition + (stepper * 20));
    }
    else
    {
        resetBit(reg, bitPosition + (stepper * 20));
    }
}

#ifdef USE_STEP_DIR

void periodic_status_check(void)
{
    hal_spi_start_spi_transaction(STEPPER_SPI, &cfg_data[SMARTEN][0], num_bytes_used, &spi_receive_buffer[0]);
    // TODO check reply
}

#else

// step is in the first 2 bytes and the 4 higher bits of the 3rd byte.
// The 4 low bits of the 3rd byte are always 0 !
static uint8_t* get_next_step(uint_fast8_t stepper_num, bool direction_is_increasing)
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

#endif
