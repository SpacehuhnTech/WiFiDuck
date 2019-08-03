/*
   Copyright (c) 2019 Stefan Kremser
   This software is licensed under the MIT License. See the license file for details.
   Source: github.com/spacehuhn/WiFiDuck
 */

#pragma once

#include <stddef.h> // size_t
#include "config.h" // BUFFER_SIZE

typedef struct buffer_t {
    char   data[BUFFER_SIZE];
    size_t len;
} buffer_t;

namespace i2c {
    void begin();

    const buffer_t& getBuffer();
    bool hasData();

    void finished();
}