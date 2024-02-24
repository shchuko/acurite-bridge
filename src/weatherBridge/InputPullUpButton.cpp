#include "weatherBridge/InputPullUpButton.hpp"

#include <ArduinoLog.h>

InputPullUpButton::InputPullUpButton(uint8_t pin) : pin(pin) {}

void InputPullUpButton::begin() const {
    Log.infoln("Creating InputPullUpButton on pin %d", pin);
    pinMode(pin, INPUT_PULLUP);
    delay(100);
}

boolean InputPullUpButton::isPressed() const {
    return digitalRead(pin) == 0;
}
