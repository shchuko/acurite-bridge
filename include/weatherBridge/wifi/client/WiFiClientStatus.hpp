#pragma once

#include <Arduino.h>
#include "weatherBridge/types.hpp"

class WiFiClientStatus {
private:
    WifiSignal signalLevel = WifiSignal::NO_CONNECTION;
    String ssid = "";

public:
    WiFiClientStatus() = default;

    explicit WiFiClientStatus(WifiSignal signalLevel, String &&ssid);

    WiFiClientStatus(WiFiClientStatus &&other) noexcept;

    WiFiClientStatus& operator=(WiFiClientStatus &&other) noexcept;

    WifiSignal getSignalLevel() const;

    const String &getSsid() const;
};
