//
// Created by Wiktor Wieclaw on 11/30/20.
//

#ifndef HABSAT_OBC_BLINK_H
#define HABSAT_OBC_BLINK_H

#include "Button.h"
#include "Led.h"

namespace blink {

void inputInterruptHandler();

[[noreturn]] void inputTaskFn(void* args);
[[noreturn]] void blinkTaskFn(void* args);

}  // namespace blink

#endif  // HABSAT_OBC_BLINK_H
