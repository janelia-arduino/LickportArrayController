// ----------------------------------------------------------------------------
// LickportArrayController.cpp
//
//
// Authors:
// Peter Polidoro peter@polidoro.io
// ----------------------------------------------------------------------------
#include "../LickportArrayController.h"


using namespace lickport_array_controller;

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
  for (Channel channel=0; channel<constants::CHANNEL_COUNT; ++channel)
  {
    pinMode(constants::channel_pins[channel],OUTPUT);
  }

  // Add Hardware
  modular_server_.addHardware(constants::hardware_info,
    pins_);

  // Pins
  modular_server::Pin & change_pin = modular_server_.createPin(constants::change_pin_name,
    constants::change_pin_number);
  change_pin.setModeDigitalInput();

  modular_server::Pin & lick_detected_pin = modular_server_.createPin(constants::lick_detected_pin_name,
    constants::lick_detected_pin_number);
  lick_detected_pin.setModePulseRising();

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

  modular_server::Property & sync_on_duration_property = modular_server_.createProperty(constants::sync_on_duration_property_name,constants::sync_on_duration_default);
  sync_on_duration_property.setRange(constants::duration_min,constants::duration_max);
  sync_on_duration_property.attachPostSetValueFunctor(makeFunctor((Functor0 *)0,*this,&LickportArrayController::setSyncPropertyHandler));

  modular_server::Property & sync_period_min_property = modular_server_.createProperty(constants::sync_period_min_property_name,constants::sync_period_min_default);
  sync_period_min_property.setRange(constants::duration_min,constants::duration_max);
  sync_period_min_property.attachPostSetValueFunctor(makeFunctor((Functor0 *)0,*this,&LickportArrayController::setSyncPropertyHandler));

  modular_server::Property & sync_period_max_property = modular_server_.createProperty(constants::sync_period_max_property_name,constants::sync_period_max_default);
  sync_period_max_property.setRange(constants::duration_min,constants::duration_max);
  sync_period_max_property.attachPostSetValueFunctor(makeFunctor((Functor0 *)0,*this,&LickportArrayController::setSyncPropertyHandler));

  modular_server::Property & dispense_delays_property = modular_server_.createProperty(constants::dispense_delays_property_name,constants::dispense_delays_default);
  dispense_delays_property.setRange(constants::dispense_delay_min,constants::dispense_delay_max);
  dispense_delays_property.setArrayLengthRange(constants::LICKPORT_COUNT,constants::LICKPORT_COUNT);

  modular_server::Property & dispense_periods_property = modular_server_.createProperty(constants::dispense_periods_property_name,constants::dispense_periods_default);
  dispense_periods_property.setRange(constants::dispense_period_min,constants::dispense_period_max);
  dispense_periods_property.setArrayLengthRange(constants::LICKPORT_COUNT,constants::LICKPORT_COUNT);

  modular_server::Property & dispense_counts_property = modular_server_.createProperty(constants::dispense_counts_property_name,constants::dispense_counts_default);
  dispense_counts_property.setRange(constants::dispense_count_min,constants::dispense_count_max);
  dispense_counts_property.setArrayLengthRange(constants::LICKPORT_COUNT,constants::LICKPORT_COUNT);

  modular_server::Property & activated_dispense_durations_property = modular_server_.createProperty(constants::activated_dispense_durations_property_name,constants::activated_dispense_durations_default);
  activated_dispense_durations_property.setRange(constants::duration_min,constants::duration_max);
  activated_dispense_durations_property.setArrayLengthRange(constants::LICKPORT_COUNT,constants::LICKPORT_COUNT);

  modular_server::Property & all_lickports_activated_initially_property = modular_server_.createProperty(constants::all_lickports_activated_initially_property_name,constants::all_lickports_activated_initially_default);

  // Parameters
  modular_server::Parameter & lickport_parameter = modular_server_.createParameter(constants::lickport_parameter_name);
  lickport_parameter.setRange(constants::lickport_min,constants::lickport_count-1);

  modular_server::Parameter & lickports_parameter = modular_server_.createParameter(constants::lickports_parameter_name);
  lickports_parameter.setRange(constants::lickport_min,constants::lickport_count-1);
  lickports_parameter.setArrayLengthRange(constants::lickports_array_length_min,constants::lickport_count);

  modular_server::Parameter & dispense_duration_parameter = modular_server_.createParameter(constants::dispense_duration_parameter_name);
  dispense_duration_parameter.setRange(constants::duration_min,constants::duration_max);
  dispense_duration_parameter.setUnits(digital_controller::constants::ms_units);

  modular_server::Parameter & dispense_durations_parameter = modular_server_.createParameter(constants::dispense_durations_parameter_name);
  dispense_durations_parameter.setArrayLengthRange(constants::dispense_durations_array_length_min,constants::lickport_count);

  setChannelCountHandler();

  // Functions
  modular_server::Function & get_lickport_count_function = modular_server_.createFunction(constants::get_lickport_count_function_name);
  get_lickport_count_function.attachFunctor(makeFunctor((Functor0 *)0,*this,&LickportArrayController::getLickportCountHandler));
  get_lickport_count_function.setResultTypeLong();

  modular_server::Function & dispense_lickport_for_duration_function = modular_server_.createFunction(constants::dispense_lickport_for_duration_function_name);
  dispense_lickport_for_duration_function.attachFunctor(makeFunctor((Functor0 *)0,*this,&LickportArrayController::dispenseLickportForDurationHandler));
  dispense_lickport_for_duration_function.addParameter(lickport_parameter);
  dispense_lickport_for_duration_function.addParameter(dispense_duration_parameter);

  modular_server::Function & dispense_lickports_for_duration_function = modular_server_.createFunction(constants::dispense_lickports_for_duration_function_name);
  dispense_lickports_for_duration_function.attachFunctor(makeFunctor((Functor0 *)0,*this,&LickportArrayController::dispenseLickportsForDurationHandler));
  dispense_lickports_for_duration_function.addParameter(lickports_parameter);
  dispense_lickports_for_duration_function.addParameter(dispense_duration_parameter);

  modular_server::Function & dispense_all_lickports_for_duration_function = modular_server_.createFunction(constants::dispense_all_lickports_for_duration_function_name);
  dispense_all_lickports_for_duration_function.attachFunctor(makeFunctor((Functor0 *)0,*this,&LickportArrayController::dispenseAllLickportsForDurationHandler));
  dispense_all_lickports_for_duration_function.addParameter(dispense_duration_parameter);

  modular_server::Function & dispense_lickports_for_durations_function = modular_server_.createFunction(constants::dispense_lickports_for_durations_function_name);
  dispense_lickports_for_durations_function.attachFunctor(makeFunctor((Functor0 *)0,*this,&LickportArrayController::dispenseLickportsForDurationsHandler));
  dispense_lickports_for_durations_function.addParameter(lickports_parameter);
  dispense_lickports_for_durations_function.addParameter(dispense_durations_parameter);

  modular_server::Function & get_activated_lickports_function = modular_server_.createFunction(constants::get_activated_lickports_function_name);
  get_activated_lickports_function.attachFunctor(makeFunctor((Functor0 *)0,*this,&LickportArrayController::getActivatedLickportsHandler));
  get_activated_lickports_function.setResultTypeLong();
  get_activated_lickports_function.setResultTypeArray();

  modular_server::Function & activate_only_lickport_function = modular_server_.createFunction(constants::activate_only_lickport_function_name);
  activate_only_lickport_function.attachFunctor(makeFunctor((Functor0 *)0,*this,&LickportArrayController::activateOnlyLickportHandler));
  activate_only_lickport_function.addParameter(lickport_parameter);
  activate_only_lickport_function.setResultTypeLong();
  activate_only_lickport_function.setResultTypeArray();

  modular_server::Function & activate_only_lickports_function = modular_server_.createFunction(constants::activate_only_lickports_function_name);
  activate_only_lickports_function.attachFunctor(makeFunctor((Functor0 *)0,*this,&LickportArrayController::activateOnlyLickportsHandler));
  activate_only_lickports_function.addParameter(lickports_parameter);
  activate_only_lickports_function.setResultTypeLong();
  activate_only_lickports_function.setResultTypeArray();

  modular_server::Function & activate_lickport_function = modular_server_.createFunction(constants::activate_lickport_function_name);
  activate_lickport_function.attachFunctor(makeFunctor((Functor0 *)0,*this,&LickportArrayController::activateLickportHandler));
  activate_lickport_function.addParameter(lickport_parameter);
  activate_lickport_function.setResultTypeLong();
  activate_lickport_function.setResultTypeArray();

  modular_server::Function & activate_lickports_function = modular_server_.createFunction(constants::activate_lickports_function_name);
  activate_lickports_function.attachFunctor(makeFunctor((Functor0 *)0,*this,&LickportArrayController::activateLickportsHandler));
  activate_lickports_function.addParameter(lickports_parameter);
  activate_lickports_function.setResultTypeLong();
  activate_lickports_function.setResultTypeArray();

  modular_server::Function & deactivate_lickport_function = modular_server_.createFunction(constants::deactivate_lickport_function_name);
  deactivate_lickport_function.attachFunctor(makeFunctor((Functor0 *)0,*this,&LickportArrayController::deactivateLickportHandler));
  deactivate_lickport_function.addParameter(lickport_parameter);
  deactivate_lickport_function.setResultTypeLong();
  deactivate_lickport_function.setResultTypeArray();

  modular_server::Function & deactivate_lickports_function = modular_server_.createFunction(constants::deactivate_lickports_function_name);
  deactivate_lickports_function.attachFunctor(makeFunctor((Functor0 *)0,*this,&LickportArrayController::deactivateLickportsHandler));
  deactivate_lickports_function.addParameter(lickports_parameter);
  deactivate_lickports_function.setResultTypeLong();
  deactivate_lickports_function.setResultTypeArray();

  modular_server::Function & get_and_clear_saved_data_function = modular_server_.createFunction(constants::get_and_clear_saved_data_function_name);
  get_and_clear_saved_data_function.attachFunctor(makeFunctor((Functor0 *)0,*this,&LickportArrayController::getAndClearSavedDataHandler));
  get_and_clear_saved_data_function.setResultTypeObject();
  get_and_clear_saved_data_function.setResultTypeArray();

  // Callbacks
  modular_server::Callback & calibrate_lick_sensor_callback = modular_server_.createCallback(constants::calibrate_lick_sensor_callback_name);
  calibrate_lick_sensor_callback.attachFunctor(makeFunctor((Functor1<modular_server::Pin *> *)0,*this,&LickportArrayController::calibrateLickSensorHandler));

  modular_server::Callback & manage_lick_status_change_callback = modular_server_.createCallback(constants::manage_lick_status_change_callback_name);
  manage_lick_status_change_callback.attachFunctor(makeFunctor((Functor1<modular_server::Pin *> *)0,*this,&LickportArrayController::manageLickStatusChangeHandler));
  manage_lick_status_change_callback.attachTo(change_pin,modular_server::constants::pin_mode_interrupt_falling);

  modular_server::Callback & activate_all_lickports_callback = modular_server_.createCallback(constants::activate_all_lickports_callback_name);
  activate_all_lickports_callback.attachFunctor(makeFunctor((Functor1<modular_server::Pin *> *)0,*this,&LickportArrayController::activateAllLickportsHandler));

  modular_server::Callback & deactivate_all_lickports_callback = modular_server_.createCallback(constants::deactivate_all_lickports_callback_name);
  deactivate_all_lickports_callback.attachFunctor(makeFunctor((Functor1<modular_server::Pin *> *)0,*this,&LickportArrayController::deactivateAllLickportsHandler));

  setAllChannelsOff();
  startSyncPwm();

  lick_sensor_.begin();
  lick_sensor_.reset();
  manage_lick_status_change_ = false;
  lickports_dispensing_ = 0;

  bool all_lickports_activated_initially;
  all_lickports_activated_initially_property.getValue(all_lickports_activated_initially);
  if (all_lickports_activated_initially)
  {
    activateAllLickports();
  }
  else
  {
    deactivateAllLickports();
  }
}

void LickportArrayController::update()
{
  // Parent Update
  DigitalController::update();

  manageLickStatusChange();
}

uint16_t LickportArrayController::getLickportCount()
{
  return constants::LICKPORT_COUNT;
}

void LickportArrayController::calibrateLickSensor()
{
  lick_sensor_.triggerCalibration();
}

void LickportArrayController::dispenseLickportForDuration(Lickport lickport,
  Duration duration)
{
  long delay;
  modular_server_.property(constants::dispense_delays_property_name).getElementValue(lickport,delay);

  long period;
  modular_server_.property(constants::dispense_periods_property_name).getElementValue(lickport,period);

  long count;
  modular_server_.property(constants::dispense_counts_property_name).getElementValue(lickport,count);

  dispense(lickport,delay,period,duration,count);
}

void LickportArrayController::dispenseLickportsForDuration(const Lickports & lickports,
  Duration duration)
{
  for (Lickport lickport : lickports)
  {
    dispenseLickportForDuration(lickport,duration);
  }
}

void LickportArrayController::dispenseAllLickportsForDuration(Duration duration)
{
  for (Lickport lickport=0; lickport<constants::lickport_count; ++lickport)
  {
    dispenseLickportForDuration(lickport,duration);
  }
}

void LickportArrayController::dispenseLickportsForDurations(const Lickports & lickports,
  const Durations & durations)
{
  if (lickports.size() != durations.size())
  {
    return;
  }
  for (uint8_t index=0; index<lickports.size(); ++index)
  {
    dispenseLickportForDuration(lickports[index],durations[index]);
  }
}

LickportArrayController::Lickports LickportArrayController::getActivatedLickports()
{
  Lickports lickports;
  for (Lickport lickport=0; lickport < constants::LICKPORT_COUNT; ++lickport)
  {
    if (bitRead(lickports_activated_,lickport))
    {
      lickports.push_back(lickport);
    }
  }
  return lickports;
}

void LickportArrayController::activateAllLickports()
{
  lickports_activated_ = ~0;
}

void LickportArrayController::deactivateAllLickports()
{
  lickports_activated_ = 0;
}

void LickportArrayController::activateOnlyLickport(Lickport lickport)
{
  lickports_activated_ = 1 << lickport;
}

void LickportArrayController::activateOnlyLickports(Lickports lickports)
{
  lickports_activated_ = 0;
  for (Lickport lickport : lickports)
  {
    activateLickport(lickport);
  }
}

void LickportArrayController::activateLickport(Lickport lickport)
{
  lickports_activated_ |= 1 << lickport;
}

void LickportArrayController::activateLickports(Lickports lickports)
{
  for (Lickport lickport : lickports)
  {
    activateLickport(lickport);
  }
}

void LickportArrayController::deactivateLickport(Lickport lickport)
{
  lickports_activated_ &= ~(1 << lickport);
}

void LickportArrayController::deactivateLickports(Lickports lickports)
{
  for (Lickport lickport : lickports)
  {
    deactivateLickport(lickport);
  }
}

LickportArrayController::SavedData LickportArrayController::getSavedData()
{
  return saved_data_;
}

double LickportArrayController::setChannelToPower(size_t lickport,
  double power)
{
  if (power > digital_controller::constants::power_mid)
  {
    power = digital_controller::constants::power_max;
    digitalWrite(constants::channel_pins[lickport],HIGH);
  }
  else
  {
    power = digital_controller::constants::power_min;
    digitalWrite(constants::channel_pins[lickport],LOW);
  }
  return power;
}

void LickportArrayController::startSyncPwm()
{
  Channels channels = (1 << constants::sync_channel);

  long period_min;
  modular_server::Property & sync_period_min_property = modular_server_.property(constants::sync_period_min_property_name);
  sync_period_min_property.getValue(period_min);

  long period_max;
  modular_server::Property & sync_period_max_property = modular_server_.property(constants::sync_period_max_property_name);
  sync_period_max_property.getValue(period_max);

  long period = random(period_min,period_max) - constants::sync_delay;

  long on_duration;
  modular_server::Property & sync_on_duration_property = modular_server_.property(constants::sync_on_duration_property_name);
  sync_on_duration_property.getValue(on_duration);

  sync_pwm_id_ = addPwm(channels,
    constants::dispense_power,
    constants::sync_delay,
    period,
    on_duration,
    constants::sync_count,
    makeFunctor((Functor1<int> *)0,*this,&DigitalController::startPulseHandler),
    makeFunctor((Functor1<int> *)0,*this,&DigitalController::stopPulseHandler),
    makeFunctor((Functor1<int> *)0,*this,&DigitalController::startPwmHandler),
    makeFunctor((Functor1<int> *)0,*this,&LickportArrayController::stopSyncPwmHandler));
}

void LickportArrayController::stopSyncPwm()
{
  stopPwm(sync_pwm_id_);
}

void LickportArrayController::dispense(Lickport lickport,
  Duration delay,
  Duration period,
  Duration duration,
  Count count)
{
  if (lickportDispensing(lickport))
  {
    return;
  }
  Channels lickport_channels = 0;
  lickport_channels |= (1 << lickport);
  addPwm(lickport_channels,
    constants::dispense_power,
    delay,
    period,
    duration,
    count,
    makeFunctor((Functor1<int> *)0,*this,&DigitalController::startPulseHandler),
    makeFunctor((Functor1<int> *)0,*this,&DigitalController::stopPulseHandler),
    makeFunctor((Functor1<int> *)0,*this,&LickportArrayController::startLickportPwmHandler),
    makeFunctor((Functor1<int> *)0,*this,&LickportArrayController::stopLickportPwmHandler));
}

void LickportArrayController::manageLickStatusChange()
{
  if (manage_lick_status_change_)
  {
    manage_lick_status_change_ = false;
    LickSensorStatus lick_sensor_status = lick_sensor_.getStatus();
    if (lick_sensor_status.any_key_touched)
    {
      updateSavedData(lick_sensor_status);
      modular_server::Pin & lick_detected_pin = modular_server_.pin(constants::lick_detected_pin_name);
      lick_detected_pin.setValue(constants::lick_detected_pulse_duration);

      for (Lickport lickport=0; lickport<constants::LICKPORT_COUNT; ++lickport)
      {
        if ((lick_sensor_.touched(lick_sensor_status,lickport)) &&
          (lickportActivated(lickport)))
        {
          long duration;
          modular_server_.property(constants::activated_dispense_durations_property_name).getElementValue(lickport,duration);

          dispenseLickportForDuration(lickport,duration);
        }
      }
    }
  }
}

bool LickportArrayController::lickportTrueInBitArray(Lickport lickport,
  BitArray bit_array)
{
  return bitRead(bit_array,lickport);
}

bool LickportArrayController::lickportDispensing(Lickport lickport)
{
  return lickportTrueInBitArray(lickport,lickports_dispensing_);
}

bool LickportArrayController::lickportActivated(Lickport lickport)
{
  return lickportTrueInBitArray(lickport,lickports_activated_);
}

void LickportArrayController::updateSavedData(LickSensorStatus lick_sensor_status)
{
  SavedDatum saved_datum;
  if (timeIsSet())
  {
    saved_datum.time = getTime();
  }
  else
  {
    saved_datum.time = 0;
  }
  saved_datum.millis = millis();
  saved_datum.lickports_licked = lick_sensor_status.keys;
  saved_datum.lickports_activated = lickports_activated_;
  saved_data_.push_back(saved_datum);
}

void LickportArrayController::clearSavedData()
{
  saved_data_.clear();
}

void LickportArrayController::setSyncPropertyHandler()
{
  stopAllPwm();
  startSyncPwm();
}

void LickportArrayController::stopSyncPwmHandler(int pwm_index)
{
  DigitalController::stopPwmHandler(pwm_index);
  startSyncPwm();
}

void LickportArrayController::startLickportPwmHandler(int pwm_index)
{
  DigitalController::startPwmHandler(pwm_index);
  lickports_dispensing_ |= getPwmChannels(pwm_index);
}

void LickportArrayController::stopLickportPwmHandler(int pwm_index)
{
  DigitalController::stopPwmHandler(pwm_index);
  lickports_dispensing_ &= ~getPwmChannels(pwm_index);
}

void LickportArrayController::getLickportCountHandler()
{
  uint16_t lickport_count = getLickportCount();

  modular_server_.response().returnResult(lickport_count);
}

void LickportArrayController::dispenseLickportForDurationHandler()
{
  Lickport lickport;
  modular_server_.parameter(constants::lickport_parameter_name).getValue(lickport);

  Duration duration;
  modular_server_.parameter(constants::dispense_duration_parameter_name).getValue(duration);

  dispenseLickportForDuration(lickport,duration);
}

void LickportArrayController::dispenseLickportsForDurationHandler()
{
  Lickports lickports;
  modular_server_.parameter(constants::lickports_parameter_name).getValue(lickports);

  Duration duration;
  modular_server_.parameter(constants::dispense_duration_parameter_name).getValue(duration);

  dispenseLickportsForDuration(lickports,duration);
}

void LickportArrayController::dispenseLickportsForDurationsHandler()
{
  Lickports lickports;
  modular_server_.parameter(constants::lickports_parameter_name).getValue(lickports);

  Durations durations;
  modular_server_.parameter(constants::dispense_durations_parameter_name).getValue(durations);

  dispenseLickportsForDurations(lickports,durations);
}

void LickportArrayController::dispenseAllLickportsForDurationHandler()
{
  Duration duration;
  modular_server_.parameter(constants::dispense_duration_parameter_name).getValue(duration);

  dispenseAllLickportsForDuration(duration);
}

void LickportArrayController::getActivatedLickportsHandler()
{
  Lickports lickports = getActivatedLickports();

  modular_server_.response().returnResult(lickports);
}

void LickportArrayController::activateOnlyLickportHandler()
{
  Lickport lickport;
  modular_server_.parameter(constants::lickport_parameter_name).getValue(lickport);

  activateOnlyLickport(lickport);

  Lickports lickports = getActivatedLickports();

  modular_server_.response().returnResult(lickports);
}

void LickportArrayController::activateOnlyLickportsHandler()
{
  Lickports lickports;
  modular_server_.parameter(constants::lickports_parameter_name).getValue(lickports);

  activateOnlyLickports(lickports);

  lickports = getActivatedLickports();

  modular_server_.response().returnResult(lickports);
}

void LickportArrayController::activateLickportHandler()
{
  Lickport lickport;
  modular_server_.parameter(constants::lickport_parameter_name).getValue(lickport);

  activateLickport(lickport);

  Lickports lickports = getActivatedLickports();

  modular_server_.response().returnResult(lickports);
}

void LickportArrayController::activateLickportsHandler()
{
  Lickports lickports;
  modular_server_.parameter(constants::lickports_parameter_name).getValue(lickports);

  activateLickports(lickports);

  lickports = getActivatedLickports();

  modular_server_.response().returnResult(lickports);
}

void LickportArrayController::deactivateLickportHandler()
{
  Lickport lickport;
  modular_server_.parameter(constants::lickport_parameter_name).getValue(lickport);

  deactivateLickport(lickport);

  Lickports lickports = getActivatedLickports();

  modular_server_.response().returnResult(lickports);
}

void LickportArrayController::deactivateLickportsHandler()
{
  Lickports lickports;
  modular_server_.parameter(constants::lickports_parameter_name).getValue(lickports);

  deactivateLickports(lickports);

  lickports = getActivatedLickports();

  modular_server_.response().returnResult(lickports);
}

void LickportArrayController::getAndClearSavedDataHandler()
{
  SavedData saved_data = getSavedData();

  modular_server_.response().writeResultKey();

  modular_server_.response().beginArray();

  for (SavedDatum saved_datum : saved_data)
  {
    modular_server_.response().beginObject();

    modular_server_.response().write(constants::saved_datum_time,saved_datum.time);
    modular_server_.response().write(constants::saved_datum_millis,saved_datum.millis);

    modular_server_.response().writeKey(constants::saved_datum_lickports_licked);
    modular_server_.response().beginArray();
    for (Lickport lickport=0; lickport<constants::LICKPORT_COUNT; ++lickport)
    {
      if (lickportTrueInBitArray(lickport,saved_datum.lickports_licked))
      {
        modular_server_.response().write(lickport);
      }
    }
    modular_server_.response().endArray();

    modular_server_.response().writeKey(constants::saved_datum_lickports_activated);
    modular_server_.response().beginArray();
    for (Lickport lickport=0; lickport<constants::LICKPORT_COUNT; ++lickport)
    {
      if (lickportTrueInBitArray(lickport,saved_datum.lickports_activated))
      {
        modular_server_.response().write(lickport);
      }
    }
    modular_server_.response().endArray();

    modular_server_.response().endObject();
  }

  modular_server_.response().endArray();

  clearSavedData();
}

void LickportArrayController::calibrateLickSensorHandler(modular_server::Pin * pin_ptr)
{
  calibrateLickSensor();
}

void LickportArrayController::manageLickStatusChangeHandler(modular_server::Pin * pin_ptr)
{
  manage_lick_status_change_ = true;
}

void LickportArrayController::activateAllLickportsHandler(modular_server::Pin * pin_ptr)
{
  activateAllLickports();
}

void LickportArrayController::deactivateAllLickportsHandler(modular_server::Pin * pin_ptr)
{
  deactivateAllLickports();
}

