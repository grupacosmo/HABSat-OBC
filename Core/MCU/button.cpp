//
// Created by Wiktor Wieclaw on 8/20/20.
//

#include "button.h"

namespace hw {

Button::Button(GPIOPin* pin)
        : pin_(pin)
{

}

bool Button::isPressed() const
{
    return pin_->read() == GPIOState::Reset;
}

}