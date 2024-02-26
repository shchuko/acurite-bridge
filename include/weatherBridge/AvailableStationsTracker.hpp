#pragma once

#include <ArduinoLog.h>
#include "weatherBridge/StationMeasurements.hpp"
#include "weatherBridge/TimeExpiringList.hpp"
#include "weatherBridge/types.hpp"

class AvailableStationsTracker {
private:
    // Keep stations in list for 10 minutes
    static constexpr unsigned int lifetimeMilli = 10 * 60 * 1000;
    TimeExpiringList<std::pair<StationModel, int>> stations =
            TimeExpiringList<std::pair<StationModel, int>>(lifetimeMilli);

public:

    void loop();

    void receiveData(const StationMeasurements &measurements);
};
