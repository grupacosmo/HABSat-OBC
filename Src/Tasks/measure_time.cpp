//
// Created by Wiktor Wieclaw on 11/30/20.
//

#include "Tasks/measure_time.hpp"

#include "obc.hpp"

namespace habsat::measureTime {

void taskFn(void* args) {
    auto obc = static_cast<Obc*>(args);

    while (true) {
        obc->rtc.readTimeAndDate(obc->rtcBuffer);
        system::thisTask::delay(500);
    }
}

}  // namespace habsat::measureTime