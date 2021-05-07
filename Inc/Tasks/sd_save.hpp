//
// Created by Jakub Zygmunt on 06.12.2020.
//

#ifndef HABSAT_OBC_TASKS_SD_SAVE_HPP
#define HABSAT_OBC_TASKS_SD_SAVE_HPP

#include <array>
#include <cstdio>

#include "Rtc/rtc.hpp"
#include "SDReader/sd_reader.hpp"
#include "Sensor/sensor.hpp"
#include "System/task.hpp"

namespace habsat::tasks::sdSave {

[[noreturn]] void taskFn(void* args);

template <size_t size>
void formatFileHeader(std::array<char, size>& buffer) {
    std::sprintf(buffer.data(), "DD/MM/YY hh:mm:ss Temp(\u2103) Press(hPa) Hum(%%RH)\r\n");
}

template <size_t size>
void formatRtcData(std::array<char, size>& buffer, const rtc::Buffer& buf) {
    std::sprintf(
          buffer.data(),
          "%'.02d/%'.02d/%'.02d %'.02d:%'.02d:%'.02d",
          buf.day,
          buf.month,
          buf.year,
          buf.hour,
          buf.minute,
          buf.second);
}

template <size_t size>
void formatSensorData(std::array<char, size>& buffer, const sensor::Buffer& buf) {
    std::sprintf(
          buffer.data(),
          "  %.2lf  %.2lf  %.2lf\r\n",
          buf.temperature,
          buf.pressure,
          buf.humidity);
}

}  // namespace habsat::sdSave

#endif  // HABSAT_OBC_TASKS_SD_SAVE_HPP
