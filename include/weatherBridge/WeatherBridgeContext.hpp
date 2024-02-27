#pragma once

#include "weatherBridge/wifi/ap/WiFiAPStatus.hpp"
#include "weatherBridge/FSSettingStore.hpp"
#include "weatherBridge/WeatherBridgeSettings.hpp"
#include "weatherBridge/wifi/client/WiFiClientStatus.hpp"
#include "WeatherExporter.hpp"
#include "MeasurementsStore.hpp"

class WeatherBridgeContext {
public:
    const bool &isConfigurationMode;
    const WiFiAPStatus &wifiApContext;
    const WiFiClientStatus &wifiConnectionStatus;
    const WeatherBridgeSettings &settings;
    const MeasurementsStore &measurementsStore;
    const WeatherExporter &exporters;

    const bool &ntpTimeSyncOk;

    WeatherBridgeContext(const bool &isConfigurationMode, const WiFiAPStatus &wifiApContext,
                         const WeatherBridgeSettings &settings, const bool &ntpTimeSyncOk,
                         const WiFiClientStatus &wifiConnectionStatus, const MeasurementsStore &measurementsStore,
                         const WeatherExporter &exportersContainer);
};