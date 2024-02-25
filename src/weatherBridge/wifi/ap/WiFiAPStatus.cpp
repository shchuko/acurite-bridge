#include "weatherBridge/wifi/ap/WiFiAPStatus.hpp"


WiFiAPStatus::WiFiAPStatus(String &&ssid, String &&password, String &&ip) noexcept: isActiveFlag(true),
                                                                                    ssid(ssid),
                                                                                    password(password),
                                                                                    ip(ip) {}

WiFiAPStatus::WiFiAPStatus(WiFiAPStatus &&other) noexcept: isActiveFlag(other.isActiveFlag),
                                                           ssid(std::move(other.ssid)),
                                                           password(std::move(other.password)),
                                                           ip(std::move(other.ip)) {}

WiFiAPStatus &WiFiAPStatus::operator=(WiFiAPStatus &&other) noexcept {
    if (this != &other) {
        isActiveFlag = other.isActiveFlag;
        ssid = std::move(other.ssid);
        password = std::move(other.password);
        ip = std::move(other.ip);
    }
    return *this;
}

bool WiFiAPStatus::isActive() const noexcept {
    return isActiveFlag;
}

const String &WiFiAPStatus::getSsid() const noexcept {
    return ssid;
}

const String &WiFiAPStatus::getPassword() const noexcept {
    return password;
}

const String &WiFiAPStatus::getIp() const noexcept {
    return ip;
}
