/*
   Copyright (c) 2019 Stefan Kremser
   This software is licensed under the MIT License. See the license file for details.
   Source: github.com/spacehuhn/WiFiDuck
 */

#include "config.h"
#include "debug.h"

#include "educk.h"
#include "webserver.h"

size_t tries { 0 };

void setup() {
#ifdef DEBUG
    Serial.begin(DEBUG_BAUD);
#endif // ifdef DEBUG

    debugln("\nStarted");

    webserver::begin();
    educk::begin();

    if (!educk::connected()) {
        for (size_t i = 0; i<NUMBER_CONNECTION_TRIES && !educk::connected(); ++i) {
            debugln("Connection error");
            delay(CONNECTION_DELAY);
            educk::begin();
        }
    }

    if (educk::connected()) {
        debugln("Connected!");

        educk::sendMessage("LED 0 100 0\n");

  #ifdef ENABLE_DEFAULT_DELAY
        educk::sendMessage("DEFAULTDELAY 200\n");
  #endif // ifdef ENABLE_DEFAULT_DELAY

  #ifdef LED_TEST
        educk::sendMessage("LED 255 0 0\n");
        educk::sendMessage("DELAY 1000\n");
        educk::sendMessage("LED 0 255 0\n");
        educk::sendMessage("DELAY 1000\n");
        educk::sendMessage("LED 0 0 255\n");
  #endif // ifdef ENABLE_DEFAULT_DELAY

  #ifdef OPEN_NOTEPAD
        educk::sendMessage("GUI r\n");
        educk::sendMessage("STRING notepad\n");
        educk::sendMessage("ENTER\n");
  #endif // ifdef OPEN_NOTEPAD

  #ifdef HELLO_WORLD
        educk::sendMessage("STRING Hello World! \n");
        educk::sendMessage("ENTER\n");
  #endif // ifdef HELLO_WORLD

  #ifdef DELAY_TEST
        educk::sendMessage("DELAY 1000\n");
        educk::sendMessage(".\n");
        educk::sendMessage("DELAY 3000\n");
        educk::sendMessage(".\n");
        educk::sendMessage("DELAY 5000\n");
        educk::sendMessage(".\n");
        educk::sendMessage("ENTER\n");
  #endif // ifdef HELLO_WORLD

  #ifdef REPEAT_TEST
        educk::sendMessage("STRING Hello World! \n");
        educk::sendMessage("REPEAT 2\n");
        educk::sendMessage("ENTER\n");
  #endif // ifdef REPEAT_TEST

  #ifdef US_CHAR_TEST
        educk::sendMessage("LOCALE US\n");
        educk::sendMessage("STRING !\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_abcdefghijklmnopqrstuvwxyz{|}~`\n");
        educk::sendMessage("ENTER\n");
  #endif // ifdef ALL_CHAR_TEST

  #ifdef DE_CHAR_TEST
        educk::sendMessage("LOCALE DE\n");
        educk::sendMessage("STRING !\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_abcdefghijklmnopqrstuvwxyz{|}~²³äöüÄÖÜß€°§` \n");
        educk::sendMessage("ENTER\n");
  #endif // ifdef ALL_CHAR_TEST

  #ifdef GB_CHAR_TEST
        educk::sendMessage("LOCALE GB\n");
        educk::sendMessage("STRING !\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_abcdefghijklmnopqrstuvwxyz{|}~£¦¬éíúóÉÍÚÓ€\n");
        educk::sendMessage("ENTER\n");
  #endif // ifdef ALL_CHAR_TEST

  #ifdef OVERFLOW_TEST
        educk::sendMessage("STRING 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 \n");
        educk::sendMessage("ENTER\n");
  #endif // ifdef OVERFLOW_TEST

  #ifdef KEYCODE_TEST
        educk::sendMessage("A\n");
        educk::sendMessage("KEYCODE 0x02 0x04\n");
        educk::sendMessage("KEYCODE 2 4\n");
  #endif // ifdef ALL_CHAR_TEST

  #ifdef CLOSE_NOTEPAD
        educk::sendMessage("DELAY 5000\n");
        educk::sendMessage("ALT F4\n");
        educk::sendMessage("RIGHT\n");
        educk::sendMessage("ENTER\n");
  #endif // ifdef CLOSE_NOTEPAD
    }
}

void loop() {}