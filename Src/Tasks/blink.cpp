//
// Created by Wiktor Wieclaw on 11/30/20.
//

#include "Tasks/blink.hpp"

#include "hardware_config.hpp"
#include "System/task.hpp"
#include "obc.hpp"

namespace habsat::tasks::blink {

void inputInterruptHandler() { getObc().inputTask.resumeFromISR(); }

void blinkTaskFn([[maybe_unused]] void* args) {
    auto& obc = getObc();

    while (true) {
        obc.led.toggle();
        system::thisTask::delay(1000);
    }
}

void inputTaskFn([[maybe_unused]] void* args) {
    auto& obc = getObc();

    while (true) {
        system::thisTask::suspend();

        // clang-format off
#       if HW_TERMINAL
            obc.terminal.pcTransmitDMA("Button was pressed!\r\n");
#       endif
        //clang-format on

        if (obc.blinkTask.getState() != system::TaskState::Suspended) {
            obc.blinkTask.suspend();
        } else {
            obc.blinkTask.resume();
        }

        system::thisTask::delay(1000);
    }
}

}  // namespace habsat::blink