// ----------------------------------------------------------------------------
// LickportArrayController.h
//
//
// Authors:
// Peter Polidoro peterpolidoro@gmail.com
// ----------------------------------------------------------------------------
#ifndef STEP_DIR_CONTROLLER_H
#define STEP_DIR_CONTROLLER_H
#include <ArduinoJson.h>
#include <JsonStream.h>
#include <Array.h>
#include <Vector.h>
#include <ConstantVariable.h>
#include <Functor.h>

#include <EventController.h>

#include <ModularServer.h>
#include <ModularDeviceBase.h>

#include "LickportArrayController/Constants.h"


class LickportArrayController : public ModularDeviceBase
{
public:
  LickportArrayController();
  ~LickportArrayController();
  virtual void setup();
  virtual void update();

private:
  modular_server::Pin pins_[lickport_array_controller::constants::PIN_COUNT_MAX];

  modular_server::Property properties_[lickport_array_controller::constants::PROPERTY_COUNT_MAX];
  modular_server::Parameter parameters_[lickport_array_controller::constants::PARAMETER_COUNT_MAX];
  modular_server::Function functions_[lickport_array_controller::constants::FUNCTION_COUNT_MAX];
  modular_server::Callback callbacks_[lickport_array_controller::constants::CALLBACK_COUNT_MAX];

  // Handlers
};

#endif
