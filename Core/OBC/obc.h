//
// Created by Wiktor Wieclaw on 10/1/20.
//

#ifndef RCC_SYS_OBC_H
#define RCC_SYS_OBC_H


#include "I2CBus.h"
#include "hardware.h"

/**
 * The main structure of the entire program, contains every part of the obc.
 */
struct Obc
{
    Hardware hardware;

    void init();
};

extern Obc obc;

#endif//RCC_SYS_OBC_H
