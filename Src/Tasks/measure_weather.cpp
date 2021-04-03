//
// Created by Wiktor Wieclaw on 11/30/20.
//

#include "Tasks/measure_weather.hpp"

#include "obc.hpp"

namespace habsat::measureWeather {

[[noreturn]] void taskFn(void* args) {
    auto obc = static_cast<Obc*>(args);

    while (true) {
        obc->sensor.readAll(obc->sensorBuffer);
        system::thisTask::delay(256);
    }
}

}  // namespace habsat::measureWeather