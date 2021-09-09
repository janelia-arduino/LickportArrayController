// ----------------------------------------------------------------------------
// TEENSY40.cpp
//
//
// Authors:
// Peter Polidoro peterpolidoro@gmail.com
// ----------------------------------------------------------------------------
#include "TEENSY40.h"


#if defined(__IMXRT1062__) && defined(ARDUINO_TEENSY40)
namespace lickport_array_controller
{
namespace constants
{
const modular_server::HardwareInfo hardware_info =
{
  .name_ptr=&hardware_name,
  .part_number=2025,
  .version_major=1,
  .version_minor=1,
};

const long channel_count = CHANNEL_COUNT;

const size_t channel_pins[CHANNEL_COUNT] =
{
  0,
  1,
  2,
  3,
  4,
  5,
  6,
  7,
  8,
  9,
  10,
  11
};

// Pins
const size_t change_pin_number = 12;

const size_t lick_detected_pin_number = 13;

const size_t sync_pin_number = 14;

// Units

// Properties

// Parameters

// Functions

// Callbacks

// Errors
}
}

#endif
