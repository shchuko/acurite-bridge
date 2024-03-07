#include <HardwareSerial.h>
#include <ArduinoLog.h>

#include "weatherBridge/LokiLogsExporter.hpp"
#include "weatherBridge/WeatherBridge.hpp"

#define SERIAL_BAUD 9600
#define CONFIG_MODE_BUTTON_PIN 4

WeatherBridge weatherBridge = WeatherBridge(SPIFFS, CONFIG_MODE_BUTTON_PIN); // NOLINT(*-interfaces-global-init)

#if defined(LOGS_LOKI_URL) && defined(LOGS_LOKI_USERNAME) && defined(LOGS_LOKI_PASSWORD)

#define STRINGIFY(x) #x

LokiLogsExporter lokiLogsExporter = LokiLogsExporter( // NOLINT(*-interfaces-global-init)
        &Serial,
        STRINGIFY(LOGS_LOKI_URL),
        STRINGIFY(LOGS_LOKI_USERNAME),
        STRINGIFY(LOGS_LOKI_PASSWORD)
);

Print *logPrint = &lokiLogsExporter;
#else
Print *logPrint = &Serial; // NOLINT(*-interfaces-global-init)
#endif


void setup() {
    Serial.begin(SERIAL_BAUD);
    Log.begin(LOG_LEVEL_TRACE, logPrint);

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
}

void loop() {
    weatherBridge.loop();
}