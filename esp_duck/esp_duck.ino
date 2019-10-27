/*
   Copyright (c) 2019 Stefan Kremser
   This software is licensed under the MIT License. See the license file for details.
   Source: github.com/spacehuhn/WiFiDuck
 */

#include "config.h"
#include "debug.h"

#include "com.h"
#include "duckscript.h"
#include "webserver.h"
#include "spiffs.h"
#include "settings.h"
#include "cli.h"

void setup() {
    debug_init();

    com::begin();
    com::onDone(duckscript::nextLine);
    com::onError(duckscript::stop);
    com::onRepeat(duckscript::repeat);

    spiffs::begin();

    settings::begin();

    cli::begin();

    webserver::begin();

    debugln("\nESP Duck Started!");
}

void loop() {
    com::update();
    webserver::update();

    debug_update();
}