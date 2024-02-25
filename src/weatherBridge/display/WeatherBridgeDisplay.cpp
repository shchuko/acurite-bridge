#include "Adafruit_SSD1306.h"
#include "ArduinoLog.h"

#include "weatherBridge/display/DisplayPage.hpp"
#include "weatherBridge/display/WeatherBridgeDisplay.hpp"
#include "weatherBridge/arrayExtensions.hpp"

#define WEATHER_EXPORTER_DISPLAY_SCREEN_WIDTH 128
#define WEATHER_EXPORTER_DISPLAY_SCREEN_HEIGHT 64
#define WEATHER_EXPORTER_DISPLAY_I2C_ADDR 0x3C // Address 0x3C for 128x64
#define WEATHER_EXPORTER_DISPLAY_OLED_RESET (-1) // Reset pin (or -1 if sharing Arduino reset pin)
#define WEATHER_EXPORTER_DISPLAY_PAGE_UPDATE_INTERVAL_MILLIS (5000)


static const unsigned char PROGMEM image_weather_cloud_sunny_bits[] = {0x00, 0x20, 0x00, 0x02, 0x02, 0x00, 0x00,
                                                                       0x70, 0x00, 0x01, 0x8c, 0x00, 0x09, 0x04,
                                                                       0x80, 0x02, 0x02, 0x00, 0x02, 0x02, 0x00,
                                                                       0x07, 0x82, 0x00, 0x08, 0x44, 0x80, 0x10,
                                                                       0x80, 0x01, 0x00, 0x7f, 0xfe, 0x00};

WeatherBridgeDisplay::WeatherBridgeDisplay()
        : delegate(WEATHER_EXPORTER_DISPLAY_SCREEN_WIDTH, WEATHER_EXPORTER_DISPLAY_SCREEN_HEIGHT, &Wire,
                   WEATHER_EXPORTER_DISPLAY_OLED_RESET) {}

WeatherBridgeDisplay::~WeatherBridgeDisplay() {
    for (auto &page: pages) {
        delete page;
    }
}

void WeatherBridgeDisplay::begin() {
    Log.noticeln("Initializing WeatherBridgeDisplay");
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

void WeatherBridgeDisplay::refresh(WeatherBridgeContext context) {
    if (millis() - lastPageSwitchMillis >= WEATHER_EXPORTER_DISPLAY_PAGE_UPDATE_INTERVAL_MILLIS) {
        Log.traceln(F("WeatherBridgeDisplay: Performing checkStatus"));
        nextPage(context);
        lastPageSwitchMillis = millis();
    }
}

void WeatherBridgeDisplay::nextPage(WeatherBridgeContext context) {
    size_t totalPages = ArraySize(pages);
    Log.traceln(F("WeatherBridgeDisplay: Painting page %d of %d"), nextPageIndex + 1, totalPages);

    delegate.clearDisplay();
    pages[nextPageIndex]->paint(context);
    delegate.display();
    nextPageIndex = (nextPageIndex + 1) % totalPages;
    Log.traceln(F("WeatherBridgeDisplay: Painting complete"));
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
