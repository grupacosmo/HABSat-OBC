//
// Created by Wiktor Wieclaw on 9/7/20.
//
#include "Inc/peripherals.h"


void Peripherals::initialize()
{
    lcd.initialize();
    wifi.initialize();
}

