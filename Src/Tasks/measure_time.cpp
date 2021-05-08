//
// Created by Wiktor Wieclaw on 11/30/20.
//

#include "Tasks/measure_time.hpp"

#include "hardware_config.hpp"
#include "obc.hpp"

namespace habsat::measureTime {

void taskFn([[maybe_unused]] void* args) {
    // clang-format off
#   if HW_TERMINAL
        static constexpr std::array dayNames = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};
        auto iter = 0;
#   endif
    // clang-format on

    auto& obc = getObc();

    while (true) {
        obc.rtc.readTimeAndDate(obc.rtcBuffer);

        // clang-format off
#       if HW_TERMINAL
            if (iter == 0) {
                std::array<char, 50> text{};

                std::sprintf(
                      text.data(),
                      "%'.02d:%'.02d:%'.02d %s %'.02d/%'.02d/%'.02d\r\n\r\n",
                      obc.rtcBuffer.hour,
                      obc.rtcBuffer.minute,
                      obc.rtcBuffer.second,
                      dayNames[obc.rtcBuffer.weekday],
                      obc.rtcBuffer.day,
                      obc.rtcBuffer.month,
                      obc.rtcBuffer.year);

                obc.terminal.pcTransmitDMA(text.data());
            }
            iter = (iter + 1) % 4;
#       endif
        // clang-format on

        system::thisTask::delay(600);
    }
}

}  // namespace habsat::tasks::measureTime