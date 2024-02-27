#pragma once

#include "weatherBridge/types.hpp"

#define PWS_WEATHER_UPDATE_INTERVAL_MILLI (60 * 1000)
#define WEATHER_UNDERGROUND_UPDATE_INTERVAL_MILLI (60 * 1000)
#define WINDY_UPDATE_INTERVAL_MILLI (2 * 60 * 1000)
#define WIND_GURU_UPDATE_INTERVAL (60 * 1000)

class WeatherBridgeContext;

class WeatherExporter {
private:
    WeatherExporterStatus pwsWeatherExporterStatus = WeatherExporterStatus::INIT;
    unsigned long pwsWeatherLastUpdated = 0 - PWS_WEATHER_UPDATE_INTERVAL_MILLI;

    WeatherExporterStatus weatherUndergroundExporterStatus = WeatherExporterStatus::INIT;
    unsigned long weatherUndergroundLastUpdated = 0 - WEATHER_UNDERGROUND_UPDATE_INTERVAL_MILLI;

    WeatherExporterStatus windGuruExporterStatus = WeatherExporterStatus::INIT;
    unsigned long windGuruLastUpdated = 0 - WIND_GURU_UPDATE_INTERVAL;

    WeatherExporterStatus windyExporterStatus = WeatherExporterStatus::INIT;
    unsigned long windyLastUpdated = 0 - WINDY_UPDATE_INTERVAL_MILLI;

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