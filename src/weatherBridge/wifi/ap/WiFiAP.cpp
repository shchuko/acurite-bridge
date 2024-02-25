#include "weatherBridge/wifi/ap/WiFiAP.hpp"


WiFiAPStatus WiFiAP::begin(WeatherBridgeSettings &settings) {
    String ssid = settings.getApSsid();
    String password = settings.getApPassword();
    String ip;

    if (ssid.isEmpty()) {
        ssid = "ESP-AP";
        Log.errorln("Empty AP SSID specified, fallback to default=%s", ssid.c_str());
    }

    Log.noticeln("Initializing WiFi in AP mode, configuration ssid=%s,password=%s", ssid.c_str(), password.c_str());
    if (password.isEmpty()) {
        WiFi.softAP(ssid);
    } else {
        WiFi.softAP(ssid, password);
    }
    ip = WiFi.softAPIP().toString();
    Log.noticeln("Initialized WiFi in AP mode, ip=%s", ip.c_str());

    return {std::move(ssid), std::move(password), std::move(ip)};
}
