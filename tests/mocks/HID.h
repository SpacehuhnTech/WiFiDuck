#pragma once

#include "Arduino.h"
#include <vector>

struct HIDSubDescriptor {
    const uint8_t* data;
    size_t length;
    HIDSubDescriptor(const uint8_t* data, size_t length) : data(data), length(length) {}
};

struct HIDReport {
    uint8_t id;
    std::vector<uint8_t> data;
};

struct TestHID {
    std::vector<HIDSubDescriptor*> descriptors;
    std::vector<HIDReport> reports;
    void AppendDescriptor(HIDSubDescriptor* node) { descriptors.push_back(node); }
    int SendReport(uint8_t id, const void* data, int length) {
        const uint8_t* bytes = (const uint8_t*)data;
        reports.push_back({ id, std::vector<uint8_t>(bytes, bytes + length) });
        return length;
    }
};

extern TestHID testHID;
inline TestHID& HID() { return testHID; }
