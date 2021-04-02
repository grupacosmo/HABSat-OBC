//
// Created by Wiktor Wieclaw on 11/30/20.
//

#ifndef HABSAT_OBC_DISPLAY_H
#define HABSAT_OBC_DISPLAY_H

#include <array>

#include "Lcd.h"
#include "Rtc.h"
#include "Sensor.h"

namespace display::impl {

void formatHeaderData(std::array<char, 20>& lineBuffer);
void formatTimeData(std::array<char, 20>& lineBuffer, const rtc::Buffer& buf);
void formatDateData(std::array<char, 20>& lineBuffer, const rtc::Buffer& buf);
void formatSensorData(std::array<char, 20>& lineBuffer, const sensor::Buffer& buf);

}  // namespace display::impl

namespace display {

[[noreturn]] void taskFn(void* args);

}  // namespace display

#endif  // HABSAT_OBC_DISPLAY_H
