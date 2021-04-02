//
// Created by Wiktor Wieclaw on 8/6/20.
//

#include "mainLoop.h"

#include "Obc.h"

void mainLoop() {
    obc().init();

    os::startScheduler();
}
