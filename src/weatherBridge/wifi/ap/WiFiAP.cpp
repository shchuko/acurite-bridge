#include "weatherBridge/wifi/ap/WiFiAP.hpp"


WiFiAPStatus WiFiAP::begin() {
    char ssid[25]{};
    char password[10]{};
    String ip;

    Log.noticeln("Initializing WiFi in AP mode");
    generateAndFillSettings(ssid, password);

    Log.noticeln("Initializing WiFi in AP mode, configuration ssid=%s,password=%s", ssid, password);

    WiFi.softAP(ssid, password);
    ip = WiFi.softAPIP().toString();
    Log.noticeln("Initialized WiFi in AP mode, ip=%s", ip.c_str());

    return { ssid, password, std::move(ip) };
}

void WiFiAP::generateAndFillSettings(char *ssid, char *password) {
    snprintf(ssid, 25, "ESP-%ld", random(1000, 9999));
    itoa(random(100000000, 999999999), password, 10);
}
