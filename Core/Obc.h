//
// Created by Wiktor Wieclaw on 10/1/20.
//

#ifndef HABSAT_OBC_OBC_H
#define HABSAT_OBC_OBC_H

#include "Button.h"
#include "I2CBus.h"
#include "SPIBus.h"
#include "Lcd.h"
#include "Led.h"
#include "Rtc.h"
#include "Sensor.h"
#include "constants.h"
#include "extern_handles.h"
#include "Blink.h"
#include "Display.h"
#include "MeasureTime.h"
#include "MeasureWeather.h"
#include "Button.h"
#include "Noncopyable.h"

class Obc : public Noncopyable {
  hw::I2CBus i2c;
  hw::SPIBus spi;

  hw::GPIOPin pinC13;
  hw::GPIOPin pinA5;
  hw::ChipSelect sensorCS;

  hw::Button button;
  hw::Led led;
  lcd::Lcd lcd;
  rtc::Rtc rtc;
  sensor::Sensor sensor;

  services::Blink blink;
  services::MeasureTime measureTime;
  services::MeasureWeather measureWeather;
  services::Display display;

 public:
  Obc();
  void init();
};

#endif  // HABSAT_OBC_OBC_H
