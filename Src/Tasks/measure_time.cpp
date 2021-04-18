//
// Created by Wiktor Wieclaw on 11/30/20.
//

#include "Tasks/measure_time.hpp"

#include "obc.hpp"

namespace habsat::tasks::measureTime {

void taskFn([[maybe_unused]] void* args) {
    static const std::array dayNames = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};

    auto& obc = getObc();

    while (true) {
        obc.rtc.readTimeAndDate(obc.rtcBuffer);

        std::array<char, 50> text{};

        std::sprintf(
              text.data(),
              "%'.02d:%'.02d:%'.02d\n%s %'.02d/%'.02d/%'.02d\n",
              obc.rtcBuffer.hour,
              obc.rtcBuffer.minute,
              obc.rtcBuffer.second,
              dayNames[obc.rtcBuffer.weekday],
              obc.rtcBuffer.day,
              obc.rtcBuffer.month,
              obc.rtcBuffer.year);

        system::thisTask::delay(500);
    }
}

}  // namespace habsat::tasks::measureTime