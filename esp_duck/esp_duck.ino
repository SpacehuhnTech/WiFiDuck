/*
   Copyright (c) 2019 Stefan Kremser
   This software is licensed under the MIT License. See the license file for details.
   Source: github.com/spacehuhn/WiFiDuck
 */

#include "config.h"
#include "debug.h"

#include "i2c.h"
#include "ducktest.h"
#include "webserver.h"

void setup() {
#ifdef ENABLE_DEBUG
    Serial.begin(DEBUG_BAUD);
#endif // ifdef DEBUG

    debugln("\nStarted");

    webserver::begin();
    i2c::begin();

    i2c::setOnOK([]() {
        debugln("OK");
    });

    i2c::setOnError([]() {
        debugln("ERROR");
    });

    i2c::setOnRepeat([]() {
        debugln("REPEAT");
    });

    i2c::setOnProcessing([]() {
        debugln("PROCESSING");
    });
}

void loop() {
    i2c::update();

    if (i2c::connected()) {
        ducktest::run();
    }
}