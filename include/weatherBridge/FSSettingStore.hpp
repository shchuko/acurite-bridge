#pragma once

#include <ArduinoLog.h>
#include <SPIFFS.h>

#include "weatherBridge/WeatherBridgeSettings.hpp"

class FSSettingStore {
private:
    fs::FS &fs;

public:
    explicit FSSettingStore(fs::FS &fs) noexcept;

    WeatherBridgeSettings loadSettings();

    bool writeSettings(WeatherBridgeSettings &settings);

private:
    String readFile(const String &path);

    bool writeFile(const String &path, const String &message);
};
