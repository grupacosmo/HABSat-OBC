//
// Created by Jakub Zygmunt on 19.12.2020.
//

#include "Terminal.h"

void Terminal::pcTransmit(std::string_view message) {
  uart_.transmit(reinterpret_cast<uint8_t*>(const_cast<char*>(message.data())), message.size());
}

void Terminal::pcTransmitDMA(std::string_view message){
  uart_.transmitDMA(reinterpret_cast<uint8_t*>(const_cast<char*>(message.data())), message.size());
}
void Terminal::pcReceiveDma(char* message) {
  //todo
}
