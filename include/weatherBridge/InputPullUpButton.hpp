#pragma once

#include <Arduino.h>

class InputPullUpButton {
private:
    uint8_t pin;

public:
    explicit InputPullUpButton(uint8_t pin);

    void begin() const;

    boolean isPressed() const;
};
