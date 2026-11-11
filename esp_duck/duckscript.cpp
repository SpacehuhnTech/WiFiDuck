/*
   This software is licensed under the MIT License. See the license file for details.
   Source: https://github.com/spacehuhntech/WiFiDuck
 */

#include "duckscript.h"
#include "config.h"
#include "debug.h"
#include "com.h"
#include "spiffs.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

namespace duckscript {
    File f;
    char* prevMessage = NULL;
    size_t prevMessageLen = 0;
    bool running = false;
    bool lineStart = true;
    const char* scriptError = NULL;

    struct loop_t {
        uint32_t position;
        uint32_t remaining; // Zero repeats until stopped; otherwise total iterations.
    };
    const size_t MAX_LOOPS = 8;
    loop_t loops[MAX_LOOPS];
    size_t loopDepth = 0;
    char stopLED[32];
    size_t stopLEDLen = 0;

    bool whitespace(char c) {
        return c == ' ' || c == '\t' || c == '\r' || c == '\n';
    }

    // Recognize whole tokens only, not STRING/REM contents or chunks in the
    // middle of a long line. Transport buffers aren't null-terminated.
    bool argument(const char* data, size_t len, const char* command,
                  const char*& value, size_t& valueLen) {
        while (len && whitespace(*data)) { ++data; --len; }
        size_t commandLen = strlen(command);
        if (len < commandLen || memcmp(data, command, commandLen) != 0 ||
            (len > commandLen && !whitespace(data[commandLen]))) return false;
        data += commandLen;
        len -= commandLen;
        while (len && whitespace(*data)) { ++data; --len; }
        while (len && whitespace(data[len - 1])) --len;
        value = data;
        valueLen = len;
        return true;
    }

    bool number(const char* data, size_t len, uint32_t limit, uint32_t& value) {
        if (!len) return false;
        value = 0;
        for (size_t i = 0; i < len; ++i) {
            if (data[i] < '0' || data[i] > '9') return false;
            uint32_t digit = data[i] - '0';
            if (value > limit / 10 ||
                (value == limit / 10 && digit > limit % 10)) return false;
            value = value * 10 + digit;
        }
        return true;
    }

    void fail(const char* message) {
        scriptError = message;
        debugln(message);
        stopAll();
    }

    bool setStopLED(const char* data, size_t len) {
        const char* value;
        size_t valueLen;
        if (!argument(data, len, "LED", value, valueLen)) return false;
        uint32_t rgb[3];
        for (size_t i = 0; i < 3; ++i) {
            size_t wordLen = 0;
            while (wordLen < valueLen && !whitespace(value[wordLen])) ++wordLen;
            if (!number(value, wordLen, 255, rgb[i])) return false;
            value += wordLen;
            valueLen -= wordLen;
            while (valueLen && whitespace(*value)) { ++value; --valueLen; }
        }
        if (valueLen) return false;
        stopLEDLen = snprintf(stopLED, sizeof(stopLED), "LED %u %u %u\n",
                              (unsigned int)rgb[0], (unsigned int)rgb[1], (unsigned int)rgb[2]);
        return true;
    }

    void run(String fileName) {
        if (!fileName.length()) return;
        stopAll();
        scriptError = NULL;
        f = spiffs::open(fileName);
        running = true;
        nextLine();
    }

    void nextLine() {
        if (!running) return;
        if (!f) { fail("Cannot open script"); return; }
        if (!f.available()) {
            if (loopDepth) fail("LOOP is missing ENDLOOP");
            else stopAll();
            return;
        }

        uint32_t position = f.position();
        bool firstSegment = lineStart;
        char buf[BUFFER_SIZE];
        size_t len = 0;
        bool eol = false;
        while (f.available() && !eol && len < sizeof(buf)) {
            uint8_t b = f.peek();
            size_t chars = (b & 0xE0) == 0xC0 ? 2 :
                           (b & 0xF0) == 0xE0 ? 3 :
                           (b & 0xF8) == 0xF0 ? 4 : 1;
            if (len + chars > sizeof(buf)) break;
            buf[len++] = f.read();
            eol = b == '\n';
        }
        lineStart = eol;

        const char* value;
        size_t valueLen;
        bool control = false;
        if (firstSegment && argument(buf, len, "LOOP", value, valueLen)) {
            uint32_t count;
            if ((!eol && f.available()) || !number(value, valueLen, UINT32_MAX, count)) {
                fail("LOOP requires a count from 0 to 4294967295"); return;
            }
            if (loopDepth == MAX_LOOPS) { fail("Too many nested loops (maximum 8)"); return; }
            loops[loopDepth++] = { f.position(), count };
            control = true;
        } else if (firstSegment && argument(buf, len, "ENDLOOP", value, valueLen)) {
            if (valueLen || !loopDepth || (!eol && f.available())) {
                fail("ENDLOOP requires a matching LOOP and no arguments"); return;
            }
            loop_t& loop = loops[loopDepth - 1];
            if (!loop.remaining && position == loop.position) {
                fail("An infinite LOOP must have a body"); return;
            }
            if (loop.remaining == 1) --loopDepth;
            else {
                if (loop.remaining) --loop.remaining;
                if (!f.seek(loop.position)) { fail("Cannot rewind loop"); return; }
                lineStart = true;
            }
            control = true;
        } else if (firstSegment && argument(buf, len, "ONSTOP", value, valueLen)) {
            if ((!eol && f.available()) || !setStopLED(value, valueLen)) {
                fail("ONSTOP requires LED r g b (values 0 to 255)"); return;
            }
            control = true;
        }

        // The normal completion callback advances one line at a time. Sending
        // a harmless comment avoids recursion and keeps Stop responsive.
        if (control) { com::send("REM\n", 4); return; }

        bool replay = firstSegment &&
            (argument(buf, len, "REPEAT", value, valueLen) ||
             argument(buf, len, "REPLAY", value, valueLen));
        if (!replay) {
            free(prevMessage);
            prevMessage = (char*)malloc(len + 1);
            if (!prevMessage) { fail("Not enough memory to read script"); return; }
            prevMessageLen = len;
            memcpy(prevMessage, buf, len);
            prevMessage[len] = '\0';
        }
        com::send(buf, len);
    }

    void repeat() {
        if (!running) return;
        if (!prevMessage) fail("REPEAT requires a preceding command");
        else com::send(prevMessage, prevMessageLen);
    }

    void stopAll() {
        if (running) {
            running = false;
            if (f) f.close();
            com::reset();
            // Opt-in cleanup also runs at EOF or on a script error; other
            // scripts retain their existing LED behavior.
            if (stopLEDLen) com::send(stopLED, stopLEDLen);
        }
        free(prevMessage);
        prevMessage = NULL;
        prevMessageLen = 0;
        loopDepth = 0;
        lineStart = true;
        stopLEDLen = 0;
    }

    void stop(String fileName) {
        if (!fileName.length() || (running && f && fileName == currentScript())) stopAll();
    }

    bool isRunning() { return running; }
    String currentScript() { return running ? String(f.name()) : String(); }
    const char* getError() { return scriptError; }
}
