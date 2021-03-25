//
// Created by Wiktor Wieclaw on 11/16/20.
//

#ifndef HABSAT_OBC_SDIOBUS_H
#define HABSAT_OBC_SDIOBUS_H

#include "Bus.h"

namespace hw {

class SDIOBus : public Bus<SDIOHandle> {
 public:
  constexpr explicit SDIOBus(SDIOHandle* handle) : Bus(handle) {}

 private:
  static constexpr uint32_t defaultTimeout_ = 100;
};

}  // namespace hw
#endif  // HABSAT_OBC_SDIOBUS_H
