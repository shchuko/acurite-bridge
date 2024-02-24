#pragma once

#include <Arduino.h>
#include <ArduinoLog.h>
#include <ESPAsyncWebServer.h>
#include "context.hpp"

class ConfigurationModeWiFi {
public:
    ConfigurationModeWiFi() = default;

#pragma clang diagnostic push
#pragma ide diagnostic ignored "readability-convert-member-functions-to-static"

    void begin();

private:
    static void generateAndFillSettings(char *ssid, char *password);
};
