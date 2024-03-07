#pragma once

#include <Arduino.h>

template<typename T, size_t N>
inline constexpr size_t ArraySize(T (&)[N]) {
    return N;
}
