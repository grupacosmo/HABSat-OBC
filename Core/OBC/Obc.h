//
// Created by Wiktor Wieclaw on 10/1/20.
//

#ifndef HABSAT_OBC_OBC_H
#define HABSAT_OBC_OBC_H

#include "Blink.h"
#include "Display.h"
#include "Hardware.h"
#include "I2CBus.h"
#include "MeasureTime.h"
#include "MeasureWeather.h"

/**
 * The main structure of the entire program, contains every part of the obc.
 */
struct Obc : public Noncopyable {
  Hardware hware;
  services::Blink blink{&hware.led};
  services::MeasureTime measureTime{&hware.rtc};
  services::MeasureWeather measureWeather{&hware.sensor};
  services::Display display{&hware.lcd, &measureWeather.getBuffer(), &measureTime.getBuffer()};

  void init();
};

#endif  // HABSAT_OBC_OBC_H
