#pragma once

#include <Arduino.h>

template<typename T>
class TimedOptional {
private:
    T value;
    bool isPresent;
    unsigned long lastUpdated;
    unsigned int timeout;


public:
    explicit TimedOptional(unsigned int timeout) : timeout(timeout), isPresent(false), lastUpdated(millis()) {}

    explicit TimedOptional(unsigned int timeout, T &&value) : timeout(timeout), value(std::move(value)),
                                                              isPresent(true),
                                                              lastUpdated(millis()) {}

    static TimedOptional<T> empty(unsigned int timeout) {
        return TimedOptional<T>(timeout);
    }

    static TimedOptional<T> of(T t) {
        return TimedOptional<T>(t);
    }

    void maybeExpireValue() {
        if (!isPresent) {
            return;
        }

        if (millis() - lastUpdated > timeout) {
            isPresent = false;
        }
    }

    bool hasValue() const {
        return isPresent;
    }

    T getValue() const {
        return value;
    }

    void set(T val) {
        isPresent = true;
        value = val;
        lastUpdated = millis();
    }

};
