// ----------------------------------------------------------------------------
// TEENSY40.h
//
//
// Authors:
// Peter Polidoro peter@polidoro.io
// ----------------------------------------------------------------------------
#ifndef LICKPORT_ARRAY_CONTROLLER_TEENSY40_CONSTANTS_H
#define LICKPORT_ARRAY_CONTROLLER_TEENSY40_CONSTANTS_H
#include "Constants.h"


#if defined(__IMXRT1062__) && defined(ARDUINO_TEENSY40)
namespace lickport_array_controller
{
namespace constants
{
//MAX values must be >= 1, >= created/copied count, < RAM limit
enum {PIN_COUNT_MAX=3};

enum {LICKPORT_COUNT=12};

extern const size_t lickport_pins[LICKPORT_COUNT];

// Pins

// Units

// Properties
// Property values must be long, double, bool, long[], double[], bool[], char[], ConstantString *, (ConstantString *)[]
extern const long dispense_delays_default[LICKPORT_COUNT];

extern const long dispense_periods_default[LICKPORT_COUNT];

extern const long dispense_counts_default[LICKPORT_COUNT];

extern const long activated_dispense_durations_default[LICKPORT_COUNT];

// Parameters

// Functions

// Callbacks

// Errors
}
}

#endif
#endif
