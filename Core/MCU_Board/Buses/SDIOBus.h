//
// Created by Wiktor Wieclaw on 11/16/20.
//

#ifndef HABSAT_OBC_SDIOBUS_H
#define HABSAT_OBC_SDIOBUS_H

#include "Bus.h"
#include "extern_handles.h"

namespace mcuBoard {

class SDIOBus {
   public:
    explicit SDIOBus(SD_HandleTypeDef* handle) : handle_(handle) {}

   private:
    static constexpr uint32_t defaultTimeout_ = 100;
    SD_HandleTypeDef* handle_;
};

}  // namespace mcuBoard
#endif  // HABSAT_OBC_SDIOBUS_H
