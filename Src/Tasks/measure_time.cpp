//
// Created by Wiktor Wieclaw on 11/30/20.
//

#include "Tasks/measure_time.hpp"

#include "obc.hpp"

namespace habsat::measureTime {

void taskFn([[maybe_unused]] void* args) {
    auto& obc = getObc();

    while (true) {
        obc.rtc.readTimeAndDate(obc.rtcBuffer);
        system::thisTask::delay(500);
    }
}

}  // namespace habsat::measureTime