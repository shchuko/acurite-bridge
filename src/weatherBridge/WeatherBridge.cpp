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

    if (!settingsSnapshot.getSelectedStationId().isEmpty()) {
        measurementsStore = MeasurementsStore(StationModel::ACURITE_5N1,
                                              settingsSnapshot.getSelectedStationId().toInt());
    }

    rfReceiver.begin();
    acurite5N1Receiver.registerCallback([&](const StationMeasurements &measurements) {
        measurementsStore.updateMeasurements(measurements);
        availableStationsTracker.receiveData(measurements);
    });
}

void WeatherBridge::loop() {
    settingStore.loop();

    measurementsStore.loop();
    availableStationsTracker.loop();
    rfReceiver.loop();

    wifiConnectionStatus = wifiClientConnector.loop();
    ntpTimeSyncOk = NTPTimeClient::checkStatus();

    while (rfReceiver.available()) {
        acurite5N1Receiver.receiveMessage(rfReceiver.read());
    }

    display.refresh(context);
}
