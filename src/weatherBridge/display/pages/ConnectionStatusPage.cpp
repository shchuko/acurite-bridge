
#include "weatherBridge/display/pages/ConnectionStatusPage.hpp"


static const unsigned char PROGMEM image_menu_tools_bits[] = {0x80, 0xe0, 0xc1, 0x60, 0x42, 0x80, 0x22, 0x8c, 0x13,
                                                              0x0c, 0x0a, 0xb4, 0x06, 0x48, 0x05, 0xf0, 0x0b, 0x00,
                                                              0x14, 0xe0, 0x29, 0xb0, 0x50, 0xd8, 0xa0, 0x6c, 0xc0,
                                                              0x34, 0x00, 0x1c, 0x00, 0x00};


static const unsigned char PROGMEM image_cloud_bits[] = {0x00, 0x00, 0x00, 0x07, 0xc0, 0x00, 0x08, 0x20, 0x00, 0x10,
                                                         0x10, 0x00, 0x30, 0x08, 0x00, 0x40, 0x0e, 0x00, 0x80, 0x01,
                                                         0x00, 0x80, 0x00, 0x80, 0x40, 0x00, 0x80, 0x3f, 0xff, 0x00,
                                                         0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                                         0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
static const unsigned char PROGMEM image_network_bits[] = {0x00, 0x0e, 0x00, 0x0a, 0x00, 0x0a, 0x00, 0x0a, 0x00, 0xea,
                                                           0x00, 0xaa, 0x00, 0xaa, 0x00, 0xaa, 0x0e, 0xaa, 0x0a, 0xaa,
                                                           0x0a, 0xaa, 0x0a, 0xaa, 0xea, 0xaa, 0xaa, 0xaa, 0xee, 0xee,
                                                           0x00, 0x00};


ConnectionStatusPage::ConnectionStatusPage(Adafruit_GFX &display) : DisplayPage(display) {}


void ConnectionStatusPage::paint(WeatherBridgeContext context) {
    // Wi-Fi details
    if (context.isConfigurationMode) {
        delegate.drawBitmap(3, 7, image_menu_tools_bits, 15, 16, 1);

        delegate.setTextColor(1);
        delegate.setTextSize(1);
        delegate.setCursor(22, 3);
        delegate.setTextWrap(false);
        if (context.wifiApContext.isActive()) {
            delegate.print("SSID: " + context.wifiApContext.getSsid());
        } else {
            delegate.print("AP_INIT_ERROR");
        }

        delegate.setTextColor(1);
        delegate.setTextSize(1);
        delegate.setCursor(22, 12);
        delegate.setTextWrap(false);
        if (context.wifiApContext.isActive()) {
            delegate.print("PWD: " + context.wifiApContext.getPassword());
        } else {
            delegate.print("");
        }

        delegate.setTextColor(1);
        delegate.setTextSize(1);
        delegate.setCursor(22, 21);
        delegate.setTextWrap(false);
        if (context.wifiApContext.isActive()) {
            delegate.print(context.wifiApContext.getIp());
        } else {
            delegate.print("");
        }
    } else {
        delegate.drawBitmap(3, 7, image_network_bits, 14, 16, 1);

        delegate.setTextColor(1);
        delegate.setTextSize(1);
        delegate.setCursor(22, 3);
        delegate.setTextWrap(false);
        const String &activeSsid = context.wifiConnectionStatus.getSsid();
        const String &settingsSsid = context.settings.getWlanSsid();

        if (!activeSsid.isEmpty()) {
            delegate.print(activeSsid);
        } else if (!settingsSsid.isEmpty()) {
            delegate.print(settingsSsid);
        } else {
            delegate.print("Not Configured");
        }

        delegate.setTextColor(1);
        delegate.setTextSize(1);
        delegate.setCursor(22, 12);
        delegate.setTextWrap(false);
        if (!activeSsid.isEmpty() || !settingsSsid.isEmpty()) {
            delegate.print(getWifiSignalName(context.wifiConnectionStatus.getSignalLevel()));
        } else {
            delegate.print("");
        }
    }

    // Station details
    delegate.drawBitmap(2, 42, image_cloud_bits, 17, 16, 1);

    delegate.setTextColor(1);
    delegate.setTextSize(1);
    delegate.setCursor(21, 35);
    delegate.setTextWrap(false);
    delegate.print(F("Acurite 5n1"));

    delegate.setTextColor(1);
    delegate.setTextSize(1);
    delegate.setCursor(21, 44);
    delegate.setTextWrap(false);
    delegate.print("Signal:");

    delegate.setTextColor(1);
    delegate.setTextSize(1);
    delegate.setCursor(66, 44);
    delegate.setTextWrap(false);
    delegate.print(getStationSignalName(StationSignal::NO_CONNECTION));

    delegate.setTextColor(1);
    delegate.setTextSize(1);
    delegate.setCursor(21, 53);
    delegate.setTextWrap(false);
    delegate.print("ID:");

    delegate.setTextColor(1);
    delegate.setTextSize(1);
    delegate.setCursor(42, 53);
    delegate.setTextWrap(false);
    delegate.print(F("Station1234"));
}

String ConnectionStatusPage::getWifiSignalName(WifiSignal signal) {
    switch (signal) {
        case WifiSignal::BAD:
            return F("Signal: BAD");
        case WifiSignal::NORMAL:
            return F("Signal: NORMAL");
        case WifiSignal::GOOD:
            return F("Signal: GOOD");
        case WifiSignal::NO_CONNECTION:
            return F("NOT CONNECTED");
    }
}

String ConnectionStatusPage::getStationSignalName(StationSignal signal) {
    switch (signal) {
        case StationSignal::BAD:
            return F("BAD");
        case StationSignal::NORMAL:
            return F("NORMAL");
        case StationSignal::GOOD:
            return F("GOOD");
        case StationSignal::NO_CONNECTION:
            return F("NO CONNECTION");
    }
}
