#include <Arduino.h>

template<typename T, size_t N>
static size_t ArraySize(T (&)[N]) {
    return N;
}
