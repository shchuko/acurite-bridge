#include "weatherBridge/MeasurementsStore.hpp"

MeasurementsStore::MeasurementsStore(StationModel stationModel, int stationId) : stationModel(stationModel), stationId(stationId) {}

void MeasurementsStore::loop() {
    rssi.maybeExpireValue();
    temperatureC.maybeExpireValue();
    windSpeedKmH.maybeExpireValue();
    windDirectorDeg.maybeExpireValue();
    rainMm.maybeExpireValue();
    humidity.maybeExpireValue();
    
    windGustKmH.maybeExpireValue();
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
    }
    if (measurements.windDirectorDeg.hasValue()) {
        windDirectorDeg.set(measurements.windDirectorDeg.getValue());
    }
    if (measurements.rainMm.hasValue()) {
        rainMm.set(measurements.rainMm.getValue());
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
    return rainMm;
}

const TimedOptional<int> &MeasurementsStore::getHumidity() const {
    return humidity;
}
