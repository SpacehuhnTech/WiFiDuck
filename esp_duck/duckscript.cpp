/*
   Copyright (c) 2019 Stefan Kremser
   This software is licensed under the MIT License. See the license file for details.
   Source: github.com/spacehuhn/WiFiDuck
 */

#include "duckscript.h"

#include "config.h"
#include "debug.h"

#include "i2c.h"
#include "spiffs.h"

#define RESPONSE_OK 0x00
#define RESPONSE_PROCESSING 0x01
#define RESPONSE_REPEAT 0x02
#define RESPONSE_I2C_ERROR 0xFF

namespace duckscript {
    // ===== PRIVATE ===== //
    File f;

    char * prevMessage    { NULL };
    size_t prevMessageLen { 0 };

    bool running { false };

    void testw(const char* msg) {
        spiffs::write("/scripts/test.script", (const uint8_t*)msg, strlen(msg));
        debugf("Writing \"%s\" to file\n", msg);
    }

    // ===== PUBLIC ===== //
    void runTest() {
        spiffs::remove("/scripts/test.script");

#ifdef ENABLE_DEFAULT_DELAY
        testw("DEFAULTDELAY 200\n");
#endif // ifdef ENABLE_DEFAULT_DELAY

#ifdef LED_TEST
        testw("LED 0 100 0\n");
        testw("LED 255 0 0\n");
        testw("DELAY 1000\n");
        testw("LED 0 255 0\n");
        testw("DELAY 1000\n");
        testw("LED 0 0 255\n");
#endif // ifdef ENABLE_DEFAULT_DELAY

#ifdef OPEN_NOTEPAD
        testw("GUI r\n");
        testw("STRING notepad\n");
        testw("ENTER\n");
#endif // ifdef OPEN_NOTEPAD

#ifdef HELLO_WORLD
        testw("STRING Hello World! \n");
        testw("ENTER\n");
#endif // ifdef HELLO_WORLD

#ifdef DELAY_TEST
        testw("DELAY 1000\n");
        testw(".\n");
        testw("DELAY 3000\n");
        testw(".\n");
        testw("DELAY 5000\n");
        testw(".\n");
        testw("ENTER\n");
#endif // ifdef HELLO_WORLD

#ifdef REPEAT_TEST
        testw("STRING Hello World! \n");
        testw("REPEAT 2\n");
        testw("ENTER\n");
#endif // ifdef REPEAT_TEST

#ifdef US_CHAR_TEST
        testw("LOCALE US\n");
        testw("STRING !\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_abcdefghijklmnopqrstuvwxyz{|}~`\n");
        testw("ENTER\n");
#endif // ifdef ALL_CHAR_TEST

#ifdef DE_CHAR_TEST
        testw("LOCALE DE\n");
        testw("STRING !\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_abcdefghijklmnopqrstuvwxyz{|}~²³äöüÄÖÜß€°§` \n");
        testw("ENTER\n");
#endif // ifdef ALL_CHAR_TEST

#ifdef GB_CHAR_TEST
        testw("LOCALE GB\n");
        testw("STRING !\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_abcdefghijklmnopqrstuvwxyz{|}~£¦¬éíúóÉÍÚÓ€\n");
        testw("ENTER\n");
#endif // ifdef ALL_CHAR_TEST

#ifdef OVERFLOW_TEST
        testw("STRING 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 \n");
        testw("ENTER\n");
#endif // ifdef OVERFLOW_TEST

#ifdef KEYCODE_TEST
        testw("A\n");
        testw("KEYCODE 0x02 0x04\n");
        testw("KEYCODE 2 4\n");
#endif // ifdef ALL_CHAR_TEST

#ifdef CLOSE_NOTEPAD
        testw("DELAY 5000\n");
        testw("ALT F4\n");
        testw("RIGHT\n");
        testw("ENTER\n");
#endif // ifdef CLOSE_NOTEPAD

        run("/scripts/test.script");
    }

    void run(const String& fileName) {
        debugf("Run file %s\n", fileName.c_str());
        f       = spiffs::open(fileName);
        running = true;
        nextLine();
    }

    void nextLine() {
        if (!running) return;

        if (!f) {
            debugln("File error");
            stop();
            return;
        }

        if (!f.available()) {
            debugln("Reached end of file");
            stop();
            return;
        }

        debugf("Sending: ");
        uint8_t buf[BUFFER_SIZE];
        size_t  buf_i { 0 };
        bool    eol { false }; // End of line

        while (f.available() && !eol && buf_i < BUFFER_SIZE) {
            uint8_t b = f.read();
            eol          = (b == '\n');
            buf[buf_i++] = b;
            debug(char(b));
        }

        debugln();

        i2c::transmit(buf, buf_i);

        if (strncmp((char*)buf, "REPEAT", _min(buf_i, 6)) != 0) {
            if (prevMessage) free(prevMessage);
            prevMessageLen = buf_i;
            prevMessage    = (char*)malloc(prevMessageLen + 1);
            memcpy(prevMessage, buf, buf_i);
            prevMessage[buf_i] = '\0';
        }
    }

    void repeat() {
        if (i2c::getStatus() == i2c::status::REPEAT) {
            if (!prevMessage) {
                stop();
            } else {
                debugln("Repeating last message");
                i2c::transmit((const uint8_t*)prevMessage, prevMessageLen);
            }
        }
    }

    void stop() {
        if (running) {
            if (f) f.close();
            running = false;
            debugln("Stopped script");
        }
    }

    void stop(const String& fileName) {
        if (running && f && ((strlen(fileName.c_str()) == 0) || (strcmp(f.name(), fileName.c_str()) == 0))) {
            f.close();
            running = false;
            debugln("Stopped script");
        }
    }

    bool isRunning() {
        return running;
    }

    String currentScript() {
        if (!running) return String();
        return String(f.name());
    }
}