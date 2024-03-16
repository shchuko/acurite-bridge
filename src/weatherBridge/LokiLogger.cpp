#include "weatherBridge/LokiLogger.hpp"

#include "weatherBridge/arrayExtensions.hpp"
#include "ArduinoLog.h"

constexpr char LokiLogger::lokiRequestBodyTemplate[];
constexpr char LokiLogger::projectLabel[];


LokiLogger::LokiLogger() = default;

LokiLogger LokiLogger::Instance = LokiLogger();

void LokiLogger::begin(const String &lokiUrl, const String &lokiUsername, const String& lokiPassword) {
    Instance.isEnabled = false;
    Instance.lokiCompleteUrl = lokiUrl + "/loki/api/v1/push";
    Instance.username = lokiUsername;
    Instance.password = lokiPassword;
    Instance.isEnabled = true;

    Log.infoln("Loki logs exporter: configured to send to %s", Instance.lokiCompleteUrl.c_str());
}

void LokiLogger::writeLog(const String &line) {
    if (!isEnabled) {
        return;
    }

    if (!WiFi.isConnected()) {
        Log.infoln("Not connected to WiFi, skipping log send to Loki");
        return;
    }
    sendToLoki(line.c_str(), line.length());
}


void LokiLogger::sendToLoki(const char *buffer, size_t size) {
    char timestamp[25];
    time_t current_time;
    unsigned long long nanoTime;
    char deviceIdLabel[13];

    sprintf(deviceIdLabel, "%012llx", ESP.getEfuseMac());

    // get current time
    current_time = time(nullptr);
    if(current_time == ((time_t)-1)) {
        Log.errorln("LokiLogger: The time function failed");
        return;
    }

    nanoTime = current_time * 1000000000LLU;
    sprintf(timestamp, "%llu", nanoTime);

    const String escapedLogLine = escapeJson(buffer, size);

    char *resultStringBuf = new char[ArraySize(lokiRequestBodyTemplate) +
                                     strlen(projectLabel) +
                                     strlen(deviceIdLabel) +
                                     strlen(timestamp) +
                                     escapedLogLine.length() +
                                     10];
    sprintf(resultStringBuf,
            lokiRequestBodyTemplate,
            projectLabel,
            deviceIdLabel,
            timestamp,
            escapedLogLine.c_str());

    HTTPClient http;
    http.begin(lokiCompleteUrl);
    http.setAuthorization(username.c_str(), password.c_str());
    http.addHeader("Content-Type", "application/json");
    int httpCode = http.POST(resultStringBuf);
    http.end();

    if (httpCode < 200 || httpCode >= 400) {
        Log.errorln("Loki logs push failed with http code %d, request body was %s\n", httpCode, resultStringBuf);
    }

    delete[] resultStringBuf;
}

String LokiLogger::escapeJson(const char *buffer, size_t size) {
    String output;
    output.reserve(size);

    for (size_t i = 0; i < size; i++) {
        switch (buffer[i]) {
            case '"':
                output += "\\\"";
                break;
            case '\\':
                output += "\\\\";
                break;
            case '\b':
                output += "\\b";
                break;
            case '\f':
                output += "\\f";
                break;
            case '\n':
                output += "\\n";
                break;
            case '\r':
                output += "\\r";
                break;
            case '\t':
                output += "\\t";
                break;
            default:
                output += buffer[i];
        }
    }

    return output;
}

void LokiLogger::beginNoop() {
    Instance.isEnabled = false;
}
