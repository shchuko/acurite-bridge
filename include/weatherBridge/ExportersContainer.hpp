#pragma once

#include "weatherBridge/types.hpp"

#define PWS_WEATHER_UPDATE_INTERVAL_MILLI (60 * 1000)

class WeatherBridgeContext;

class ExportersContainer {
private:
    WeatherExporterStatus pwsWeatherExporterStatus = WeatherExporterStatus::INIT;
    unsigned long pwsWeatherLastUpdated = 0 - PWS_WEATHER_UPDATE_INTERVAL_MILLI;

    WeatherExporterStatus weatherUndergroundExporterStatus = WeatherExporterStatus::INIT;
    unsigned long weatherUndergroundLastUpdated = 0;

    WeatherExporterStatus windGuruExporterStatus = WeatherExporterStatus::INIT;
    unsigned long windGuruLastUpdated = 0;

    WeatherExporterStatus windyExporterStatus = WeatherExporterStatus::INIT;
    unsigned long windyLastUpdated = 0;

public:
    void loop(const WeatherBridgeContext &context);

    WeatherExporterStatus getPwsWeatherExporterStatus() const;

    WeatherExporterStatus getWeatherUndergroundExporterStatus() const;

    WeatherExporterStatus getWindGuruExporterStatus() const;

    WeatherExporterStatus getWindyExporterStatus() const;

private:
    void pwsWeatherExport(const WeatherBridgeContext &context);

    void weatherUndergroundExport(const WeatherBridgeContext &context);

    void windyGuruExport(const WeatherBridgeContext &context);

    void windyExport(const WeatherBridgeContext &context);
};