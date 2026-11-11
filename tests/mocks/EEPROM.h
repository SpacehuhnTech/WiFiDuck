#pragma once

#include "Arduino.h"
#include <vector>

struct TestEEPROM {
    std::vector<uint8_t> bytes = std::vector<uint8_t>(4096, 0xff);
    int commits = 0;
    void begin(size_t) {}
    void end() {}
    bool commit() { ++commits; return true; }
    template<typename T> void put(int address, const T& value) {
        std::memcpy(bytes.data() + address, &value, sizeof(value));
    }
    template<typename T> void get(int address, T& value) {
        std::memcpy((void*)&value, bytes.data() + address, sizeof(value));
    }
};

extern TestEEPROM EEPROM;
