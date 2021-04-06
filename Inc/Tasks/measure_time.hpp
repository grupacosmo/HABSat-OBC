//
// Created by Wiktor Wieclaw on 11/30/20.
//

#ifndef HABSAT_OBC_TASKS_MEASURE_TIME_HPP
#define HABSAT_OBC_TASKS_MEASURE_TIME_HPP

#include "Rtc/rtc.hpp"

namespace habsat::tasks::measureTime {

[[noreturn]] void taskFn(void* args);

}  // namespace habsat::measureTime

#endif  // HABSAT_OBC_TASKS_MEASURE_TIME_HPP
