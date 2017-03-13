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
#include "log.h"
}

TEST_GROUP(SrcLibLogTestGroup)
{

};

TEST(SrcLibLogTestGroup, log)
{
    // (Expected, actual, tollerance)
    DOUBLES_EQUAL(0, logf(1), 0.001)
    DOUBLES_EQUAL(3, logf(20.0855), 0.001)
    DOUBLES_EQUAL(10, logf(22026.46579), 0.001)
    DOUBLES_EQUAL(-3.2995437279, logf(0.0369), 0.001)
}


