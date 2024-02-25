#include "weatherBridge/WeatherBridge.hpp"

WeatherBridge::WeatherBridge(FS &fs, int configModeButtonPin) noexcept: settingStore(FSSettingStore(fs)),
                                                                        configModeButton(InputPullUpButton(configModeButtonPin)) {}

void WeatherBridge::begin() {
    configModeButton.begin();
    settings = settingStore.loadSettings();
    display.begin();

    if (configModeButton.isPressed()) {
        isConfigurationMode = true;
        wifiApStatus = WiFiAP::begin();
    } else {
        wifiClientConnector.begin(settings.getWlanSsid(), settings.getWlanPassword());
    }

    NTPTimeClient::begin(settings);
}

void WeatherBridge::loop() {
    ntpTimeSyncOk = NTPTimeClient::checkStatus();
    wifiConnectionStatus = wifiClientConnector.loop();

    display.refresh(context);
}
