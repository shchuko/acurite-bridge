#include "weatherBridge/wifi/client/WiFiClientConnector.hpp"

WiFiClientStatus WiFiClientConnector::loop() noexcept {
    int8_t rssi;
    switch (WiFiClass::status()) {
        case WL_CONNECTED:
            lastConnectionRefresh = millis();
            rssi = WiFi.RSSI();
            return WiFiClientStatus(rssiToSignalValue(rssi), WiFi.SSID(), rssi);
        case WL_CONNECT_FAILED:
        case WL_NO_SSID_AVAIL:
        case WL_IDLE_STATUS:
        case WL_DISCONNECTED:
        case WL_CONNECTION_LOST:
        case WL_NO_SHIELD:
        case WL_SCAN_COMPLETED:
            if (millis() - lastConnectionRefresh > RECONNECT_INTERVAL_MILLIS) {
                Log.noticeln("WiFi Connection lost, attempting reconnect");
                WiFi.reconnect();
                lastConnectionRefresh = millis();
            }
            return WiFiClientStatus(WifiSignal::NO_CONNECTION, WiFi.SSID(), 0);
    }
}

void WiFiClientConnector::begin(const String &ssid, const String &password) noexcept {
    if (ssid.isEmpty()) {
        Log.noticeln("WiFi SSID is not provided, skipping initialization");
        return;
    }
    WiFi.begin(ssid.c_str(), password.isEmpty() ? nullptr : password.c_str());
    lastConnectionRefresh = millis();
}

WifiSignal WiFiClientConnector::rssiToSignalValue(int8_t rssi) noexcept {
    if (rssi >= -50) {
        return WifiSignal::GOOD;
    } else if (rssi >= -70) {
        return WifiSignal::NORMAL;
    }
    return WifiSignal::BAD;
}
