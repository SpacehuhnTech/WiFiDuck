/*
   This software is licensed under the MIT License. See the license file for details.
   Source: https://github.com/spacehuhntech/WiFiDuck
 */

#pragma once

#include "serial_bridge.h"

#include "config.h"
#include "led.h"

#include <Arduino.h> // pinMode, digitalWrite, ...

#ifdef BRIDGE_SAFE
#define BRIDGE_SAFE_NUM 123
#include <FlashStorage.h>
#endif

namespace serial_bridge {
#ifdef BRIDGE_SAFE
    FlashStorage(esp_was_flashed, int);
#endif

#ifdef BRIDGE_ENABLE
    bool enabled = false;

    unsigned long baud = 115200;

    int rts = -1;
    int dtr = -1;

    void begin() {
        BRIDGE_PORT.begin(baud);

        pinMode(BRIDGE_SWITCH, INPUT_PULLUP);

        pinMode(BRIDGE_0, OUTPUT);
        pinMode(BRIDGE_RST, OUTPUT);

#ifdef BRIDGE_0_INVERTED
        digitalWrite(BRIDGE_0, LOW);
#else // ifdef BRIDGE_0_INVERTED
        digitalWrite(BRIDGE_0, HIGH);
#endif // ifdef BRIDGE_0_INVERTED
        digitalWrite(BRIDGE_RST, HIGH);

        if ((digitalRead(BRIDGE_SWITCH) == LOW) 
#ifdef BRIDGE_SAFE
        || (esp_was_flashed.read() != BRIDGE_SAFE_NUM)
#endif
        ) {
            enabled = true;
            led::setColor(COLOR_ESP_UNFLASHED);

            // Wait until user releases button
            while (digitalRead(BRIDGE_SWITCH) == LOW) {}

            // Go into serial bridge mode until user presses button again
            while (digitalRead(BRIDGE_SWITCH) == HIGH) {
                update();
            }

            stop();
        }
    }

    void stop() {
#ifdef BRIDGE_SAFE
    if(esp_was_flashed.read() != BRIDGE_SAFE_NUM) {
        esp_was_flashed.write(BRIDGE_SAFE_NUM);
    }
#endif
        enabled = false;
        led::setColor(0,0,0);  
    }

    void update() {
        if (enabled) {
            if (rts != Serial.rts()) {
                digitalWrite(BRIDGE_RST, !Serial.rts());
                rts = Serial.rts();
#ifdef BRIDGE_SAFE
                if(esp_was_flashed.read() != BRIDGE_SAFE_NUM) {
                    esp_was_flashed.write(BRIDGE_SAFE_NUM);
                }
#endif
            }

            if (dtr != Serial.dtr()) {
    #ifdef BRIDGE_0_INVERTED
                digitalWrite(BRIDGE_0, Serial.dtr());
    #else // ifdef BRIDGE_0_INVERTED
                digitalWrite(BRIDGE_0, !Serial.dtr());
    #endif // ifdef BRIDGE_0_INVERTED

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

#else /* ifdef SERIAL_BRIDGE_ENABLE */
    void begin() {}

    void stop() {}

    void update() {}

#endif /* ifdef SERIAL_BRIDGE_ENABLE */
}