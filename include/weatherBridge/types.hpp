#pragma once

#include <cstdint>

enum class StationModel {
    NOT_SELECTED,
    ACURITE_5N1
};

enum class WeatherExporterStatus {
    OFF, OK, ERROR, INIT, NO_DATA, NTP_ERR
};
