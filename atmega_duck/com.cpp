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

typedef struct status_t {
    uint8_t  version;
    uint16_t wait;
    uint8_t  repeat;
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
        status.wait   = (uint16_t)receive_buf.len + (uint16_t)data_buf.len + (uint16_t)duckparser::getDelayTime();
        status.repeat = (uint8_t)duckparser::getRepeats();

        Wire.write((uint8_t*)&status, sizeof(status_t));
    }

    // time sensetive!
    void i2c_receive(int len) {
        if (receive_buf.len + (unsigned int)len <= BUFFER_SIZE) {
            Wire.readBytes(&receive_buf.data[receive_buf.len], len);
            receive_buf.len += len;
        }
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
        status.version = 3;
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
                debug("...");
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