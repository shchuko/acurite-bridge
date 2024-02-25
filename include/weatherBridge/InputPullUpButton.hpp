#pragma once

#include <Arduino.h>

class InputPullUpButton {
private:
    const uint8_t pin;

public:
    explicit InputPullUpButton(uint8_t pin) noexcept;

    void begin() const noexcept;

    bool isPressed() const noexcept;
};
