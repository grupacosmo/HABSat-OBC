#include "main_loop.hpp"

#include "obc.hpp"

void mainLoop() {
    using namespace habsat;

    obc().init();

    system::startScheduler();
}
