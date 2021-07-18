//
// Created by Jakub Zygmunt on 06.12.2020.
//

#include "Tasks/sd_save.hpp"
#include "hardware_config.hpp"
#include "obc.hpp"

namespace habsat::sdSave {

void taskFn([[maybe_unused]] void* args) {
    auto& obc = getObc();
    std::array<std::array<char, 100>, 3> buffer{};
    std::array<char, 1000> cardMemoryInfo{};
    std::pair<uint32_t, uint32_t> sdCardSpace;
    const char* path{"test1.txt"};

    if (obc.sdReader.mount() != FR_OK) {
        system::thisTask::suspend();
    }

    obc.sdReader.format();
    formatFileHeader(buffer[0]);
    obc.sdReader.makeFile(path);
    obc.sdReader.update(path, buffer[0].data());
    system::thisTask::delay(1000);

    while (true) {
        // clang-format off
#       if HW_RTC
            formatRtcData(buffer[1], obc.rtcBuffer);
#       endif
#       if HW_SENSOR
            formatSensorData(buffer[2], obc.sensorBuffer);
#       endif
        // clang-format on

        //sdCardSpace = obc.sdReader.checkFreeSpace();
        //std::sprintf(cardMemoryInfo.data(), "memory: %lu kB  /  %lu kB \r\n", sdCardSpace.first);
        //Terminal::pcTransmitDMA(cardMemoryInfo.data());
        obc.sdReader.update(path, buffer[1].data());
        obc.sdReader.update(path, buffer[2].data());
        system::thisTask::delay(1000);
    }
}

}  // namespace habsat::tasks::sdSave