#include "weatherBridge/LokiLogsExporter.hpp"

#include "weatherBridge/arrayExtensions.hpp"

constexpr char LokiLogsExporter::lokiRequestBodyTemplate[];
constexpr char LokiLogsExporter::projectLabel[];

LokiLogsExporter::LokiLogsExporter(Print *delegate, const String &lokiUrl, String username, String password)
        : delegate(delegate),
          lokiCompleteUrl(lokiUrl + "/loki/api/v1/push"),
          username(std::move(username)),
          password(std::move(password)) {}

size_t LokiLogsExporter::write(uint8_t c) {
    return write(&c, 1);
}

size_t LokiLogsExporter::write(const uint8_t *buffer, size_t size) {
    size_t result = delegate->write(buffer, size);
    sendToLoki(buffer, size);
    return result;
}

int LokiLogsExporter::availableForWrite() {
    int result = delegate->availableForWrite();
    return result;
}

void LokiLogsExporter::sendToLoki(const uint8_t *buffer, size_t size) {
    struct timespec ts{};
    char timestamp[25];
    unsigned long long nanoTime;
    char deviceIdLabel[13];

    sprintf(deviceIdLabel, "%012llx", ESP.getEfuseMac());

    clock_gettime(CLOCK_REALTIME, &ts);
    nanoTime = ts.tv_sec * 1000000000L + ts.tv_nsec;
    sprintf(timestamp, "%llu", nanoTime);

    const String escapedLogLine = escapeJson(buffer, size);

    char *resultStringBuf = new char[ArraySize(lokiRequestBodyTemplate) +
                                     strlen(projectLabel) +
                                     strlen(deviceIdLabel) +
                                     strlen(timestamp) +
                                     escapedLogLine.length() +
                                     1];
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
        delegate->printf("Loki logs push failed with http code %d\n", httpCode);
    }

    delete[] resultStringBuf;
}

String LokiLogsExporter::escapeJson(const uint8_t *buffer, size_t size) {
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
