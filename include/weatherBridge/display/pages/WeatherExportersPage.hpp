#pragma once

#include "Adafruit_GFX.h"
#include "weatherBridge/display/DisplayPage.hpp"
#include "weatherBridge/types.hpp"

class WeatherExportersPage : public DisplayPage {
public:
    explicit WeatherExportersPage(Adafruit_GFX &display);

private:
    void paint(WeatherBridgeContext context) override;

private:
    static String getWeatherExporterStatusName(WeatherExporterStatus status);
};