
#include "end_stop_handling.h"

void endStopHandling_init(void)
{

}

bool dev_input_enable(uint_fast8_t switch_number, uint_fast8_t enable)
{
    return false;
}
bool dev_input_is_enabled(uint_fast8_t switch_number)
{
    return false;
}

bool dev_stepper_configure_end_stops(uint_fast8_t StepperNumber, uint_fast8_t switch_number, uint_fast8_t min_Max)
{
    return false;
}

uint_fast8_t dev_input_get_switch_state(uint_fast8_t number)
{
    return 1;
}

bool dev_stepper_is_end_stop_triggered(uint_fast8_t stepper, uint_fast8_t min_max)
{
    return false;
}
