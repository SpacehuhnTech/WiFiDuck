#pragma once

#include "Arduino.h"
#include <vector>

struct TestWire {
    std::vector<uint8_t> incoming;
    std::vector<uint8_t> outgoing;
    void (*receive)(int) = nullptr;
    void (*request)() = nullptr;

    void begin(int) {}
    void onReceive(void (*callback)(int)) { receive = callback; }
    void onRequest(void (*callback)()) { request = callback; }
    void write(const uint8_t* data, size_t length) {
        outgoing.insert(outgoing.end(), data, data + length);
    }
    void readBytes(char* data, size_t length) {
        std::memcpy(data, incoming.data(), length);
        incoming.erase(incoming.begin(), incoming.begin() + length);
    }
    void deliver(const std::vector<uint8_t>& bytes) {
        incoming = bytes;
        receive((int)bytes.size());
    }
};

extern TestWire Wire;
