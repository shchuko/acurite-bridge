#include <HardwareSerial.h>
#include <ArduinoLog.h>
#include "weatherBridge/display.hpp"

WeatherBridgeContext weatherBridgeContext = WeatherBridgeContext();
WeatherBridgeDisplay display = WeatherBridgeDisplay();

void setup() {
    Serial.begin(9600);
    Log.begin(LOG_LEVEL_INFO, &Serial);
    display.begin();
}

void loop() {
    display.refresh();
}