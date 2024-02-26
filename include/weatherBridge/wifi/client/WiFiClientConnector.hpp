#pragma once

#include <Arduino.h>
#include "ArduinoLog.h"
#include "ESPAsyncWebServer.h"
#include "weatherBridge/wifi/client/WiFiClientStatus.hpp"

class WiFiClientConnector {
private:
    static constexpr long RECONNECT_INTERVAL_MILLIS = 60000;
    unsigned long lastConnectionRefresh = 0;

public:
    WiFiClientConnector() = default;

    void begin(const String &ssid, const String &password) noexcept;

    WiFiClientStatus loop() noexcept;
};
