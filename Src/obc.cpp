//
// Created by Wiktor Wieclaw on 10/1/20.
//
#include "obc.hpp"

#include "hardware_config.h"

using habsat::system::Priority;

extern I2C_HandleTypeDef hi2c3;
extern SPI_HandleTypeDef hspi2;
extern UART_HandleTypeDef huart1;
extern SD_HandleTypeDef hsd;

#if HW_LCD_I2C_CONVERTER_TYPE_A
constexpr uint8_t lcdSlaveAddress = 0x7E;
#else
constexpr uint8_t lcdSlaveAddress = 0x4E;
#endif

constexpr uint8_t rtcSlaveAddress = 0x68 << 1;

habsat::Obc::Obc()
    : i2c{hi2c3},
      spi{hspi2},
      uart(huart1),
      pinC13{GPIOC, GPIO_PIN_13},
      pinA5{GPIOA, GPIO_PIN_5},
      sensorCS{GPIOC, GPIO_PIN_3},
      button{&pinC13},
      led{pinA5},
      lcd{4, 20, i2c, lcdSlaveAddress},
      rtc{i2c, rtcSlaveAddress},
      sensor{spi, sensorCS},
      inputTask{128, Priority::Interrupt, tasks::blink::inputTaskFn},
      blinkTask{128, Priority::Idle, tasks::blink::blinkTaskFn},
      displayTask{256, Priority::Idle, tasks::display::taskFn},
      measureTimeTask{128, Priority::Idle, tasks::measureTime::taskFn},
      measureWeatherTask{256, system::Priority::Idle, tasks::measureWeather::taskFn},
      sdSaveTask{1024, Priority::Idle, tasks::sdSave::taskFn} {}

void habsat::Obc::init() {
#if HW_LCD == 1
    lcd.init();
#endif
#if HW_RTC == 1
    rtc.init();
#endif
#if HW_SENSOR == 1
    sensor::Settings sensorSettings {
          .temperatureResolution = sensor::TemperatureResolution::SixteenBit,
          .pressureOversampling = sensor::PressureOversampling::UltraLowPower,
          .humidityOversampling = sensor::HumidityOversampling::Standard,
          .mode = sensor::Mode::Normal,
          .standbyTime = sensor::StandbyTime::TenMs,
          .filter = sensor::Filter::Off
    };

    sensor.init(sensorSettings);
#endif
#if HW_RTC && HW_RTC_SET_TIME
    rtc::Buffer data;
    data.second  = HW_RTC_SECOND;
    data.minute  = HW_RTC_MINUTE;
    data.hour    = HW_RTC_HOUR;
    data.weekday = HW_RTC_WEEKDAY;
    data.day     = HW_RTC_DAY;
    data.month   = HW_RTC_MONTH;
    data.year    = HW_RTC_YEAR;

    rtc.setTimeAndDate(data);
#endif
    inputTask.addToScheduler();
    blinkTask.addToScheduler();
    displayTask.addToScheduler();
    measureTimeTask.addToScheduler();
    measureWeatherTask.addToScheduler();
    sdSaveTask.addToScheduler();
}

auto habsat::getObc() -> Obc& {
    static Obc obc;
    return obc;
}