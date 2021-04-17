//
// Created by Jakub Zygmunt on 06.12.2020.
//

#include "Tasks/sd_save.hpp"

#include "hardware_config.hpp"
#include "obc.hpp"

namespace habsat::tasks::sdSave {

void taskFn([[maybe_unused]] void* args) {
    auto& obc = getObc();
    std::array<std::array<char, 100>, 3> buffer{};
    const char* path{"test.txt"};

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

        obc.sdReader.update(path, buffer[1].data());
        obc.sdReader.update(path, buffer[2].data());

        system::thisTask::delay(1000);
    }
}

}  // namespace habsat::tasks::sdSave