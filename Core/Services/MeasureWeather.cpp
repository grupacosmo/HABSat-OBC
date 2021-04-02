//
// Created by Wiktor Wieclaw on 11/30/20.
//

#include "measureWeather.h"

#include "../Obc.h"

namespace measureWeather {

[[noreturn]] void taskFn(void* args) {
    auto obc = static_cast<Obc*>(args);

    while (true) {
        obc->sensor.readAll(obc->sensorBuffer);
        os::thisTask::delay(256);
    }
}

}  // namespace measureWeather