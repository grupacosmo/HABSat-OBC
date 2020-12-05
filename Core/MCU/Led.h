//
// Created by Wiktor Wieclaw on 8/7/20.
//

#ifndef FREERTOS_INTRO_LED_H
#define FREERTOS_INTRO_LED_H

#include "GPIOPin.h"

namespace hw {

class Led : public Noncopyable
{
public:

    explicit Led(GPIOPin* pin);

    /**
     * Turns the LED on.
     */
    void on();

    /**
     * Turns the LED off.
     */
    void off();

    /**
     * Toggles the LED.
     */
    void toggle();

    /**
     * Checks whether the LED is on.
     * @return
     */
    bool isOn() const;

private:
    GPIOPin *const pin_;
};

}

#endif //FREERTOS_INTRO_LED_H
