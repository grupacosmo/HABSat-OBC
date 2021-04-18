//
// Created by Wiktor Wieclaw on 11/30/20.
//

#include "Tasks/measure_weather.hpp"

#include "hardware_config.hpp"
#include "obc.hpp"

namespace habsat::tasks::measureWeather {

void taskFn([[maybe_unused]] void* args) {
    auto& obc = getObc();

    while (true) {
        obc.sensor.readAll(obc.sensorBuffer);

        // clang-format off
#       if HW_TERMINAL
            std::array<char, 50> text{};

            std::sprintf(
                  text.data(),
                  "Temp: %.2lf C Press: %.2lf hPa, Hum: %.2lf %%RH\r\n",
                  obc.sensorBuffer.temperature,
                  obc.sensorBuffer.pressure,
                  obc.sensorBuffer.humidity);

            obc.terminal.pcTransmitDMA(text.data());
#       endif
        // clang-format on

        system::thisTask::delay(256);
    }
}

}  // namespace habsat::tasks::measureWeather