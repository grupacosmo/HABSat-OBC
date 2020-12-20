//
// Created by Wiktor Wieclaw on 8/6/20.
//

#include "mainLoop.h"

#include "Obc.h"
#include "Gps.h"

extern UART_HandleTypeDef huart1;
gps_state gps_handle;
uint8_t recv_char;

void HAL_UART_RxCpltCallback(UART_HandleTypeDef * uart) {
  if (uart == &huart1) {
    gps_recv_char(&gps_handle, recv_char);
    HAL_UART_Receive_IT(&huart1, &recv_char, 1);
  }
}

void mainLoop() {
  static Obc obc;
  obc.init();


  gps_handle = gps_init(&huart1);
  HAL_UART_Receive_IT(&huart1, &recv_char, 1);

  auto& lcd = obc.hware.lcd;
  while (1) {
    lcd.printLine(1, std::to_string(gps_handle.time_hour));
    lcd.printLine(2, std::to_string(gps_handle.time_min));
    lcd.printLine(3, std::to_string(gps_handle.time_sec));
    HAL_Delay(1000);
  }

  HAL_Delay(10000);
  os::Task::startScheduler();
}
