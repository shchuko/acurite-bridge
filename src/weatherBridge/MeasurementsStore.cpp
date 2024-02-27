#include "weatherBridge/MeasurementsStore.hpp"

MeasurementsStore::MeasurementsStore(StationModel stationModel, int stationId) : stationModel(stationModel), stationId(stationId) {}

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
        windGustKmH.set(*std::max_element(std::begin(records), std::end(records)));
        windGustKmH.set(*std::min_element(std::begin(records), std::end(records)));
        double sum = 0.0;
        for (const auto &item: records) {
            sum += item;
        }
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
        const std::deque<std::pair<unsigned long, float>>& values = rainMmRecords.getValuesRaw();
        if (values.size() > 1) {
            float prevValue = std::begin(values)->second;
            float diff = value - prevValue;
            if (diff > 0) {
                rainMmLastHour.set(diff);
            } else {
                rainMmLastHour.set(0);
            }
        }
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
