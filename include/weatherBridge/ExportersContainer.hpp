#pragma once

#include "weatherBridge/types.hpp"

class WeatherBridgeContext;

class ExportersContainer {
private:
    WeatherExporterStatus pwsWeatherExporterStatus = WeatherExporterStatus::OFF;
    WeatherExporterStatus weatherUndergroundExporterStatus = WeatherExporterStatus::OFF;
    WeatherExporterStatus windGuruExporterStatus = WeatherExporterStatus::OFF;
    WeatherExporterStatus windyExporterStatus = WeatherExporterStatus::OFF;

public:
    void loop(const WeatherBridgeContext &context);

    WeatherExporterStatus getPwsWeatherExporterStatus() const;

    WeatherExporterStatus getWeatherUndergroundExporterStatus() const;

    WeatherExporterStatus getWindGuruExporterStatus() const;

    WeatherExporterStatus getWindyExporterStatus() const;

private:
    WeatherExporterStatus pwsWeatherExport(const WeatherBridgeContext &context);

    WeatherExporterStatus weatherUndergroundExport(const WeatherBridgeContext &context);

    WeatherExporterStatus windyGuruExport(const WeatherBridgeContext &context);

    WeatherExporterStatus windyExport(const WeatherBridgeContext &context);
};