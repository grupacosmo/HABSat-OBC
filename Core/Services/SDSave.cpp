//
// Created by Jakub Zygmunt on 06.12.2020.
//

#include "SDSave.h"

namespace services {

os::Task SDSave::sdSaveTask_{"SDsave_task", 1024, os::Priority::Idle, sdSaveTaskFunction};

SDSave::SDSave(hw::SDReader* sdReader, const hw::Sensor::Buffer* sensorBuffer,
                         const hw::Rtc::Buffer* rtcBuffer)
    : params_(sdReader, sensorBuffer, rtcBuffer){}

void SDSave::init() { sdSaveTask_.addToScheduler(static_cast<void*>(&params_)); }

void SDSave::prepareFileHeader(std::array<char, 100>&buffer) {
  std::sprintf(buffer.data(),"DD/MM/YY hh:mm:ss Temp(\u2103) Press(hPa) Hum(%%RH)\n");
}

void SDSave::prepareRTCData(std::array<char, 100>&buffer, const hw::Rtc::Buffer* buf) {
  std::sprintf(buffer.data(), "%'.02d/%'.02d/%'.02d %'.02d:%'.02d:%'.02d", buf->day,
               buf->month, buf->year, buf->hour, buf->minute, buf->second);
}

void SDSave::prepareSensorData(std::array<char, 20>&buffer, const hw::Sensor::Buffer* buf) {
  std::sprintf(buffer.data(), "  %.2lf  %.2lf  %.2lf\n", buf->temperature, buf->pressure, buf->humidity);
}

[[noreturn]] void SDSave::sdSaveTaskFunction(void* args) {
  auto params = static_cast<Params*>(args);
  std::array<std::array<char, 100>, 2> buffer{};
  std::array<char, 20> buffer1{};
  std::array<char, 256> path{"test2.txt"};

  if(params->sdReader_->mount() != FR_OK)
  {
    os::Task::suspendItself();
  }

  params->sdReader_->format();

  prepareFileHeader(buffer[0]);
  params->sdReader_->makeFile(path);
  params->sdReader_->update(path, buffer[0].data());
  os::Task::delay(1000);

  while (true) {
    prepareRTCData(buffer[1], params->rtcBuffer_);
    prepareSensorData(buffer1, params->sensorBuffer_);

    params->sdReader_->update(path, buffer[1].data());
    params->sdReader_->update(path, buffer1.data());

    os::Task::delay(1000);
  }
}

}  // namespace services