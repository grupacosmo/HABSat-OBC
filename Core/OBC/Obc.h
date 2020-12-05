//
// Created by Wiktor Wieclaw on 10/1/20.
//

#ifndef RCC_SYS_OBC_H
#define RCC_SYS_OBC_H


#include "Blink.h"
#include "Display.h"
#include "Hardware.h"
#include "I2CBus.h"
#include "MeasureTime.h"
#include "MeasureWeather.h"

/**
 * The main structure of the entire program, contains every part of the obc.
 */
struct Obc : public Noncopyable
{
    Hardware h;
    services::Blink blink{&h.led};
    services::MeasureTime measureTime{&h.rtc};
    services::MeasureWeather measureWeather{&h.sensor};
    services::Display display{&h.lcd, &measureWeather.getBuffer(), &measureTime.getBuffer()};

    void init();
};

#endif//RCC_SYS_OBC_H
