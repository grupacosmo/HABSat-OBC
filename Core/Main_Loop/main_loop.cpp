//
// Created by Wiktor Wieclaw on 8/6/20.
//

#include "main_loop.h"
#include "obc.h"

Obc obc;

void mainLoop()
{
    obc.init();

    os::Task::start_scheduler();
}
