#pragma once

#include <Arduino.h>

class WiFiAPStatus {
private:
    bool isActiveFlag = false;
    String ssid = "";
    String password = "";
    String ip = "";

public:

    WiFiAPStatus() = default;

    WiFiAPStatus(String &&ssid, String &&password, String &&ip) noexcept;

    WiFiAPStatus(WiFiAPStatus &&other) noexcept;

    // Move assignment operator
    WiFiAPStatus &operator=(WiFiAPStatus &&other) noexcept;

    bool isActive() const noexcept;

    const String &getSsid() const noexcept;

    const String &getPassword() const noexcept;

    const String &getIp() const noexcept;
};
