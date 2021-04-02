//
// Created by Wiktor Wieclaw on 11/30/20.
//

#include "Display.h"

#include "../Obc.h"

namespace display::impl {

void formatHeaderData(std::array<char, 20>& lineBuffer) {
    constexpr std::array options{"display", "test"};
    static std::size_t index;

    std::sprintf(lineBuffer.data(), "%s", options[index]);
    index = (index + 1) % options.size();
}

void formatTimeData(std::array<char, 20>& lineBuffer, const rtc::Buffer& buf) {
    std::sprintf(lineBuffer.data(), "      %'.02d:%'.02d:%'.02d", buf.hour, buf.minute, buf.second);
}

void formatDateData(std::array<char, 20>& lineBuffer, const rtc::Buffer& buf) {
    static const std::array dayNames = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};

    std::sprintf(
          lineBuffer.data(),
          "    %s %'.02d/%'.02d/%'.02d",
          dayNames[buf.weekday],
          buf.day,
          buf.month,
          buf.year);
}

void formatSensorData(std::array<char, 20>& lineBuffer, const sensor::Buffer& buf) {
    static const std::array bufferArray{&buf.temperature, &buf.pressure, &buf.humidity};

    static constexpr std::array options{"Temp: %.2lf C", "Press: %.2lf hPa", "Hum: %.2lf %%RH"};
    static std::size_t index = 0;

    std::sprintf(lineBuffer.data(), options[index], *bufferArray[index]);
    index = (index + 1) % options.size();
}

}  // namespace display::impl

namespace display {

using namespace impl;

[[noreturn]] void taskFn(void* args) {
    auto obc = static_cast<Obc*>(args);
    std::array<std::array<char, 20>, 4> lineBuffers{};

    while (true) {
        formatHeaderData(lineBuffers[0]);
        formatTimeData(lineBuffers[1], obc->rtcBuffer);
        formatDateData(lineBuffers[2], obc->rtcBuffer);
        formatSensorData(lineBuffers[3], obc->sensorBuffer);

        for (std::size_t i = 0; i < 4; ++i) {
            obc->lcd.printLine(i, lineBuffers[i].data());
        }

        os::thisTask::delay(980);
    }
}

}  // namespace display