#pragma once

#include "Optional.hpp"
#include "types.hpp"

class StationMeasurements {
public:
    const StationModel stationModel;
    const int stationId;
    const int rssi;
    const Optional<float> temperatureC = Optional<float>::empty();
    const Optional<float> windSpeedKmH = Optional<float>::empty();
    const Optional<float> windDirectorDeg = Optional<float>::empty();
    const Optional<float> rainMmAccumulation = Optional<float>::empty();
    const Optional<int> humidity = Optional<int>::empty();

    StationMeasurements(StationModel stationModel, int stationId, int rssi,
                        const Optional<float> &temperatureC, const Optional<float> &windSpeedKmH,
                        const Optional<float> &windDirectorDeg, const Optional<float> &rainMm,
                        const Optional<int> &humidity);
};