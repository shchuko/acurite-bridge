#pragma once

#include <Arduino.h>
#include "ArduinoLog.h"
#include "ESPAsyncWebServer.h"
#include "WiFiAPStatus.hpp"

class WiFiAP {
public:
    WiFiAP() = default;

    static WiFiAPStatus begin();

private:
    static void generateAndFillSettings(char *ssid, char *password);
};
