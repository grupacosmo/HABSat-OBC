//
// Created by vixu on 5/9/21.
//

#ifndef HABSAT_OBC_TRANSMITTER_HPP
#define HABSAT_OBC_TRANSMITTER_HPP

#include <SX1278.h>

namespace loraSX1278 {

class Transmitter {
   public:

   private:
    SX1278_hw_t sx1278hw;
    SX1278_t sx1278;
};

}  // namespace loraSX1278

#endif  // HABSAT_OBC_TRANSMITTER_HPP
