#include "weatherBridge/units.hpp"

String convertDegreesToWindDirection(float degrees) {
    String directions[] = {"N",
                           "NNE",
                           "NE",
                           "ENE",
                           "E",
                           "ESE",
                           "SE",
                           "SSE",
                           "S",
                           "SSW",
                           "SW",
                           "WSW",
                           "W",
                           "WNW",
                           "NW",
                           "NNW",
                           "N"
    };
    return directions[static_cast<int>(round(degrees / 22.5 + 0.5)) % 16];
}

float kmPerHourToKnots(float speedInKmH) {
    const float conversionFactor = 0.539957;
    return speedInKmH * conversionFactor;
}
