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
enum{PROPERTY_COUNT_MAX=7};
enum{PARAMETER_COUNT_MAX=4};
enum{FUNCTION_COUNT_MAX=12};
enum{CALLBACK_COUNT_MAX=4};

extern ConstantString device_name;

extern ConstantString firmware_name;
extern const modular_server::FirmwareInfo firmware_info;

extern ConstantString hardware_name;
extern const modular_server::HardwareInfo hardware_info;

extern const double dispense_power;

extern const long lickport_count;
extern const long channel_count;

extern const size_t sync_channel;
extern const size_t sync_delay;
extern const size_t sync_count;

enum{LICK_DATUM_COUNT_MAX=100};

extern ConstantString lick_datum_time;
extern ConstantString lick_datum_millis;
extern ConstantString lick_datum_lickports_licked;
extern ConstantString lick_datum_lickports_activated;

extern const long duration_min;
extern const long duration_max;

// Pins
extern ConstantString change_pin_name;
extern const size_t change_pin_number;

extern ConstantString lick_detected_pin_name;
extern const size_t lick_detected_pin_number;
extern const size_t lick_detected_pulse_duration;

// Units

// Properties
// Property values must be long, double, bool, long[], double[], bool[], char[], ConstantString *, (ConstantString *)[]
extern ConstantString sync_period_min_property_name;
extern const long sync_period_min_default;

extern ConstantString sync_period_max_property_name;
extern const long sync_period_max_default;

extern ConstantString sync_on_duration_property_name;
extern const long sync_on_duration_default;

extern ConstantString dispense_delays_property_name;
extern const long dispense_delay_min;
extern const long dispense_delay_max;
extern const long dispense_delay_default;

extern ConstantString dispense_periods_property_name;
extern const long dispense_period_min;
extern const long dispense_period_max;
extern const long dispense_period_default;

extern ConstantString dispense_counts_property_name;
extern const long dispense_count_min;
extern const long dispense_count_max;
extern const long dispense_count_default;

extern ConstantString activated_dispense_durations_property_name;
extern const long activated_dispense_duration_default;

// Parameters
extern ConstantString lickport_parameter_name;
extern const long lickport_min;

extern ConstantString lickports_parameter_name;
extern const long lickports_array_length_min;

extern ConstantString dispense_duration_parameter_name;

extern ConstantString dispense_durations_parameter_name;
extern const long dispense_durations_array_length_min;

// Functions
extern ConstantString dispense_lickport_for_duration_function_name;
extern ConstantString dispense_lickports_for_duration_function_name;
extern ConstantString dispense_lickports_for_durations_function_name;
extern ConstantString dispense_all_lickports_for_duration_function_name;
extern ConstantString get_activated_lickports_function_name;
extern ConstantString activate_only_lickport_function_name;
extern ConstantString activate_only_lickports_function_name;
extern ConstantString activate_lickport_function_name;
extern ConstantString activate_lickports_function_name;
extern ConstantString deactivate_lickport_function_name;
extern ConstantString deactivate_lickports_function_name;
extern ConstantString get_and_clear_lick_data_function_name;

// Callbacks
extern ConstantString calibrate_lick_sensor_callback_name;
extern ConstantString manage_lick_status_change_callback_name;
extern ConstantString activate_all_lickports_callback_name;
extern ConstantString deactivate_all_lickports_callback_name;

// Errors
}
}
#include "TEENSY40.h"
#include "TEENSY41.h"
#include "5x3.h"
#include "3x2.h"
#endif
