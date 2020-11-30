//
// Created by Wiktor Wieclaw on 11/30/20.
//

#ifndef RCC_SYS_MEASUREWEATHER_H
#define RCC_SYS_MEASUREWEATHER_H

#include "os_task.h"
#include "sensor.h"

namespace services {

class MeasureWeather
{
public:
    MeasureWeather(hw::Sensor* sensor);
    void init();
    const hw::Sensor::Buffer& getBuffer() const;
private:
    static void measureTaskFunction(void *args);
    static os::Task measureTask_;
private:
    struct Params
    {
        hw::Sensor* sensor_;
        hw::Sensor::Buffer buffer_;
        explicit Params(hw::Sensor* sensor) : sensor_(sensor) {}
    } params_;
};

}

#endif//RCC_SYS_MEASUREWEATHER_H
