#pragma once

#include <HTTPClient.h>
#include <Print.h>

#include "weatherBridge/arrayExtensions.hpp"

class LokiLogsExporter : public Print {
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
                  [ "%s", "%s" ],
              ]
            }
          ]
        })json";

    static constexpr char projectLabel[] = "weather-exporter";

    Print *delegate;
    const String lokiCompleteUrl;
    const String username;
    const String password;

public:
    LokiLogsExporter(Print *delegate, const String& lokiUrl, String username, String password);

    size_t write(uint8_t c) override;

    size_t write(const uint8_t *buffer, size_t size) override;

    int availableForWrite() override;

    void flush() override {
        delegate->flush();
    }

private:
    void sendToLoki(const uint8_t *buffer, size_t size);

    static String escapeJson(const uint8_t *buffer, size_t size);
};
