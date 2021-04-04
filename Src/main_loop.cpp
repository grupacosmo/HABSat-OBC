#include "main_loop.hpp"

#include "obc.hpp"

void mainLoop() {
    using namespace habsat;

    getObc().init();

    system::startScheduler();
}
