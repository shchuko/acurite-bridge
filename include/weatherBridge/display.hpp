#pragma once

#include "Adafruit_SSD1306.h"
#include "TimeLib.h"

#include "types.hpp"
#include "context.hpp"


template<typename T, size_t N>
static size_t ArraySize(T (&)[N]);

class DisplayPage {
protected:
    Adafruit_GFX &delegate;

    explicit DisplayPage(Adafruit_GFX &display) : delegate(display) {};
public:
    virtual void paint() {}
};

class ConnectionStatusPage : public DisplayPage {
public:
    explicit ConnectionStatusPage(Adafruit_GFX &display);

private:
    void paint() override;
private:
    static String getWifiSignalName(WifiSignal signal);

    static String getStationSignalName(StationSignal signal);
};

class DateTimePage : public DisplayPage {
public:
    explicit DateTimePage(Adafruit_GFX &display);

private:
    void paint() override;
};

class WeatherPage : public DisplayPage {
public:
    explicit WeatherPage(Adafruit_GFX &display);

private:
    void paint() override;
};

class WeatherExportersPage : public DisplayPage {
public:
    explicit WeatherExportersPage(Adafruit_GFX &display);

private:
    void paint() override;

private:
    static String getWeatherExporterStatusName(WeatherExporterStatus status);
};

class ICMPChecksPage : public DisplayPage {
public:
    explicit ICMPChecksPage(Adafruit_GFX &display);

private:
    void paint() override;
};

class WeatherBridgeDisplay {
private:
    Adafruit_SSD1306 delegate;
    DisplayPage *pages[5] {
            new ConnectionStatusPage(delegate),
            new DateTimePage(delegate),
            new WeatherPage(delegate),
            new WeatherExportersPage(delegate),
            new ICMPChecksPage(delegate),
    };

    size_t nextPageIndex = 0;
    unsigned long lastPageSwitchMillis = millis();

public:
    WeatherBridgeDisplay();

    ~WeatherBridgeDisplay();

    void begin();

    void refresh();

    void nextPage();

private:
    void paintSplash();
};

