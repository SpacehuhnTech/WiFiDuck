/*
   This software is licensed under the MIT License. See the license file for details.
   Source: https://github.com/spacehuhntech/WiFiDuck
 */

#include "config.h"
#include "debug.h"

#include "keyboard.h"
#include "led.h"
#include "com.h"
#include "duckparser.h"
#include "serial_bridge.h"

// ===== SETUP ====== //
void setup() {
    debug_init();

    led::begin();
    serial_bridge::begin();
    keyboard::begin();
    com::begin();

    debugs("Started! ");
    debugln(VERSION);
}

// ===== LOOOP ===== //
void loop() {
    com::update();
    if (com::hasData()) {
        const buffer_t& buffer = com::getBuffer();

        debugs("Interpreting: ");

        for (size_t i = 0; i<buffer.len; i++) debug(buffer.data[i]);

        duckparser::parse(buffer.data, buffer.len);

        com::sendDone();
    }
}