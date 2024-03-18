#pragma once

#include "weatherBridge/StationMeasurements.hpp"
#include "weatherBridge/TimedOptional.hpp"
#include "weatherBridge/TimeExpiringList.hpp"
#include "weatherBridge/types.hpp"

class MeasurementsStore {
public:
    static constexpr unsigned int WIND_AGGREGATES_MEASUREMENT_WINDOW = 4 * 60 * 1000;
private:
    // keep temperature, humidity etc. for 5 minutes as they are updated not as frequently as wind data
    static constexpr unsigned int NOT_WIND_MEASUREMENTS_EXPIRE_TIMEOUT = 5 * 60 * 1000;

    // keep wind measurements for 2 minutes as they are updated frequently
    static constexpr unsigned int WIND_MEASUREMENTS_EXPIRE_TIMEOUT = 2 * 60 * 1000;

    // keep
    int stationId = -1;

    StationModel stationModel = StationModel::NOT_SELECTED;
    TimedOptional<int> rssi = TimedOptional<int>::empty(NOT_WIND_MEASUREMENTS_EXPIRE_TIMEOUT);

    TimedOptional<int> humidity = TimedOptional<int>::empty(NOT_WIND_MEASUREMENTS_EXPIRE_TIMEOUT);
    TimedOptional<float> temperatureC = TimedOptional<float>::empty(NOT_WIND_MEASUREMENTS_EXPIRE_TIMEOUT);

    TimedOptional<float> windSpeedKmH = TimedOptional<float>::empty(WIND_MEASUREMENTS_EXPIRE_TIMEOUT);
    TimedOptional<float> windDirectorDeg = TimedOptional<float>::empty(WIND_MEASUREMENTS_EXPIRE_TIMEOUT);
    TimedOptional<float> windGustKmH = TimedOptional<float>::empty(WIND_MEASUREMENTS_EXPIRE_TIMEOUT);

    // Keep records for 1 hour
    TimeExpiringList<float> rainMmRecords = TimeExpiringList<float>(static_cast<unsigned int>(60 * 60 * 1000));
    TimedOptional<float> rainMmLastHour = TimedOptional<float>::empty(NOT_WIND_MEASUREMENTS_EXPIRE_TIMEOUT);

    TimedOptional<float> windMinKmH = TimedOptional<float>::empty(WIND_MEASUREMENTS_EXPIRE_TIMEOUT);
    TimedOptional<float> windAvgKmH = TimedOptional<float>::empty(WIND_MEASUREMENTS_EXPIRE_TIMEOUT);
    TimeExpiringList<float> windSpeedRecords = TimeExpiringList<float>(WIND_AGGREGATES_MEASUREMENT_WINDOW);

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