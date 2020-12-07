//
// Created by Jakub Zygmunt on 06.12.2020.
//

#include "SDSave.h"

namespace services {

os::Task SDSave::sdSaveTask_{"SDsave_task", 256, os::Priority::Idle, displayTaskFunction};

SDSave::SDSave(hw::SDReader* sdReader, const hw::Sensor::Buffer* sensorBuffer,
                         const hw::Rtc::Buffer* rtcBuffer)
    : params_(sdReader, sensorBuffer, rtcBuffer){}

void SDSave::init() { sdSaveTask_.addToScheduler(static_cast<void*>(&params_)); }

void SDSave::prepareFileHeader(std::array<char, 100>&buffer) {
  std::sprintf(buffer.data(),"DAY DD/MM/YYYY hh:mm:ss Temp(°C) Press(hPa) Hum(%%RH)\n");
}

void SDSave::prepareRTCData(std::array<char, 100>&buffer, const hw::Rtc::Buffer* buf) {
  static const std::array dayNames = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};

  std::sprintf(buffer.data(), "%s %'.02d/%'.02d/%'.02d %'.02d:%'.02d:%'.02d", dayNames[buf->weekday], buf->day,
               buf->month, buf->year, buf->hour, buf->minute, buf->second);
}

void SDSave::prepareSensorData(std::array<char, 20>&buffer, const hw::Sensor::Buffer* buf) {
  std::sprintf(buffer.data(), "%.2lf %.2lf %.2lf\n", buf->temperature, buf->pressure, buf->humidity);
}

[[noreturn]] void SDSave::displayTaskFunction(void* args) {
  auto params = static_cast<Params*>(args);
  std::array<std::array<char, 100>, 2> buffer{};
  std::array<char, 20> buffer1{};
  std::array<char, 256> path{"test.txt"};

  prepareFileHeader(buffer[0]);

  if(params->sdReader_->mount() != FR_OK)
  {
    os::Task::suspendItself();
  }

  params->sdReader_->makeFile(path);
  params->sdReader_->update(path, buffer[0].data());

  while (true) {
    prepareRTCData(buffer[1], params->rtcBuffer_);
    prepareSensorData(buffer1, params->sensorBuffer_);

    params->sdReader_->update(path, buffer[1].data());
    params->sdReader_->update(path, buffer1.data());

    os::Task::delay(1000);
  }
}

}  // namespace services