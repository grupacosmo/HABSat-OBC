//
// Created by Filip on 15.07.2021.
//

#include "Inc/Gps/gpsMessages.hpp"

namespace habsat::gps {

void GpsMessages::print() {
    Terminal::pcTransmit("---  Gps Messages  ---\r\n");
    Terminal::pcTransmit(this->Rmc+"\r\n");
    Terminal::pcTransmit(this->Vtg+"\r\n");
    Terminal::pcTransmit(this->Gga+"\r\n");
    Terminal::pcTransmit(this->Gsa+"\r\n");
    Terminal::pcTransmit(this->Gsv+"\r\n");
    Terminal::pcTransmit(this->Gll+"\r\n");
}

GpsMessages::GpsMessages(std::string & rawFrames) {
    while(!rawFrames.empty()){
        auto beginOfMessage = rawFrames.find("$");
        if(beginOfMessage == std::string::npos)
            break;
        rawFrames = rawFrames.substr(beginOfMessage, rawFrames.length());
        auto endOfMessage = rawFrames.find("\r\n");
        if(endOfMessage == std::string::npos)
            break;
        std::string singleMessage = rawFrames.substr(0, endOfMessage);
        auto type = singleMessage.substr(3,3);

        if(type == "RMC")
            this->Rmc = singleMessage;
        else if(type == "VTG")
            this->Vtg = singleMessage;
        else if(type == "GGA")
            this->Gga = singleMessage;
        else if(type == "GSA")
            this->Gsa = singleMessage;
        else if(type == "GSV")
            this->Gsv = singleMessage;
        else if(type == "GLL")
            this->Gll = singleMessage;
        else
            break;
        rawFrames.erase(0, endOfMessage+2);
    }
}
GpsMessages::GpsMessages() {}

}



