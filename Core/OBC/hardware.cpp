//
// Created by Wiktor Wieclaw on 9/7/20.
//
#include "hardware.h"
#include "hardware_config.h"

void Hardware::init()
{
#if HW_LCD == 1
    lcd.init();
#endif
#if HW_WIFI == 1
    wifi.init();
#endif
#if HW_RTC == 1
    rtc.init();
#endif
#if HW_SENSOR == 1
    sensor.init(hw::Sensor::ConfigFlags::BME280_TEMPERATURE_16BIT,
                hw::Sensor::ConfigFlags::BME280_PRESSURE_ULTRALOWPOWER,
                hw::Sensor::ConfigFlags::BME280_HUMINIDITY_STANDARD,
                hw::Sensor::ConfigFlags::BME280_NORMALMODE);
    sensor.configure(hw::Sensor::ConfigFlags::BME280_STANDBY_MS_10,
                     hw::Sensor::ConfigFlags::FILTER_OFF);
    sensor.getMeasureTask().addToScheduler();
#endif
    button.init();
    led.init();
}

