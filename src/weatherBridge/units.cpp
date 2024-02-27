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
    return speedInKmH * 0.539957f;
}

float celsiusToFahrenheit(float celsiusTemperature) {
    return celsiusTemperature * 9 / 5 + 32;
}

float kmPerHourToMilesPerHour(float speedInKmH) {
    return speedInKmH * 0.621371f;
}

float mmToInch(float mm) {
    return mm * 0.0393701f;
}
