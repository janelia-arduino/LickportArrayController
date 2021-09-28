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
  typedef uint32_t BitArray;
  typedef Array<Lickport,lickport_array_controller::constants::LICKPORT_COUNT> Lickports;
  typedef Array<Duration,lickport_array_controller::constants::LICKPORT_COUNT> Durations;

  LickportArrayController();
  virtual void setup();
  virtual void update();

  void calibrateLickSensor();

  void dispenseLickportForDuration(Lickport lickport,
    Duration duration);
  void dispenseLickportsForDuration(const Lickports & lickports,
    Duration duration);
  void dispenseLickportsForDurations(const Lickports & lickports,
    const Durations & durations);
  void dispenseAllLickportsForDuration(Duration duration);

  Lickports getActivatedLickports();
  void activateAllLickports();
  void deactivateAllLickports();
  void activateOnlyLickport(Lickport lickport);
  void activateOnlyLickports(Lickports lickports);
  void activateLickport(Lickport lickport);
  void activateLickports(Lickports lickports);
  void deactivateLickport(Lickport lickport);
  void deactivateLickports(Lickports lickports);

  struct LickDatum
  {
    time_t time;
    unsigned long millis;
    BitArray lickports_licked;
    BitArray lickports_activated;
  };
  typedef Array<LickDatum,lickport_array_controller::constants::LICK_DATUM_COUNT_MAX> LickData;
  LickData getLickData();
  void clearLickData();

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
  BitArray lickports_dispensing_;
  BitArray lickports_activated_;
  LickData lick_data_;
  digital_controller::constants::PwmId sync_pwm_id_;

  virtual double setChannelToPower(size_t lickport,
    double power);

  void startSyncPwm();
  void stopSyncPwm();

  void dispense(Lickport lickport,
    Duration delay,
    Duration period,
    Duration duration,
    Count count);

  void manageLickStatusChange();
  bool lickportTrueInBitArray(Lickport lickport,
    BitArray bit_array);
  bool lickportDispensing(Lickport lickport);
  bool lickportActivated(Lickport lickport);
  void updateLickData(LickSensorStatus lick_sensor_status);

  // Handlers
  void setSyncPropertyHandler();
  void stopSyncPwmHandler(int pwm_index);
  void startLickportPwmHandler(int pwm_index);
  void stopLickportPwmHandler(int pwm_index);

  void calibrateLickSensorHandler(modular_server::Pin * pin_ptr);
  void manageLickStatusChangeHandler(modular_server::Pin * pin_ptr);
  void activateAllLickportsHandler(modular_server::Pin * pin_ptr);
  void deactivateAllLickportsHandler(modular_server::Pin * pin_ptr);

  void dispenseLickportForDurationHandler();
  void dispenseLickportsForDurationHandler();
  void dispenseLickportsForDurationsHandler();
  void dispenseAllLickportsForDurationHandler();

  void getActivatedLickportsHandler();
  void activateOnlyLickportHandler();
  void activateOnlyLickportsHandler();
  void activateLickportHandler();
  void activateLickportsHandler();
  void deactivateLickportHandler();
  void deactivateLickportsHandler();

  void getAndClearLickDataHandler();
};

#endif
