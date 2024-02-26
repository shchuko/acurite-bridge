#include "weatherBridge/wifi/client/WiFiClientStatus.hpp"

WiFiClientStatus::WiFiClientStatus(WifiSignal signalLevel, String &&ssid, int rssi) : signalLevel(signalLevel),
                                                                                      ssid(ssid), rssi(rssi) {}

WiFiClientStatus::WiFiClientStatus(WiFiClientStatus &&other) noexcept: signalLevel(other.signalLevel),
                                                                       ssid(std::move(other.ssid)), rssi(other.rssi) {}

WiFiClientStatus &WiFiClientStatus::operator=(WiFiClientStatus &&other) noexcept {
    if (this != &other) {
        this->signalLevel = other.signalLevel;
        this->rssi = other.rssi;
        this->ssid = std::move(other.ssid);
    }
    return *this;
}

int WiFiClientStatus::getRSSI() const {
    return rssi;
}

WifiSignal WiFiClientStatus::getSignalLevel() const {
    return signalLevel;
}

const String &WiFiClientStatus::getSsid() const {
    return ssid;
}
