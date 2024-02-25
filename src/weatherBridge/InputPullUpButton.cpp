#include "weatherBridge/InputPullUpButton.hpp"

#include <ArduinoLog.h>

InputPullUpButton::InputPullUpButton(uint8_t pin) noexcept: pin(pin) {}

void InputPullUpButton::begin() const noexcept {
    Log.infoln("Creating InputPullUpButton on pin %d", pin);
    pinMode(pin, INPUT_PULLUP);
    delay(100);
}

bool InputPullUpButton::isPressed() const noexcept {
    return digitalRead(pin) == 0;
}
