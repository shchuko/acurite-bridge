#pragma once

#include "weatherBridge/StationMeasurements.hpp"
#include "weatherBridge/TimedOptional.hpp"
#include "weatherBridge/TimeExpiringList.hpp"
#include "weatherBridge/types.hpp"

class MeasurementsStore {
private:
    // keep measurements for 2 minutes
    static constexpr unsigned int measurementExpireTimeout = 2 * 60 * 1000;
    static constexpr unsigned int windGustMeasurementWindow = 4 * 60 * 1000;
    int stationId = -1;


    StationModel stationModel = StationModel::NOT_SELECTED;
    TimedOptional<int> rssi = TimedOptional<int>::empty(measurementExpireTimeout);
    TimedOptional<float> temperatureC = TimedOptional<float>::empty(measurementExpireTimeout);
    TimedOptional<float> windSpeedKmH = TimedOptional<float>::empty(measurementExpireTimeout);
    TimedOptional<float> windDirectorDeg = TimedOptional<float>::empty(measurementExpireTimeout);
    TimedOptional<float> rainMm = TimedOptional<float>::empty(measurementExpireTimeout);
    TimedOptional<int> humidity = TimedOptional<int>::empty(measurementExpireTimeout);

    TimedOptional<float> windGustKmH = TimedOptional<float>::empty(measurementExpireTimeout);
    TimeExpiringList<float> windSpeedRecords = TimeExpiringList<float>(windGustMeasurementWindow);

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