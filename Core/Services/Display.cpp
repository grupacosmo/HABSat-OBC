//
// Created by Wiktor Wieclaw on 11/30/20.
//

#include "Display.h"

namespace services {

os::Task Display::displayTask_{"display_task", 256, os::Priority::Idle, displayTaskFunction};

Display::Display(hw::Lcd* lcd, const hw::Sensor::Buffer* sensorBuffer,
                 const hw::Rtc::Buffer* timeAndDate)
    : params_(lcd, sensorBuffer, timeAndDate) {}

void Display::init() { displayTask_.addToScheduler(static_cast<void*>(&params_)); }

void Display::prepareHeaderData(std::array<char, 20>& lineBuffer) {
  constexpr std::array options{"display", "test"};
  static std::size_t index;

  std::sprintf(lineBuffer.data(), "%s", options[index]);
  index = (index + 1) % options.size();
}

void Display::prepareTimeData(std::array<char, 20>& lineBuffer, const hw::Rtc::Buffer* buf) {
  std::sprintf(lineBuffer.data(), "      %'.02d:%'.02d:%'.02d", buf->hour, buf->minute,
               buf->second);
}

void Display::prepareDateData(std::array<char, 20>& lineBuffer, const hw::Rtc::Buffer* buf) {
  static const std::array dayNames = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};

  std::sprintf(lineBuffer.data(), "    %s %'.02d/%'.02d/%'.02d", dayNames[buf->weekday], buf->day,
               buf->month, buf->year);
}

void Display::prepareSensorData(std::array<char, 20>& lineBuffer, const hw::Sensor::Buffer* buf) {
  static const std::array bufferArray{&buf->temperature, &buf->pressure, &buf->humidity};

  static constexpr std::array options{"Temp: %.2lf C", "Press: %.2lf hPa", "Hum: %.2lf %%RH"};
  static std::size_t index = 0;

  std::sprintf(lineBuffer.data(), options[index], *bufferArray[index]);
  index = (index + 1) % options.size();
}

[[noreturn]] void Display::displayTaskFunction(void* args) {
  auto params = static_cast<Params*>(args);
  std::array<std::array<char, 20>, 4> lineBuffers{};

  while (true) {
    prepareHeaderData(lineBuffers[0]);
    prepareTimeData(lineBuffers[1], params->timeAndDate_);
    prepareDateData(lineBuffers[2], params->timeAndDate_);
    prepareSensorData(lineBuffers[3], params->sensorBuffer_);

    for (std::size_t i = 0; i < 4; ++i) params->lcd_->printLine(i, lineBuffers[i].data());

    os::Task::delay(980);
  }
}

}  // namespace services