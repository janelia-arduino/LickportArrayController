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

LickportArrayController::~LickportArrayController()
{
}

void LickportArrayController::setup()
{
  // Parent Setup
  ModularDeviceBase::setup();

  // Reset Watchdog
  resetWatchdog();

  // Variable Setup

  // Event Controller Setup
  event_controller_.setup();

  // Set Device ID
  modular_server_.setDeviceName(constants::device_name);

  // Add Hardware
  modular_server_.addHardware(constants::hardware_info,
    pins_);

  // Pins
  modular_server::Pin & input_pin = modular_server_.createPin(constants::input_pin_name,
    constants::input_pin_number);
  input_pin.setModeDigitalInputPullup();

  modular_server::Pin & output_pin = modular_server_.createPin(constants::output_pin_name,
    constants::output_pin_number);
  output_pin.setModeDigitalOutput();
  output_pin.setValue(LOW);

  // Add Firmware
  modular_server_.addFirmware(constants::firmware_info,
    properties_,
    parameters_,
    functions_,
    callbacks_);

  // Properties

  // Parameters

  // Functions

  // Callbacks
  modular_server::Callback & trigger_output_callback = modular_server_.createCallback(constants::trigger_output_callback_name);
  trigger_output_callback.attachFunctor(makeFunctor((Functor1<modular_server::Pin *> *)0,*this,&LickportArrayController::triggerOutputHandler));
  trigger_output_callback.attachTo(input_pin,modular_server::constants::pin_mode_interrupt_falling);

}

void LickportArrayController::update()
{
  // Parent Update
  ModularDeviceBase::update();
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
// modular_server_.property(property_name).setValue(value) value type must match the property default type
// modular_server_.property(property_name).getElementValue(element_index,value) value type must match the property array element default type
// modular_server_.property(property_name).setElementValue(element_index,value) value type must match the property array element default type

void LickportArrayController::triggerOutputHandler(modular_server::Pin * pin_ptr)
{
  modular_server::Callback & trigger_output_callback = modular_server_.callback(constants::trigger_output_callback_name);
  trigger_output_callback.detachFrom(*pin_ptr);

  modular_server::Pin & output_pin = modular_server_.pin(constants::output_pin_name);
  output_pin.setValue(HIGH);

  EventId end_output_event_id = event_controller_.addEventUsingDelay(makeFunctor((Functor1<int> *)0,*this,&LickportArrayController::endOutputHandler),
    constants::end_output_delay);
  event_controller_.enable(end_output_event_id);
}

void LickportArrayController::endOutputHandler(int output_index)
{
  modular_server::Pin & output_pin = modular_server_.pin(constants::output_pin_name);
  output_pin.setValue(LOW);

  modular_server::Pin & input_pin = modular_server_.pin(constants::input_pin_name);
  modular_server::Callback & trigger_output_callback = modular_server_.callback(constants::trigger_output_callback_name);
  trigger_output_callback.attachTo(input_pin,modular_server::constants::pin_mode_interrupt_falling);
}
