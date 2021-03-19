//
// Created by Wiktor Wieclaw on 11/30/20.
//

#include "MeasureWeather.h"

namespace services {

os::Task MeasureWeather::measureTask_{"measure", 256, os::Priority::Idle, measureTaskFunction};

MeasureWeather::MeasureWeather(sensor::Sensor* sensor) : params_{.sensor = sensor, .buffer{}} {}

void MeasureWeather::init() { measureTask_.addToScheduler(static_cast<void*>(&params_)); }

[[noreturn]] void MeasureWeather::measureTaskFunction(void* args) {
  auto params = static_cast<Params*>(args);

  while (true) {
    params->sensor->readAll(params->buffer);
    os::Task::delay(256);
  }
}

auto MeasureWeather::getBuffer() const -> const sensor::SensorBuffer& { return params_.buffer; }

}  // namespace services