//
// Created by Wiktor Wieclaw on 8/7/20.
//


#include "led.h"

namespace hw {

Led::Led(GPIOPin *pin)
    : pin_(pin)
{

}

void Led::on()
{
    pin_->set();
}

void Led::off()
{
    pin_->reset();
}

void Led::toggle()
{
    pin_->toggle();
}

bool Led::isOn() const
{
    return  pin_->read() == GPIOState::Set;
}

}
