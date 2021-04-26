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

  // Pin Setup
  pinMode(constants::sensor_pin,INPUT_PULLUP);
  pinMode(constants::led_pin,OUTPUT);

  // Pins

  // Set Device ID
  modular_server_.setDeviceName(constants::device_name);

  // Add Hardware
  modular_server_.addHardware(constants::hardware_info,
    pins_);

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

}

void LickportArrayController::update()
{
  // Parent Update
  ModularDeviceBase::update();
}
