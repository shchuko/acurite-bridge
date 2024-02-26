#include "weatherBridge/display/pages/WeatherPage.hpp"
#include "weatherBridge/units.hpp"

static const unsigned char PROGMEM image_weather_temperature_bits[] = {0x1c, 0x00, 0x22, 0x02, 0x2b, 0x05, 0x2a,
                                                                       0x02, 0x2b, 0x38, 0x2a, 0x60, 0x2b, 0x40,
                                                                       0x2a, 0x40, 0x2a, 0x60, 0x49, 0x38, 0x9c,
                                                                       0x80, 0xae, 0x80, 0xbe, 0x80, 0x9c, 0x80,
                                                                       0x41, 0x00, 0x3e, 0x00};
static const unsigned char PROGMEM image_weather_wind_bits[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0x03, 0x88,
                                                                0x04, 0x44, 0x04, 0x44, 0x00, 0x44, 0x00, 0x88,
                                                                0xff, 0x32, 0x00, 0x00, 0xad, 0x82, 0x00, 0x60,
                                                                0x00, 0x10, 0x00, 0x10, 0x01, 0x20, 0x00, 0xc0};


WeatherPage::WeatherPage(Adafruit_GFX &display) : DisplayPage(display) {}

void WeatherPage::paint(WeatherBridgeContext context) {
    char buf[10];
    delegate.drawBitmap(9, 5, image_weather_temperature_bits, 16, 16, 1);
    delegate.setTextColor(1);
    delegate.setTextSize(2);
    delegate.setCursor(29, 26);
    delegate.setTextWrap(false);
    if (context.measurementsStore.getWindSpeedKmH().hasValue()) {
        float knots = kmPerHourToKnots(context.measurementsStore.getWindSpeedKmH().getValue());
        sprintf(buf, "%.1fkts", knots);
        delegate.print(buf);
    } else {
        delegate.print("---kts");
    }

    delegate.setTextColor(1);
    delegate.setTextSize(2);
    delegate.setCursor(29, 6);
    delegate.setTextWrap(false);
    if (context.measurementsStore.getTemperatureC().hasValue()) {
        float tempC = context.measurementsStore.getTemperatureC().getValue();
        if (tempC > 0.0) {
            sprintf(buf, "+%.1fC", tempC);
        } else {
            sprintf(buf, "%.1fC", tempC);
        }
        delegate.print(buf);
    } else {
        delegate.print("---C");
    }

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
    if (context.measurementsStore.getWindDirectorDeg().hasValue()) {
        float directionDegrees = context.measurementsStore.getWindDirectorDeg().getValue();
        delegate.print(convertDegreesToWindDirection(directionDegrees));
    } else {
        delegate.print("---");
    }

    delegate.setTextColor(1);
    delegate.setTextSize(1);
    delegate.setCursor(56, 42);
    delegate.setTextWrap(false);
    if (context.measurementsStore.getWindGustKmH().hasValue()) {
        float knots = kmPerHourToKnots(context.measurementsStore.getWindGustKmH().getValue());
        sprintf(buf, "%.1fkts", knots);
        delegate.print(buf);
    } else {
        delegate.print("---kts");
    }
}

