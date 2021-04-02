//
// Created by Wiktor Wieclaw on 11/30/20.
//

#include "Blink.h"

#include "../Obc.h"
#include "osTask.h"

namespace blink {

void inputInterruptHandler() { obc().inputTask.resumeFromISR(); }

[[noreturn]] void blinkTaskFn(void* args) {
    auto obc = static_cast<Obc*>(args);

    while (true) {
        obc->led.toggle();
        os::thisTask::delay(1000);
    }
}

[[noreturn]] void inputTaskFn([[maybe_unused]] void* args) {
    auto obc = static_cast<Obc*>(args);

    while (true) {
        os::thisTask::suspend();
        if (obc->blinkTask.getState() != os::TaskState::Suspended)
            obc->blinkTask.suspend();
        else
            obc->blinkTask.resume();
        os::thisTask::delay(1000);
    }
}

}  // namespace blink