//
// Created by Wiktor Wieclaw on 11/30/20.
//

#include "Tasks/getGpsData.hpp"

#include "hardware_config.hpp"
#include "System/task.hpp"
#include "obc.hpp"

namespace habsat::getGpsData {

void taskFn([[maybe_unused]] void* args) {
    auto& obc = getObc();

    while (true) {
#       if HW_TERMINAL
        obc.terminal.pcTransmitDMA(obc.gps.getPrintedData());
#       endif
        system::thisTask::delay(4000);
    }
}
}  // namespace habsat::getGpsData