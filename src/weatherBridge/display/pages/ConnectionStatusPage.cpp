
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

static const unsigned char PROGMEM image_wifi_bits[] = {0x01, 0xf0, 0x00, 0x06, 0x0c, 0x00, 0x18, 0x03, 0x00, 0x21,
                                                        0xf0, 0x80, 0x46, 0x0c, 0x40, 0x88, 0x02, 0x20, 0x10, 0xe1,
                                                        0x00, 0x23, 0x18, 0x80, 0x04, 0x04, 0x00, 0x08, 0x42, 0x00,
                                                        0x01, 0xb0, 0x00, 0x02, 0x08, 0x00, 0x00, 0x40, 0x00, 0x00,
                                                        0xa0, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 0x00};


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
            if (context.wifiApContext.getPassword().isEmpty()) {
                delegate.print("(No password)");
            } else {
                delegate.print("PWD: " + context.wifiApContext.getPassword());
            }
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
        delegate.drawBitmap(1, 7, image_wifi_bits, 19, 16, 1);

        delegate.setTextColor(1);
        delegate.setTextSize(1);
        delegate.setCursor(22, 3);
        delegate.setTextWrap(false);
        const String &settingsSsid = context.settings.getWlanSsid();
        const String &activeSsid = context.wifiConnectionStatus.getSsid();
        bool isConnected = context.wifiConnectionStatus.isConnected();
        int rssi = context.wifiConnectionStatus.getRSSI();

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
        if (isConnected) {
            delegate.printf("RSSI: %d", rssi);
        } else if (!activeSsid.isEmpty() || !settingsSsid.isEmpty()) {
            delegate.print("Not connected");
        }
    }

    // Station details
    const String &stationId = context.settings.getSelectedStationId();
    bool isStationSelected = !stationId.isEmpty();
    int isConnected = context.measurementsStore.getRssi().hasValue();
    int rssi = isConnected ? context.measurementsStore.getRssi().getValue() : 0;

    delegate.drawBitmap(2, 42, image_cloud_bits, 17, 16, 1);

    delegate.setTextColor(1);
    delegate.setTextSize(1);
    delegate.setCursor(21, 35);
    delegate.setTextWrap(false);
    if (isStationSelected) {
        delegate.print("Acurite 5n1");
    } else {
        delegate.print("Not Configured");
    }

    if (isStationSelected) {
        delegate.setTextColor(1);
        delegate.setTextSize(1);
        delegate.setCursor(21, 44);
        delegate.setTextWrap(false);

        delegate.printf("ID: %s", stationId.c_str());
    }

    delegate.setTextColor(1);
    delegate.setTextSize(1);
    delegate.setCursor(21, 53);
    delegate.setTextWrap(false);
    if (isConnected) {
        delegate.printf("RSSI: %d", rssi);
    } else if (isStationSelected) {
        delegate.print("No connection");
    }
}

