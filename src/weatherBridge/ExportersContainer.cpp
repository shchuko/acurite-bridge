#include <HTTPClient.h>
#include "weatherBridge/ExportersContainer.hpp"
#include "weatherBridge/WeatherBridgeContext.hpp"
#include "weatherBridge/units.hpp"

void ExportersContainer::loop(const WeatherBridgeContext &context) {
    pwsWeatherExport(context);
    weatherUndergroundExport(context);
    windyGuruExport(context);
    windyExport(context);
}

void ExportersContainer::pwsWeatherExport(const WeatherBridgeContext &context) {
    const WeatherBridgeSettings &settings = context.settings;
    const String &apiKey = settings.getPwsWeatherApiKey();
    const String &stationId = settings.getPwsWeatherStationId();
    if (apiKey.isEmpty() || stationId.isEmpty()) {
        pwsWeatherExporterStatus = WeatherExporterStatus::OFF;
        return;
    }

    if (millis() - pwsWeatherLastUpdated < PWS_WEATHER_UPDATE_INTERVAL_MILLI) {
        return;
    }

    const MeasurementsStore &measurements = context.measurementsStore;
    if (!measurements.getRssi().hasValue()) {
        pwsWeatherExporterStatus = WeatherExporterStatus::NO_DATA;
        return;
    }

    String request = "https://pwsupdate.pwsweather.com/api/v1/submitwx";
    request += "?ID=" + stationId;
    request += "&PASSWORD=" + apiKey;

    time_t now = time(nullptr);
    tm *t = gmtime(&now);
    static char formattedTime[25];
    strftime(formattedTime, sizeof(formattedTime), "%Y-%m-%d+%H:%M:%S", t);

    request += "&dateutc=";
    request += formattedTime;

    if (measurements.getTemperatureC().hasValue()) {
        request += "&tempf=";
        request += celsiusToFahrenheit(measurements.getTemperatureC().getValue());
    }
    if (measurements.getWindSpeedKmH().hasValue()) {
        request += "&windspeedmph=";
        request += kmPerHourToMilesPerHour(measurements.getWindSpeedKmH().getValue());
    }
    if (measurements.getWindGustKmH().hasValue()) {
        request += "&windgustmph=";
        request += kmPerHourToMilesPerHour(measurements.getWindGustKmH().getValue());
    }
    if (measurements.getWindDirectorDeg().hasValue()) {
        request += "&winddir=";
        request += measurements.getWindDirectorDeg().getValue();
    }
    if (measurements.getHumidity().hasValue()) {
        request += "&humidity=";
        request += measurements.getHumidity().getValue();
    }
    if (measurements.getRainMm().hasValue()) {
        request += "&rainin=";
        request += mmToInch(measurements.getRainMm().getValue());
    }
    request += "&softwaretype=esp32bridge&action=updateraw";

    HTTPClient http;
    http.begin(request);
    int httpCode = http.GET();
    http.end();

    pwsWeatherLastUpdated = millis();

    if (httpCode < 200 || httpCode >= 400) {
        Log.warningln("Failed to perform request %s, code %d", request.c_str(), httpCode);
        pwsWeatherExporterStatus = WeatherExporterStatus::ERROR;
        return;
    }
    Log.traceln("Request %s returned code %d", request.c_str(), httpCode);
    pwsWeatherExporterStatus = WeatherExporterStatus::OK;
}

void ExportersContainer::weatherUndergroundExport(const WeatherBridgeContext &context) {
    const WeatherBridgeSettings &settings = context.settings;
    const String &stationId = settings.getWuStationId();
    const String &apiKey = settings.getWuApiKey();
    if (stationId.isEmpty() || apiKey.isEmpty()) {
        weatherUndergroundExporterStatus = WeatherExporterStatus::OFF;
    }

    if (millis() - weatherUndergroundLastUpdated < WEATHER_UNDERGROUND_UPDATE_INTERVAL_MILLI) {
        return;
    }

    const MeasurementsStore &measurements = context.measurementsStore;
    if (!measurements.getRssi().hasValue()) {
        weatherUndergroundExporterStatus = WeatherExporterStatus::NO_DATA;
        return;
    }

    String request = "https://weatherstation.wunderground.com/weatherstation/updateweatherstation.php";
    request += "?ID=" + stationId;
    request += "&PASSWORD=" + apiKey;
    request += "&dateutc=now";

    if (measurements.getTemperatureC().hasValue()) {
        request += "&tempf=";
        request += celsiusToFahrenheit(measurements.getTemperatureC().getValue());
    }
    if (measurements.getWindSpeedKmH().hasValue()) {
        request += "&windspeedmph=";
        request += kmPerHourToMilesPerHour(measurements.getWindSpeedKmH().getValue());
    }
    if (measurements.getWindGustKmH().hasValue()) {
        request += "&windgustmph=";
        request += kmPerHourToMilesPerHour(measurements.getWindGustKmH().getValue());
    }
    if (measurements.getWindDirectorDeg().hasValue()) {
        request += "&winddir=";
        request += measurements.getWindDirectorDeg().getValue();
    }
    if (measurements.getHumidity().hasValue()) {
        request += "&humidity=";
        request += measurements.getHumidity().getValue();
    }
    if (measurements.getRainMm().hasValue()) {
        request += "&rainin=";
        request += mmToInch(measurements.getRainMm().getValue());
    }
    request += "&softwaretype=esp32bridge&action=updateraw&realtime=1";
    request += "&rtfreq=";
    request += WEATHER_UNDERGROUND_UPDATE_INTERVAL_MILLI / 1000.f;

    HTTPClient http;
    http.begin(request);
    int httpCode = http.GET();
    http.end();

    weatherUndergroundLastUpdated = millis();

    if (httpCode < 200 || httpCode >= 400) {
        Log.warningln("Failed to perform request %s, code %d", request.c_str(), httpCode);
        weatherUndergroundExporterStatus = WeatherExporterStatus::ERROR;
        return;
    }
    Log.traceln("Request %s returned code %d", request.c_str(), httpCode);
    weatherUndergroundExporterStatus = WeatherExporterStatus::OK;
}

void ExportersContainer::windyGuruExport(const WeatherBridgeContext &context) {
    const WeatherBridgeSettings &settings = context.settings;
    const String &stationUid = settings.getWindGuruStationUid();
    const String &password = settings.getWindGuruStationPassword();
    if (stationUid.isEmpty() || password.isEmpty()) {
        windGuruExporterStatus = WeatherExporterStatus::OFF;
    }

    windGuruExporterStatus = WeatherExporterStatus::ERROR;
}

void ExportersContainer::windyExport(const WeatherBridgeContext &context) {
    const WeatherBridgeSettings &settings = context.settings;
    const String &stationId = settings.getWindyStationId();
    const String &apiKey = settings.getWindyApiKey();
    if (stationId.isEmpty() || apiKey.isEmpty()) {
        windyExporterStatus = WeatherExporterStatus::OFF;
    }

    windyExporterStatus = WeatherExporterStatus::ERROR;
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
