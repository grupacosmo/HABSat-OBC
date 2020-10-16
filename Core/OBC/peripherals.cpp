//
// Created by Wiktor Wieclaw on 9/7/20.
//
#include "Inc/peripherals.h"
#include "hardware_config.h"
void Peripherals::init()
{
#if CORE_LCD == 1
    lcd.init();
#endif
#if CORE_WIFI == 1
    wifi.init();
#endif
#if CORE_RTC == 1
    rtc.init();
#endif
#if CORE_SENSOR == 1
    sensor.init(Sensor::InitConfigFlags::BME280_TEMPERATURE_16BIT,
                Sensor::InitConfigFlags::BME280_PRESSURE_ULTRALOWPOWER,
                Sensor::InitConfigFlags::BME280_HUMINIDITY_STANDARD,
                Sensor::InitConfigFlags::BME280_NORMALMODE);
    sensor.sensor_set_config(Sensor::InitConfigFlags::BME280_STANDBY_MS_10,
                             Sensor::InitConfigFlags::FILTER_OFF);
    sensor.getMeasureTask().add_to_scheduler();
#endif
}

