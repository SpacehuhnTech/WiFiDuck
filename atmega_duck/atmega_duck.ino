/*
   Copyright (c) 2019 Stefan Kremser
   This software is licensed under the MIT License. See the license file for details.
   Source: github.com/spacehuhn/WiFiDuck
 */

#include "config.h"
#include "debug.h"

#include "keyboard.h"
#include "led.h"
#include "i2c.h"
#include "duckparser.h"

// ===== SETUP ====== //
void setup() {
#ifdef ENABLE_DEBUG
    Serial.begin(DEBUG_BAUD);
#endif // ifdef ENABLE_DEBUG

    keyboard::begin();
    led::begin();
    i2c::begin();

    debugln("Started!");
}

// ===== LOOOP ===== //
void loop() {
    if (i2c::hasData()) {
        const buffer_t& buffer = i2c::getBuffer();

        debug("Interpreting: ");

        for (size_t i = 0; i<buffer.len; i++) debug(buffer.data[i]);

        duckparser::parse(buffer.data, buffer.len);

        i2c::sendACK();
    }
}