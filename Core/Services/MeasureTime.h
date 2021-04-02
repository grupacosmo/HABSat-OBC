//
// Created by Wiktor Wieclaw on 11/30/20.
//

#ifndef HABSAT_OBC_MEASURETIME_H
#define HABSAT_OBC_MEASURETIME_H

#include "Rtc.h"

namespace measureTime {

[[noreturn]] void taskFn(void* args);

}  // namespace measureTime

#endif  // HABSAT_OBC_MEASURETIME_H
