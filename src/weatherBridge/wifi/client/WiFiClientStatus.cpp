#include "weatherBridge/wifi/client/WiFiClientStatus.hpp"

WiFiClientStatus::WiFiClientStatus(WifiSignal signalLevel, String &&ssid) : signalLevel(signalLevel), ssid(ssid) {}

WiFiClientStatus::WiFiClientStatus(WiFiClientStatus &&other) noexcept: signalLevel(other.signalLevel), ssid(std::move(other.ssid)) {}

WiFiClientStatus &WiFiClientStatus::operator=(WiFiClientStatus &&other) noexcept {
    if (this != &other) {
        this->signalLevel = other.signalLevel;
        this->ssid = std::move(other.ssid);
    }
    return *this;
}

WifiSignal WiFiClientStatus::getSignalLevel() const {
    return signalLevel;
}

const String &WiFiClientStatus::getSsid() const {
    return ssid;
}
