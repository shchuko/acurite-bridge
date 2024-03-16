#include <HTTPClient.h>
#include <MD5Builder.h>

#include "weatherBridge/WeatherExporter.hpp"
#include "weatherBridge/WeatherBridgeContext.hpp"
#include "weatherBridge/units.hpp"
#include "weatherBridge/LokiLogger.hpp"

void WeatherExporter::loop(const WeatherBridgeContext &context) {
    if (context.ntpTimeSyncOk) {
        pwsWeatherExport(context);
        weatherUndergroundExport(context);
        windyGuruExport(context);
        windyExport(context);
    } else {
        pwsWeatherExporterStatus = WeatherExporterStatus::NTP_ERR;
        weatherUndergroundExporterStatus = WeatherExporterStatus::NTP_ERR;
        windGuruExporterStatus = WeatherExporterStatus::NTP_ERR;
        windyExporterStatus = WeatherExporterStatus::NTP_ERR;
    }
}

void WeatherExporter::pwsWeatherExport(const WeatherBridgeContext &context) {
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

    // https://github.com/OurColonial/WeatherLink-to-PWSweather
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
    if (measurements.getRainMmLastHour().hasValue()) {
        Log.traceln("Skipping rainin send to pwsweather, value=%f", measurements.getRainMmLastHour().getValue());
//        request += "&rainin=";
//        request += mmToInch(measurements.getRainMmLastHour().getValue());
    }
    request += "&softwaretype=esp32bridge&action=updateraw";

    HTTPClient http;
    http.begin(request);
    int httpCode = http.GET();

    // Avoid sensitive data leak to logs
    request.replace(apiKey, "[REDACTED]"); 

    pwsWeatherLastUpdated = millis();

    String logLine;
    if (httpCode < 200 || httpCode >= 400) {
        logLine = "Export failed ";
        pwsWeatherExporterStatus = WeatherExporterStatus::ERROR;
    } else {
        logLine = "Export OK ";
        pwsWeatherExporterStatus = WeatherExporterStatus::OK;
    }

    logLine += request;
    logLine += " code=";
    logLine += httpCode;
    logLine += " body=";
    logLine += http.getString();

    Log.warningln(logLine.c_str());
    LokiLogger::Instance.writeLog(logLine);
    Log.traceln(logLine.c_str());

    http.end();
}

void WeatherExporter::weatherUndergroundExport(const WeatherBridgeContext &context) {
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

    // https://support.weather.com/s/article/PWS-Upload-Protocol
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
    if (measurements.getRainMmLastHour().hasValue()) {
        Log.traceln("Skipping rainin send to wunderground, value=%f", measurements.getRainMmLastHour().getValue());
//        request += "&rainin=";
//        request += mmToInch(measurements.getRainMmLastHour().getValue());
    }
    request += "&softwaretype=esp32bridge&action=updateraw&realtime=1";
    request += "&rtfreq=";
    request += WEATHER_UNDERGROUND_UPDATE_INTERVAL_MILLI / 1000.f;

    HTTPClient http;
    http.begin(request);
    int httpCode = http.GET();

    // Avoid sensitive data leak to logs
    request.replace(apiKey, "[REDACTED]");

    weatherUndergroundLastUpdated = millis();

    String logLine;
    if (httpCode < 200 || httpCode >= 400) {
        logLine = "Export failed ";
        weatherUndergroundExporterStatus = WeatherExporterStatus::ERROR;
    } else {
        logLine = "Export OK ";
        weatherUndergroundExporterStatus = WeatherExporterStatus::OK;
    }

    logLine += request;
    logLine += " code=";
    logLine += httpCode;
    logLine += " body=";
    logLine += http.getString();

    Log.warningln(logLine.c_str());
    LokiLogger::Instance.writeLog(logLine);
    Log.traceln(logLine.c_str());

    http.end();
}

void WeatherExporter::windyGuruExport(const WeatherBridgeContext &context) {
    const WeatherBridgeSettings &settings = context.settings;
    const String &stationUid = settings.getWindGuruStationUid();
    const String &password = settings.getWindGuruStationPassword();
    if (stationUid.isEmpty() || password.isEmpty()) {
        windGuruExporterStatus = WeatherExporterStatus::OFF;
    }

    if (millis() - windGuruLastUpdated < WIND_GURU_UPDATE_INTERVAL) {
        return;
    }

    const MeasurementsStore &measurements = context.measurementsStore;
    if (!measurements.getRssi().hasValue()) {
        windGuruExporterStatus = WeatherExporterStatus::NO_DATA;
        return;
    }

    // https://stations.windguru.cz/upload_api.php
    String request = "https://www.windguru.cz/upload/api.php";
    request += "?uid=" + stationUid;

    char salt[25];
    sprintf(salt, "%ld", time(nullptr));
    request += "&salt=";
    request += salt;

    MD5Builder hashBuilder{};
    hashBuilder.begin();
    hashBuilder.add(salt + stationUid + password);
    hashBuilder.calculate();
    String hash = hashBuilder.toString();

    request += "&hash=" + hash;

    if (measurements.getTemperatureC().hasValue()) {
        request += "&temperature=";
        request += measurements.getTemperatureC().getValue();
    }
    if (measurements.getWindMinKmH().hasValue()) {
        request += "&wind_min=";
        request += kmPerHourToKnots(measurements.getWindMinKmH().getValue());
    }
    if (measurements.getWindAvgKmH().hasValue()) {
        request += "&wind_avg=";
        request += kmPerHourToKnots(measurements.getWindAvgKmH().getValue());
    }
    if (measurements.getWindGustKmH().hasValue()) {
        request += "&wind_max=";
        request += kmPerHourToKnots(measurements.getWindGustKmH().getValue());
    }
    if (measurements.getWindDirectorDeg().hasValue()) {
        request += "&wind_direction=";
        request += measurements.getWindDirectorDeg().getValue();
    }
    if (measurements.getHumidity().hasValue()) {
        request += "&rh=";
        request += measurements.getHumidity().getValue();
    }
    if (measurements.getRainMmLastHour().hasValue()) {
        Log.traceln("Skipping rainin send to windguru, value=%f", measurements.getRainMmLastHour().getValue());
//        request += "&precip=";
//        request += measurements.getRainMmLastHour().getValue();
    }
    request += "&interval=";
    request += (MeasurementsStore::windAggregatesMeasurementWindow / 1000.f);

    HTTPClient http;
    http.begin(request);
    int httpCode = http.GET();

    // Avoid sensitive data leak to logs
    request.replace(hash, "[REDACTED]");

    windGuruLastUpdated = millis();

    String logLine;
    if (httpCode < 200 || httpCode >= 400) {
        logLine = "Export failed ";
        windGuruExporterStatus = WeatherExporterStatus::ERROR;
    } else {
        logLine = "Export OK ";
        windGuruExporterStatus = WeatherExporterStatus::OK;
    }

    logLine += request;
    logLine += " code=";
    logLine += httpCode;
    logLine += " body=";
    logLine += http.getString();

    Log.warningln(logLine.c_str());
    LokiLogger::Instance.writeLog(logLine);
    Log.traceln(logLine.c_str());

    http.end();
}

void WeatherExporter::windyExport(const WeatherBridgeContext &context) {
    const WeatherBridgeSettings &settings = context.settings;
    const String &stationId = settings.getWindyStationId();
    const String &apiKey = settings.getWindyApiKey();
    if (stationId.isEmpty() || apiKey.isEmpty()) {
        windyExporterStatus = WeatherExporterStatus::OFF;
    }


    if (millis() - windyLastUpdated < WINDY_UPDATE_INTERVAL_MILLI) {
        return;
    }

    const MeasurementsStore &measurements = context.measurementsStore;
    if (!measurements.getRssi().hasValue()) {
        windyExporterStatus = WeatherExporterStatus::NO_DATA;
        return;
    }

    // https://community.windy.com/topic/8168/report-your-weather-station-data-to-windy
    String request = "https://stations.windy.com/pws/update/" + apiKey;
    request += "?stationId=" + stationId;

    time_t now = time(nullptr);
    tm *t = gmtime(&now);
    static char formattedTime[25];
    strftime(formattedTime, sizeof(formattedTime), "%Y-%m-%dT%H:%M:%S", t);

    request += "&time=";
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
    if (measurements.getRainMmLastHour().hasValue()) {
        Log.traceln("Skipping rainin send to windy, value=%f", measurements.getRainMmLastHour().getValue());
//        request += "&rainin=";
//        request += mmToInch(measurements.getRainMmLastHour().getValue());
    }

    HTTPClient http;
    http.begin(request);
    int httpCode = http.GET();

    // Avoid sensitive data leak to logs
    request.replace(apiKey, "[REDACTED]");

    windyLastUpdated = millis();

    String logLine;
    if (httpCode < 200 || httpCode >= 400) {
        logLine = "Export failed ";
        windyExporterStatus = WeatherExporterStatus::ERROR;
    } else {
        logLine = "Export OK ";
        windyExporterStatus = WeatherExporterStatus::OK;
    }

    logLine += request;
    logLine += " code=";
    logLine += httpCode;
    logLine += " body=";
    logLine += http.getString();

    Log.warningln(logLine.c_str());
    LokiLogger::Instance.writeLog(logLine);
    Log.traceln(logLine.c_str());

    http.end();
}

WeatherExporterStatus WeatherExporter::getPwsWeatherExporterStatus() const {
    return pwsWeatherExporterStatus;
}

WeatherExporterStatus WeatherExporter::getWeatherUndergroundExporterStatus() const {
    return weatherUndergroundExporterStatus;
}

WeatherExporterStatus WeatherExporter::getWindGuruExporterStatus() const {
    return windGuruExporterStatus;
}

WeatherExporterStatus WeatherExporter::getWindyExporterStatus() const {
    return windyExporterStatus;
}
