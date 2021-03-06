// ----------------------------------------------------------------------------
// Constants.cpp
//
//
// Authors:
// Peter Polidoro peter@polidoro.io
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
  .version_major=1,
  .version_minor=0,
  .version_patch=0,
};

CONSTANT_STRING(hardware_name,"lickport_array_controller");

const double dispense_power = digital_controller::constants::power_max;

const size_t sync_delay = 10;
const size_t sync_count = 1;

CONSTANT_STRING(saved_datum_time,"time");
CONSTANT_STRING(saved_datum_millis,"millis");
CONSTANT_STRING(saved_datum_lickports_licked,"lickports_licked");
CONSTANT_STRING(saved_datum_lickports_activated,"lickports_activated");

const long duration_min = 10;
const long duration_max = 2000000000;

// Pins
CONSTANT_STRING(change_pin_name,"change");

CONSTANT_STRING(lick_detected_pin_name,"lick_detected");
const size_t lick_detected_pulse_duration = 200;

// Units

// Properties
CONSTANT_STRING(sync_period_min_property_name,"syncPeriodMin");
const long sync_period_min_default = 9000;

CONSTANT_STRING(sync_period_max_property_name,"syncPeriodMax");
const long sync_period_max_default = 11000;

CONSTANT_STRING(sync_on_duration_property_name,"syncOnDuration");
const long sync_on_duration_default = 50;

CONSTANT_STRING(dispense_delays_property_name,"dispenseDelays");
const long dispense_delay_min = 0;
const long dispense_delay_max = 1000;
const long dispense_delay_default = 10;

CONSTANT_STRING(dispense_periods_property_name,"dispensePeriods");
const long dispense_period_min = 1;
const long dispense_period_max = 10000;
const long dispense_period_default = 100;

CONSTANT_STRING(dispense_counts_property_name,"dispenseCounts");
const long dispense_count_min = 1;
const long dispense_count_max = 10;
const long dispense_count_default = 1;

CONSTANT_STRING(activated_dispense_durations_property_name,"activatedDispenseDurations");
const long activated_dispense_duration_default = 500;

CONSTANT_STRING(all_lickports_activated_initially_property_name,"allLickportsActivatedInitially");
const bool all_lickports_activated_initially_default = false;

// Parameters
CONSTANT_STRING(lickport_parameter_name,"lickport");
const long lickport_min = 0;

CONSTANT_STRING(lickports_parameter_name,"lickports");
const long lickports_array_length_min = 0;

CONSTANT_STRING(dispense_duration_parameter_name,"dispense_duration");

CONSTANT_STRING(dispense_durations_parameter_name,"dispense_durations");
const long dispense_durations_array_length_min = 0;

// Functions
CONSTANT_STRING(get_lickport_count_function_name,"getLickportCount");
CONSTANT_STRING(dispense_lickport_for_duration_function_name,"dispenseLickportForDuration");
CONSTANT_STRING(dispense_lickports_for_duration_function_name,"dispenseLickportsForDuration");
CONSTANT_STRING(dispense_lickports_for_durations_function_name,"dispenseLickportsForDurations");
CONSTANT_STRING(dispense_all_lickports_for_duration_function_name,"dispenseAllLickportsForDuration");
CONSTANT_STRING(get_activated_lickports_function_name,"getActivatedLickports");
CONSTANT_STRING(activate_only_lickport_function_name,"activateOnlyLickport");
CONSTANT_STRING(activate_only_lickports_function_name,"activateOnlyLickports");
CONSTANT_STRING(activate_lickport_function_name,"activateLickport");
CONSTANT_STRING(activate_lickports_function_name,"activateLickports");
CONSTANT_STRING(deactivate_lickport_function_name,"deactivateLickport");
CONSTANT_STRING(deactivate_lickports_function_name,"deactivateLickports");
CONSTANT_STRING(get_and_clear_saved_data_function_name,"getAndClearSavedData");

// Callbacks
CONSTANT_STRING(calibrate_lick_sensor_callback_name,"calibrateLickSensor");
CONSTANT_STRING(manage_lick_status_change_callback_name,"manageLickStatusChange");
CONSTANT_STRING(activate_all_lickports_callback_name,"activateAllLickports");
CONSTANT_STRING(deactivate_all_lickports_callback_name,"deactivateAllLickports");

// Errors
}
}
