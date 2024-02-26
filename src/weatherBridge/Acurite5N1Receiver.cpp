#include <ArduinoLog.h>

#include "weatherBridge/Acurite5N1Receiver.hpp"
#include "weatherBridge/types.hpp"

#define ACURITE_5N1_MODEL_NAME "Acurite-5n1"
#define ACURITE_MSGTYPE_5N1_WINDSPEED_WINDDIR_RAINFALL  0x31
#define ACURITE_MSGTYPE_5N1_WINDSPEED_TEMP_HUMIDITY     0x38


void Acurite5N1Receiver::registerCallback(const std::function<void(StationMeasurements &)> &callback) {
    callbacks.push_back(callback);
}

void Acurite5N1Receiver::receiveMessage(String &&message) {
    JsonDocument doc;
    deserializeJson(doc, message);
    Log.traceln("Received message: %s", message.c_str());
    String model = doc["model"];
    if (model != ACURITE_5N1_MODEL_NAME) {
        Log.traceln("Model not matches expected=%s,provided=%s", ACURITE_5N1_MODEL_NAME, model.c_str());
        return;
    }

    int id = doc["id"];
    int messageType = doc["message_type"];

    if (messageType == ACURITE_MSGTYPE_5N1_WINDSPEED_WINDDIR_RAINFALL) {
        Log.traceln("Reading WINDSPEED_WINDDIR_RAINFALL message");
        auto measurements = StationMeasurements(
                StationModel::ACURITE_5N1,
                id,
                doc["rssi"],
                Optional<float>::empty(),
                Optional<float>::of(doc["wind_avg_km_h"]),
                Optional<float>::of(doc["wind_dir_deg"]),
                Optional<float>::of(doc["rain_mm"]),
                Optional<int>::empty()
        );
        for (const auto &item: callbacks) {
            item(measurements);
        }
    } else if (messageType == ACURITE_MSGTYPE_5N1_WINDSPEED_TEMP_HUMIDITY) {
        Log.traceln("Reading WINDSPEED_TEMP_HUMIDITY message");

        auto measurements = StationMeasurements(
                StationModel::ACURITE_5N1,
                id,
                doc["rssi"],
                Optional<float>::of(doc["temperature_C"]),
                Optional<float>::of(doc["wind_avg_km_h"]),
                Optional<float>::empty(),
                Optional<float>::empty(),
                Optional<int>::of(doc["humidity"])
        );
        for (const auto &item: callbacks) {
            item(measurements);
        }
    } else {
        Log.warningln("Acurite5N1Receiver: Unknown message type %d", messageType);
        return;
    }
}
