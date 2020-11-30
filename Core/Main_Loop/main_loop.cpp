//
// Created by Wiktor Wieclaw on 8/6/20.
//

#include "main_loop.h"
#include "obc.h"

void mainLoop()
{
    static Obc obc;
    obc.init();
    os::Task::startScheduler();
}
