//
// Created by Wiktor Wieclaw on 10/1/20.
//

#ifndef HABSAT_OBC_OBC_HPP
#define HABSAT_OBC_OBC_HPP

#include "Buses/i2c.hpp"
#include "Buses/spi.hpp"
#include "Buses/uart.hpp"
#include "Lcd/lcd.hpp"
#include "MCU_Board/button.hpp"
#include "MCU_Board/led.hpp"
#include "Rtc/rtc.hpp"
#include "SDReader/sd_reader.hpp"
#include "Sensor/sensor.hpp"
#include "System/mutex.hpp"
#include "System/queue.hpp"
#include "System/task.hpp"
#include "Tasks/blink.hpp"
#include "Tasks/display.hpp"
#include "Tasks/measure_time.hpp"
#include "Tasks/measure_weather.hpp"
#include "Tasks/sd_save.hpp"
#include "Utils/noncopyable.hpp"
#include "constants.hpp"
#include "extern_handles.hpp"

namespace habsat {

struct Obc : public utils::Noncopyable {
    buses::I2C i2c;
    buses::SPI spi;
    buses::UART uart;

    mcuBoard::GPIOPin pinC13;
    mcuBoard::GPIOPin pinA5;
    mcuBoard::GPIOPin sensorCS;
    mcuBoard::Button button;
    mcuBoard::Led led;

    lcd::Lcd lcd;
    rtc::Rtc rtc;
    sensor::Sensor sensor;
    sdReader::SDReader sdReader;

    rtc::Buffer rtcBuffer;
    sensor::Buffer sensorBuffer;

    system::Task inputTask;
    system::Task blinkTask;
    system::Task displayTask;
    system::Task measureTimeTask;
    system::Task measureWeatherTask;
    system::Task sdSaveTask;

    Obc();
    void init();
};

auto obc() -> Obc&;

}  // namespace habsat
#endif  // HABSAT_OBC_OBC_HPP
