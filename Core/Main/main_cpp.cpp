//
// Created by Wiktor Wieclaw on 8/6/20.
//


#include "main_cpp.h"
#include "obc.h"

Obc obc;

void main_cpp()
{
    obc.init();

    os::Task::start_scheduler();
}
