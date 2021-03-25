//
// Created by Wiktor Wieclaw on 11/30/20.
//

#include "Blink.h"

namespace services {

os::Task Blink::inputTask_{"input", 128, os::Priority::Interrupt, inputTaskFunction};
os::Task Blink::blinkTask_{"blink", 128, os::Priority::Idle, blinkTaskFunction};

Blink::Blink(hw::Led* led) : led_(led) {}

void Blink::init() {
  blinkTask_.addToScheduler(static_cast<void*>(led_));
  inputTask_.addToScheduler();
}

void Blink::inputInterruptHandler() { inputTask_.resumeFromISR(); }

[[noreturn]] void Blink::blinkTaskFunction(void* args) {
  auto led = static_cast<hw::Led*>(args);
  while (true) {
    led->toggle();
    os::Task::delay(1000);
  }
}

[[noreturn]] void Blink::inputTaskFunction([[maybe_unused]] void* args) {
  while (true) {
    os::Task::suspendItself();
    if (blinkTask_.getState() != os::TaskState::Suspended)
      blinkTask_.suspend();
    else
      blinkTask_.resume();
    os::Task::delay(1000);
  }
}

}  // namespace services