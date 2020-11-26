//
// Created by Wiktor Wieclaw on 8/6/20.
//

#include "main_loop.h"
#include "obc.h"

void mainLoop()
{
    obc().init();
    os::Task::startScheduler();
}
