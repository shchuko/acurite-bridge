#pragma once

#include "Adafruit_GFX.h"
#include "weatherBridge/WeatherBridgeContext.hpp"

class DisplayPage {
protected:
    Adafruit_GFX &delegate;

    explicit DisplayPage(Adafruit_GFX &display);
public:
    virtual void paint(WeatherBridgeContext context);
};