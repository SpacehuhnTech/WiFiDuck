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
#include "spiffs.h"

void setup() {
#ifdef ENABLE_DEBUG
    Serial.begin(DEBUG_BAUD);
#endif // ifdef DEBUG

    webserver::begin();
    i2c::begin();

    spiffs::begin();
    // spiffs::format();

    debugln("\nStarted");

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

    ducktest::runTest();
}

void loop() {
    i2c::update();

    switch (i2c::getStatus()) {
        case i2c::status::OK:
            ducktest::update();
            break;
        case i2c::status::ERROR:
            ducktest::stop();
            break;
        case i2c::status::REPEAT:
            ducktest::repeat();
            break;
    }
}