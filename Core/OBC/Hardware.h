//
// Created by Wiktor Wieclaw on 9/7/20.
//

#ifndef HABSAT_OBC_HARDWARE_H
#define HABSAT_OBC_HARDWARE_H

#include "Button.h"
#include "I2CBus.h"
#include "Lcd.h"
#include "Led.h"
#include "Rtc.h"
#include "Sensor.h"
#include "constants.h"
#include "handles.h"

/**
 * Holds all instances of the classes that represent peripherals
 */
struct Hardware : public Noncopyable {
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

  Hardware();
  void init();
};

#endif  // HABSAT_OBC_HARDWARE_H
