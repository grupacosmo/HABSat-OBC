//
// Created by Wiktor Wieclaw on 10/1/20.
//
#include "Obc.h"

Obc::Obc()
    : hardware{},
      blink{&hardware.led},
      measureTime{&hardware.rtc},
      measureWeather{&hardware.sensor},
      display{&hardware.lcd, &measureWeather.getBuffer(), &measureTime.getBuffer()} {}

void Obc::init() {
  hardware.init();
  blink.init();
  display.init();
  measureTime.init();
  measureWeather.init();
}
