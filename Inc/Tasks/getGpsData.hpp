//
// Created by Wiktor Wieclaw on 11/30/20.
//

#ifndef HABSAT_OBC_TASKS_GETGPSDATA_HPP
#define HABSAT_OBC_TASKS_GETGPSDATA_HPP

#include "Gps/gps.hpp"

namespace habsat::getGpsData {

[[noreturn]] void taskFn(void* args);

}  // namespace habsat::getGpsData

#endif  // HABSAT_OBC_TASKS_GETGPSDATA_HPP
