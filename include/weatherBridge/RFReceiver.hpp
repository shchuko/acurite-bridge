#pragma once

#include <Arduino.h>
#include <ArduinoLog.h>
#include <rtl_433_ESP.h>


class RFReceiver {
private:
    rtl_433_ESP rf;
public:

    void begin();

    void loop();

    bool available() const;

    String read();
};


