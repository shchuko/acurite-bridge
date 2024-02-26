#include "weatherBridge/wifi/client/WiFiClientStatus.hpp"

WiFiClientStatus::WiFiClientStatus(String &&ssid, bool isConnected, int rssi) : ssid(ssid),
                                                                                isConnectedFlag(isConnected),
                                                                                rssi(rssi) {}

WiFiClientStatus::WiFiClientStatus(WiFiClientStatus &&other) noexcept: ssid(std::move(other.ssid)),
                                                                       isConnectedFlag(other.isConnectedFlag),
                                                                       rssi(other.rssi) {}

WiFiClientStatus &WiFiClientStatus::operator=(WiFiClientStatus &&other) noexcept {
    if (this != &other) {
        this->isConnectedFlag = other.isConnectedFlag;
        this->rssi = other.rssi;
        this->ssid = std::move(other.ssid);
    }
    return *this;
}

int WiFiClientStatus::getRSSI() const {
    return rssi;
}

bool WiFiClientStatus::isConnected() const {
    return isConnectedFlag;
}

const String &WiFiClientStatus::getSsid() const {
    return ssid;
}
