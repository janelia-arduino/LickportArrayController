// ----------------------------------------------------------------------------
// Constants.h
//
//
// Authors:
// Peter Polidoro peterpolidoro@gmail.com
// ----------------------------------------------------------------------------
#ifndef LICKPORT_ARRAY_CONTROLLER_CONSTANTS_H
#define LICKPORT_ARRAY_CONTROLLER_CONSTANTS_H
#include <ConstantVariable.h>
#include <ModularServer.h>


namespace lickport_array_controller
{
namespace constants
{
//MAX values must be >= 1, >= created/copied count, < RAM limit
enum{PROPERTY_COUNT_MAX=1};
enum{PARAMETER_COUNT_MAX=1};
enum{FUNCTION_COUNT_MAX=1};
enum{CALLBACK_COUNT_MAX=1};

extern ConstantString device_name;

extern ConstantString firmware_name;
extern const modular_server::FirmwareInfo firmware_info;

extern ConstantString hardware_name;
extern const modular_server::HardwareInfo hardware_info;

enum{EVENT_COUNT_MAX=32};

extern const uint32_t end_output_delay;

// Pins
extern ConstantString input_pin_name;
extern const size_t input_pin_number;

extern ConstantString output_pin_name;
extern const size_t output_pin_number;

// Units

// Properties
// Property values must be long, double, bool, long[], double[], bool[], char[], ConstantString *, (ConstantString *)[]

// Parameters

// Functions

// Callbacks

// Errors
}
}
#include "TEENSY40.h"
#include "TEENSY41.h"
#include "5x3.h"
#include "3x2.h"
#endif
