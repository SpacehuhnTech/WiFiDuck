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

    i2c::setOnOK(ducktest::nextLine);
    // i2c::setOnProcessing();
    i2c::setOnError(ducktest::stop);
    i2c::setOnRepeat(ducktest::repeat);

    ducktest::runTest();
}

void loop() {
    i2c::update();
}