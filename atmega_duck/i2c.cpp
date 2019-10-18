/*
   Copyright (c) 2019 Stefan Kremser
   This software is licensed under the MIT License. See the license file for details.
   Source: github.com/spacehuhn/WiFiDuck
 */

#include "i2c.h"

#include <Wire.h>

#include "debug.h"
#include "duckparser.h"

#define RESPONSE_OK 0x00
#define RESPONSE_PROCESSING 0x01
#define RESPONSE_REPEAT 0x02

#define MIN_DELAY 5

namespace i2c {
    // ===== PRIVATE ===== //
    buffer_t buffer;
    bool     startParser { false };

    void requestEvent() {
        debugln("I2C REQUEST");

        if (buffer.len > 0) {
            unsigned int delayTime = duckparser::getDelayTime();
            if (delayTime > 255) {
                Wire.write(255);
            } else {
                Wire.write((uint8_t)delayTime | RESPONSE_PROCESSING | MIN_DELAY);
            }
            startParser = true;
            debug(".");
        } else if (duckparser::getRepeats() > 0) {
            Wire.write(RESPONSE_REPEAT);
            debugln("\nI2C REPEAT");
        } else {
            Wire.write(RESPONSE_OK);
            debugln("\nDone");
        }
    }

    void receiveEvent(int len) {
        debugln("RECEIVE");

        if (buffer.len + (unsigned int)len <= BUFFER_SIZE) {
            debugln("Received packet");
            Wire.readBytes(&buffer.data[buffer.len], len);
            buffer.len += len;
        } else {
            debugln("!!! Buffer is full !!!");
        }
    }

    // ===== PUBLIC ===== //
    void begin() {
        Wire.begin(I2C_ADDR);
        Wire.onRequest(requestEvent);
        Wire.onReceive(receiveEvent);
    }

    const buffer_t& getBuffer() {
        return buffer;
    }

    bool hasData() {
        return buffer.len > 0 && startParser;
    }

    void finished() {
        buffer.len  = 0;
        startParser = false;
    }
}