//
// Created by Wiktor Wieclaw on 10/1/20.
//

#ifndef RCC_SYS_OBC_H
#define RCC_SYS_OBC_H

#include "peripherals.h"
#include "utils.h"

struct Obc
{
    Peripherals peripherals;
    Utils utils;

public:
    void init();
};

extern Obc obc;

#endif//RCC_SYS_OBC_H
