//
// Created by Jakub Zygmunt on 06.12.2020.
//

#include "Tasks/sd_save.hpp"

#include "Filesystem/file.hpp"
#include "hardware_config.hpp"
#include "obc.hpp"

namespace habsat::sdSave {

// todo hardware config
// todo error handling
void taskFn([[maybe_unused]] void* args) {
    auto& obc = getObc();

    filesystem::File f{};

    using Mode = filesystem::File::Mode;
    const auto mode = Mode::OpenExisting | Mode::Write | Mode::Read;

    if (f.open("test.txt", mode) != filesystem::Result::Ok) {
        f.write("hello");
    }

    while (true) {
        system::thisTask::delay(1000);
    }
}

}  // namespace habsat::sdSave