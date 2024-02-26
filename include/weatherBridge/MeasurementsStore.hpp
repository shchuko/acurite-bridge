#pragma once

#include "weatherBridge/StationMeasurements.hpp"
#include "weatherBridge/TimedOptional.hpp"
#include "weatherBridge/types.hpp"

class MeasurementsStore {
private:
    // keep measurements for 2 minutes
    static constexpr unsigned int timeoutMilli = 120 * 1000;

    StationModel stationModel = StationModel::NOT_SELECTED;
    int stationId = -1;

    TimedOptional<int> rssi = TimedOptional<int>::empty(timeoutMilli);
    TimedOptional<float> temperatureC = TimedOptional<float>::empty(timeoutMilli);
    TimedOptional<float> windSpeedKmH = TimedOptional<float>::empty(timeoutMilli);
    TimedOptional<float> windGustKmH = TimedOptional<float>::empty(timeoutMilli);
    TimedOptional<float> windDirectorDeg = TimedOptional<float>::empty(timeoutMilli);
    TimedOptional<float> rainMm = TimedOptional<float>::empty(timeoutMilli);
    TimedOptional<int> humidity = TimedOptional<int>::empty(timeoutMilli);

public:
    MeasurementsStore() = default;

    MeasurementsStore(StationModel stationModel, int stationId);

    void loop();

    void updateMeasurements(const StationMeasurements &measurements);


    StationModel getStationModel() const;

    int getStationId() const;

    const TimedOptional<int> &getRssi() const;

    const TimedOptional<float> &getTemperatureC() const;

    const TimedOptional<float> &getWindSpeedKmH() const;

    const TimedOptional<float> &getWindGustKmH() const;

    const TimedOptional<float> &getWindDirectorDeg() const;

    const TimedOptional<float> &getRainMm() const;

    const TimedOptional<int> &getHumidity() const;
};