//
// Created by Wiktor Wieclaw on 9/7/20.
//
#include "Inc/peripherals.h"


void Peripherals::init()
{
    lcd.init();
    wifi.initialize();
}

