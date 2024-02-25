#pragma once

#include "Adafruit_GFX.h"
#include "weatherBridge/display/DisplayPage.hpp"


class DateTimePage : public DisplayPage {
public:
    explicit DateTimePage(Adafruit_GFX &display);

private:
    void paint(WeatherBridgeContext context) override;
};