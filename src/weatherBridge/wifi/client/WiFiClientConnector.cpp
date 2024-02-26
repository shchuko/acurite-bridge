#include "weatherBridge/wifi/client/WiFiClientConnector.hpp"

WiFiClientStatus WiFiClientConnector::loop() noexcept {
    int8_t rssi;
    switch (WiFiClass::status()) {
        case WL_CONNECTED:
            lastConnectionRefresh = millis();
            rssi = WiFi.RSSI();
            return WiFiClientStatus(WiFi.SSID(), true, rssi);
        case WL_CONNECT_FAILED:
        case WL_NO_SSID_AVAIL:
        case WL_IDLE_STATUS:
        case WL_DISCONNECTED:
        case WL_CONNECTION_LOST:
        case WL_NO_SHIELD:
        case WL_SCAN_COMPLETED:
            if (millis() - lastConnectionRefresh > RECONNECT_INTERVAL_MILLIS) {
                Log.noticeln("WiFi not connected, attempting reconnect");
                WiFi.reconnect();
                lastConnectionRefresh = millis();
            }
            return WiFiClientStatus(WiFi.SSID(), false, 0);
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
