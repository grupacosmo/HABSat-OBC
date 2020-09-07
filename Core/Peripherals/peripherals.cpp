//
// Created by Wiktor Wieclaw on 9/7/20.
//
#include <peripherals.h>

namespace global
{

    void Peripherals::initialize()
    {
        lcd.initialize();
        wifi.initialize();
    }

}
