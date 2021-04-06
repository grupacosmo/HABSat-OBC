//
// Created by Wiktor Wieclaw on 11/30/20.
//

#ifndef HABSAT_OBC_TASKS_MEASURE_WEATHER_HPP
#define HABSAT_OBC_TASKS_MEASURE_WEATHER_HPP

#include "Sensor/sensor.hpp"

namespace habsat::tasks::measureWeather {

[[noreturn]] void taskFn(void* args);

}  // namespace habsat::measureWeather

#endif  // HABSAT_OBC_TASKS_MEASURE_WEATHER_HPP
