#pragma once

#include <cstdint>

enum class WifiSignal {
    BAD, NORMAL, GOOD, NO_SIGNAL
};

enum class StationSignal {
    BAD, NORMAL, GOOD, NO_SIGNAL
};

enum class WeatherExporterStatus {
    OFF, OK, ERROR
};
