#include "weatherBridge/WeatherBridgeSettings.hpp"

WeatherBridgeSettings::WeatherBridgeSettings(
        const String &&wlanSsid,
        const String &&wlanPassword,
        const String &&posixTzString,
        const String &&pwsWeatherStationId,
        const String &&pwsWeatherApiKey,
        const String &&windGuruStationUid,
        const String &&windGuruStationPwd,
        const String &&windyApiKey,
        const String &&windyStationId,
        const String &&wuApiKey,
        const String &&wuStationId
) : wlanSsid(wlanSsid),
    wlanPassword(wlanPassword),
    posixTzString(posixTzString),
    pwsWeatherStationId(pwsWeatherStationId),
    pwsWeatherApiKey(pwsWeatherApiKey),
    windGuruStationUid(windGuruStationUid),
    windGuruStationPassword(windGuruStationPwd),
    windyApiKey(windyApiKey),
    windyStationId(windyStationId),
    wuApiKey(wuApiKey),
    wuStationId(wuStationId) {}


WeatherBridgeSettings::WeatherBridgeSettings(WeatherBridgeSettings &&other) noexcept:
        wlanSsid(std::move(other.wlanSsid)),
        wlanPassword(std::move(other.wlanPassword)),
        posixTzString(std::move(other.posixTzString)),
        pwsWeatherStationId(std::move(other.pwsWeatherStationId)),
        pwsWeatherApiKey(std::move(other.pwsWeatherApiKey)),
        windGuruStationUid(std::move(other.windGuruStationUid)),
        windGuruStationPassword(std::move(other.windGuruStationPassword)),
        windyApiKey(std::move(other.windyApiKey)),
        windyStationId(std::move(other.windyStationId)),
        wuApiKey(std::move(other.wuApiKey)),
        wuStationId(std::move(other.wuStationId)) {}

WeatherBridgeSettings &WeatherBridgeSettings::operator=(WeatherBridgeSettings &&other) noexcept {
    if (this != &other) {
        wlanSsid = std::move(other.wlanSsid);
        wlanPassword = std::move(other.wlanPassword);
        posixTzString = std::move(other.posixTzString);
        pwsWeatherStationId = std::move(other.pwsWeatherStationId);
        pwsWeatherApiKey = std::move(other.pwsWeatherApiKey);
        windGuruStationUid = std::move(other.windGuruStationUid);
        windGuruStationPassword = std::move(other.windGuruStationPassword);
        windyApiKey = std::move(other.windyApiKey);
        windyStationId = std::move(other.windyStationId);
        wuApiKey = std::move(other.wuApiKey);
        wuStationId = std::move(other.wuStationId);
    }
    return *this;
}


const String &WeatherBridgeSettings::getWlanSsid() const {
    return wlanSsid;
}

const String &WeatherBridgeSettings::getWlanPassword() const {
    return wlanPassword;
}

const String &WeatherBridgeSettings::getPwsWeatherStationId() const {
    return pwsWeatherStationId;
}

const String &WeatherBridgeSettings::getPwsWeatherApiKey() const {
    return pwsWeatherApiKey;
}

const String &WeatherBridgeSettings::getWindGuruStationUid() const {
    return windGuruStationUid;
}

const String &WeatherBridgeSettings::getWindGuruStationPassword() const {
    return windGuruStationPassword;
}

const String &WeatherBridgeSettings::getWindyApiKey() const {
    return windyApiKey;
}

const String &WeatherBridgeSettings::getWindyStationId() const {
    return windyStationId;
}

const String &WeatherBridgeSettings::getWuApiKey() const {
    return wuApiKey;
}

const String &WeatherBridgeSettings::getWuStationId() const {
    return wuStationId;
}

const String &WeatherBridgeSettings::getPosixTzString() const {
    return posixTzString;
}
