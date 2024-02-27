#include "weatherBridge/MeasurementsStore.hpp"

MeasurementsStore::MeasurementsStore(StationModel stationModel, int stationId) : stationModel(stationModel),
                                                                                 stationId(stationId) {}

void MeasurementsStore::loop() {
    rssi.maybeExpireValue();
    temperatureC.maybeExpireValue();
    windSpeedKmH.maybeExpireValue();
    windDirectorDeg.maybeExpireValue();
    rainMmLastHour.maybeExpireValue();
    rainMmRecords.loop();
    humidity.maybeExpireValue();

    windGustKmH.maybeExpireValue();
    windMinKmH.maybeExpireValue();
    windAvgKmH.maybeExpireValue();
    windSpeedRecords.loop();
}

void MeasurementsStore::updateMeasurements(const StationMeasurements &measurements) {
    if (measurements.stationModel != stationModel || measurements.stationId != stationId) {
        return;
    }

    rssi.set(measurements.rssi);
    if (measurements.temperatureC.hasValue()) {
        temperatureC.set(measurements.temperatureC.getValue());
    }
    if (measurements.windSpeedKmH.hasValue()) {
        float speed = measurements.windSpeedKmH.getValue();
        windSpeedKmH.set(speed);
        windSpeedRecords.add(speed);

        std::vector<float> records = windSpeedRecords.getValues();
        double sum = 0.0;
        float min = speed;
        float max = speed;
        for (const auto &item: records) {
            sum += item;
            if (item < min) {
                min = item;
            }
            if (item > max) {
                max = item;
            }
        }
        windMinKmH.set(min);
        windGustKmH.set(min);
        windAvgKmH.set(static_cast<float>(sum / records.size()));
    }
    if (measurements.windDirectorDeg.hasValue()) {
        windDirectorDeg.set(measurements.windDirectorDeg.getValue());
    }
    if (measurements.rainMmAccumulation.hasValue()) {
        float value = measurements.rainMmAccumulation.getValue();
        rainMmRecords.add(value);
        rainMmLastHour.set(value);

        // May contain many measurements, avoid copying, read internals
        const std::deque<std::pair<unsigned long, float>> &values = rainMmRecords.getValuesRaw();
        auto it = values.begin();
        float increase = 0.0f;
        while (it != values.end() && it != values.end() - 1) {
            auto first = it->second;
            auto second = (it + 1)->second;
            // handle sequence resets
            if (second > first) {
                increase += second - first;
            }
            ++it;
        }
        rainMmLastHour.set(increase);
    }
    if (measurements.humidity.hasValue()) {
        humidity.set(measurements.humidity.getValue());
    }
}

StationModel MeasurementsStore::getStationModel() const {
    return stationModel;
}

int MeasurementsStore::getStationId() const {
    return stationId;
}

const TimedOptional<int> &MeasurementsStore::getRssi() const {
    return rssi;
}

const TimedOptional<float> &MeasurementsStore::getTemperatureC() const {
    return temperatureC;
}

const TimedOptional<float> &MeasurementsStore::getWindSpeedKmH() const {
    return windSpeedKmH;
}

const TimedOptional<float> &MeasurementsStore::getWindGustKmH() const {
    return windGustKmH;
}

const TimedOptional<float> &MeasurementsStore::getWindDirectorDeg() const {
    return windDirectorDeg;
}

const TimedOptional<float> &MeasurementsStore::getRainMm() const {
    return rainMmLastHour;
}

const TimedOptional<int> &MeasurementsStore::getHumidity() const {
    return humidity;
}

const TimedOptional<float> &MeasurementsStore::getWindMinKmH() const {
    return windMinKmH;
}

const TimedOptional<float> &MeasurementsStore::getWindAvgKmH() const {
    return windAvgKmH;
}
