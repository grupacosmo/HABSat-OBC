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
struct Obc : public Noncopyable{
  Hardware hardware;
  services::Blink blink;
  services::MeasureTime measureTime;
  services::MeasureWeather measureWeather;
  services::Display display;

  Obc();
  void init();
};

#endif  // HABSAT_OBC_OBC_H
