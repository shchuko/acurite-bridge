
#include "weatherBridge/WeatherBridgeContext.hpp"

WeatherBridgeContext::WeatherBridgeContext(const bool &isConfigurationMode, const WiFiAPStatus &wifiApContext,
                                           const WeatherBridgeSettings &settings, const bool &ntpTimeSyncOk,
                                           const WiFiClientStatus &wifiConnectionStatus,
                                           const MeasurementsStore &measurementsStore,
                                           const WeatherExporter &exportersContainer)
        : wifiApContext(wifiApContext),
          settings(settings),
          isConfigurationMode(isConfigurationMode),
          ntpTimeSyncOk(ntpTimeSyncOk), wifiConnectionStatus(wifiConnectionStatus),
          measurementsStore(measurementsStore),
          exporters(exportersContainer) {}
