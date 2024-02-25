#pragma once

#include <Arduino.h>

class WeatherBridgeSettings {
private:
    String wlanSsid = "";
    String wlanPassword = "";
    String pwsWeatherStationId = "";
    String pwsWeatherApiKey = "";
    String windGuruStationUid = "";
    String windGuruStationPassword = "";
    String windyApiKey = "";
    String windyStationId = "";
    String wuApiKey = "";
    String wuStationId = "";
    String posixTzString = "";

public:
    WeatherBridgeSettings() = default;

    WeatherBridgeSettings(const String &&wlanSsid,
                          const String &&wlanPassword,
                          const String &&posixTzString,
                          const String &&pwsWeatherStationId,
                          const String &&pwsWeatherApiKey,
                          const String &&windGuruStationUid,
                          const String &&windGuruStationPassword,
                          const String &&windyApiKey,
                          const String &&windyStationId,
                          const String &&wuApiKey,
                          const String &&wuStationId
    );

    WeatherBridgeSettings(WeatherBridgeSettings &&other) noexcept;

    WeatherBridgeSettings &operator=(WeatherBridgeSettings &&other) noexcept;

    const String &getWlanSsid() const;

    const String &getWlanPassword() const;

    const String &getPwsWeatherStationId() const;

    const String &getPwsWeatherApiKey() const;

    const String &getWindGuruStationUid() const;

    const String &getWindGuruStationPassword() const;

    const String &getWindyApiKey() const;

    const String &getWindyStationId() const;

    const String &getWuApiKey() const;

    const String &getWuStationId() const;

    const String &getPosixTzString() const;

};