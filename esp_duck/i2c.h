/*
   Copyright (c) 2019 Stefan Kremser
   This software is licensed under the MIT License. See the license file for details.
   Source: github.com/spacehuhn/WiFiDuck
 */

#pragma once

#include <stdint.h>  // uint8_t
#include <stddef.h>  // size_t
#include <Arduino.h> // Stream

typedef void (* i2c_callback)();

namespace i2c {
    enum class status {
        OK,
        PROCESSING,
        REPEAT,
        ERROR
    };

    void begin();
    void update();

    bool connected();
    uint8_t getResponse();
    status getStatus();

    void sendRequest();
    size_t transmit(const uint8_t* data, size_t len);

    void setOnOK(i2c_callback c);
    void setOnProcessing(i2c_callback c);
    void setOnRepeat(i2c_callback c);
    void setOnError(i2c_callback c);
}