//
// Created by Filip on 15.07.2021.
//

#ifndef HABSAT_OBC_GPSMESSAGES_HPP
#define HABSAT_OBC_GPSMESSAGES_HPP

#include <string>

namespace habsat::gps {

struct GpsMessages {
    std::string Rmc;
    std::string Vtg;
    std::string Gga;
    std::string Gsa;
    std::string Gsv;
    std::string Gll;

    GpsMessages();

    GpsMessages(std::string & rawFrames);

    void print();

};

}

#endif  // HABSAT_OBC_GPSMESSAGES_HPP
