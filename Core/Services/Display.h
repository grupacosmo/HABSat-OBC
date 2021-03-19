//
// Created by Wiktor Wieclaw on 11/30/20.
//

#ifndef HABSAT_OBC_DISPLAY_H
#define HABSAT_OBC_DISPLAY_H

#include <array>

#include "Lcd.h"
#include "Rtc.h"
#include "Sensor.h"
#include "osTask.h"

namespace services {

class Display : public Noncopyable {
 public:
  Display(lcd::Lcd* lcd, const sensor::SensorBuffer* sensorBuffer, const rtc::RtcBuffer* timeAndDate);
  void init();

 private:
  static void prepareHeaderData(std::array<char, 20>& lineBuffer);
  static void prepareTimeData(std::array<char, 20>& lineBuffer, const rtc::RtcBuffer* buf);
  static void prepareDateData(std::array<char, 20>& lineBuffer, const rtc::RtcBuffer* buf);
  static void prepareSensorData(std::array<char, 20>& lineBuffer, const sensor::SensorBuffer* buf);
  [[noreturn]] static void displayTaskFunction(void* args);

 private:
  static os::Task displayTask_;
  struct Params {
    lcd::Lcd* lcd;
    const sensor::SensorBuffer* sensorBuffer;
    const rtc::RtcBuffer* timeAndDate;
  } params_;
};

}  // namespace services

#endif  // HABSAT_OBC_DISPLAY_H
