#include <HardwareSerial.h>
#include <ArduinoLog.h>

#include "weatherBridge/LokiLogger.hpp"
#include "weatherBridge/WeatherBridge.hpp"

#define SERIAL_BAUD 9600
#define CONFIG_MODE_BUTTON_PIN 4

WeatherBridge weatherBridge = WeatherBridge(SPIFFS, CONFIG_MODE_BUTTON_PIN); // NOLINT(*-interfaces-global-init)

void setup() {
    Serial.begin(SERIAL_BAUD);
    Log.begin(LOG_LEVEL_TRACE, &Serial);

#ifdef LOKI_LOGGING_ENABLED
    LokiLogger::begin(LOKI_URL, LOKI_USERNAME, LOKI_PASSWORD);
#else
    LokiLogger::beginNoop();
#endif

#pragma clang diagnostic push
#pragma ide diagnostic ignored "EndlessLoop"
    if (!SPIFFS.begin(false)) {
        for (;;) {
            Log.errorln("An error has occurred while mounting SPIFFS");
            delay(1000);
        }
#pragma clang diagnostic pop
    }

    weatherBridge.begin();
    LokiLogger::Instance.writeLog("WeatherExporter initialized");
}

void loop() {
    weatherBridge.loop();
}