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
  Display(hw::Lcd* lcd, const hw::Sensor::Buffer* sensorBuffer, const hw::Rtc::Buffer* timeAndDate);
  void init();

 private:
  static void prepareHeaderData(std::array<char, 20>& lineBuffer);
  static void prepareTimeData(std::array<char, 20>& lineBuffer, const hw::Rtc::Buffer* buf);
  static void prepareDateData(std::array<char, 20>& lineBuffer, const hw::Rtc::Buffer* buf);
  static void prepareSensorData(std::array<char, 20>& lineBuffer, const hw::Sensor::Buffer* buf);
  [[noreturn]] static void displayTaskFunction(void* args);

 private:
  static os::Task displayTask_;
  struct Params {
    hw::Lcd* lcd_;
    const hw::Sensor::Buffer* sensorBuffer_;
    const hw::Rtc::Buffer* timeAndDate_;
    Params(hw::Lcd* lcd, const hw::Sensor::Buffer* sensorBuffer, const hw::Rtc::Buffer* timeAndDate)
        : lcd_(lcd), sensorBuffer_(sensorBuffer), timeAndDate_(timeAndDate) {}
  } params_;
};

}  // namespace services

#endif  // HABSAT_OBC_DISPLAY_H
