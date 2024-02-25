#include <HardwareSerial.h>
#include <ArduinoLog.h>

#include "weatherBridge/WeatherBridge.hpp"

#define SERIAL_BAUD 9600
#define CONFIG_MODE_BUTTON_PIN 4

WeatherBridge weatherBridge = WeatherBridge(SPIFFS, CONFIG_MODE_BUTTON_PIN); // NOLINT(*-interfaces-global-init)

void setup() {
    Serial.begin(SERIAL_BAUD);
    Log.begin(LOG_LEVEL_TRACE, &Serial);

#pragma clang diagnostic push
#pragma ide diagnostic ignored "EndlessLoop"
    if (!SPIFFS.begin(false)) {
        for (;;) {
            Log.errorln("An error has occurred while mounting SPIFFS");
        }
#pragma clang diagnostic pop
    }

    weatherBridge.begin();
}

void loop() {
    weatherBridge.loop();
}