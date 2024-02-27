#include "weatherBridge/StationMeasurements.hpp"

StationMeasurements::StationMeasurements(StationModel stationModel, int stationId, int rssi,
                                         const Optional<float> &temperatureC, const Optional<float> &windSpeedKmH,
                                         const Optional<float> &windDirectorDeg, const Optional<float> &rainMm,
                                         const Optional<int> &humidity) : stationModel(stationModel),
                                                                          stationId(stationId), rssi(rssi),
                                                                          temperatureC(temperatureC),
                                                                          windSpeedKmH(windSpeedKmH),
                                                                          windDirectorDeg(windDirectorDeg),
                                                                          rainMmAccumulation(rainMm),
                                                                          humidity(humidity) {}
