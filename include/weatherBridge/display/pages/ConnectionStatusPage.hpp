#pragma once

#include "Adafruit_GFX.h"
#include "weatherBridge/display/DisplayPage.hpp"
#include "weatherBridge/types.hpp"

class ConnectionStatusPage : public DisplayPage {
public:
    explicit ConnectionStatusPage(Adafruit_GFX &display);

private:
    void paint(WeatherBridgeContext context) override;

private:
    static const char *rssiToSignalString(int rssi) noexcept;
};