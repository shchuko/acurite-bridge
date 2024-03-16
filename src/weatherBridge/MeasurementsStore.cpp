#include "weatherBridge/MeasurementsStore.hpp"
#include "ArduinoLog.h"
#include "weatherBridge/LokiLogger.hpp"

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
        windGustKmH.set(max);
        windAvgKmH.set(static_cast<float>(sum / records.size()));
    }
    if (measurements.windDirectorDeg.hasValue()) {
        windDirectorDeg.set(measurements.windDirectorDeg.getValue());
    }
    if (measurements.rainMmAccumulation.hasValue()) {
        float accumulation = measurements.rainMmAccumulation.getValue();
        rainMmRecords.add(accumulation);

        // May contain many measurements, avoid copying, read internals
        const std::deque<std::pair<unsigned long, float>> &values = rainMmRecords.getValuesRaw();
        auto it = values.begin();
        float increase = 0.0f;

        String accumulationValuesAsString = "[";
        while (it != values.end()) {
            auto prev = it->second;
            accumulationValuesAsString += prev;

            if (it != values.end() - 1) {
                auto next = (it + 1)->second;
                // handle sequence resets
                if (next > prev) {
                    increase += next - prev;
                }
            }
            ++it;
        }
        accumulationValuesAsString += "]";

        String logLine = "Rain mm debug: accumulation=";
        logLine += accumulation;
        logLine += ", accumulationValues=";
        logLine += accumulationValuesAsString;
        logLine += ", lastHourIncrease=";
        logLine += increase;

        Log.infoln(logLine.c_str());
        LokiLogger::Instance.writeLog(logLine);

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

const TimedOptional<float> &MeasurementsStore::getRainMmLastHour() const {
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
