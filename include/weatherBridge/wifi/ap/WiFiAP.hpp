#pragma once

#include <Arduino.h>
#include "ArduinoLog.h"
#include "ESPAsyncWebServer.h"
#include "WiFiAPStatus.hpp"
#include "weatherBridge/WeatherBridgeSettings.hpp"

class WiFiAP {
public:
    WiFiAP() = default;

    static WiFiAPStatus begin(WeatherBridgeSettings &settings);
};
