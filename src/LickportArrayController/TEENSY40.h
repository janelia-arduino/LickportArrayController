// ----------------------------------------------------------------------------
// TEENSY40.h
//
//
// Authors:
// Peter Polidoro peterpolidoro@gmail.com
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
enum{PIN_COUNT_MAX=1};

// Pins

// Units

// Properties
// Property values must be long, double, bool, long[], double[], bool[], char[], ConstantString *, (ConstantString *)[]

// Parameters

// Functions

// Callbacks

// Errors
}
}

#endif
#endif
