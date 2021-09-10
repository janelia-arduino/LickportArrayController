// ----------------------------------------------------------------------------
// Constants.cpp
//
//
// Authors:
// Peter Polidoro peterpolidoro@gmail.com
// ----------------------------------------------------------------------------
#include "Constants.h"


namespace lickport_array_controller
{
namespace constants
{
CONSTANT_STRING(device_name,"lickport_array_controller");

CONSTANT_STRING(firmware_name,"LickportArrayController");
// Use semantic versioning http://semver.org/
const modular_server::FirmwareInfo firmware_info =
{
  .name_ptr=&firmware_name,
  .version_major=0,
  .version_minor=1,
  .version_patch=0,
};

CONSTANT_STRING(hardware_name,"lickport_array_controller");

const double dispense_power = digital_controller::constants::power_max;
const long dispense_delay = 10;
const long dispense_count = 1;

// Pins
CONSTANT_STRING(change_pin_name,"change");

CONSTANT_STRING(lick_detected_pin_name,"lick_detected");
const size_t lick_detected_pulse_duration = 200;

CONSTANT_STRING(sync_pin_name,"sync");

// Units

// Properties

// Parameters
CONSTANT_STRING(dispense_duration_parameter_name,"dispense_duration");
const long dispense_duration_min = 1;
const long dispense_duration_max = 2000000000;

// Functions
CONSTANT_STRING(dispense_all_for_duration_function_name,"dispenseAllForDuration");

// Callbacks
CONSTANT_STRING(check_lick_status_callback_name,"checkLickStatus");

// Errors
}
}
