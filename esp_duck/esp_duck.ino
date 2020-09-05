/*
   This software is licensed under the MIT License. See the license file for details.
   Source: https://github.com/spacehuhntech/WiFiDuck
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

    delay(200);

    com::begin();

    spiffs::begin();
    settings::begin();
    cli::begin();
    webserver::begin();

    com::onDone(duckscript::nextLine);
    com::onError(duckscript::stopAll);
    com::onRepeat(duckscript::repeat);

    if (spiffs::freeBytes() > 0) com::send(MSG_STARTED);

    delay(10);
    com::update();

    debug("\n[~~~ WiFi Duck v");
    debug(VERSION);
    debugln(" Started! ~~~]");
    debugln("    __");
    debugln("___( o)>");
    debugln("\\ <_. )");
    debugln(" `---'   hjw\n");

    duckscript::run(settings::getAutorun());
}

void loop() {
    com::update();
    webserver::update();

    debug_update();
}