//
// Created by Wiktor Wieclaw on 10/1/20.
//
#include "obc.h"

Obc& obc()
{
    static Obc onBoardComputer;
    return onBoardComputer;
}

void Obc::init()
{
    hardware.init();
}
