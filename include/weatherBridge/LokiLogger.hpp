#pragma once

#include <HTTPClient.h>
#include <Print.h>

#include "weatherBridge/arrayExtensions.hpp"

class LokiLogger {
private:
    static constexpr char lokiRequestBodyTemplate[] = R"json(
        {
          "streams": [
            {
              "stream": {
                "project": "%s",
                "deviceId": "%s"
              },
              "values": [
                  [ "%s", "%s" ]
              ]
            }
          ]
        })json";

    static constexpr char projectLabel[] = "weather-exporter";

    String lokiCompleteUrl = "";
    String username = "";
    String password = "";
    bool isEnabled = false;

    LokiLogger();


public:
    static LokiLogger Instance;

    static void beginNoop();

    static void begin(const String &lokiUrl, const String &lokiUsername, const String& lokiPassword);

    void writeLog(const String &line);


private:
    void sendToLoki(const char *buffer, size_t size);

    static String escapeJson(const char *buffer, size_t size);
};
