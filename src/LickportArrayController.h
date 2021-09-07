// ----------------------------------------------------------------------------
// LickportArrayController.h
//
//
// Authors:
// Peter Polidoro peterpolidoro@gmail.com
// ----------------------------------------------------------------------------
#ifndef LICKPORT_ARRAY_CONTROLLER_H
#define LICKPORT_ARRAY_CONTROLLER_H
#include <ArduinoJson.h>
#include <JsonStream.h>
#include <Array.h>
#include <Vector.h>
#include <ConstantVariable.h>
#include <Functor.h>

#include <AT42QT2120.h>

#include <ModularServer.h>
#include <ModularDeviceBase.h>
#include <DigitalController.h>

#include "LickportArrayController/Constants.h"


class LickportArrayController : public DigitalController
{
public:
  LickportArrayController();
  virtual void setup();
  virtual void update();

protected:
  virtual double setChannelToPower(size_t channel,
    double power);

private:
  modular_server::Pin pins_[lickport_array_controller::constants::PIN_COUNT_MAX];

  modular_server::Property properties_[lickport_array_controller::constants::PROPERTY_COUNT_MAX];
  modular_server::Parameter parameters_[lickport_array_controller::constants::PARAMETER_COUNT_MAX];
  modular_server::Function functions_[lickport_array_controller::constants::FUNCTION_COUNT_MAX];
  modular_server::Callback callbacks_[lickport_array_controller::constants::CALLBACK_COUNT_MAX];

  typedef AT42QT2120 LickSensor;
  typedef AT42QT2120::Status LickSensorStatus;
  LickSensor lick_sensor_;
  volatile bool check_lick_status_;

  void checkLickStatus();

  // Handlers
  void checkLickStatusHandler(modular_server::Pin * pin_ptr);
};

#endif
