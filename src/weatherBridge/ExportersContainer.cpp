#include "weatherBridge/ExportersContainer.hpp"
#include "weatherBridge/WeatherBridgeContext.hpp"

void ExportersContainer::loop(const WeatherBridgeContext &context) {
    pwsWeatherExporterStatus = pwsWeatherExport(context);
    weatherUndergroundExporterStatus = weatherUndergroundExport(context);
    windGuruExporterStatus = windyGuruExport(context);
    windyExporterStatus = windyExport(context);
}

WeatherExporterStatus ExportersContainer::pwsWeatherExport(const WeatherBridgeContext &context) {
    const WeatherBridgeSettings &settings = context.settings;
    const String &apiKey = settings.getPwsWeatherApiKey();
    const String &stationId = settings.getPwsWeatherStationId();
    if (apiKey.isEmpty() || !stationId.isEmpty()) {
        return WeatherExporterStatus::OFF;
    }

    return WeatherExporterStatus::ERROR;
}

WeatherExporterStatus ExportersContainer::weatherUndergroundExport(const WeatherBridgeContext &context) {
    const WeatherBridgeSettings &settings = context.settings;
    const String &stationId = settings.getWuStationId();
    const String &apiKey = settings.getWuApiKey();
    if (stationId.isEmpty() || apiKey.isEmpty()) {
        return WeatherExporterStatus::OFF;
    }

    return WeatherExporterStatus::ERROR;
}

WeatherExporterStatus ExportersContainer::windyGuruExport(const WeatherBridgeContext &context) {
    const WeatherBridgeSettings &settings = context.settings;
    const String &stationUid = settings.getWindGuruStationUid();
    const String &password = settings.getWindGuruStationPassword();
    if (stationUid.isEmpty() || password.isEmpty()) {
        return WeatherExporterStatus::OFF;
    }

    return WeatherExporterStatus::ERROR;
}

WeatherExporterStatus ExportersContainer::windyExport(const WeatherBridgeContext &context) {
    const WeatherBridgeSettings &settings = context.settings;
    const String &stationId = settings.getWindyStationId();
    const String &apiKey = settings.getWindyApiKey();
    if (stationId.isEmpty() || apiKey.isEmpty()) {
        return WeatherExporterStatus::OFF;
    }

    return WeatherExporterStatus::ERROR;
}

WeatherExporterStatus ExportersContainer::getPwsWeatherExporterStatus() const {
    return pwsWeatherExporterStatus;
}

WeatherExporterStatus ExportersContainer::getWeatherUndergroundExporterStatus() const {
    return weatherUndergroundExporterStatus;
}

WeatherExporterStatus ExportersContainer::getWindGuruExporterStatus() const {
    return windGuruExporterStatus;
}

WeatherExporterStatus ExportersContainer::getWindyExporterStatus() const {
    return windyExporterStatus;
}
