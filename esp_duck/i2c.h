/*
   Copyright (c) 2019 Stefan Kremser
   This software is licensed under the MIT License. See the license file for details.
   Source: github.com/spacehuhn/WiFiDuck
 */

#pragma once

#include <stdint.h> // uint8_t
#include <stddef.h> // size_t

namespace i2c {
    enum class status {
        OK         = 0x00,
        PROCESSING = 0x01,
        REPEAT     = 0x02,
        ERROR      = 0xFF
    };

    void begin();
    void update();

    bool connected();
    uint8_t getResponse();
    status getStatus();

    void sendRequest();
    void transmit(uint8_t* data, size_t len);
}