//
// Created by Wiktor Wieclaw on 10/1/20.
//

#ifndef RCC_SYS_OBC_H
#define RCC_SYS_OBC_H

#include "peripherals.h"
#include "utils.h"

/**
 * The main structure of the entire program, contains every part of the obc.
 */
struct Obc
{
    Peripherals peripherals;
    Utils utils;

    void init();
};

extern Obc obc;

#endif//RCC_SYS_OBC_H
