/*!
   \file atmega_duck/com.cpp
   \brief Communication Module source
   \author Stefan Kremser
   \copyright MIT License
 */

#include "com.h"

#include <Wire.h> // Arduino i2c

#include "debug.h"
#include "duckparser.h"

// ! Communication request codes
#define REQ_SOT 0x01     // !< Start of transmission
#define REQ_EOT 0x04     // !< End of transmission
#define REQ_VERSION 0x02 // !< Request current version

// ! Communication response codes
#define RES_OK 0x00
#define RES_PROCESSING 0x01
#define RES_REPEAT 0x02

typedef struct status_t {
    uint8_t  legacy_response;
    uint8_t  version_major;
    uint8_t  version_minor;
    uint8_t  version_revision;
    uint8_t  repeat;
    uint16_t buffer;
    uint16_t delay;
} status_t;

namespace com {
    // =========== PRIVATE ========= //
    buffer_t receive_buf;
    buffer_t data_buf;

    bool start_parser         = false;
    bool ongoing_transmission = false;

    status_t status;

    // ========== PRIVATE I2C ========== //
    #ifdef ENABLE_I2C

    // time sensetive!
    void i2c_request() {
        // debugs("REQUEST...");


        uint8_t response;

        debug("(REQ)");
        // debug(duckparser::getDelayTime());

        if ((receive_buf.len > 0) || (data_buf.len > 0)) {
            int delayTime = duckparser::getDelayTime() + receive_buf.len + data_buf.len;
            delayTime = min(delayTime, 255);
            response  = (uint8_t)delayTime | (uint8_t)RES_PROCESSING;
            // debugs("Responding PROCESSING");
        } else if (duckparser::getRepeats()) {
            response = (uint8_t)RES_REPEAT;
            // debugs("Responding REPEAT");
        } else {
            response = (uint8_t)RES_OK;
            // debugs("Responding OK");
        }

        status.legacy_response = response;

        status.repeat = (uint8_t)duckparser::getRepeats();
        status.buffer = (uint16_t)receive_buf.len + (uint16_t)data_buf.len;
        status.delay  = (uint16_t)duckparser::getDelayTime();

        debug(status.delay);

        Wire.write((uint8_t*)&status, sizeof(status_t));

        // debugln(int(response));

        // debugs(" [");
        // debug(response);
        // debugsln("]");

        // Wire.write(response);
    }

    // time sensetive!
    void i2c_receive(int len) {
        if (receive_buf.len + (unsigned int)len <= BUFFER_SIZE) {
            Wire.readBytes(&receive_buf.data[receive_buf.len], len);
            receive_buf.len += len;
        }
        debug("(REC)\n");
    }

    void i2c_begin() {
        debugsln("ENABLED I2C");
        Wire.begin(I2C_ADDR);
        Wire.onRequest(i2c_request);
        Wire.onReceive(i2c_receive);
    }

    #else // ifdef ENABLE_I2C
    void i2c_begin() {}

    #endif // ifdef ENABLE_I2C

    // ========== PUBLIC ========== //
    void begin() {
        status.version_major    = 1;
        status.version_minor    = 0;
        status.version_revision = 1;

        i2c_begin();
    }

    void update() {
        if (!start_parser && (receive_buf.len > 0) && (data_buf.len < BUFFER_SIZE)) {
            unsigned int i = 0;

            debugs("RECEIVED ");

            // ! Skip bytes until start of transmission
            while (i < receive_buf.len && !ongoing_transmission) {
                if (receive_buf.data[i] == REQ_SOT) {
                    ongoing_transmission = true;
                    debugs("[SOT] ");
                }
                ++i;
            }

            debugs("'");

            while (i < receive_buf.len && ongoing_transmission) {
                char c = receive_buf.data[i];

                if (c == REQ_EOT) {
                    start_parser         = true;
                    ongoing_transmission = false;
                } else {
                    if ((c != '\n') && (c != '\r')) debug(c);
                    data_buf.data[data_buf.len] = c;
                    ++data_buf.len;
                }

                if (data_buf.len == BUFFER_SIZE) {
                    start_parser         = true;
                    ongoing_transmission = false;
                }

                ++i;
            }

            debugs("' ");

            if (start_parser && !ongoing_transmission) {
                debugs("[EOT]");
            } else if (!start_parser && ongoing_transmission) {
                debug(" ... ");
            } else if (!start_parser && !ongoing_transmission) {
                debugs("DROPPED");
            }

            debugln();

            receive_buf.len = 0;
        }
    }

    bool hasData() {
        return data_buf.len > 0 && start_parser;
    }

    const buffer_t& getBuffer() {
        return data_buf;
    }

    void sendDone() {
        data_buf.len = 0;
        start_parser = false;
    }
}