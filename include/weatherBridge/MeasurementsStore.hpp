#pragma once

#include "weatherBridge/StationMeasurements.hpp"
#include "weatherBridge/TimedOptional.hpp"
#include "weatherBridge/TimeExpiringList.hpp"
#include "weatherBridge/types.hpp"

class MeasurementsStore {
public:
    static constexpr unsigned int windAggregatesMeasurementWindow = 4 * 60 * 1000;
private:
    // keep measurements for 2 minutes
    static constexpr unsigned int measurementExpireTimeout = 2 * 60 * 1000;
    int stationId = -1;


    StationModel stationModel = StationModel::NOT_SELECTED;
    TimedOptional<int> rssi = TimedOptional<int>::empty(measurementExpireTimeout);
    TimedOptional<float> temperatureC = TimedOptional<float>::empty(measurementExpireTimeout);
    TimedOptional<float> windSpeedKmH = TimedOptional<float>::empty(measurementExpireTimeout);
    TimedOptional<float> windDirectorDeg = TimedOptional<float>::empty(measurementExpireTimeout);
    TimedOptional<int> humidity = TimedOptional<int>::empty(measurementExpireTimeout);
    TimedOptional<float> windGustKmH = TimedOptional<float>::empty( measurementExpireTimeout);

    // Keep records for 1 hour
    TimeExpiringList<float> rainMmRecords = TimeExpiringList<float>(static_cast<unsigned int>(60 * 60 * 1000));
    TimedOptional<float> rainMmLastHour = TimedOptional<float>::empty(measurementExpireTimeout);

    TimedOptional<float> windMinKmH = TimedOptional<float>::empty(measurementExpireTimeout);
    TimedOptional<float> windAvgKmH = TimedOptional<float>::empty(measurementExpireTimeout);
    TimeExpiringList<float> windSpeedRecords = TimeExpiringList<float>(windAggregatesMeasurementWindow);

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

    const TimedOptional<float> &getRainMmLastHour() const;

    const TimedOptional<int> &getHumidity() const;

    const TimedOptional<float> &getWindMinKmH() const;

    const TimedOptional<float> &getWindAvgKmH() const;
};