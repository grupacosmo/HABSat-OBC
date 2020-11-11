//
// Created by Wiktor Wieclaw on 9/7/20.
//
#include "Inc/peripherals.h"
#include "hardware_config.h"
void Peripherals::init()
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
    sensor.init(Sensor::ConfigFlags::BME280_TEMPERATURE_16BIT,
                Sensor::ConfigFlags::BME280_PRESSURE_ULTRALOWPOWER,
                Sensor::ConfigFlags::BME280_HUMINIDITY_STANDARD,
                Sensor::ConfigFlags::BME280_NORMALMODE);
    sensor.configure(Sensor::ConfigFlags::BME280_STANDBY_MS_10,
                     Sensor::ConfigFlags::FILTER_OFF);
    sensor.getMeasureTask().addToScheduler();
#endif
}

