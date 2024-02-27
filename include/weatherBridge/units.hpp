#pragma once

#include <Arduino.h>

float kmPerHourToKnots(float speedInKmH);

float kmPerHourToMilesPerHour(float speedInKmH);

float celsiusToFahrenheit(float celsiusTemperature);

String convertDegreesToWindDirection(float degrees);

float mmToInch(float mm);