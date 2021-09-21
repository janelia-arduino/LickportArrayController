// ----------------------------------------------------------------------------
// TEENSY40.cpp
//
//
// Authors:
// Peter Polidoro peter@polidoro.io
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

const long lickport_count = LICKPORT_COUNT;

const size_t lickport_pins[LICKPORT_COUNT] =
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
const long dispense_delays_default[LICKPORT_COUNT] =
{
  dispense_delay_default,
  dispense_delay_default,
  dispense_delay_default,
  dispense_delay_default,
  dispense_delay_default,
  dispense_delay_default,
  dispense_delay_default,
  dispense_delay_default,
  dispense_delay_default,
  dispense_delay_default,
  dispense_delay_default,
  dispense_delay_default,
};

const long dispense_periods_default[LICKPORT_COUNT] =
{
  dispense_period_default,
  dispense_period_default,
  dispense_period_default,
  dispense_period_default,
  dispense_period_default,
  dispense_period_default,
  dispense_period_default,
  dispense_period_default,
  dispense_period_default,
  dispense_period_default,
  dispense_period_default,
  dispense_period_default,
};

const long dispense_counts_default[LICKPORT_COUNT] =
{
  dispense_count_default,
  dispense_count_default,
  dispense_count_default,
  dispense_count_default,
  dispense_count_default,
  dispense_count_default,
  dispense_count_default,
  dispense_count_default,
  dispense_count_default,
  dispense_count_default,
  dispense_count_default,
  dispense_count_default,
};

const long activated_dispense_durations_default[LICKPORT_COUNT] =
{
  activated_dispense_duration_default,
  activated_dispense_duration_default,
  activated_dispense_duration_default,
  activated_dispense_duration_default,
  activated_dispense_duration_default,
  activated_dispense_duration_default,
  activated_dispense_duration_default,
  activated_dispense_duration_default,
  activated_dispense_duration_default,
  activated_dispense_duration_default,
  activated_dispense_duration_default,
  activated_dispense_duration_default,
};

// Parameters

// Functions

// Callbacks

// Errors
}
}

#endif
