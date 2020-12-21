//
// Created by Jakub Zygmunt on 19.12.2020.
//

#include "Terminal.h"

void Terminal::pcTransmit(char* message) {
  uart_.transmit(reinterpret_cast<uint8_t*>(message), strlen(message));
}

void Terminal::pcTransmitDMA(char* message){
  uart_.transmitDMA(reinterpret_cast<uint8_t*>(message), strlen(message));
}
void Terminal::pcReceiveDma(char* message) {
  //todo
}
