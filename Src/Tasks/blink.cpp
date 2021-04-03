//
// Created by Wiktor Wieclaw on 11/30/20.
//

#include "Tasks/blink.hpp"

#include "System/task.hpp"
#include "obc.hpp"

namespace habsat::blink {

void inputInterruptHandler() { habsat::obc().inputTask.resumeFromISR(); }

[[noreturn]] void blinkTaskFn(void* args) {
    auto obc = static_cast<habsat::Obc*>(args);

    while (true) {
        obc->led.toggle();
        habsat::system::thisTask::delay(1000);
    }
}

[[noreturn]] void inputTaskFn([[maybe_unused]] void* args) {
    auto obc = static_cast<habsat::Obc*>(args);

    while (true) {
        habsat::system::thisTask::suspend();
        if (obc->blinkTask.getState() != habsat::system::TaskState::Suspended)
            obc->blinkTask.suspend();
        else
            obc->blinkTask.resume();
        habsat::system::thisTask::delay(1000);
    }
}

}  // namespace habsat::blink