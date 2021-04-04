//
// Created by Wiktor Wieclaw on 11/16/20.
//

#ifndef HABSAT_OBC_BUSES_SDIO_HPP
#define HABSAT_OBC_BUSES_SDIO_HPP

#include "result.hpp"
#include "stm32f4xx.h"

namespace habsat::buses {

class SDIOBus {
   public:
    explicit SDIOBus(SD_HandleTypeDef* handle) : handle_(handle) {}

   private:
    static constexpr uint32_t defaultTimeout_ = 100;
    SD_HandleTypeDef* handle_;
};

}  // namespace habsat::buses

#endif  // HABSAT_OBC_BUSES_SDIO_HPP
