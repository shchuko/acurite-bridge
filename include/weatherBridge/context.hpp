#pragma once

#include <Arduino.h>

class TimeContext {
private:
    int timezoneShift;

public:
    int getTimezoneShift() const {
        return timezoneShift;
    }

#pragma clang diagnostic push
#pragma ide diagnostic ignored "readability-convert-member-functions-to-static"

    time_t getCurrentTime() {
        return now();
    }

    timeStatus_t getCurrentTimeStatus() {
        return timeStatus();
    }

#pragma clang diagnostic pop
};


struct WeatherBridgeContext {
    WeatherBridgeContext(): time(TimeContext()) {}

    TimeContext time;
};

extern WeatherBridgeContext weatherBridgeContext;

