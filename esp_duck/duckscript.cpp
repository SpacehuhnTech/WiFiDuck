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

    // ===== PUBLIC ===== //
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