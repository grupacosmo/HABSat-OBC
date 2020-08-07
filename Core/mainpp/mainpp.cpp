//
// Created by vixu on 8/6/20.
//

#include "mainpp.h"
//#include "led.h"

void mainpp()
{
    Led led;
    while(true)
    {
        led.toggle();
        HAL_Delay(1000);
    }
}
