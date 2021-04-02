//
// Created by Wiktor Wieclaw on 11/30/20.
//

#include "measureTime.h"

#include "../Obc.h"

namespace measureTime {

void taskFn(void* args) {
    auto obc = static_cast<Obc*>(args);

    while (true) {
        obc->rtc.readTimeAndDate(obc->rtcBuffer);
        os::thisTask::delay(500);
    }
}

}  // namespace measureTime