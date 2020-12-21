//
// Created by Jakub Zygmunt on 19.12.2020.
//

#ifndef HABSAT_OBC_TERMINAL_H
#define HABSAT_OBC_TERMINAL_H

#include <cstring>
#include <string_view>

#include "UARTBus.h"

class Terminal : public Noncopyable{
 public:
  static void pcTransmit(std::string_view message);
  static void pcTransmitDMA(std::string_view message);
  static void pcReceiveDma(char* message);

 private:
  constexpr static hw::UARTBus uart_{hw::Handles::uart2};
};

#endif  // HABSAT_OBC_TERMINAL_H
