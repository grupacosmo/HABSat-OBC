//
// Created by Wiktor Wieclaw on 11/30/20.
//

#include "Tasks/measure_weather.hpp"

#include "obc.hpp"

namespace habsat::measureWeather {

void taskFn([[maybe_unused]] void* args) {
    auto& obc = getObc();

    while (true) {
        obc.sensor.readAll(obc.sensorBuffer);
        system::thisTask::delay(256);
    }
}

}  // namespace habsat::measureWeather