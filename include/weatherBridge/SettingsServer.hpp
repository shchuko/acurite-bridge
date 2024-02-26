#pragma once

#include <ESPAsyncWebServer.h>

#include "weatherBridge/WeatherBridgeSettings.hpp"
#include "weatherBridge/FSSettingStore.hpp"
#include "weatherBridge/AvailableStationsTracker.hpp"

class SettingsServer {
private:
    AsyncWebServer server = AsyncWebServer(80);

public:
    void startServer(fs::FS &fs, FSSettingStore &settingStore, const AvailableStationsTracker &availableStationsTracker);
};
