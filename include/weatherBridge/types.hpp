#pragma once

#include <cstdint>

enum class StationModel {
    NOT_SELECTED,
    ACURITE_5N1
};

enum class WifiSignal {
    BAD, NORMAL, GOOD, NO_CONNECTION
};

enum class StationSignal {
    BAD, NORMAL, GOOD, NO_CONNECTION
};

enum class WeatherExporterStatus {
    OFF, OK, ERROR
};
