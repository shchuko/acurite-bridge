#pragma once

#include "Adafruit_SSD1306.h"

#include "weatherBridge/types.hpp"
#include "weatherBridge/WeatherBridgeContext.hpp"
#include "weatherBridge/display/pages/ConnectionStatusPage.hpp"
#include "weatherBridge/display/pages/DateTimePage.hpp"
#include "weatherBridge/display/pages/WeatherPage.hpp"
#include "weatherBridge/display/pages/WeatherExportersPage.hpp"


class WeatherBridgeDisplay {
private:
    Adafruit_SSD1306 delegate;
    DisplayPage *pages[4]{
            new ConnectionStatusPage(delegate),
            new DateTimePage(delegate),
            new WeatherPage(delegate),
            new WeatherExportersPage(delegate),
    };

    size_t nextPageIndex = 0;
    unsigned long lastPageSwitchMillis = millis();

public:
    WeatherBridgeDisplay();

    ~WeatherBridgeDisplay();

    void begin();

    void refresh(WeatherBridgeContext context);

private:
    void nextPage(WeatherBridgeContext context);

    void paintSplash();
};

