#include "weatherBridge/WeatherBridge.hpp"

#include "weatherBridge/SettingsServer.hpp"

WeatherBridge::WeatherBridge(FS &fs, int configModeButtonPin) noexcept
        : fs(fs),
          settingStore(FSSettingStore(fs)),
          configModeButton(InputPullUpButton(configModeButtonPin)) {}

void WeatherBridge::begin() {
    settingStore.begin();
    // Settings are applied only after reboot
    settingsSnapshot = settingStore.getSettings();

    configModeButton.begin();
    display.begin();

    NTPTimeClient::begin(settingsSnapshot);

    if (configModeButton.isPressed()) {
        isConfigurationMode = true;
        wifiApStatus = WiFiAP::begin(settingsSnapshot);
        settingsServer.startServer(fs, settingStore);
    } else {
        wifiClientConnector.begin(settingsSnapshot.getWlanSsid(), settingsSnapshot.getWlanPassword());
    }
}

void WeatherBridge::loop() {
    settingStore.loop();
    ntpTimeSyncOk = NTPTimeClient::checkStatus();
    wifiConnectionStatus = wifiClientConnector.loop();
    display.refresh(context);
}
