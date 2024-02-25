#include "weatherBridge/display/pages/WeatherExportersPage.hpp"


static const unsigned char PROGMEM image_menu_arrow_down_left_bits[] = {0x80, 0xc0, 0xe0};



static const unsigned char PROGMEM image_list_element_tick[] = {0x80, 0xc0, 0xe0};


WeatherExportersPage::WeatherExportersPage(Adafruit_GFX &display) : DisplayPage(display) {}

void WeatherExportersPage::paint(WeatherBridgeContext context) {
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

