/*
   Copyright (c) 2019 Stefan Kremser
   This software is licensed under the MIT License. See the license file for details.
   Source: github.com/spacehuhn/WiFiDuck
 */

#include "config.h"
#include "debug.h"

#include "keyboard.h"
#include "led.h"
#include "com.h"
#include "duckparser.h"

const char* test_str = "GUI\n";

// ===== SETUP ====== //
void setup() {
#ifdef ENABLE_DEBUG
    DEBUG_PORT.begin(DEBUG_BAUD);

    while (!DEBUG_PORT) {}
#endif // ifdef ENABLE_DEBUG

    keyboard::begin();
    led::begin();
    // com::begin();

    debugln("Started!");
}

// ===== LOOOP ===== //
void loop() {
    debugln(test_str);
    duckparser::parse(test_str, strlen(test_str));
    delay(1000);

    /*
       com::update();
       if (com::hasData()) {
          const buffer_t& buffer = com::getBuffer();

          debug("Interpreting: ");

          for (size_t i = 0; i<buffer.len; i++) debug(buffer.data[i]);

          duckparser::parse(buffer.data, buffer.len);

          com::sendDone();
       }*/
}