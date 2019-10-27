/*
   Copyright (c) 2019 Stefan Kremser
   This software is licensed under the MIT License. See the license file for details.
   Source: github.com/spacehuhn/WiFiDuck
 */

#pragma once

#include "config.h"

#include <Arduino.h> // pinMode, digitalWrite, ...

namespace serial_bridge {
#ifdef BRIDGE_ENABLE
    bool enabled = false;

    unsigned long baud = 115200;

    int rts = -1;
    int dtr = -1;

    void update() {
        if (enabled) {
            if (rts != Serial.rts()) {
                digitalWrite(BRIDGE_RST, !Serial.rts());
                rts = Serial.rts();
            }

            if (dtr != Serial.dtr()) {
                digitalWrite(BRIDGE_0, !Serial.dtr());
                dtr = Serial.dtr();
            }

            if (Serial.available()) {
                BRIDGE_PORT.write(Serial.read());
            }

            if (BRIDGE_PORT.available()) {
                Serial.write(BRIDGE_PORT.read());
            }

            if (Serial.baud() != baud) {
                rts = -1;
                dtr = -1;

                baud = Serial.baud();
                BRIDGE_PORT.begin(baud);
            }
        }
    }

    void begin() {
        BRIDGE_PORT.begin(baud);

        pinMode(BRIDGE_SWITCH, INPUT_PULLUP);

        pinMode(BRIDGE_0, OUTPUT);
        pinMode(BRIDGE_RST, OUTPUT);

        digitalWrite(BRIDGE_0, HIGH);
        digitalWrite(BRIDGE_RST, HIGH);

        if (digitalRead(BRIDGE_SWITCH) == LOW) {
            enabled = true;

            while (true) update();
        }
    }

    void stop() {
        enabled = false;
    }

#else /* ifdef SERIAL_BRIDGE_ENABLE */
    void begin() {}

    void stop() {}

    void update() {}

#endif /* ifdef SERIAL_BRIDGE_ENABLE */
}