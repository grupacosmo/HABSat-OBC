//
// Created by Wiktor Wieclaw on 11/30/20.
//

#ifndef HABSAT_OBC_TASKS_BLINK_HPP
#define HABSAT_OBC_TASKS_BLINK_HPP

#include "MCU_Board/button.hpp"
#include "MCU_Board/led.hpp"

namespace habsat::tasks::blink {

void inputInterruptHandler();

[[noreturn]] void inputTaskFn(void* args);
[[noreturn]] void blinkTaskFn(void* args);

}  // namespace habsat::blink

#endif  // HABSAT_OBC_TASKS_BLINK_HPP
