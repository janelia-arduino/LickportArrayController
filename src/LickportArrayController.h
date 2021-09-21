// ----------------------------------------------------------------------------
// LickportArrayController.h
//
//
// Authors:
// Peter Polidoro peter@polidoro.io
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
  typedef uint8_t Lickport;
  typedef uint32_t Duration;
  typedef uint8_t Count;
  typedef Array<Lickport,lickport_array_controller::constants::LICKPORT_COUNT> Lickports;
  typedef Array<Duration,lickport_array_controller::constants::LICKPORT_COUNT> Durations;

  LickportArrayController();
  virtual void setup();
  virtual void update();

  void dispenseLickportForDuration(Lickport lickport,
    Duration duration);
  void dispenseLickportsForDuration(const Lickports & lickports,
    Duration duration);
  void dispenseLickportsForDurations(const Lickports & lickports,
    const Durations & durations);
  void dispenseAllLickportsForDuration(Duration duration);

  Lickports getActivatedLickports();
  void activateOnlyLickport(Lickport lickport);
  void activateOnlyLickports(Lickports lickports);

protected:
  virtual double setChannelToPower(size_t lickport,
    double power);

  virtual void startPwmHandler(int pwm_index);
  virtual void stopPwmHandler(int pwm_index);

private:
  modular_server::Pin pins_[lickport_array_controller::constants::PIN_COUNT_MAX];

  modular_server::Property properties_[lickport_array_controller::constants::PROPERTY_COUNT_MAX];
  modular_server::Parameter parameters_[lickport_array_controller::constants::PARAMETER_COUNT_MAX];
  modular_server::Function functions_[lickport_array_controller::constants::FUNCTION_COUNT_MAX];
  modular_server::Callback callbacks_[lickport_array_controller::constants::CALLBACK_COUNT_MAX];

  typedef AT42QT2120 LickSensor;
  typedef AT42QT2120::Status LickSensorStatus;
  LickSensor lick_sensor_;
  volatile bool manage_lick_status_change_;
  uint32_t lickports_dispensing_;
  uint32_t lickports_activated_;

  void dispense(Lickport lickport,
    Duration delay,
    Duration period,
    Duration duration,
    Count count);

  void manageLickStatusChange();
  bool lickportDispensing(Lickport lickport);
  bool lickportActivated(Lickport lickport);

  // Handlers
  void manageLickStatusChangeHandler(modular_server::Pin * pin_ptr);

  void dispenseLickportForDurationHandler();
  void dispenseLickportsForDurationHandler();
  void dispenseLickportsForDurationsHandler();
  void dispenseAllLickportsForDurationHandler();

  void getActivatedLickportsHandler();
  void activateOnlyLickportHandler();
  void activateOnlyLickportsHandler();
};

#endif
