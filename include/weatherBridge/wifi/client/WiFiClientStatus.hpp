#pragma once

#include <Arduino.h>
#include "weatherBridge/types.hpp"

class WiFiClientStatus {
private:
    bool isConnectedFlag = false;
    int rssi = 0;
    String ssid = "";

public:
    WiFiClientStatus() = default;

    explicit WiFiClientStatus(String &&ssid, bool isConnected, int rssi);

    WiFiClientStatus(WiFiClientStatus &&other) noexcept;

    WiFiClientStatus& operator=(WiFiClientStatus &&other) noexcept;

    int getRSSI() const;

    bool isConnected() const;

    const String &getSsid() const;
};
