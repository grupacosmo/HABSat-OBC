//
// Created by Wiktor Wieclaw on 11/30/20.
//

#ifndef HABSAT_OBC_DISPLAY_H
#define HABSAT_OBC_DISPLAY_H

#include <array>

#include "Lcd/lcd.hpp"
#include "Rtc/rtc.hpp"
#include "Sensor/sensor.hpp"

namespace habsat::display {

namespace detail {

void formatHeaderData(gsl::span<char> lineBuffer);
void formatTimeData(gsl::span<char> lineBuffer, const rtc::Buffer& buf);
void formatDateData(gsl::span<char> lineBuffer, const rtc::Buffer& buf);
void formatSensorData(gsl::span<char> lineBuffer, const sensor::Buffer& buf);

}  // namespace detail

[[noreturn]] void taskFn(void* args);

}  // namespace habsat::display

#endif  // HABSAT_OBC_DISPLAY_H
