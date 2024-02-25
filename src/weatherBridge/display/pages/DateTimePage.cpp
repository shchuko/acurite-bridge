
#include "ArduinoLog.h"
#include "weatherBridge/arrayExtensions.hpp"
#include "weatherBridge/display/pages/DateTimePage.hpp"

DateTimePage::DateTimePage(Adafruit_GFX &display) : DisplayPage(display) {}


void DateTimePage::paint(WeatherBridgeContext context) {
    char charBuf[64];
    for (auto &item: charBuf) { item = '-'; }
    struct tm timeInfo{};

    bool localTimeGetResult = getLocalTime(&timeInfo, 1000);
    if (!localTimeGetResult) {
        Log.warningln("Failed to get local time with getLocalTime()");
    }

    delegate.setTextColor(1);
    delegate.setTextSize(2);
    delegate.setCursor(36, 4);
    delegate.setTextWrap(false);
    if (strftime(charBuf, ArraySize(charBuf), "%H:%M", &timeInfo) == 0) {
        Log.warningln("Failed to extract HH:mm with strftime()");
    }
    delegate.print(charBuf);
    delegate.setTextColor(1);
    delegate.setTextSize(2);
    delegate.setCursor(5, 22);
    delegate.setTextWrap(false);
    if (strftime(charBuf, ArraySize(charBuf), "%d-%m-%Y", &timeInfo) == 0) {
        Log.warningln("Failed to extract HH:mm with strftime()");
    }
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
    if (strftime(charBuf, ArraySize(charBuf), "%Z%z", &timeInfo) == 0) {
        Log.warningln("Failed to extract TZ with strftime()");
    }
    delegate.print(charBuf);

    delegate.setTextColor(1);
    delegate.setTextSize(1);
    delegate.setCursor(70, 54);
    delegate.setTextWrap(false);
    if (context.ntpTimeSyncOk) {
        delegate.print("SYNC OK");
    } else {
        delegate.print("NEED SYNC");
    }

    delegate.drawLine(23, 60, 68, 60, 1);
    delegate.drawLine(53, 51, 68, 51, 1);
}
