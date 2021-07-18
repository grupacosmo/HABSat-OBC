//
// Created by Filip on 20.04.2021.
//

#include "Inc/Gps/gps.hpp"

namespace habsat::gps {

Gps::Gps(UART_HandleTypeDef& uart, DMA_HandleTypeDef& dma_usart_rx)
    : uart(uart), dmaRx(dma_usart_rx) {
    init();
}

void gps::Gps::init() {
    __HAL_UART_ENABLE_IT(&uart, UART_IT_IDLE);
    HAL_UART_Receive_DMA(&uart, reinterpret_cast<uint8_t*>(rawBuffor), rawBufferSize);
}

void gps::Gps::callback() {
    HAL_UART_DMAStop(&uart);

    rawMessageBuffor = std::string(reinterpret_cast<char*>(rawBuffor));

    gpsMessages = GpsMessages{rawMessageBuffor};

    gpsData = GpsData{gpsMessages};

    preparePrint();

    // transmission restart
    HAL_UART_Receive_DMA(&uart, rawBuffor, rawBufferSize);
}

auto Gps::getData() -> GpsData { return gpsData; }

auto Gps::getPrintedData() -> std::string_view { return GpsDataPrint; }

void Gps::preparePrint() {

    using namespace std;
    std::string dataValid = (gpsData.isDataValid) ? "true" : "false";

    GpsDataPrint = "---  Gps Data  ---\r\n";
    GpsDataPrint += "Time:\t\t"
                    + to_string(gpsData.utcHour) + ":"
                    + to_string(gpsData.utcMin) + ":"
                    + to_string(gpsData.utcSec) + "\r\n";
    GpsDataPrint += "Date:\t\t"
                    + to_string(gpsData.day)
                    + "/" + to_string(gpsData.month)
                    + "/20" +to_string(gpsData.year) + "\r\n";
    GpsDataPrint += "Is Data Valid:\t" + dataValid + "\r\n";
    GpsDataPrint += "Coordinates:\t"
                    + string(1, gpsData.nsIndicator) + " " + to_string(gpsData.latitude)+ ", "
                    + string(1, gpsData.ewIndicator) +" " + to_string(gpsData.longitude) + "\r\n";
    GpsDataPrint += "Speed:\t\t" + to_string(gpsData.speedOverGroundKmPerH) + " Km/H" + "\r\n";
    GpsDataPrint += "Fix Indicator:\t" + habsat::gps::GpsData::getLabelPositionFiXIndicator(gpsData.positionFixIndicator) + "\r\n";
    GpsDataPrint += "Number of SAT:\t" + to_string(gpsData.satellitesUsed) + "\r\n";
    GpsDataPrint += "HDOP:\t\t" + to_string(gpsData.hdop) + "\r\n";
    GpsDataPrint += "MSL Altitude:\t" + to_string(gpsData.mslAltitude)+ " "
                    + string(1, gpsData.mslUnits) + "\r\n";
}

}  // namespace habsat::gps
