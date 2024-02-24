#pragma once

#include <Arduino.h>
#include <TimeLib.h>

class TimeContext {
private:
    int timezoneShift;

public:
    explicit TimeContext(int timezoneShift) : timezoneShift(timezoneShift) {}

#pragma clang diagnostic push
#pragma ide diagnostic ignored "readability-convert-member-functions-to-static"

    time_t getCurrentTime() {
        return now();
    }

    timeStatus_t getCurrentTimeStatus() {
        return timeStatus();
    }

#pragma clang diagnostic pop

    int getTimezoneShift() const {
        return timezoneShift;
    }
};


struct WifiAPContext {
    const String ssid;
    const String password;
    const String ip;

    WifiAPContext(String &&ssid, String &&password, String &&ip) : ssid(std::move(ssid)), password(std::move(password)),
                                                                   ip(std::move(ip)) {}
};

struct WeatherBridgeContext {
    bool isInConfigurationMode = false;

    TimeContext *time = nullptr;
    WifiAPContext *wifiApContext = nullptr;

    ~WeatherBridgeContext() {
        free(time);
        free(wifiApContext);
    }

    void updateTimeContext(TimeContext *time) {
        free(this->time);
        this->time = time;
    }

    void updateWiFiAPContext(WifiAPContext *wifiApContext) {
        free(this->wifiApContext);
        this->wifiApContext = wifiApContext;
    }
};

extern WeatherBridgeContext weatherBridgeContext;

