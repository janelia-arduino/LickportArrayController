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

// Pins
CONSTANT_STRING(change_pin_name,"change");

CONSTANT_STRING(lick_detected_pin_name,"lick_detected");
const size_t lick_detected_pulse_duration = 200;

CONSTANT_STRING(sync_pin_name,"sync");

// Units

// Properties

// Parameters

// Functions

// Callbacks
CONSTANT_STRING(check_lick_status_callback_name,"checkLickStatus");

// Errors
}
}
