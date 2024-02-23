#include "Adafruit_SSD1306.h"
#include "ArduinoLog.h"
#include "TimeLib.h"

#include "weatherBridge/display.hpp"
#include "weatherBridge/types.hpp"
#include "weatherBridge/context.hpp"

#define WEATHER_EXPORTER_DISPLAY_SCREEN_WIDTH 128
#define WEATHER_EXPORTER_DISPLAY_SCREEN_HEIGHT 64
#define WEATHER_EXPORTER_DISPLAY_I2C_ADDR 0x3C // Address 0x3C for 128x64
#define WEATHER_EXPORTER_DISPLAY_OLED_RESET (-1) // Reset pin (or -1 if sharing Arduino reset pin)
#define WEATHER_EXPORTER_DISPLAY_PAGE_UPDATE_INTERVAL_MILLIS (5000)

template<typename T, size_t N>
size_t ArraySize(T (&)[N]) {
    return N;
}

static const unsigned char PROGMEM image_cloud_bits[] = {0x00, 0x00, 0x00, 0x07, 0xc0, 0x00, 0x08, 0x20, 0x00, 0x10,
                                                         0x10, 0x00, 0x30, 0x08, 0x00, 0x40, 0x0e, 0x00, 0x80, 0x01,
                                                         0x00, 0x80, 0x00, 0x80, 0x40, 0x00, 0x80, 0x3f, 0xff, 0x00,
                                                         0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                                         0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
static const unsigned char PROGMEM image_network_bits[] = {0x00, 0x0e, 0x00, 0x0a, 0x00, 0x0a, 0x00, 0x0a, 0x00, 0xea,
                                                           0x00, 0xaa, 0x00, 0xaa, 0x00, 0xaa, 0x0e, 0xaa, 0x0a, 0xaa,
                                                           0x0a, 0xaa, 0x0a, 0xaa, 0xea, 0xaa, 0xaa, 0xaa, 0xee, 0xee,
                                                           0x00, 0x00};

static const unsigned char PROGMEM image_weather_temperature_bits[] = {0x1c, 0x00, 0x22, 0x02, 0x2b, 0x05, 0x2a,
                                                                       0x02, 0x2b, 0x38, 0x2a, 0x60, 0x2b, 0x40,
                                                                       0x2a, 0x40, 0x2a, 0x60, 0x49, 0x38, 0x9c,
                                                                       0x80, 0xae, 0x80, 0xbe, 0x80, 0x9c, 0x80,
                                                                       0x41, 0x00, 0x3e, 0x00};
static const unsigned char PROGMEM image_weather_wind_bits[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0x03, 0x88,
                                                                0x04, 0x44, 0x04, 0x44, 0x00, 0x44, 0x00, 0x88,
                                                                0xff, 0x32, 0x00, 0x00, 0xad, 0x82, 0x00, 0x60,
                                                                0x00, 0x10, 0x00, 0x10, 0x01, 0x20, 0x00, 0xc0};


static const unsigned char PROGMEM image_weather_cloud_sunny_bits[] = {0x00, 0x20, 0x00, 0x02, 0x02, 0x00, 0x00,
                                                                       0x70, 0x00, 0x01, 0x8c, 0x00, 0x09, 0x04,
                                                                       0x80, 0x02, 0x02, 0x00, 0x02, 0x02, 0x00,
                                                                       0x07, 0x82, 0x00, 0x08, 0x44, 0x80, 0x10,
                                                                       0x2c, 0x00, 0x30, 0x30, 0x00, 0x60, 0x1e,
                                                                       0x00, 0x80, 0x03, 0x00, 0x80, 0x01, 0x00,
                                                                       0x80, 0x01, 0x00, 0x7f, 0xfe, 0x00};

static const unsigned char PROGMEM image_menu_arrow_down_left_bits[] = {0x80, 0xc0, 0xe0};


static const unsigned char PROGMEM image_earth_bits[] = {0x07, 0xc0, 0x1e, 0x70, 0x27, 0xf8, 0x61, 0xe4, 0x43,
                                                         0xe4,
                                                         0x87, 0xca, 0x9f, 0xf6, 0xdf, 0x82, 0xdf, 0x82, 0xe3,
                                                         0xc2,
                                                         0x61, 0xf4, 0x70, 0xf4, 0x31, 0xf8, 0x1b, 0xf0, 0x07,
                                                         0xc0,
                                                         0x00, 0x00};


static const unsigned char PROGMEM image_list_element_tick[] = {0x80, 0xc0, 0xe0};

ConnectionStatusPage::ConnectionStatusPage(Adafruit_GFX &display) : DisplayPage(display) {}

void ConnectionStatusPage::paint() {
    // Wi-Fi details
    delegate.drawBitmap(3, 7, image_network_bits, 15, 16, 1);

    delegate.setTextColor(1);
    delegate.setTextSize(1);
    delegate.setCursor(22, 3);
    delegate.setTextWrap(false);
    delegate.print("TEST_WIFI_NETWORK");

    delegate.setTextColor(1);
    delegate.setTextSize(1);
    delegate.setCursor(22, 12);
    delegate.setTextWrap(false);
    delegate.print("Signal:");

    delegate.setTextColor(1);
    delegate.setTextSize(1);
    delegate.setCursor(67, 12);
    delegate.setTextWrap(false);
    delegate.print(getWifiSignalName(WifiSignal::NO_SIGNAL));

    delegate.setTextColor(1);
    delegate.setTextSize(1);
    delegate.setCursor(22, 21);
    delegate.setTextWrap(false);
    delegate.print(F("255.255.255.255"));

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
    delegate.print(getStationSignalName(StationSignal::NO_SIGNAL));

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
            return F("BAD");
        case WifiSignal::NORMAL:
            return F("NORMAL");
        case WifiSignal::GOOD:
            return F("GOOD");
        case WifiSignal::NO_SIGNAL:
            return F("NO SIGNAL");
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
        case StationSignal::NO_SIGNAL:
            return F("NO SIGNAL");
    }
}

DateTimePage::DateTimePage(Adafruit_GFX &display) : DisplayPage(display) {}

void DateTimePage::paint() {
    TimeContext &timeContext = weatherBridgeContext.time;

    uint8_t timezoneShift = timeContext.getTimezoneShift();
    time_t time = timeContext.getCurrentTime();
    timeStatus_t status = timeContext.getCurrentTimeStatus();
    char charBuf[20];

    delegate.setTextColor(1);
    delegate.setTextSize(2);
    delegate.setCursor(36, 4);
    delegate.setTextWrap(false);
    sprintf(charBuf, "%02d:%02d", hour(time), minute(time));
    delegate.print(charBuf);
    delegate.setTextColor(1);
    delegate.setTextSize(2);
    delegate.setCursor(5, 22);
    delegate.setTextWrap(false);
    sprintf(charBuf, "%02d-%02d-%02d", day(time), month(time), year(time));
    delegate.print(charBuf);
    delegate.setTextColor(1);
    delegate.setTextSize(1);
    delegate.setCursor(5, 45);
    delegate.setTextWrap(false);
    delegate.print("Timezone");
    delegate.setTextColor(1);
    delegate.setTextSize(1);
    delegate.setCursor(5, 54);
    delegate.setTextWrap(false);
    delegate.print("NTP");
    delegate.setTextColor(1);
    delegate.setTextSize(1);
    delegate.setCursor(70, 45);
    delegate.setTextWrap(false);
    if (timezoneShift >= 0) {
        sprintf(charBuf, "GMT+%d", timezoneShift);
    } else {
        sprintf(charBuf, "GMT-%d", -timezoneShift);
    }
    delegate.print(charBuf);

    delegate.setTextColor(1);
    delegate.setTextSize(1);
    delegate.setCursor(70, 54);
    delegate.setTextWrap(false);
    if (status == timeSet) {
        delegate.print("SYNCED");
    } else {
        delegate.print("NEED SYNC");
    }

    delegate.drawLine(23, 60, 68, 60, 1);
    delegate.drawLine(53, 51, 68, 51, 1);
}

WeatherPage::WeatherPage(Adafruit_GFX &display) : DisplayPage(display) {}

void WeatherPage::paint() {
    delegate.drawBitmap(9, 5, image_weather_temperature_bits, 16, 16, 1);
    delegate.setTextColor(1);
    delegate.setTextSize(2);
    delegate.setCursor(29, 26);
    delegate.setTextWrap(false);
    delegate.print("50.5kts"); // Speed
    delegate.setTextColor(1);
    delegate.setTextSize(2);
    delegate.setCursor(29, 6);
    delegate.setTextWrap(false);
    delegate.print("+25.7C");
    delegate.drawBitmap(8, 24, image_weather_wind_bits, 15, 16, 1);
    delegate.setTextColor(1);
    delegate.setTextSize(1);
    delegate.setCursor(29, 42);
    delegate.setTextWrap(false);
    delegate.print("Gust");
    delegate.setTextColor(1);
    delegate.setTextSize(1);
    delegate.setCursor(29, 51);
    delegate.setTextWrap(false);
    delegate.print("NNW");
    delegate.setTextColor(1);
    delegate.setTextSize(1);
    delegate.setCursor(56, 42);
    delegate.setTextWrap(false);
    delegate.print("50.7kts"); // Gust
}


WeatherExportersPage::WeatherExportersPage(Adafruit_GFX &display) : DisplayPage(display) {}

String WeatherExportersPage::getWeatherExporterStatusName(WeatherExporterStatus status) {
    switch (status) {
        case WeatherExporterStatus::OFF:
            return F("OFF");
        case WeatherExporterStatus::OK:
            return F("OK");
        case WeatherExporterStatus::ERROR:
            return F("ERROR");
    }
}

void WeatherExportersPage::paint() {
    delegate.drawBitmap(3, 17, image_menu_arrow_down_left_bits, 3, 3, 1);
    delegate.drawBitmap(3, 7, image_list_element_tick, 3, 3, 1);
    delegate.drawBitmap(3, 27, image_list_element_tick, 3, 3, 1);
    delegate.drawBitmap(3, 37, image_list_element_tick, 3, 3, 1);

    delegate.setTextColor(1);
    delegate.setTextSize(1);
    delegate.setCursor(7, 3);
    delegate.setTextWrap(false);
    delegate.print("PWS Weather");
    delegate.setTextColor(1);
    delegate.setTextSize(1);
    delegate.setCursor(7, 13);
    delegate.setTextWrap(false);
    delegate.print("Windy");
    delegate.setTextColor(1);
    delegate.setTextSize(1);
    delegate.setCursor(7, 23);
    delegate.setTextWrap(false);
    delegate.print("WUnderground");

    delegate.setTextColor(1);
    delegate.setTextSize(1);
    delegate.setCursor(7, 33);
    delegate.setTextWrap(false);
    delegate.print("WindGuru");
    delegate.drawLine(37, 19, 91, 19, 1);
    delegate.drawLine(79, 29, 91, 29, 1);
    delegate.setTextColor(1);
    delegate.setTextSize(1);
    delegate.setCursor(93, 3);
    delegate.setTextWrap(false);
    delegate.print(getWeatherExporterStatusName(WeatherExporterStatus::ERROR)); // pws weather
    delegate.setTextColor(1);
    delegate.setTextSize(1);
    delegate.setCursor(93, 13);
    delegate.setTextWrap(false);
    delegate.print(getWeatherExporterStatusName(WeatherExporterStatus::OK)); // windy
    delegate.setTextColor(1);
    delegate.setTextSize(1);
    delegate.setCursor(93, 23);
    delegate.setTextWrap(false);
    delegate.print(getWeatherExporterStatusName(WeatherExporterStatus::ERROR)); // weatherUndergroundStatus
    delegate.setTextColor(1);
    delegate.setTextSize(1);
    delegate.setCursor(93, 33);
    delegate.setTextWrap(false);
    delegate.print(getWeatherExporterStatusName(WeatherExporterStatus::OFF)); // windGuruStatus
    delegate.drawLine(55, 39, 91, 39, 1);
    delegate.drawLine(72, 9, 91, 9, 1);
}

ICMPChecksPage::ICMPChecksPage(Adafruit_GFX &display) : DisplayPage(display) {}

void ICMPChecksPage::paint() {
    delegate.drawBitmap(5, 31, image_menu_arrow_down_left_bits, 3, 3, 1);
    delegate.setTextColor(1);
    delegate.setTextSize(1);
    delegate.setCursor(9, 27);
    delegate.setTextWrap(false);
    delegate.print("GOOGLE.COM");
    delegate.setTextColor(1);
    delegate.setTextSize(1);
    delegate.setCursor(9, 39);
    delegate.setTextWrap(false);
    delegate.print("1.1.1.1");
    delegate.drawBitmap(5, 43, image_list_element_tick, 3, 3, 1);
    delegate.drawLine(69, 33, 103, 33, 1);
    delegate.drawLine(51, 45, 103, 45, 1);
    delegate.setTextColor(1);
    delegate.setTextSize(1);
    delegate.setCursor(105, 27);
    delegate.setTextWrap(false);
    delegate.print("OK"); // google
    delegate.setTextColor(1);
    delegate.setTextSize(1);
    delegate.setCursor(105, 39);
    delegate.setTextWrap(false);
    delegate.print("ERR"); // 1.1.1.1
    delegate.setTextColor(1);
    delegate.setTextSize(1);
    delegate.setCursor(105, 52);
    delegate.setTextWrap(false);
    delegate.print("ERR"); // 8.8.8.8
    delegate.drawLine(51, 58, 103, 58, 1);
    delegate.drawBitmap(5, 56, image_list_element_tick, 3, 3, 1);
    delegate.setTextColor(1);
    delegate.setTextSize(1);
    delegate.setCursor(9, 52);
    delegate.setTextWrap(false);
    delegate.print("8.8.8.8");
    delegate.drawBitmap(5, 4, image_earth_bits, 15, 16, 1);
    delegate.setTextColor(1);
    delegate.setTextSize(1);
    delegate.setCursor(24, 5);
    delegate.setTextWrap(false);
    delegate.print("ICMP Checks");
}

WeatherBridgeDisplay::WeatherBridgeDisplay()
        : delegate(WEATHER_EXPORTER_DISPLAY_SCREEN_WIDTH, WEATHER_EXPORTER_DISPLAY_SCREEN_HEIGHT, &Wire,
                   WEATHER_EXPORTER_DISPLAY_OLED_RESET) {}

WeatherBridgeDisplay::~WeatherBridgeDisplay() {
    for (auto &page: pages) {
        delete page;
    }
}

void WeatherBridgeDisplay::begin() {
    Log.infoln("Initializing WeatherBridgeDisplay");
    if (delegate.begin(SSD1306_SWITCHCAPVCC, WEATHER_EXPORTER_DISPLAY_I2C_ADDR)) {
        delegate.clearDisplay();
        paintSplash();
        delegate.display();
        lastPageSwitchMillis = millis();
        return;
    }

#pragma clang diagnostic push
#pragma ide diagnostic ignored "EndlessLoop"
    for (;;) {
#pragma clang diagnostic pop
        Log.errorln("WeatherBridgeDisplay initialization failed");
        delay(2000);
    }
}

void WeatherBridgeDisplay::refresh() {
    unsigned long currentMillis = millis();
    if (currentMillis - lastPageSwitchMillis >= WEATHER_EXPORTER_DISPLAY_PAGE_UPDATE_INTERVAL_MILLIS) {
        Log.infoln(F("WeatherBridgeDisplay: Performing refresh"));
        lastPageSwitchMillis = currentMillis;
        nextPage();
    }
}

void WeatherBridgeDisplay::nextPage() {
    size_t totalPages = ArraySize(pages);
    Log.infoln(F("WeatherBridgeDisplay: Painting page %d of %d"), nextPageIndex + 1, totalPages);

    delegate.clearDisplay();
    pages[nextPageIndex]->paint();
    delegate.display();
    nextPageIndex = (nextPageIndex + 1) % totalPages;
    Log.infoln(F("WeatherBridgeDisplay: Painting complete"));
}

void WeatherBridgeDisplay::paintSplash() {
    delegate.setTextColor(1);
    delegate.setTextSize(2);
    delegate.setCursor(7, 6);
    delegate.setTextWrap(false);
    delegate.print(F("Weather"));
    delegate.setTextColor(1);
    delegate.setTextSize(2);
    delegate.setCursor(7, 25);
    delegate.setTextWrap(false);
    delegate.print(F("Bridge"));
    delegate.setTextColor(1);
    delegate.setTextSize(1);
    delegate.setCursor(7, 50);
    delegate.setTextWrap(false);
    delegate.print(F("github.com/shchuko"));
    delegate.drawBitmap(105, 5, image_weather_cloud_sunny_bits, 17, 16, 1);
}
