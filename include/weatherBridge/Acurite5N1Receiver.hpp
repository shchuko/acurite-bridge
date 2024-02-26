#pragma once

#include <ArduinoJson.h>
#include "weatherBridge/TimeExpiringList.hpp"
#include "weatherBridge/StationMeasurements.hpp"

class Acurite5N1Receiver {
private:
    std::vector<std::function<void(StationMeasurements &)>> callbacks;
public:
    void registerCallback(const std::function<void(StationMeasurements &)> &callback);

    void receiveMessage(String &&message);
};