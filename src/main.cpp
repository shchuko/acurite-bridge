#include <HardwareSerial.h>
#include <ArduinoLog.h>
#include "weatherBridge/display.hpp"
#include "weatherBridge/InputPullUpButton.hpp"
#include "weatherBridge/ConfigurationModeWiFi.hpp"

#define SERIAL_BAUD 9600
#define CONFIG_MODE_BUTTON_PIN 21

WeatherBridgeContext weatherBridgeContext = WeatherBridgeContext();
WeatherBridgeDisplay display = WeatherBridgeDisplay();
InputPullUpButton configModeButton = InputPullUpButton(CONFIG_MODE_BUTTON_PIN);
ConfigurationModeWiFi configurationModeWiFi = ConfigurationModeWiFi();


void setup() {
    Serial.begin(SERIAL_BAUD);
    Log.begin(LOG_LEVEL_INFO, &Serial);
    display.begin();
    configModeButton.begin();

    // TODO use timeProvider.begin()
    weatherBridgeContext.updateTimeContext(new TimeContext(0));

    if (configModeButton.isPressed()) {
        Log.noticeln("Starting in configuration mode");
        weatherBridgeContext.isInConfigurationMode = true;
        configurationModeWiFi.begin();
    } else {
        Log.noticeln("Starting in regular mode");
    }
}

void loop() {
    display.refresh();
}