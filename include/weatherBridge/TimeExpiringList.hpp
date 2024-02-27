#pragma once

#include <Arduino.h>
#include <vector>
#include <deque>

template<typename T>
class TimeExpiringList {
private:
    unsigned int lifetimeMillis;
    std::deque<std::pair<unsigned long, T>> values;

public:
    explicit TimeExpiringList(unsigned int lifetimeMillis) : lifetimeMillis(lifetimeMillis) {}

    void loop() {
        auto currentTimeMillis = millis();
        auto iter = std::remove_if(std::begin(values), std::end(values),
                                   [&](std::pair<unsigned long, T> value) {
                                       return currentTimeMillis - value.first > lifetimeMillis;
                                   });
        values.erase(iter, std::end(values));
    }

    void add(T &&value) {
        values.push_back(std::make_pair(millis(), std::move(value)));
    }

    void add(T value) {
        values.push_back(std::make_pair(millis(), value));
    }

    bool contains(const T &value) {
        return std::find_if(values.begin(), values.end(), [&](const std::pair<unsigned long, T> &pair) {
            return pair.second == value;
        }) != values.end();
    }

    std::vector<T> getValues() const {
        std::vector<T> result(values.size());
        std::transform(values.begin(), values.end(), result.begin(),
                       [](const std::pair<unsigned long, T> &value) {
                           return value.second;
                       });
        return result;
    }
    const std::deque<std::pair<unsigned long, T>> &getValuesRaw() const {
        return values;
    }
};