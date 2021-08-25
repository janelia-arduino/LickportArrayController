// ----------------------------------------------------------------------------
// LickportArrayController.cpp
//
//
// Authors:
// Peter Polidoro peterpolidoro@gmail.com
// ----------------------------------------------------------------------------
#include "../LickportArrayController.h"


using namespace lickport_array_controller;

LickportArrayController::LickportArrayController()
{
}

void LickportArrayController::setup()
{
  // Parent Setup
  DigitalController::setup();

  // Reset Watchdog
  resetWatchdog();

  // Variable Setup

  // Set Device ID
  modular_server_.setDeviceName(constants::device_name);

  // Pin Setup
  pinMode(constants::enable_all_pin,OUTPUT);
  analogWriteResolution(constants::analog_write_resolution);
  for (size_t channel=0; channel<constants::CHANNEL_COUNT; ++channel)
  {
    if (constants::analog_writeable[channel])
    {
      size_t channel_pin = constants::channel_pins[channel];
      // Pins 3 and 4 interfere with TimerOne
      // Crude hack until pcb can be rerouted!
      if ((channel_pin != 3) && (channel_pin != 4))
      {
        analogWriteFrequency(channel_pin,constants::analog_write_frequency);
      }
    }
  }

  for (size_t channel=0; channel<constants::CHANNEL_COUNT; ++channel)
  {
    pinMode(constants::channel_pins[channel],OUTPUT);
  }

  // Add Hardware
  modular_server_.addHardware(constants::hardware_info,
    pins_);

  // Pins
  modular_server::Pin & btn_enable_pin = modular_server_.createPin(constants::btn_enable_pin_name,
    constants::btn_enable_pin_number);

  modular_server::Pin & btn_test_pin = modular_server_.createPin(constants::btn_test_pin_name,
    constants::btn_test_pin_number);

  // Add Firmware
  modular_server_.addFirmware(constants::firmware_info,
    properties_,
    parameters_,
    functions_,
    callbacks_);

  // Properties
  modular_server::Property & channel_count_property = modular_server_.property(digital_controller::constants::channel_count_property_name);
  channel_count_property.disableFunctors();
  channel_count_property.setDefaultValue(constants::channel_count);
  channel_count_property.setRange(constants::channel_count,constants::channel_count);
  channel_count_property.reenableFunctors();

  modular_server::Property & ir_backlight_power_to_intensity_ratio_property = modular_server_.createProperty(constants::ir_backlight_power_to_intensity_ratio_property_name,constants::ir_backlight_power_to_intensity_ratio_default);
  ir_backlight_power_to_intensity_ratio_property.setRange(constants::power_to_intensity_ratio_min,constants::power_to_intensity_ratio_max);
  ir_backlight_power_to_intensity_ratio_property.attachPostSetElementValueFunctor(makeFunctor((Functor1<size_t> *)0,*this,&LickportArrayController::setIrBacklightPowerToIntensityRatioHandler));

  modular_server::Property & ir_backlight_intensity_max_property = modular_server_.createProperty(constants::ir_backlight_intensity_max_property_name,constants::ir_backlight_intensity_max_default);
  ir_backlight_intensity_max_property.setRange(constants::intensity_min,constants::intensity_max);
  ir_backlight_intensity_max_property.setUnits(constants::intensity_units);
  ir_backlight_intensity_max_property.attachPostSetElementValueFunctor(makeFunctor((Functor1<size_t> *)0,*this,&LickportArrayController::setIrBacklightIntensityMaxHandler));

  modular_server::Property & visible_backlight_power_to_intensity_ratio_property = modular_server_.createProperty(constants::visible_backlight_power_to_intensity_ratio_property_name,constants::visible_backlight_power_to_intensity_ratio_default);
  visible_backlight_power_to_intensity_ratio_property.setRange(constants::power_to_intensity_ratio_min,constants::power_to_intensity_ratio_max);
  visible_backlight_power_to_intensity_ratio_property.attachPostSetElementValueFunctor(makeFunctor((Functor1<size_t> *)0,*this,&LickportArrayController::setVisibleBacklightPowerToIntensityRatioHandler));

  modular_server::Property & visible_backlight_intensity_max_property = modular_server_.createProperty(constants::visible_backlight_intensity_max_property_name,constants::visible_backlight_intensity_max_default);
  visible_backlight_intensity_max_property.setRange(constants::intensity_min,constants::intensity_max);
  visible_backlight_intensity_max_property.setUnits(constants::intensity_units);
  visible_backlight_intensity_max_property.attachPostSetElementValueFunctor(makeFunctor((Functor1<size_t> *)0,*this,&LickportArrayController::setVisibleBacklightIntensityMaxHandler));

  modular_server::Property & high_voltage_power_max_property = modular_server_.createProperty(constants::high_voltage_power_max_property_name,constants::high_voltage_power_max_default);
  high_voltage_power_max_property.setRange(digital_controller::constants::power_min,digital_controller::constants::power_max);
  high_voltage_power_max_property.setUnits(digital_controller::constants::percent_units);
  high_voltage_power_max_property.attachPostSetElementValueFunctor(makeFunctor((Functor1<size_t> *)0,*this,&LickportArrayController::setHighVoltagePowerMaxHandler));

  modular_server::Property & low_voltage_power_max_property = modular_server_.createProperty(constants::low_voltage_power_max_property_name,constants::low_voltage_power_max_default);
  low_voltage_power_max_property.setRange(digital_controller::constants::power_max,digital_controller::constants::power_max);
  low_voltage_power_max_property.setUnits(digital_controller::constants::percent_units);
  low_voltage_power_max_property.attachPostSetElementValueFunctor(makeFunctor((Functor1<size_t> *)0,*this,&LickportArrayController::setLowVoltagePowerMaxHandler));

  modular_server::Property & ir_backlight_switching_frequency_max_property = modular_server_.createProperty(constants::ir_backlight_switching_frequency_max_property_name,constants::ir_backlight_switching_frequency_max_default);
  ir_backlight_switching_frequency_max_property.setRange(constants::ir_backlight_switching_frequency_max_element_min,constants::ir_backlight_switching_frequency_max_element_max);
  ir_backlight_switching_frequency_max_property.setUnits(constants::hz_units);
  ir_backlight_switching_frequency_max_property.attachPostSetElementValueFunctor(makeFunctor((Functor1<size_t> *)0,*this,&LickportArrayController::setIrBacklightSwitchingFrequencyMaxHandler));

  modular_server::Property & visible_backlight_switching_frequency_max_property = modular_server_.createProperty(constants::visible_backlight_switching_frequency_max_property_name,constants::visible_backlight_switching_frequency_max_default);
  visible_backlight_switching_frequency_max_property.setRange(constants::visible_backlight_switching_frequency_max_element_min,constants::visible_backlight_switching_frequency_max_element_max);
  visible_backlight_switching_frequency_max_property.setUnits(constants::hz_units);
  visible_backlight_switching_frequency_max_property.attachPostSetElementValueFunctor(makeFunctor((Functor1<size_t> *)0,*this,&LickportArrayController::setVisibleBacklightSwitchingFrequencyMaxHandler));

  modular_server::Property & high_voltage_switching_frequency_max_property = modular_server_.createProperty(constants::high_voltage_switching_frequency_max_property_name,constants::high_voltage_switching_frequency_max_default);
  high_voltage_switching_frequency_max_property.setRange(constants::high_voltage_switching_frequency_max_element_min,constants::high_voltage_switching_frequency_max_element_max);
  high_voltage_switching_frequency_max_property.setUnits(constants::hz_units);
  high_voltage_switching_frequency_max_property.attachPostSetElementValueFunctor(makeFunctor((Functor1<size_t> *)0,*this,&LickportArrayController::setHighVoltageSwitchingFrequencyMaxHandler));

  modular_server::Property & low_voltage_switching_frequency_max_property = modular_server_.createProperty(constants::low_voltage_switching_frequency_max_property_name,constants::low_voltage_switching_frequency_max_default);
  low_voltage_switching_frequency_max_property.setRange(constants::low_voltage_switching_frequency_max_element_max,constants::low_voltage_switching_frequency_max_element_max);
  low_voltage_switching_frequency_max_property.setUnits(constants::hz_units);
  low_voltage_switching_frequency_max_property.attachPostSetElementValueFunctor(makeFunctor((Functor1<size_t> *)0,*this,&LickportArrayController::setLowVoltageSwitchingFrequencyMaxHandler));

  updateAllSwitchingFrequencies();

  // Parameters
  modular_server::Parameter & power_parameter = modular_server_.parameter(digital_controller::constants::power_parameter_name);

  modular_server::Parameter & intensity_parameter = modular_server_.createParameter(constants::intensity_parameter_name);
  intensity_parameter.setRange(constants::intensity_min,constants::intensity_max);

  modular_server::Parameter & ir_backlight_parameter = modular_server_.createParameter(constants::ir_backlight_parameter_name);
  ir_backlight_parameter.setRange(digital_controller::constants::channel_min,(long)(getIrBacklightCount() - 1));

  modular_server::Parameter & visible_backlight_parameter = modular_server_.createParameter(constants::visible_backlight_parameter_name);
  visible_backlight_parameter.setRange(digital_controller::constants::channel_min,(long)(getVisibleBacklightCount() - 1));

  modular_server::Parameter & high_voltage_parameter = modular_server_.createParameter(constants::high_voltage_parameter_name);
  high_voltage_parameter.setRange(digital_controller::constants::channel_min,(long)(getHighVoltageCount() - 1));

  modular_server::Parameter & low_voltage_parameter = modular_server_.createParameter(constants::low_voltage_parameter_name);
  low_voltage_parameter.setRange(digital_controller::constants::channel_min,(long)(getLowVoltageCount() - 1));

  setChannelCountHandler();

  // Functions
  modular_server::Function & set_all_ir_backlights_on_at_power_function = modular_server_.createFunction(lickport_array_controller::constants::set_all_ir_backlights_on_at_power_function_name);
  set_all_ir_backlights_on_at_power_function.attachFunctor(makeFunctor((Functor0 *)0,*this,&LickportArrayController::setAllIrBacklightsOnAtPowerHandler));
  set_all_ir_backlights_on_at_power_function.addParameter(power_parameter);

  modular_server::Function & set_all_ir_backlights_on_at_intensity_function = modular_server_.createFunction(lickport_array_controller::constants::set_all_ir_backlights_on_at_intensity_function_name);
  set_all_ir_backlights_on_at_intensity_function.attachFunctor(makeFunctor((Functor0 *)0,*this,&LickportArrayController::setAllIrBacklightsOnAtIntensityHandler));
  set_all_ir_backlights_on_at_intensity_function.addParameter(intensity_parameter);

  modular_server::Function & set_ir_backlight_on_function = modular_server_.createFunction(lickport_array_controller::constants::set_ir_backlight_on_function_name);
  set_ir_backlight_on_function.attachFunctor(makeFunctor((Functor0 *)0,*this,&LickportArrayController::setIrBacklightOnHandler));
  set_ir_backlight_on_function.addParameter(ir_backlight_parameter);

  modular_server::Function & set_ir_backlight_on_at_power_function = modular_server_.createFunction(lickport_array_controller::constants::set_ir_backlight_on_at_power_function_name);
  set_ir_backlight_on_at_power_function.attachFunctor(makeFunctor((Functor0 *)0,*this,&LickportArrayController::setIrBacklightOnAtPowerHandler));
  set_ir_backlight_on_at_power_function.addParameter(ir_backlight_parameter);
  set_ir_backlight_on_at_power_function.addParameter(power_parameter);

  modular_server::Function & set_ir_backlight_on_at_intensity_function = modular_server_.createFunction(lickport_array_controller::constants::set_ir_backlight_on_at_intensity_function_name);
  set_ir_backlight_on_at_intensity_function.attachFunctor(makeFunctor((Functor0 *)0,*this,&LickportArrayController::setIrBacklightOnAtIntensityHandler));
  set_ir_backlight_on_at_intensity_function.addParameter(ir_backlight_parameter);
  set_ir_backlight_on_at_intensity_function.addParameter(intensity_parameter);

  modular_server::Function & set_ir_backlight_off_function = modular_server_.createFunction(lickport_array_controller::constants::set_ir_backlight_off_function_name);
  set_ir_backlight_off_function.attachFunctor(makeFunctor((Functor0 *)0,*this,&LickportArrayController::setIrBacklightOffHandler));
  set_ir_backlight_off_function.addParameter(ir_backlight_parameter);

  modular_server::Function & toggle_ir_backlight_function = modular_server_.createFunction(lickport_array_controller::constants::toggle_ir_backlight_function_name);
  toggle_ir_backlight_function.attachFunctor(makeFunctor((Functor0 *)0,*this,&LickportArrayController::toggleIrBacklightHandler));
  toggle_ir_backlight_function.addParameter(ir_backlight_parameter);

  modular_server::Function & get_ir_backlight_powers_when_on_function = modular_server_.createFunction(constants::get_ir_backlight_powers_when_on_function_name);
  get_ir_backlight_powers_when_on_function.attachFunctor(makeFunctor((Functor0 *)0,*this,&LickportArrayController::getIrBacklightPowersWhenOnHandler));
  get_ir_backlight_powers_when_on_function.setResultTypeArray();
  get_ir_backlight_powers_when_on_function.setResultTypeDouble();

  modular_server::Function & get_ir_backlight_intensities_when_on_function = modular_server_.createFunction(constants::get_ir_backlight_intensities_when_on_function_name);
  get_ir_backlight_intensities_when_on_function.attachFunctor(makeFunctor((Functor0 *)0,*this,&LickportArrayController::getIrBacklightIntensitiesWhenOnHandler));
  get_ir_backlight_intensities_when_on_function.setResultTypeArray();
  get_ir_backlight_intensities_when_on_function.setResultTypeDouble();

  modular_server::Function & get_ir_backlight_powers_function = modular_server_.createFunction(constants::get_ir_backlight_powers_function_name);
  get_ir_backlight_powers_function.attachFunctor(makeFunctor((Functor0 *)0,*this,&LickportArrayController::getIrBacklightPowersHandler));
  get_ir_backlight_powers_function.setResultTypeArray();
  get_ir_backlight_powers_function.setResultTypeDouble();

  modular_server::Function & get_ir_backlight_intensities_function = modular_server_.createFunction(constants::get_ir_backlight_intensities_function_name);
  get_ir_backlight_intensities_function.attachFunctor(makeFunctor((Functor0 *)0,*this,&LickportArrayController::getIrBacklightIntensitiesHandler));
  get_ir_backlight_intensities_function.setResultTypeArray();
  get_ir_backlight_intensities_function.setResultTypeDouble();

  modular_server::Function & get_ir_backlight_power_bounds_function = modular_server_.createFunction(constants::get_ir_backlight_power_bounds_function_name);
  get_ir_backlight_power_bounds_function.attachFunctor(makeFunctor((Functor0 *)0,*this,&LickportArrayController::getIrBacklightPowerBoundsHandler));
  get_ir_backlight_power_bounds_function.setResultTypeArray();
  get_ir_backlight_power_bounds_function.setResultTypeObject();

  modular_server::Function & get_ir_backlight_intensity_bounds_function = modular_server_.createFunction(constants::get_ir_backlight_intensity_bounds_function_name);
  get_ir_backlight_intensity_bounds_function.attachFunctor(makeFunctor((Functor0 *)0,*this,&LickportArrayController::getIrBacklightIntensityBoundsHandler));
  get_ir_backlight_intensity_bounds_function.setResultTypeArray();
  get_ir_backlight_intensity_bounds_function.setResultTypeObject();

  modular_server::Function & ir_backlight_power_to_intensities_function = modular_server_.createFunction(constants::ir_backlight_power_to_intensities_function_name);
  ir_backlight_power_to_intensities_function.attachFunctor(makeFunctor((Functor0 *)0,*this,&LickportArrayController::irBacklightPowerToIntensitiesHandler));
  ir_backlight_power_to_intensities_function.addParameter(power_parameter);
  ir_backlight_power_to_intensities_function.setResultTypeArray();
  ir_backlight_power_to_intensities_function.setResultTypeDouble();

  modular_server::Function & ir_backlight_intensity_to_powers_function = modular_server_.createFunction(constants::ir_backlight_intensity_to_powers_function_name);
  ir_backlight_intensity_to_powers_function.attachFunctor(makeFunctor((Functor0 *)0,*this,&LickportArrayController::irBacklightIntensityToPowersHandler));
  ir_backlight_intensity_to_powers_function.addParameter(intensity_parameter);
  ir_backlight_intensity_to_powers_function.setResultTypeArray();
  ir_backlight_intensity_to_powers_function.setResultTypeDouble();

  modular_server::Function & set_all_visible_backlights_on_at_power_function = modular_server_.createFunction(lickport_array_controller::constants::set_all_visible_backlights_on_at_power_function_name);
  set_all_visible_backlights_on_at_power_function.attachFunctor(makeFunctor((Functor0 *)0,*this,&LickportArrayController::setAllVisibleBacklightsOnAtPowerHandler));
  set_all_visible_backlights_on_at_power_function.addParameter(power_parameter);

  modular_server::Function & set_all_visible_backlights_on_at_intensity_function = modular_server_.createFunction(lickport_array_controller::constants::set_all_visible_backlights_on_at_intensity_function_name);
  set_all_visible_backlights_on_at_intensity_function.attachFunctor(makeFunctor((Functor0 *)0,*this,&LickportArrayController::setAllVisibleBacklightsOnAtIntensityHandler));
  set_all_visible_backlights_on_at_intensity_function.addParameter(intensity_parameter);

  modular_server::Function & set_visible_backlight_on_function = modular_server_.createFunction(lickport_array_controller::constants::set_visible_backlight_on_function_name);
  set_visible_backlight_on_function.attachFunctor(makeFunctor((Functor0 *)0,*this,&LickportArrayController::setVisibleBacklightOnHandler));
  set_visible_backlight_on_function.addParameter(visible_backlight_parameter);

  modular_server::Function & set_visible_backlight_on_at_power_function = modular_server_.createFunction(lickport_array_controller::constants::set_visible_backlight_on_at_power_function_name);
  set_visible_backlight_on_at_power_function.attachFunctor(makeFunctor((Functor0 *)0,*this,&LickportArrayController::setVisibleBacklightOnAtPowerHandler));
  set_visible_backlight_on_at_power_function.addParameter(visible_backlight_parameter);
  set_visible_backlight_on_at_power_function.addParameter(power_parameter);

  modular_server::Function & set_visible_backlight_on_at_intensity_function = modular_server_.createFunction(lickport_array_controller::constants::set_visible_backlight_on_at_intensity_function_name);
  set_visible_backlight_on_at_intensity_function.attachFunctor(makeFunctor((Functor0 *)0,*this,&LickportArrayController::setVisibleBacklightOnAtIntensityHandler));
  set_visible_backlight_on_at_intensity_function.addParameter(visible_backlight_parameter);
  set_visible_backlight_on_at_intensity_function.addParameter(intensity_parameter);

  modular_server::Function & set_visible_backlight_off_function = modular_server_.createFunction(lickport_array_controller::constants::set_visible_backlight_off_function_name);
  set_visible_backlight_off_function.attachFunctor(makeFunctor((Functor0 *)0,*this,&LickportArrayController::setVisibleBacklightOffHandler));
  set_visible_backlight_off_function.addParameter(visible_backlight_parameter);

  modular_server::Function & toggle_visible_backlight_function = modular_server_.createFunction(lickport_array_controller::constants::toggle_visible_backlight_function_name);
  toggle_visible_backlight_function.attachFunctor(makeFunctor((Functor0 *)0,*this,&LickportArrayController::toggleVisibleBacklightHandler));
  toggle_visible_backlight_function.addParameter(visible_backlight_parameter);

  modular_server::Function & get_visible_backlight_powers_when_on_function = modular_server_.createFunction(constants::get_visible_backlight_powers_when_on_function_name);
  get_visible_backlight_powers_when_on_function.attachFunctor(makeFunctor((Functor0 *)0,*this,&LickportArrayController::getVisibleBacklightPowersWhenOnHandler));
  get_visible_backlight_powers_when_on_function.setResultTypeArray();
  get_visible_backlight_powers_when_on_function.setResultTypeDouble();

  modular_server::Function & get_visible_backlight_intensities_when_on_function = modular_server_.createFunction(constants::get_visible_backlight_intensities_when_on_function_name);
  get_visible_backlight_intensities_when_on_function.attachFunctor(makeFunctor((Functor0 *)0,*this,&LickportArrayController::getVisibleBacklightIntensitiesWhenOnHandler));
  get_visible_backlight_intensities_when_on_function.setResultTypeArray();
  get_visible_backlight_intensities_when_on_function.setResultTypeDouble();

  modular_server::Function & get_visible_backlight_powers_function = modular_server_.createFunction(constants::get_visible_backlight_powers_function_name);
  get_visible_backlight_powers_function.attachFunctor(makeFunctor((Functor0 *)0,*this,&LickportArrayController::getVisibleBacklightPowersHandler));
  get_visible_backlight_powers_function.setResultTypeArray();
  get_visible_backlight_powers_function.setResultTypeDouble();

  modular_server::Function & get_visible_backlight_intensities_function = modular_server_.createFunction(constants::get_visible_backlight_intensities_function_name);
  get_visible_backlight_intensities_function.attachFunctor(makeFunctor((Functor0 *)0,*this,&LickportArrayController::getVisibleBacklightIntensitiesHandler));
  get_visible_backlight_intensities_function.setResultTypeArray();
  get_visible_backlight_intensities_function.setResultTypeDouble();

  modular_server::Function & get_visible_backlight_power_bounds_function = modular_server_.createFunction(constants::get_visible_backlight_power_bounds_function_name);
  get_visible_backlight_power_bounds_function.attachFunctor(makeFunctor((Functor0 *)0,*this,&LickportArrayController::getVisibleBacklightPowerBoundsHandler));
  get_visible_backlight_power_bounds_function.setResultTypeArray();
  get_visible_backlight_power_bounds_function.setResultTypeObject();

  modular_server::Function & get_visible_backlight_intensity_bounds_function = modular_server_.createFunction(constants::get_visible_backlight_intensity_bounds_function_name);
  get_visible_backlight_intensity_bounds_function.attachFunctor(makeFunctor((Functor0 *)0,*this,&LickportArrayController::getVisibleBacklightIntensityBoundsHandler));
  get_visible_backlight_intensity_bounds_function.setResultTypeArray();
  get_visible_backlight_intensity_bounds_function.setResultTypeObject();

  modular_server::Function & visible_backlight_power_to_intensities_function = modular_server_.createFunction(constants::visible_backlight_power_to_intensities_function_name);
  visible_backlight_power_to_intensities_function.attachFunctor(makeFunctor((Functor0 *)0,*this,&LickportArrayController::visibleBacklightPowerToIntensitiesHandler));
  visible_backlight_power_to_intensities_function.addParameter(power_parameter);
  visible_backlight_power_to_intensities_function.setResultTypeArray();
  visible_backlight_power_to_intensities_function.setResultTypeDouble();

  modular_server::Function & visible_backlight_intensity_to_powers_function = modular_server_.createFunction(constants::visible_backlight_intensity_to_powers_function_name);
  visible_backlight_intensity_to_powers_function.attachFunctor(makeFunctor((Functor0 *)0,*this,&LickportArrayController::visibleBacklightIntensityToPowersHandler));
  visible_backlight_intensity_to_powers_function.addParameter(intensity_parameter);
  visible_backlight_intensity_to_powers_function.setResultTypeArray();
  visible_backlight_intensity_to_powers_function.setResultTypeDouble();

  modular_server::Function & set_all_high_voltages_on_at_power_function = modular_server_.createFunction(lickport_array_controller::constants::set_all_high_voltages_on_at_power_function_name);
  set_all_high_voltages_on_at_power_function.attachFunctor(makeFunctor((Functor0 *)0,*this,&LickportArrayController::setAllHighVoltagesOnAtPowerHandler));
  set_all_high_voltages_on_at_power_function.addParameter(power_parameter);

  modular_server::Function & set_high_voltage_on_function = modular_server_.createFunction(lickport_array_controller::constants::set_high_voltage_on_function_name);
  set_high_voltage_on_function.attachFunctor(makeFunctor((Functor0 *)0,*this,&LickportArrayController::setHighVoltageOnHandler));
  set_high_voltage_on_function.addParameter(high_voltage_parameter);

  modular_server::Function & set_high_voltage_on_at_power_function = modular_server_.createFunction(lickport_array_controller::constants::set_high_voltage_on_at_power_function_name);
  set_high_voltage_on_at_power_function.attachFunctor(makeFunctor((Functor0 *)0,*this,&LickportArrayController::setHighVoltageOnAtPowerHandler));
  set_high_voltage_on_at_power_function.addParameter(high_voltage_parameter);
  set_high_voltage_on_at_power_function.addParameter(power_parameter);

  modular_server::Function & set_high_voltage_off_function = modular_server_.createFunction(lickport_array_controller::constants::set_high_voltage_off_function_name);
  set_high_voltage_off_function.attachFunctor(makeFunctor((Functor0 *)0,*this,&LickportArrayController::setHighVoltageOffHandler));
  set_high_voltage_off_function.addParameter(high_voltage_parameter);

  modular_server::Function & toggle_high_voltage_function = modular_server_.createFunction(lickport_array_controller::constants::toggle_high_voltage_function_name);
  toggle_high_voltage_function.attachFunctor(makeFunctor((Functor0 *)0,*this,&LickportArrayController::toggleHighVoltageHandler));
  toggle_high_voltage_function.addParameter(high_voltage_parameter);

  modular_server::Function & get_high_voltage_powers_when_on_function = modular_server_.createFunction(constants::get_high_voltage_powers_when_on_function_name);
  get_high_voltage_powers_when_on_function.attachFunctor(makeFunctor((Functor0 *)0,*this,&LickportArrayController::getHighVoltagePowersWhenOnHandler));
  get_high_voltage_powers_when_on_function.setResultTypeArray();
  get_high_voltage_powers_when_on_function.setResultTypeDouble();

  modular_server::Function & get_high_voltage_powers_function = modular_server_.createFunction(constants::get_high_voltage_powers_function_name);
  get_high_voltage_powers_function.attachFunctor(makeFunctor((Functor0 *)0,*this,&LickportArrayController::getHighVoltagePowersHandler));
  get_high_voltage_powers_function.setResultTypeArray();
  get_high_voltage_powers_function.setResultTypeDouble();

  modular_server::Function & get_high_voltage_power_bounds_function = modular_server_.createFunction(constants::get_high_voltage_power_bounds_function_name);
  get_high_voltage_power_bounds_function.attachFunctor(makeFunctor((Functor0 *)0,*this,&LickportArrayController::getHighVoltagePowerBoundsHandler));
  get_high_voltage_power_bounds_function.setResultTypeArray();
  get_high_voltage_power_bounds_function.setResultTypeObject();

  modular_server::Function & set_all_low_voltages_on_at_power_function = modular_server_.createFunction(lickport_array_controller::constants::set_all_low_voltages_on_at_power_function_name);
  set_all_low_voltages_on_at_power_function.attachFunctor(makeFunctor((Functor0 *)0,*this,&LickportArrayController::setAllLowVoltagesOnAtPowerHandler));
  set_all_low_voltages_on_at_power_function.addParameter(power_parameter);

  modular_server::Function & set_low_voltage_on_function = modular_server_.createFunction(lickport_array_controller::constants::set_low_voltage_on_function_name);
  set_low_voltage_on_function.attachFunctor(makeFunctor((Functor0 *)0,*this,&LickportArrayController::setLowVoltageOnHandler));
  set_low_voltage_on_function.addParameter(low_voltage_parameter);

  modular_server::Function & set_low_voltage_on_at_power_function = modular_server_.createFunction(lickport_array_controller::constants::set_low_voltage_on_at_power_function_name);
  set_low_voltage_on_at_power_function.attachFunctor(makeFunctor((Functor0 *)0,*this,&LickportArrayController::setLowVoltageOnAtPowerHandler));
  set_low_voltage_on_at_power_function.addParameter(low_voltage_parameter);
  set_low_voltage_on_at_power_function.addParameter(power_parameter);

  modular_server::Function & set_low_voltage_off_function = modular_server_.createFunction(lickport_array_controller::constants::set_low_voltage_off_function_name);
  set_low_voltage_off_function.attachFunctor(makeFunctor((Functor0 *)0,*this,&LickportArrayController::setLowVoltageOffHandler));
  set_low_voltage_off_function.addParameter(low_voltage_parameter);

  modular_server::Function & toggle_low_voltage_function = modular_server_.createFunction(lickport_array_controller::constants::toggle_low_voltage_function_name);
  toggle_low_voltage_function.attachFunctor(makeFunctor((Functor0 *)0,*this,&LickportArrayController::toggleLowVoltageHandler));
  toggle_low_voltage_function.addParameter(low_voltage_parameter);

  modular_server::Function & get_low_voltage_powers_when_on_function = modular_server_.createFunction(constants::get_low_voltage_powers_when_on_function_name);
  get_low_voltage_powers_when_on_function.attachFunctor(makeFunctor((Functor0 *)0,*this,&LickportArrayController::getLowVoltagePowersWhenOnHandler));
  get_low_voltage_powers_when_on_function.setResultTypeArray();
  get_low_voltage_powers_when_on_function.setResultTypeDouble();

  modular_server::Function & get_low_voltage_powers_function = modular_server_.createFunction(constants::get_low_voltage_powers_function_name);
  get_low_voltage_powers_function.attachFunctor(makeFunctor((Functor0 *)0,*this,&LickportArrayController::getLowVoltagePowersHandler));
  get_low_voltage_powers_function.setResultTypeArray();
  get_low_voltage_powers_function.setResultTypeDouble();

  modular_server::Function & get_low_voltage_power_bounds_function = modular_server_.createFunction(constants::get_low_voltage_power_bounds_function_name);
  get_low_voltage_power_bounds_function.attachFunctor(makeFunctor((Functor0 *)0,*this,&LickportArrayController::getLowVoltagePowerBoundsHandler));
  get_low_voltage_power_bounds_function.setResultTypeArray();
  get_low_voltage_power_bounds_function.setResultTypeObject();

  // Callbacks
  modular_server::Callback & toggle_all_channels_callback = modular_server_.callback(digital_controller::constants::toggle_all_channels_callback_name);
  toggle_all_channels_callback.attachTo(btn_test_pin,modular_server::constants::pin_mode_interrupt_falling);

  modular_server::Callback & enable_all_callback = modular_server_.callback(digital_controller::constants::enable_all_callback_name);
  enable_all_callback.attachFunctor(makeFunctor((Functor1<modular_server::Pin *> *)0,*this,&LickportArrayController::enableAllHandler));

  modular_server::Callback & disable_all_callback = modular_server_.callback(digital_controller::constants::disable_all_callback_name);
  disable_all_callback.attachFunctor(makeFunctor((Functor1<modular_server::Pin *> *)0,*this,&LickportArrayController::disableAllHandler));

  modular_server::Callback & set_all_ir_backlights_on_callback = modular_server_.createCallback(lickport_array_controller::constants::set_all_ir_backlights_on_callback_name);
  set_all_ir_backlights_on_callback.attachFunctor(makeFunctor((Functor1<modular_server::Pin *> *)0,*this,&LickportArrayController::setAllIrBacklightsOnHandler));

  modular_server::Callback & set_all_ir_backlights_off_callback = modular_server_.createCallback(lickport_array_controller::constants::set_all_ir_backlights_off_callback_name);
  set_all_ir_backlights_off_callback.attachFunctor(makeFunctor((Functor1<modular_server::Pin *> *)0,*this,&LickportArrayController::setAllIrBacklightsOffHandler));

  modular_server::Callback & toggle_all_ir_backlights_callback = modular_server_.createCallback(lickport_array_controller::constants::toggle_all_ir_backlights_callback_name);
  toggle_all_ir_backlights_callback.attachFunctor(makeFunctor((Functor1<modular_server::Pin *> *)0,*this,&LickportArrayController::toggleAllIrBacklightsHandler));

  modular_server::Callback & set_all_visible_backlights_on_callback = modular_server_.createCallback(lickport_array_controller::constants::set_all_visible_backlights_on_callback_name);
  set_all_visible_backlights_on_callback.attachFunctor(makeFunctor((Functor1<modular_server::Pin *> *)0,*this,&LickportArrayController::setAllVisibleBacklightsOnHandler));

  modular_server::Callback & set_all_visible_backlights_off_callback = modular_server_.createCallback(lickport_array_controller::constants::set_all_visible_backlights_off_callback_name);
  set_all_visible_backlights_off_callback.attachFunctor(makeFunctor((Functor1<modular_server::Pin *> *)0,*this,&LickportArrayController::setAllVisibleBacklightsOffHandler));

  modular_server::Callback & toggle_all_visible_backlights_callback = modular_server_.createCallback(lickport_array_controller::constants::toggle_all_visible_backlights_callback_name);
  toggle_all_visible_backlights_callback.attachFunctor(makeFunctor((Functor1<modular_server::Pin *> *)0,*this,&LickportArrayController::toggleAllVisibleBacklightsHandler));

  modular_server::Callback & set_all_high_voltages_on_callback = modular_server_.createCallback(lickport_array_controller::constants::set_all_high_voltages_on_callback_name);
  set_all_high_voltages_on_callback.attachFunctor(makeFunctor((Functor1<modular_server::Pin *> *)0,*this,&LickportArrayController::setAllHighVoltagesOnHandler));

  modular_server::Callback & set_all_high_voltages_off_callback = modular_server_.createCallback(lickport_array_controller::constants::set_all_high_voltages_off_callback_name);
  set_all_high_voltages_off_callback.attachFunctor(makeFunctor((Functor1<modular_server::Pin *> *)0,*this,&LickportArrayController::setAllHighVoltagesOffHandler));

  modular_server::Callback & toggle_all_high_voltages_callback = modular_server_.createCallback(lickport_array_controller::constants::toggle_all_high_voltages_callback_name);
  toggle_all_high_voltages_callback.attachFunctor(makeFunctor((Functor1<modular_server::Pin *> *)0,*this,&LickportArrayController::toggleAllHighVoltagesHandler));

  modular_server::Callback & set_all_low_voltages_on_callback = modular_server_.createCallback(lickport_array_controller::constants::set_all_low_voltages_on_callback_name);
  set_all_low_voltages_on_callback.attachFunctor(makeFunctor((Functor1<modular_server::Pin *> *)0,*this,&LickportArrayController::setAllLowVoltagesOnHandler));

  modular_server::Callback & set_all_low_voltages_off_callback = modular_server_.createCallback(lickport_array_controller::constants::set_all_low_voltages_off_callback_name);
  set_all_low_voltages_off_callback.attachFunctor(makeFunctor((Functor1<modular_server::Pin *> *)0,*this,&LickportArrayController::setAllLowVoltagesOffHandler));

  modular_server::Callback & toggle_all_low_voltages_callback = modular_server_.createCallback(lickport_array_controller::constants::toggle_all_low_voltages_callback_name);
  toggle_all_low_voltages_callback.attachFunctor(makeFunctor((Functor1<modular_server::Pin *> *)0,*this,&LickportArrayController::toggleAllLowVoltagesHandler));

  modular_server::Callback & toggle_enable_all_callback = modular_server_.createCallback(lickport_array_controller::constants::toggle_enable_all_callback_name);
  toggle_enable_all_callback.attachFunctor(makeFunctor((Functor1<modular_server::Pin *> *)0,*this,&LickportArrayController::toggleEnableAllHandler));
  toggle_enable_all_callback.attachTo(btn_enable_pin,modular_server::constants::pin_mode_interrupt_falling);

  enableAll();
  setAllChannelsOff();
}

void LickportArrayController::enableAll()
{
  digitalWrite(constants::enable_all_pin,HIGH);
  DigitalController::enableAll();
}

void LickportArrayController::disableAll()
{
  digitalWrite(constants::enable_all_pin,LOW);
  DigitalController::disableAll();
}

void LickportArrayController::toggleEnableAll()
{
  if (allEnabled())
  {
    disableAll();
  }
  else
  {
    enableAll();
  }
}

void LickportArrayController::setAllIrBacklightsOnAtPower(double power)
{
  setChannelsOnAtPower(getIrBacklightDigitalChannels(),power);
}

void LickportArrayController::setAllIrBacklightsOnAtIntensity(double intensity)
{
  double power;
  for (size_t ir_backlight=0; ir_backlight<getIrBacklightCount(); ++ir_backlight)
  {
    power = irBacklightIntensityToPower(ir_backlight,intensity);
    setIrBacklightOnAtPower(ir_backlight,power);
  }
}

void LickportArrayController::setAllIrBacklightsOn()
{
  setChannelsOn(getIrBacklightDigitalChannels());
}

void LickportArrayController::setAllIrBacklightsOff()
{
  setChannelsOff(getIrBacklightDigitalChannels());
}

void LickportArrayController::toggleAllIrBacklights()
{
  toggleChannels(getIrBacklightDigitalChannels());
}

size_t LickportArrayController::getIrBacklightCount()
{
  return constants::IR_BACKLIGHT_COUNT;
}

void LickportArrayController::setIrBacklightOn(size_t ir_backlight)
{
  setChannelOn(irBacklightToDigitalChannel(ir_backlight));
}

void LickportArrayController::setIrBacklightOnAtPower(size_t ir_backlight,
  double power)
{
  setChannelOnAtPower(irBacklightToDigitalChannel(ir_backlight),power);
}

void LickportArrayController::setIrBacklightOnAtIntensity(size_t ir_backlight,
  double intensity)
{
  double power = irBacklightIntensityToPower(ir_backlight,intensity);
  setIrBacklightOnAtPower(ir_backlight,power);
}

void LickportArrayController::setIrBacklightOff(size_t ir_backlight)
{
  setChannelOff(irBacklightToDigitalChannel(ir_backlight));
}

void LickportArrayController::toggleIrBacklight(size_t ir_backlight)
{
  toggleChannel(irBacklightToDigitalChannel(ir_backlight));
}

double LickportArrayController::getIrBacklightPowerWhenOn(size_t ir_backlight)
{
  return getPowerWhenOn(irBacklightToDigitalChannel(ir_backlight));
}

double LickportArrayController::getIrBacklightIntensityWhenOn(size_t ir_backlight)
{
  double power = getIrBacklightPowerWhenOn(ir_backlight);
  double intensity = irBacklightPowerToIntensity(ir_backlight,power);
  return intensity;
}

double LickportArrayController::getIrBacklightPower(size_t ir_backlight)
{
  return getPower(irBacklightToDigitalChannel(ir_backlight));
}

double LickportArrayController::getIrBacklightIntensity(size_t ir_backlight)
{
  double power = getIrBacklightPower(ir_backlight);
  double intensity = irBacklightPowerToIntensity(ir_backlight,power);
  return intensity;
}

double LickportArrayController::irBacklightIntensityToPower(size_t ir_backlight,
  double intensity)
{
  if (ir_backlight >= getIrBacklightCount())
  {
    return intensity;
  }
  double ir_backlight_power_to_intensity_ratio;
  modular_server::Property & ir_backlight_power_to_intensity_ratio_property = modular_server_.property(constants::ir_backlight_power_to_intensity_ratio_property_name);
  ir_backlight_power_to_intensity_ratio_property.getElementValue(ir_backlight,ir_backlight_power_to_intensity_ratio);

  double power = intensity * ir_backlight_power_to_intensity_ratio;
  if (power > digital_controller::constants::power_max)
  {
    power = digital_controller::constants::power_max;
  }
  return power;
}

double LickportArrayController::irBacklightPowerToIntensity(size_t ir_backlight,
  double power)
{
  if (ir_backlight >= getIrBacklightCount())
  {
    return power;
  }
  double ir_backlight_power_to_intensity_ratio;
  modular_server::Property & ir_backlight_power_to_intensity_ratio_property = modular_server_.property(constants::ir_backlight_power_to_intensity_ratio_property_name);
  ir_backlight_power_to_intensity_ratio_property.getElementValue(ir_backlight,ir_backlight_power_to_intensity_ratio);

  double intensity;
  if (ir_backlight_power_to_intensity_ratio > 0.0)
  {
    intensity = power / ir_backlight_power_to_intensity_ratio;
  }
  else
  {
    intensity = power;
  }
  return intensity;
}

void LickportArrayController::setAllVisibleBacklightsOnAtPower(double power)
{
  setChannelsOnAtPower(getVisibleBacklightDigitalChannels(),power);
}

void LickportArrayController::setAllVisibleBacklightsOnAtIntensity(double intensity)
{
  double power;
  for (size_t visible_backlight=0; visible_backlight<getVisibleBacklightCount(); ++visible_backlight)
  {
    power = visibleBacklightIntensityToPower(visible_backlight,intensity);
    setVisibleBacklightOnAtPower(visible_backlight,power);
  }
}

void LickportArrayController::setAllVisibleBacklightsOn()
{
  setChannelsOn(getVisibleBacklightDigitalChannels());
}

void LickportArrayController::setAllVisibleBacklightsOff()
{
  setChannelsOff(getVisibleBacklightDigitalChannels());
}

void LickportArrayController::toggleAllVisibleBacklights()
{
  toggleChannels(getVisibleBacklightDigitalChannels());
}

size_t LickportArrayController::getVisibleBacklightCount()
{
  return constants::VISIBLE_BACKLIGHT_COUNT;
}

void LickportArrayController::setVisibleBacklightOn(size_t visible_backlight)
{
  setChannelOn(visibleBacklightToDigitalChannel(visible_backlight));
}

void LickportArrayController::setVisibleBacklightOnAtPower(size_t visible_backlight,
  double power)
{
  setChannelOnAtPower(visibleBacklightToDigitalChannel(visible_backlight),power);
}

void LickportArrayController::setVisibleBacklightOnAtIntensity(size_t visible_backlight,
  double intensity)
{
  double power = visibleBacklightIntensityToPower(visible_backlight,intensity);
  setVisibleBacklightOnAtPower(visible_backlight,power);
}

void LickportArrayController::setVisibleBacklightOff(size_t visible_backlight)
{
  setChannelOff(visibleBacklightToDigitalChannel(visible_backlight));
}

void LickportArrayController::toggleVisibleBacklight(size_t visible_backlight)
{
  toggleChannel(visibleBacklightToDigitalChannel(visible_backlight));
}

double LickportArrayController::getVisibleBacklightPowerWhenOn(size_t visible_backlight)
{
  return getPowerWhenOn(visibleBacklightToDigitalChannel(visible_backlight));
}

double LickportArrayController::getVisibleBacklightIntensityWhenOn(size_t visible_backlight)
{
  double power = getVisibleBacklightPowerWhenOn(visible_backlight);
  double intensity = visibleBacklightPowerToIntensity(visible_backlight,power);
  return intensity;
}

double LickportArrayController::getVisibleBacklightPower(size_t visible_backlight)
{
  return getPower(visibleBacklightToDigitalChannel(visible_backlight));
}

double LickportArrayController::getVisibleBacklightIntensity(size_t visible_backlight)
{
  double power = getVisibleBacklightPower(visible_backlight);
  double intensity = visibleBacklightPowerToIntensity(visible_backlight,power);
  return intensity;
}

double LickportArrayController::visibleBacklightIntensityToPower(size_t visible_backlight,
  double intensity)
{
  if (visible_backlight >= getVisibleBacklightCount())
  {
    return intensity;
  }
  double visible_backlight_power_to_intensity_ratio;
  modular_server::Property & visible_backlight_power_to_intensity_ratio_property = modular_server_.property(constants::visible_backlight_power_to_intensity_ratio_property_name);
  visible_backlight_power_to_intensity_ratio_property.getElementValue(visible_backlight,visible_backlight_power_to_intensity_ratio);

  double power = intensity * visible_backlight_power_to_intensity_ratio;
  if (power > digital_controller::constants::power_max)
  {
    power = digital_controller::constants::power_max;
  }
  return power;
}

double LickportArrayController::visibleBacklightPowerToIntensity(size_t visible_backlight,
  double power)
{
  if (visible_backlight >= getVisibleBacklightCount())
  {
    return power;
  }
  double visible_backlight_power_to_intensity_ratio;
  modular_server::Property & visible_backlight_power_to_intensity_ratio_property = modular_server_.property(constants::visible_backlight_power_to_intensity_ratio_property_name);
  visible_backlight_power_to_intensity_ratio_property.getElementValue(visible_backlight,visible_backlight_power_to_intensity_ratio);

  double intensity;
  if (visible_backlight_power_to_intensity_ratio > 0.0)
  {
    intensity = power / visible_backlight_power_to_intensity_ratio;
  }
  else
  {
    intensity = power;
  }
  return intensity;
}

void LickportArrayController::setAllHighVoltagesOnAtPower(double power)
{
  setChannelsOnAtPower(getHighVoltageDigitalChannels(),power);
}

void LickportArrayController::setAllHighVoltagesOn()
{
  setChannelsOn(getHighVoltageDigitalChannels());
}

void LickportArrayController::setAllHighVoltagesOff()
{
  setChannelsOff(getHighVoltageDigitalChannels());
}

void LickportArrayController::toggleAllHighVoltages()
{
  toggleChannels(getHighVoltageDigitalChannels());
}

size_t LickportArrayController::getHighVoltageCount()
{
  return constants::HIGH_VOLTAGE_COUNT;
}

void LickportArrayController::setHighVoltageOn(size_t high_voltage)
{
  setChannelOn(highVoltageToDigitalChannel(high_voltage));
}

void LickportArrayController::setHighVoltageOnAtPower(size_t high_voltage,
  double power)
{
  setChannelOnAtPower(highVoltageToDigitalChannel(high_voltage),power);
}

void LickportArrayController::setHighVoltageOff(size_t high_voltage)
{
  setChannelOff(highVoltageToDigitalChannel(high_voltage));
}

void LickportArrayController::toggleHighVoltage(size_t high_voltage)
{
  toggleChannel(highVoltageToDigitalChannel(high_voltage));
}

double LickportArrayController::getHighVoltagePowerWhenOn(size_t high_voltage)
{
  return getPowerWhenOn(highVoltageToDigitalChannel(high_voltage));
}

double LickportArrayController::getHighVoltagePower(size_t high_voltage)
{
  return getPower(highVoltageToDigitalChannel(high_voltage));
}

void LickportArrayController::setAllLowVoltagesOnAtPower(double power)
{
  setChannelsOnAtPower(getLowVoltageDigitalChannels(),power);
}

void LickportArrayController::setAllLowVoltagesOn()
{
  setChannelsOn(getLowVoltageDigitalChannels());
}

void LickportArrayController::setAllLowVoltagesOff()
{
  setChannelsOff(getLowVoltageDigitalChannels());
}

void LickportArrayController::toggleAllLowVoltages()
{
  toggleChannels(getLowVoltageDigitalChannels());
}

size_t LickportArrayController::getLowVoltageCount()
{
  return constants::LOW_VOLTAGE_COUNT;
}

void LickportArrayController::setLowVoltageOn(size_t low_voltage)
{
  setChannelOn(lowVoltageToDigitalChannel(low_voltage));
}

void LickportArrayController::setLowVoltageOnAtPower(size_t low_voltage,
  double power)
{
  setChannelOnAtPower(lowVoltageToDigitalChannel(low_voltage),power);
}

void LickportArrayController::setLowVoltageOff(size_t low_voltage)
{
  setChannelOff(lowVoltageToDigitalChannel(low_voltage));
}

void LickportArrayController::toggleLowVoltage(size_t low_voltage)
{
  toggleChannel(lowVoltageToDigitalChannel(low_voltage));
}

double LickportArrayController::getLowVoltagePowerWhenOn(size_t low_voltage)
{
  return getPowerWhenOn(lowVoltageToDigitalChannel(low_voltage));
}

double LickportArrayController::getLowVoltagePower(size_t low_voltage)
{
  return getPower(lowVoltageToDigitalChannel(low_voltage));
}

double LickportArrayController::getPowerLowerBound(size_t channel)
{
  if (channel >= getChannelCount())
  {
    return digital_controller::constants::power_lower_bound_default;
  }
  double channel_switching_frequency_max = channels_switching_frequency_max_[channel];

  double power_lower_bound;
  if (channel_switching_frequency_max > 0.0)
  {
    power_lower_bound = constants::switching_frequency_max / channel_switching_frequency_max;
  }
  else
  {
    power_lower_bound = digital_controller::constants::power_mid;
  }
  if (power_lower_bound > digital_controller::constants::power_mid)
  {
    power_lower_bound = digital_controller::constants::power_mid;
  }
  return power_lower_bound;
}

double LickportArrayController::getPowerUpperBound(size_t channel)
{
  if (channel >= getChannelCount())
  {
    return digital_controller::constants::power_upper_bound_default;
  }
  double power_upper_bound = digital_controller::constants::power_max - getPowerLowerBound(channel);
  return power_upper_bound;
}

double LickportArrayController::setChannelToPower(size_t channel,
  double power)
{
  long analog_write_value = powerToAnalogValue(channel,power);
  channelAnalogWrite(channel,analog_write_value);
  power = ((double)analog_write_value * digital_controller::constants::power_max) / (double)constants::analog_value_max;
  return power;
}

size_t LickportArrayController::irBacklightToDigitalChannel(size_t ir_backlight)
{
  if (ir_backlight >= getIrBacklightCount())
  {
    ir_backlight = 0;
  }
  return constants::ir_backlight_channels[ir_backlight];
}

uint32_t LickportArrayController::getIrBacklightDigitalChannels()
{
  uint32_t channels = 0;
  uint32_t bit = 1;
  size_t channel;
  for (size_t ir_backlight=0; ir_backlight<getIrBacklightCount(); ++ir_backlight)
  {
    channel = irBacklightToDigitalChannel(ir_backlight);
    channels |= (bit << channel);
  }
  return channels;
}

size_t LickportArrayController::visibleBacklightToDigitalChannel(size_t visible_backlight)
{
  if (visible_backlight >= getVisibleBacklightCount())
  {
    visible_backlight = 0;
  }
  return constants::visible_backlight_channels[visible_backlight];
}

uint32_t LickportArrayController::getVisibleBacklightDigitalChannels()
{
  uint32_t channels = 0;
  uint32_t bit = 1;
  size_t channel;
  for (size_t visible_backlight=0; visible_backlight<getVisibleBacklightCount(); ++visible_backlight)
  {
    channel = visibleBacklightToDigitalChannel(visible_backlight);
    channels |= (bit << channel);
  }
  return channels;
}

size_t LickportArrayController::highVoltageToDigitalChannel(size_t high_voltage)
{
  if (high_voltage >= getHighVoltageCount())
  {
    high_voltage = 0;
  }
  return constants::high_voltage_channels[high_voltage];
}

uint32_t LickportArrayController::getHighVoltageDigitalChannels()
{
  uint32_t channels = 0;
  uint32_t bit = 1;
  size_t channel;
  for (size_t high_voltage=0; high_voltage<getHighVoltageCount(); ++high_voltage)
  {
    channel = highVoltageToDigitalChannel(high_voltage);
    channels |= (bit << channel);
  }
  return channels;
}

size_t LickportArrayController::lowVoltageToDigitalChannel(size_t low_voltage)
{
  if (low_voltage >= getLowVoltageCount())
  {
    low_voltage = 0;
  }
  return constants::low_voltage_channels[low_voltage];
}

uint32_t LickportArrayController::getLowVoltageDigitalChannels()
{
  uint32_t channels = 0;
  uint32_t bit = 1;
  size_t channel;
  for (size_t low_voltage=0; low_voltage<getLowVoltageCount(); ++low_voltage)
  {
    channel = lowVoltageToDigitalChannel(low_voltage);
    channels |= (bit << channel);
  }
  return channels;
}

double LickportArrayController::getIrBacklightIntensityLowerBound(size_t ir_backlight)
{
  double power = getPowerLowerBound(irBacklightToDigitalChannel(ir_backlight));
  double intensity = irBacklightPowerToIntensity(ir_backlight,power);
  return intensity;
}

double LickportArrayController::getIrBacklightIntensityUpperBound(size_t ir_backlight)
{
  double power = getPowerUpperBound(irBacklightToDigitalChannel(ir_backlight));
  double intensity = irBacklightPowerToIntensity(ir_backlight,power);
  return intensity;
}

double LickportArrayController::getVisibleBacklightIntensityLowerBound(size_t visible_backlight)
{
  double power = getPowerLowerBound(visibleBacklightToDigitalChannel(visible_backlight));
  double intensity = visibleBacklightPowerToIntensity(visible_backlight,power);
  return intensity;
}

double LickportArrayController::getVisibleBacklightIntensityUpperBound(size_t visible_backlight)
{
  double power = getPowerUpperBound(visibleBacklightToDigitalChannel(visible_backlight));
  double intensity = visibleBacklightPowerToIntensity(visible_backlight,power);
  return intensity;
}


void LickportArrayController::updateAllSwitchingFrequencies()
{
  for (size_t ir_backlight=0; ir_backlight<getIrBacklightCount(); ++ir_backlight)
  {
    setIrBacklightSwitchingFrequencyMaxHandler(ir_backlight);
  }

  for (size_t visible_backlight=0; visible_backlight<getVisibleBacklightCount(); ++visible_backlight)
  {
    setVisibleBacklightSwitchingFrequencyMaxHandler(visible_backlight);
  }

  for (size_t high_voltage=0; high_voltage<getHighVoltageCount(); ++high_voltage)
  {
    setHighVoltageSwitchingFrequencyMaxHandler(high_voltage);
  }

  for (size_t low_voltage=0; low_voltage<getLowVoltageCount(); ++low_voltage)
  {
    setLowVoltageSwitchingFrequencyMaxHandler(low_voltage);
  }
}

void LickportArrayController::channelAnalogWrite(size_t channel,
  long value)
{
  if (!constants::analog_writeable[channel])
  {
    if (value < (constants::analog_value_max / 2))
    {
      digitalWrite(constants::channel_pins[channel],LOW);
      return;
    }
    else
    {
      digitalWrite(constants::channel_pins[channel],HIGH);
      return;
    }
  }
  analogWrite(constants::channel_pins[channel],value);
}

long LickportArrayController::powerToAnalogValue(size_t channel,
  double power)
{
  if (power < getPowerLowerBound(channel))
  {
    return constants::analog_value_min;
  }
  else if (power > getPowerUpperBound(channel))
  {
    return constants::analog_value_max;
  }

  long analog_value = map(power,
    (long)digital_controller::constants::power_min,
    (long)digital_controller::constants::power_max,
    constants::analog_value_min,
    constants::analog_value_max);
  return analog_value;
}

// Handlers must be non-blocking (avoid 'delay')
//
// modular_server_.parameter(parameter_name).getValue(value) value type must be either:
// fixed-point number (int, long, etc.)
// floating-point number (float, double)
// bool
// const char *
// ArduinoJson::JsonArray
// ArduinoJson::JsonObject
// const ConstantString *
//
// For more info read about ArduinoJson parsing https://github.com/janelia-arduino/ArduinoJson
//
// modular_server_.property(property_name).getValue(value) value type must match the property default type
// modular_server_.property(property_name).setalue(value) value type must match the property default type
// modular_server_.property(property_name).getElementValue(element_index,value) value type must match the property array element default type
// modular_server_.property(property_name).setlementValue(element_index,value) value type must match the property array element default type

void LickportArrayController::enableAllHandler(modular_server::Pin * pin_ptr)
{
  enableAll();
}

void LickportArrayController::disableAllHandler(modular_server::Pin * pin_ptr)
{
  disableAll();
}

void LickportArrayController::setIrBacklightPowerToIntensityRatioHandler(size_t ir_backlight)
{
  double intensity_max_max = irBacklightPowerToIntensity(ir_backlight,digital_controller::constants::power_max);

  modular_server::Property & ir_backlight_intensity_max_property = modular_server_.property(constants::ir_backlight_intensity_max_property_name);

  double intensity_max;

  ir_backlight_intensity_max_property.getElementValue(ir_backlight,intensity_max);

  if (intensity_max > intensity_max_max)
  {
    ir_backlight_intensity_max_property.setElementValue(ir_backlight,intensity_max_max);
  }
}

void LickportArrayController::setIrBacklightIntensityMaxHandler(size_t ir_backlight)
{
  double intensity_max;
  modular_server::Property & ir_backlight_intensity_max_property = modular_server_.property(constants::ir_backlight_intensity_max_property_name);
  ir_backlight_intensity_max_property.getElementValue(ir_backlight,intensity_max);

  double intensity_max_max = irBacklightPowerToIntensity(ir_backlight,digital_controller::constants::power_max);

  if (intensity_max > intensity_max_max)
  {
    intensity_max = intensity_max_max;
    ir_backlight_intensity_max_property.disableFunctors();
    ir_backlight_intensity_max_property.setElementValue(ir_backlight,intensity_max);
    ir_backlight_intensity_max_property.reenableFunctors();
  }

  double power_max = irBacklightIntensityToPower(ir_backlight,intensity_max);

  size_t digital_channel = irBacklightToDigitalChannel(ir_backlight);

  modular_server::Property & power_max_property = modular_server_.property(digital_controller::constants::power_max_property_name);
  power_max_property.setElementValue(digital_channel,power_max);
}

void LickportArrayController::setVisibleBacklightPowerToIntensityRatioHandler(size_t visible_backlight)
{
  double intensity_max_max = visibleBacklightPowerToIntensity(visible_backlight,digital_controller::constants::power_max);

  modular_server::Property & visible_backlight_intensity_max_property = modular_server_.property(constants::visible_backlight_intensity_max_property_name);

  double intensity_max;

  visible_backlight_intensity_max_property.getElementValue(visible_backlight,intensity_max);

  if (intensity_max > intensity_max_max)
  {
    visible_backlight_intensity_max_property.setElementValue(visible_backlight,intensity_max_max);
  }
}

void LickportArrayController::setVisibleBacklightIntensityMaxHandler(size_t visible_backlight)
{
  double intensity_max;
  modular_server::Property & visible_backlight_intensity_max_property = modular_server_.property(constants::visible_backlight_intensity_max_property_name);
  visible_backlight_intensity_max_property.getElementValue(visible_backlight,intensity_max);

  double intensity_max_max = visibleBacklightPowerToIntensity(visible_backlight,digital_controller::constants::power_max);

  if (intensity_max > intensity_max_max)
  {
    intensity_max = intensity_max_max;
    visible_backlight_intensity_max_property.disableFunctors();
    visible_backlight_intensity_max_property.setElementValue(visible_backlight,intensity_max);
    visible_backlight_intensity_max_property.reenableFunctors();
  }

  double power_max = visibleBacklightIntensityToPower(visible_backlight,intensity_max);

  size_t digital_channel = visibleBacklightToDigitalChannel(visible_backlight);

  modular_server::Property & power_max_property = modular_server_.property(digital_controller::constants::power_max_property_name);
  power_max_property.setElementValue(digital_channel,power_max);
}

void LickportArrayController::setHighVoltagePowerMaxHandler(size_t high_voltage)
{
  double power_max;
  modular_server::Property & high_voltage_power_max_property = modular_server_.property(constants::high_voltage_power_max_property_name);
  high_voltage_power_max_property.getElementValue(high_voltage,power_max);

  size_t digital_channel = highVoltageToDigitalChannel(high_voltage);

  modular_server::Property & power_max_property = modular_server_.property(digital_controller::constants::power_max_property_name);
  power_max_property.setElementValue(digital_channel,power_max);
}

void LickportArrayController::setLowVoltagePowerMaxHandler(size_t low_voltage)
{
  double power_max;
  modular_server::Property & low_voltage_power_max_property = modular_server_.property(constants::low_voltage_power_max_property_name);
  low_voltage_power_max_property.getElementValue(low_voltage,power_max);

  size_t digital_channel = lowVoltageToDigitalChannel(low_voltage);

  modular_server::Property & power_max_property = modular_server_.property(digital_controller::constants::power_max_property_name);
  power_max_property.setElementValue(digital_channel,power_max);
}

void LickportArrayController::setIrBacklightSwitchingFrequencyMaxHandler(size_t ir_backlight)
{
  long switching_frequency_max;
  modular_server::Property & ir_backlight_switching_frequency_max_property = modular_server_.property(constants::ir_backlight_switching_frequency_max_property_name);
  ir_backlight_switching_frequency_max_property.getElementValue(ir_backlight,switching_frequency_max);

  size_t digital_channel = irBacklightToDigitalChannel(ir_backlight);

  channels_switching_frequency_max_[digital_channel] = switching_frequency_max;
}

void LickportArrayController::setVisibleBacklightSwitchingFrequencyMaxHandler(size_t visible_backlight)
{
  long switching_frequency_max;
  modular_server::Property & visible_backlight_switching_frequency_max_property = modular_server_.property(constants::visible_backlight_switching_frequency_max_property_name);
  visible_backlight_switching_frequency_max_property.getElementValue(visible_backlight,switching_frequency_max);

  size_t digital_channel = visibleBacklightToDigitalChannel(visible_backlight);

  channels_switching_frequency_max_[digital_channel] = switching_frequency_max;
}

void LickportArrayController::setHighVoltageSwitchingFrequencyMaxHandler(size_t high_voltage)
{
  long switching_frequency_max;
  modular_server::Property & high_voltage_switching_frequency_max_property = modular_server_.property(constants::high_voltage_switching_frequency_max_property_name);
  high_voltage_switching_frequency_max_property.getElementValue(high_voltage,switching_frequency_max);

  size_t digital_channel = highVoltageToDigitalChannel(high_voltage);

  channels_switching_frequency_max_[digital_channel] = switching_frequency_max;
}

void LickportArrayController::setLowVoltageSwitchingFrequencyMaxHandler(size_t low_voltage)
{
  long switching_frequency_max;
  modular_server::Property & low_voltage_switching_frequency_max_property = modular_server_.property(constants::low_voltage_switching_frequency_max_property_name);
  low_voltage_switching_frequency_max_property.getElementValue(low_voltage,switching_frequency_max);

  size_t digital_channel = lowVoltageToDigitalChannel(low_voltage);

  channels_switching_frequency_max_[digital_channel] = switching_frequency_max;
}

void LickportArrayController::setAllIrBacklightsOnAtPowerHandler()
{
  double power;
  modular_server_.parameter(digital_controller::constants::power_parameter_name).getValue(power);

  setAllIrBacklightsOnAtPower(power);
}

void LickportArrayController::setAllIrBacklightsOnAtIntensityHandler()
{
  double intensity;
  modular_server_.parameter(constants::intensity_parameter_name).getValue(intensity);

  setAllIrBacklightsOnAtIntensity(intensity);
}

void LickportArrayController::setAllIrBacklightsOnHandler(modular_server::Pin * pin_ptr)
{
  setAllIrBacklightsOn();
}

void LickportArrayController::setAllIrBacklightsOffHandler(modular_server::Pin * pin_ptr)
{
  setAllIrBacklightsOff();
}

void LickportArrayController::toggleAllIrBacklightsHandler(modular_server::Pin * pin_ptr)
{
  toggleAllIrBacklights();
}

void LickportArrayController::setIrBacklightOnHandler()
{
  long ir_backlight;
  modular_server_.parameter(constants::ir_backlight_parameter_name).getValue(ir_backlight);

  setIrBacklightOn(ir_backlight);
}

void LickportArrayController::setIrBacklightOnAtPowerHandler()
{
  long ir_backlight;
  modular_server_.parameter(constants::ir_backlight_parameter_name).getValue(ir_backlight);

  double power;
  modular_server_.parameter(digital_controller::constants::power_parameter_name).getValue(power);

  setIrBacklightOnAtPower(ir_backlight,power);
}

void LickportArrayController::setIrBacklightOnAtIntensityHandler()
{
  long ir_backlight;
  modular_server_.parameter(constants::ir_backlight_parameter_name).getValue(ir_backlight);

  double intensity;
  modular_server_.parameter(constants::intensity_parameter_name).getValue(intensity);

  setIrBacklightOnAtIntensity(ir_backlight,intensity);
}

void LickportArrayController::setIrBacklightOffHandler()
{
  long ir_backlight;
  modular_server_.parameter(constants::ir_backlight_parameter_name).getValue(ir_backlight);

  setIrBacklightOff(ir_backlight);
}

void LickportArrayController::toggleIrBacklightHandler()
{
  long ir_backlight;
  modular_server_.parameter(constants::ir_backlight_parameter_name).getValue(ir_backlight);

  toggleIrBacklight(ir_backlight);
}

void LickportArrayController::getIrBacklightPowersWhenOnHandler()
{
  modular_server_.response().writeResultKey();
  modular_server_.response().beginArray();
  double power;
  for (size_t ir_backlight=0; ir_backlight<getIrBacklightCount(); ++ir_backlight)
  {
    power = getIrBacklightPowerWhenOn(ir_backlight);
    modular_server_.response().write(power);
  }
  modular_server_.response().endArray();
}

void LickportArrayController::getIrBacklightIntensitiesWhenOnHandler()
{
  modular_server_.response().writeResultKey();
  modular_server_.response().beginArray();
  double intensity;
  for (size_t ir_backlight=0; ir_backlight<getIrBacklightCount(); ++ir_backlight)
  {
    intensity = getIrBacklightIntensityWhenOn(ir_backlight);
    modular_server_.response().write(intensity);
  }
  modular_server_.response().endArray();
}

void LickportArrayController::getIrBacklightPowersHandler()
{
  modular_server_.response().writeResultKey();
  modular_server_.response().beginArray();
  double power;
  for (size_t ir_backlight=0; ir_backlight<getIrBacklightCount(); ++ir_backlight)
  {
    power = getIrBacklightPower(ir_backlight);
    modular_server_.response().write(power);
  }
  modular_server_.response().endArray();
}

void LickportArrayController::getIrBacklightIntensitiesHandler()
{
  modular_server_.response().writeResultKey();
  modular_server_.response().beginArray();
  double intensity;
  for (size_t ir_backlight=0; ir_backlight<getIrBacklightCount(); ++ir_backlight)
  {
    intensity = getIrBacklightIntensity(ir_backlight);
    modular_server_.response().write(intensity);
  }
  modular_server_.response().endArray();
}

void LickportArrayController::getIrBacklightPowerBoundsHandler()
{
  modular_server_.response().writeResultKey();
  modular_server_.response().beginArray();
  for (size_t ir_backlight=0; ir_backlight<getIrBacklightCount(); ++ir_backlight)
  {
    modular_server_.response().beginObject();

    double power_lower_bound = getPowerLowerBound(irBacklightToDigitalChannel(ir_backlight));
    modular_server_.response().write(digital_controller::constants::power_lower_bound_string,power_lower_bound);

    double power_upper_bound = getPowerUpperBound(irBacklightToDigitalChannel(ir_backlight));
    modular_server_.response().write(digital_controller::constants::power_upper_bound_string,power_upper_bound);

    modular_server_.response().endObject();
  }
  modular_server_.response().endArray();
}

void LickportArrayController::getIrBacklightIntensityBoundsHandler()
{
  modular_server_.response().writeResultKey();
  modular_server_.response().beginArray();
  for (size_t ir_backlight=0; ir_backlight<getIrBacklightCount(); ++ir_backlight)
  {
    modular_server_.response().beginObject();

    double intensity_lower_bound = getIrBacklightIntensityLowerBound(ir_backlight);
    modular_server_.response().write(constants::intensity_lower_bound_string,intensity_lower_bound);

    double intensity_upper_bound = getIrBacklightIntensityUpperBound(ir_backlight);
    modular_server_.response().write(constants::intensity_upper_bound_string,intensity_upper_bound);

    modular_server_.response().endObject();
  }
  modular_server_.response().endArray();
}

void LickportArrayController::irBacklightPowerToIntensitiesHandler()
{
  double power;
  modular_server_.parameter(digital_controller::constants::power_parameter_name).getValue(power);

  modular_server_.response().writeResultKey();
  modular_server_.response().beginArray();
  double intensity;
  for (size_t ir_backlight=0; ir_backlight<getIrBacklightCount(); ++ir_backlight)
  {
    intensity = irBacklightPowerToIntensity(ir_backlight,power);
    modular_server_.response().write(intensity);
  }
  modular_server_.response().endArray();
}

void LickportArrayController::irBacklightIntensityToPowersHandler()
{
  double intensity;
  modular_server_.parameter(constants::intensity_parameter_name).getValue(intensity);

  modular_server_.response().writeResultKey();
  modular_server_.response().beginArray();
  double power;
  for (size_t ir_backlight=0; ir_backlight<getIrBacklightCount(); ++ir_backlight)
  {
    power = irBacklightIntensityToPower(ir_backlight,intensity);
    modular_server_.response().write(power);
  }
  modular_server_.response().endArray();
}

void LickportArrayController::setAllVisibleBacklightsOnAtPowerHandler()
{
  double power;
  modular_server_.parameter(digital_controller::constants::power_parameter_name).getValue(power);

  setAllVisibleBacklightsOnAtPower(power);
}

void LickportArrayController::setAllVisibleBacklightsOnAtIntensityHandler()
{
  double intensity;
  modular_server_.parameter(constants::intensity_parameter_name).getValue(intensity);

  setAllVisibleBacklightsOnAtIntensity(intensity);
}

void LickportArrayController::setAllVisibleBacklightsOnHandler(modular_server::Pin * pin_ptr)
{
  setAllVisibleBacklightsOn();
}

void LickportArrayController::setAllVisibleBacklightsOffHandler(modular_server::Pin * pin_ptr)
{
  setAllVisibleBacklightsOff();
}

void LickportArrayController::toggleAllVisibleBacklightsHandler(modular_server::Pin * pin_ptr)
{
  toggleAllVisibleBacklights();
}

void LickportArrayController::setVisibleBacklightOnHandler()
{
  long visible_backlight;
  modular_server_.parameter(constants::visible_backlight_parameter_name).getValue(visible_backlight);

  setVisibleBacklightOn(visible_backlight);
}

void LickportArrayController::setVisibleBacklightOnAtPowerHandler()
{
  long visible_backlight;
  modular_server_.parameter(constants::visible_backlight_parameter_name).getValue(visible_backlight);

  double power;
  modular_server_.parameter(digital_controller::constants::power_parameter_name).getValue(power);

  setVisibleBacklightOnAtPower(visible_backlight,power);
}

void LickportArrayController::setVisibleBacklightOnAtIntensityHandler()
{
  long visible_backlight;
  modular_server_.parameter(constants::visible_backlight_parameter_name).getValue(visible_backlight);

  double intensity;
  modular_server_.parameter(constants::intensity_parameter_name).getValue(intensity);

  setVisibleBacklightOnAtIntensity(visible_backlight,intensity);
}

void LickportArrayController::setVisibleBacklightOffHandler()
{
  long visible_backlight;
  modular_server_.parameter(constants::visible_backlight_parameter_name).getValue(visible_backlight);

  setVisibleBacklightOff(visible_backlight);
}

void LickportArrayController::toggleVisibleBacklightHandler()
{
  long visible_backlight;
  modular_server_.parameter(constants::visible_backlight_parameter_name).getValue(visible_backlight);

  toggleVisibleBacklight(visible_backlight);
}

void LickportArrayController::getVisibleBacklightPowersWhenOnHandler()
{
  modular_server_.response().writeResultKey();
  modular_server_.response().beginArray();
  double power;
  for (size_t visible_backlight=0; visible_backlight<getVisibleBacklightCount(); ++visible_backlight)
  {
    power = getVisibleBacklightPowerWhenOn(visible_backlight);
    modular_server_.response().write(power);
  }
  modular_server_.response().endArray();
}

void LickportArrayController::getVisibleBacklightIntensitiesWhenOnHandler()
{
  modular_server_.response().writeResultKey();
  modular_server_.response().beginArray();
  double intensity;
  for (size_t visible_backlight=0; visible_backlight<getVisibleBacklightCount(); ++visible_backlight)
  {
    intensity = getVisibleBacklightIntensityWhenOn(visible_backlight);
    modular_server_.response().write(intensity);
  }
  modular_server_.response().endArray();
}

void LickportArrayController::getVisibleBacklightPowersHandler()
{
  modular_server_.response().writeResultKey();
  modular_server_.response().beginArray();
  double power;
  for (size_t visible_backlight=0; visible_backlight<getVisibleBacklightCount(); ++visible_backlight)
  {
    power = getVisibleBacklightPower(visible_backlight);
    modular_server_.response().write(power);
  }
  modular_server_.response().endArray();
}

void LickportArrayController::getVisibleBacklightIntensitiesHandler()
{
  modular_server_.response().writeResultKey();
  modular_server_.response().beginArray();
  double intensity;
  for (size_t visible_backlight=0; visible_backlight<getVisibleBacklightCount(); ++visible_backlight)
  {
    intensity = getVisibleBacklightIntensity(visible_backlight);
    modular_server_.response().write(intensity);
  }
  modular_server_.response().endArray();
}

void LickportArrayController::getVisibleBacklightPowerBoundsHandler()
{
  modular_server_.response().writeResultKey();
  modular_server_.response().beginArray();
  for (size_t visible_backlight=0; visible_backlight<getVisibleBacklightCount(); ++visible_backlight)
  {
    modular_server_.response().beginObject();

    double power_lower_bound = getPowerLowerBound(visibleBacklightToDigitalChannel(visible_backlight));
    modular_server_.response().write(digital_controller::constants::power_lower_bound_string,power_lower_bound);

    double power_upper_bound = getPowerUpperBound(visibleBacklightToDigitalChannel(visible_backlight));
    modular_server_.response().write(digital_controller::constants::power_upper_bound_string,power_upper_bound);

    modular_server_.response().endObject();
  }
  modular_server_.response().endArray();
}

void LickportArrayController::getVisibleBacklightIntensityBoundsHandler()
{
  modular_server_.response().writeResultKey();
  modular_server_.response().beginArray();
  for (size_t visible_backlight=0; visible_backlight<getVisibleBacklightCount(); ++visible_backlight)
  {
    modular_server_.response().beginObject();

    double intensity_lower_bound = getVisibleBacklightIntensityLowerBound(visible_backlight);
    modular_server_.response().write(constants::intensity_lower_bound_string,intensity_lower_bound);

    double intensity_upper_bound = getVisibleBacklightIntensityUpperBound(visible_backlight);
    modular_server_.response().write(constants::intensity_upper_bound_string,intensity_upper_bound);

    modular_server_.response().endObject();
  }
  modular_server_.response().endArray();
}

void LickportArrayController::visibleBacklightPowerToIntensitiesHandler()
{
  double power;
  modular_server_.parameter(digital_controller::constants::power_parameter_name).getValue(power);

  modular_server_.response().writeResultKey();
  modular_server_.response().beginArray();
  double intensity;
  for (size_t visible_backlight=0; visible_backlight<getVisibleBacklightCount(); ++visible_backlight)
  {
    intensity = visibleBacklightPowerToIntensity(visible_backlight,power);
    modular_server_.response().write(intensity);
  }
  modular_server_.response().endArray();
}

void LickportArrayController::visibleBacklightIntensityToPowersHandler()
{
  double intensity;
  modular_server_.parameter(constants::intensity_parameter_name).getValue(intensity);

  modular_server_.response().writeResultKey();
  modular_server_.response().beginArray();
  double power;
  for (size_t visible_backlight=0; visible_backlight<getVisibleBacklightCount(); ++visible_backlight)
  {
    power = visibleBacklightIntensityToPower(visible_backlight,intensity);
    modular_server_.response().write(power);
  }
  modular_server_.response().endArray();
}

void LickportArrayController::setAllHighVoltagesOnAtPowerHandler()
{
  double power;
  modular_server_.parameter(digital_controller::constants::power_parameter_name).getValue(power);

  setAllHighVoltagesOnAtPower(power);
}

void LickportArrayController::setAllHighVoltagesOnHandler(modular_server::Pin * pin_ptr)
{
  setAllHighVoltagesOn();
}

void LickportArrayController::setAllHighVoltagesOffHandler(modular_server::Pin * pin_ptr)
{
  setAllHighVoltagesOff();
}

void LickportArrayController::toggleAllHighVoltagesHandler(modular_server::Pin * pin_ptr)
{
  toggleAllHighVoltages();
}

void LickportArrayController::setHighVoltageOnHandler()
{
  long high_voltage;
  modular_server_.parameter(constants::high_voltage_parameter_name).getValue(high_voltage);

  setHighVoltageOn(high_voltage);
}

void LickportArrayController::setHighVoltageOnAtPowerHandler()
{
  long high_voltage;
  modular_server_.parameter(constants::high_voltage_parameter_name).getValue(high_voltage);

  double power;
  modular_server_.parameter(digital_controller::constants::power_parameter_name).getValue(power);

  setHighVoltageOnAtPower(high_voltage,power);
}

void LickportArrayController::setHighVoltageOffHandler()
{
  long high_voltage;
  modular_server_.parameter(constants::high_voltage_parameter_name).getValue(high_voltage);

  setHighVoltageOff(high_voltage);
}

void LickportArrayController::toggleHighVoltageHandler()
{
  long high_voltage;
  modular_server_.parameter(constants::high_voltage_parameter_name).getValue(high_voltage);

  toggleHighVoltage(high_voltage);
}

void LickportArrayController::getHighVoltagePowersWhenOnHandler()
{
  modular_server_.response().writeResultKey();
  modular_server_.response().beginArray();
  double power;
  for (size_t high_voltage=0; high_voltage<getHighVoltageCount(); ++high_voltage)
  {
    power = getHighVoltagePowerWhenOn(high_voltage);
    modular_server_.response().write(power);
  }
  modular_server_.response().endArray();
}

void LickportArrayController::getHighVoltagePowersHandler()
{
  modular_server_.response().writeResultKey();
  modular_server_.response().beginArray();
  double power;
  for (size_t high_voltage=0; high_voltage<getHighVoltageCount(); ++high_voltage)
  {
    power = getHighVoltagePower(high_voltage);
    modular_server_.response().write(power);
  }
  modular_server_.response().endArray();
}

void LickportArrayController::getHighVoltagePowerBoundsHandler()
{
  modular_server_.response().writeResultKey();
  modular_server_.response().beginArray();
  for (size_t high_voltage=0; high_voltage<getHighVoltageCount(); ++high_voltage)
  {
    modular_server_.response().beginObject();

    double power_lower_bound = getPowerLowerBound(highVoltageToDigitalChannel(high_voltage));
    modular_server_.response().write(digital_controller::constants::power_lower_bound_string,power_lower_bound);

    double power_upper_bound = getPowerUpperBound(highVoltageToDigitalChannel(high_voltage));
    modular_server_.response().write(digital_controller::constants::power_upper_bound_string,power_upper_bound);

    modular_server_.response().endObject();
  }
  modular_server_.response().endArray();
}

void LickportArrayController::setAllLowVoltagesOnAtPowerHandler()
{
  double power;
  modular_server_.parameter(digital_controller::constants::power_parameter_name).getValue(power);

  setAllLowVoltagesOnAtPower(power);
}

void LickportArrayController::setAllLowVoltagesOnHandler(modular_server::Pin * pin_ptr)
{
  setAllLowVoltagesOn();
}

void LickportArrayController::setAllLowVoltagesOffHandler(modular_server::Pin * pin_ptr)
{
  setAllLowVoltagesOff();
}

void LickportArrayController::toggleAllLowVoltagesHandler(modular_server::Pin * pin_ptr)
{
  toggleAllLowVoltages();
}

void LickportArrayController::setLowVoltageOnHandler()
{
  long low_voltage;
  modular_server_.parameter(constants::low_voltage_parameter_name).getValue(low_voltage);

  setLowVoltageOn(low_voltage);
}

void LickportArrayController::setLowVoltageOnAtPowerHandler()
{
  long low_voltage;
  modular_server_.parameter(constants::low_voltage_parameter_name).getValue(low_voltage);

  double power;
  modular_server_.parameter(digital_controller::constants::power_parameter_name).getValue(power);

  setLowVoltageOnAtPower(low_voltage,power);
}

void LickportArrayController::setLowVoltageOffHandler()
{
  long low_voltage;
  modular_server_.parameter(constants::low_voltage_parameter_name).getValue(low_voltage);

  setLowVoltageOff(low_voltage);
}

void LickportArrayController::toggleLowVoltageHandler()
{
  long low_voltage;
  modular_server_.parameter(constants::low_voltage_parameter_name).getValue(low_voltage);

  toggleLowVoltage(low_voltage);
}

void LickportArrayController::getLowVoltagePowersWhenOnHandler()
{
  modular_server_.response().writeResultKey();
  modular_server_.response().beginArray();
  double power;
  for (size_t low_voltage=0; low_voltage<getLowVoltageCount(); ++low_voltage)
  {
    power = getLowVoltagePowerWhenOn(low_voltage);
    modular_server_.response().write(power);
  }
  modular_server_.response().endArray();
}

void LickportArrayController::getLowVoltagePowersHandler()
{
  modular_server_.response().writeResultKey();
  modular_server_.response().beginArray();
  double power;
  for (size_t low_voltage=0; low_voltage<getLowVoltageCount(); ++low_voltage)
  {
    power = getLowVoltagePower(low_voltage);
    modular_server_.response().write(power);
  }
  modular_server_.response().endArray();
}

void LickportArrayController::getLowVoltagePowerBoundsHandler()
{
  modular_server_.response().writeResultKey();
  modular_server_.response().beginArray();
  for (size_t low_voltage=0; low_voltage<getLowVoltageCount(); ++low_voltage)
  {
    modular_server_.response().beginObject();

    double power_lower_bound = getPowerLowerBound(lowVoltageToDigitalChannel(low_voltage));
    modular_server_.response().write(digital_controller::constants::power_lower_bound_string,power_lower_bound);

    double power_upper_bound = getPowerUpperBound(lowVoltageToDigitalChannel(low_voltage));
    modular_server_.response().write(digital_controller::constants::power_upper_bound_string,power_upper_bound);

    modular_server_.response().endObject();
  }
  modular_server_.response().endArray();
}

void LickportArrayController::toggleEnableAllHandler(modular_server::Pin * pin_ptr)
{
  toggleEnableAll();
}
