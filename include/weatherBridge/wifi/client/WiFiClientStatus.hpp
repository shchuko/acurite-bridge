#pragma once

#include <Arduino.h>
#include "weatherBridge/types.hpp"

class WiFiClientStatus {
private:
    int rssi = 0;
    WifiSignal signalLevel = WifiSignal::NO_CONNECTION;
    String ssid = "";

public:
    WiFiClientStatus() = default;

    explicit WiFiClientStatus(WifiSignal signalLevel, String &&ssid, int rssi);

    WiFiClientStatus(WiFiClientStatus &&other) noexcept;

    WiFiClientStatus& operator=(WiFiClientStatus &&other) noexcept;

    int getRSSI() const;

    WifiSignal getSignalLevel() const;

    const String &getSsid() const;
};
