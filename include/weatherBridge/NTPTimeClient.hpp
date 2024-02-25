#pragma once

#include "weatherBridge/WeatherBridgeSettings.hpp"

class NTPTimeClient {
public:
    static void begin(WeatherBridgeSettings &settings);

    static bool checkStatus();
private:

    static void setInitialTime(const char *timezoneString);
};