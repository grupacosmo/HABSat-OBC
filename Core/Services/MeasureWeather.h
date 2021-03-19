//
// Created by Wiktor Wieclaw on 11/30/20.
//

#ifndef HABSAT_OBC_MEASUREWEATHER_H
#define HABSAT_OBC_MEASUREWEATHER_H

#include "Sensor.h"
#include "osTask.h"

namespace services {

class MeasureWeather : public Noncopyable {
 public:
  explicit MeasureWeather(sensor::Sensor* sensor);
  void init();
  [[nodiscard]] auto getBuffer() const -> const sensor::SensorBuffer&;

 private:
  [[noreturn]] static void measureTaskFunction(void* args);
  static os::Task measureTask_;

 private:
  struct Params {
    sensor::Sensor* sensor;
    sensor::SensorBuffer buffer;
  } params_;
};

}  // namespace services

#endif  // HABSAT_OBC_MEASUREWEATHER_H
