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
  explicit MeasureWeather(hw::Sensor* sensor);
  void init();
  [[nodiscard]] auto getBuffer() const -> const hw::Sensor::Buffer&;

 private:
  [[noreturn]] static void measureTaskFunction(void* args);
  static os::Task measureTask_;

 private:
  struct Params {
    hw::Sensor* sensor_;
    hw::Sensor::Buffer buffer_;
    explicit Params(hw::Sensor* sensor) : sensor_(sensor) {}
  } params_;
};

}  // namespace services

#endif  // HABSAT_OBC_MEASUREWEATHER_H
