//
// Created by Wiktor Wieclaw on 11/30/20.
//

#include "MeasureWeather.h"

namespace services {

os::Task MeasureWeather::measureTask_{"measure", 256, os::Priority::Idle, measureTaskFunction};

MeasureWeather::MeasureWeather(hw::Sensor* sensor) : params_(sensor) {}

void MeasureWeather::init() { measureTask_.addToScheduler(static_cast<void*>(&params_)); }

[[noreturn]] void MeasureWeather::measureTaskFunction(void* args) {
  auto params = static_cast<Params*>(args);

  while (true) {
    params->sensor_->readAll(params->buffer_);
    os::Task::delay(256);
  }
}

auto MeasureWeather::getBuffer() const -> const hw::Sensor::Buffer& { return params_.buffer_; }

}  // namespace services