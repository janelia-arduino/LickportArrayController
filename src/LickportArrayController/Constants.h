// ----------------------------------------------------------------------------
// Constants.h
//
//
// Authors:
// Peter Polidoro peter@polidoro.io
// ----------------------------------------------------------------------------
#ifndef LICKPORT_ARRAY_CONTROLLER_CONSTANTS_H
#define LICKPORT_ARRAY_CONTROLLER_CONSTANTS_H
#include <ConstantVariable.h>
#include <DigitalController.h>


namespace lickport_array_controller
{
namespace constants
{
//MAX values must be >= 1, >= created/copied count, < RAM limit
enum{PROPERTY_COUNT_MAX=1};
enum{PARAMETER_COUNT_MAX=4};
enum{FUNCTION_COUNT_MAX=4};
enum{CALLBACK_COUNT_MAX=1};

extern ConstantString device_name;

extern ConstantString firmware_name;
extern const modular_server::FirmwareInfo firmware_info;

extern ConstantString hardware_name;
extern const modular_server::HardwareInfo hardware_info;

extern const double dispense_power;
extern const long dispense_delay;
extern const long dispense_count;

extern const long lickport_count;

// Pins
extern ConstantString change_pin_name;
extern const size_t change_pin_number;

extern ConstantString lick_detected_pin_name;
extern const size_t lick_detected_pin_number;
extern const size_t lick_detected_pulse_duration;

extern ConstantString sync_pin_name;
extern const size_t sync_pin_number;

// Units

// Properties
// Property values must be long, double, bool, long[], double[], bool[], char[], ConstantString *, (ConstantString *)[]

// Parameters
extern ConstantString lickport_parameter_name;
extern const long lickport_min;

extern ConstantString lickports_parameter_name;
extern const long lickports_array_length_min;

extern ConstantString dispense_duration_parameter_name;
extern const long dispense_duration_min;
extern const long dispense_duration_max;

extern ConstantString dispense_durations_parameter_name;
extern const long dispense_durations_array_length_min;

// Functions
extern ConstantString dispense_lickport_for_duration_function_name;
extern ConstantString dispense_lickports_for_duration_function_name;
extern ConstantString dispense_lickports_for_durations_function_name;
extern ConstantString dispense_all_lickports_for_duration_function_name;

// Callbacks
extern ConstantString manage_lick_status_change_callback_name;

// Errors
}
}
#include "TEENSY40.h"
#include "TEENSY41.h"
#include "5x3.h"
#include "3x2.h"
#endif
