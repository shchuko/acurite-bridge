#include "weatherBridge/AvailableStationsTracker.hpp"


void AvailableStationsTracker::loop() {
    stations.loop();
}

void AvailableStationsTracker::receiveData(const StationMeasurements &measurements) {
    if (measurements.stationModel == StationModel::NOT_SELECTED) {
        return;
    }

    auto newStation = std::make_pair(measurements.stationModel, measurements.stationId);
    if (!stations.contains(newStation)) {
        stations.add(newStation);
        Log.noticeln("Detected new station, model=%d id=%d", measurements.stationModel, measurements.stationId);
    }
}

std::vector<std::pair<StationModel, int>> AvailableStationsTracker::getStations() const {
    return stations.getValues();
}
