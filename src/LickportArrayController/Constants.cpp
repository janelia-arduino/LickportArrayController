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

const uint32_t end_output_delay = 100;

// Pins
CONSTANT_STRING(input_pin_name,"input");

CONSTANT_STRING(output_pin_name,"output");

// Units

// Properties

// Parameters

// Functions

// Callbacks
CONSTANT_STRING(trigger_output_callback_name,"triggerOutput");

// Errors
}
}
