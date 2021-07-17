//
// Created by Wiktor Wieclaw on 10/1/20.
//
#include "obc.hpp"

#include "hardware_config.hpp"

extern I2C_HandleTypeDef hi2c3;
extern SPI_HandleTypeDef hspi2;
extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart2;
extern SD_HandleTypeDef hsd;
extern DMA_HandleTypeDef hdma_usart1_rx;

constexpr uint8_t lcdSlaveAddress = HW_LCD_SLAVE_ADDRESS;
constexpr uint8_t rtcSlaveAddress = 0x68 << 1;

using habsat::system::Priority;

habsat::Obc::Obc()
    : i2c{hi2c3},
      spi{hspi2},
      uart(huart2),
      uartGps(huart1),
      pinC13{GPIOC, GPIO_PIN_13},
      pinA5{GPIOA, GPIO_PIN_5},
      sensorCS{GPIOC, GPIO_PIN_3},
      button{&pinC13},
      led{pinA5},
      lcd{4, 20, i2c, lcdSlaveAddress},
      rtc{i2c, rtcSlaveAddress},
      sensor{spi, sensorCS},
      terminal{uart},
      gps(huart1, hdma_usart1_rx),
      inputTask{128, Priority::Interrupt, blink::inputTaskFn},
      blinkTask{128, Priority::Idle, blink::blinkTaskFn},
      displayTask{256, Priority::Idle, display::taskFn},
      measureTimeTask{512, Priority::Idle, measureTime::taskFn},
      measureWeatherTask{512, Priority::Idle, measureWeather::taskFn},
      sdSaveTask{512, Priority::Idle, sdSave::taskFn} {}

void habsat::Obc::init() {
    inputTask.addToScheduler();
    blinkTask.addToScheduler();

    // clang-format off
#   if HW_LCD
        lcd.init();
        displayTask.addToScheduler();
#   endif

#   if HW_RTC
        measureTimeTask.addToScheduler();

#       if HW_RTC_SET_TIME
            rtc::Buffer data;
            data.second  = HW_RTC_SECOND;
            data.minute  = HW_RTC_MINUTE;
            data.hour    = HW_RTC_HOUR;
            data.weekday = HW_RTC_WEEKDAY;
            data.day     = HW_RTC_DAY;
            data.month   = HW_RTC_MONTH;
            data.year    = HW_RTC_YEAR;

            rtc.setTimeAndDate(data);
#       endif
#   endif

#   if HW_SENSOR
        using namespace sensor;
        Settings sensorSettings {
              TemperatureResolution::SixteenBit,
              PressureOversampling::UltraLowPower,
              HumidityOversampling::Standard,
              Mode::Normal,
              StandbyTime::TenMs,
              Filter::Off
        };

        sensor.init(sensorSettings);
        measureWeatherTask.addToScheduler();
#   endif

#   if HW_SD_READER
        sdReader.init();
        sdSaveTask.addToScheduler();
#   endif
    // clang-format on
}

auto habsat::getObc() -> Obc& {
    static Obc obc;
    return obc;
}