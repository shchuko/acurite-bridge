#pragma once

#include <ArduinoLog.h>
#include <SPIFFS.h>

#include "weatherBridge/WeatherBridgeSettings.hpp"

class FSSettingStore {
private:
    fs::FS &fs;
    WeatherBridgeSettings settingsSnapshot;
    bool flushPending = false;

public:
    explicit FSSettingStore(fs::FS &fs) noexcept;

    void begin();

    void loop();

    WeatherBridgeSettings getSettings() const;

    void updateSettings(WeatherBridgeSettings &&settings);

private:
    WeatherBridgeSettings loadSettings();

    bool writeSettings(WeatherBridgeSettings &settings);

    String readFile(const String &path);

    bool writeFile(const String &path, const String &message);
};
