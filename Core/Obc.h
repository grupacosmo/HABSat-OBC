//
// Created by Wiktor Wieclaw on 10/1/20.
//

#ifndef HABSAT_OBC_OBC_H
#define HABSAT_OBC_OBC_H

#include "Blink.h"
#include "Button.h"
#include "Display.h"
#include "I2CBus.h"
#include "Lcd.h"
#include "Led.h"
#include "MeasureTime.h"
#include "MeasureWeather.h"
#include "Noncopyable.h"
#include "Rtc.h"
#include "SPIBus.h"
#include "Sensor.h"
#include "constants.h"
#include "extern_handles.h"
#include "osMutex.h"  // unused
#include "osQueue.h"  // unused
#include "osTask.h"

struct Obc : public Noncopyable {
    mcuBoard::I2CBus i2c;
    mcuBoard::SPIBus spi;

    mcuBoard::GPIOPin pinC13;
    mcuBoard::GPIOPin pinA5;
    mcuBoard::ChipSelect sensorCS;
    mcuBoard::Button button;
    mcuBoard::Led led;

    lcd::Lcd lcd;
    rtc::Rtc rtc;
    sensor::Sensor sensor;

    rtc::Buffer rtcBuffer;
    sensor::Buffer sensorBuffer;

    os::Task inputTask;
    os::Task blinkTask;
    os::Task displayTask;
    os::Task measureTimeTask;
    os::Task measureWeatherTask;

    Obc();
    void init();
};

auto obc() -> Obc&;

#endif  // HABSAT_OBC_OBC_H
