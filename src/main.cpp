#include <HardwareSerial.h>
#include <ArduinoLog.h>
#include "weatherBridge/display.hpp"
#include "weatherBridge/InputPullUpButton.hpp"

#define SERIAL_BAUD 9600
#define CONFIG_MODE_BUTTON_PIN 21

WeatherBridgeContext weatherBridgeContext = WeatherBridgeContext();
WeatherBridgeDisplay display = WeatherBridgeDisplay();
InputPullUpButton configModeButton = InputPullUpButton(CONFIG_MODE_BUTTON_PIN);

void setup() {
    Serial.begin(SERIAL_BAUD);
    Log.begin(LOG_LEVEL_INFO, &Serial);
    display.begin();
    configModeButton.begin();

    Log.infoln("Is in config mode: %d", configModeButton.isPressed());
}

void loop() {
    display.refresh();
}