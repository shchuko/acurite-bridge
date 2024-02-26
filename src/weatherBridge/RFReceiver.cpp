#include <ArduinoLog.h>

#include "weatherBridge/RFReceiver.hpp"
#include "weatherBridge/RingBuffer.hpp"
#include "weatherBridge/arrayExtensions.hpp"

static constexpr float RF_MODULE_FREQUENCY = 433.92;
static constexpr size_t MESSAGE_QUEUE_SIZE = 30;
static char receiveBuffer[512];
static RingBuffer<String> messageQueue(MESSAGE_QUEUE_SIZE);

void RFReceiver::begin() {
    Log.noticeln("Starting %f receiver", RF_MODULE_FREQUENCY);
    rtl_433_ESP::initReceiver(RF_MODULE_RECEIVER_GPIO, RF_MODULE_FREQUENCY);
    rf.setCallback([](char *message) -> void {
        if (!messageQueue.enqueue(String(message))) {
            Log.errorln("RFReceiver: message queue is full. Dropping message.");
        }
    }, receiveBuffer, static_cast<int>(ArraySize(receiveBuffer)));
    rtl_433_ESP::enableReceiver();
    rtl_433_ESP::getModuleStatus();
}

#pragma clang diagnostic push
#pragma ide diagnostic ignored "readability-convert-member-functions-to-static"

bool RFReceiver::available() const {
    return !messageQueue.isEmpty();
}

String RFReceiver::read() {
    return std::move(messageQueue.dequeue());
}

void RFReceiver::loop() {
    rf.loop();
}

#pragma clang diagnostic pop

