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

#define COM_VERSION 4

typedef struct status_t {
    unsigned int version : 8;
    unsigned int wait    : 16;
    unsigned int repeat  : 8;
} status_t;

namespace com {
    // =========== PRIVATE ========= //
    buffer_t receive_buf;
    buffer_t data_buf;

    bool start_parser         = false;
    bool ongoing_transmission = false;

    status_t status;

    void update_status() {
        status.wait = (uint16_t)receive_buf.len
                      + (uint16_t)data_buf.len
                      + (uint16_t)duckparser::getDelayTime();
        status.repeat = (uint8_t)(duckparser::getRepeats() > 255 ? 255 : duckparser::getRepeats());
    }

    // ========== PRIVATE I2C ========== //
#ifdef ENABLE_I2C

    // time sensetive!
    void i2c_request() {
        update_status();
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

        data_buf.len    = 0;
        receive_buf.len = 0;
    }

#else // ifdef ENABLE_I2C
    void i2c_begin() {}

#endif // ifdef ENABLE_I2C

    // ========== PRIVATE SERIAL ========== //
#ifdef ENABLE_SERIAL
    void serial_begin() {
        debugsln("ENABLED SERIAL");
        SERIAL_COM.begin(SERIAL_BAUD);
    }

    void serial_send_status() {
        update_status();
#ifdef ENABLE_DEBUG
        debugs("Replying with status {");
        debugs("wait: ");
        debug(status.wait);
        debugs(",repeat: ");
        debug(status.repeat);
        debugs("} [");

        for (int i = 0; i<sizeof(status_t); ++i) {
            char b = ((uint8_t*)&status)[i];
            if (b < 0x10) debug('0');
            debug(String(b, HEX));
            debug(' ');
        }
        debugsln("]");
#endif // ifdef ENABLE_DEBUG

        SERIAL_COM.write(REQ_SOT);
        SERIAL_COM.write((uint8_t*)&status, sizeof(status_t));
        SERIAL_COM.write(REQ_EOT);
        SERIAL_COM.flush();
    }

    void serial_update() {
        unsigned int len = SERIAL_COM.available();

        if ((len > 0) && (receive_buf.len+len <= BUFFER_SIZE)) {
            SERIAL_COM.readBytes(&receive_buf.data[receive_buf.len], len);
            receive_buf.len += len;
        }
    }

#else // ifdef ENABLE_SERIAL
    void serial_begin() {}

    void serial_send_status() {}

    void serial_update() {}

#endif // ifdef ENABLE_SERIAL

    // ========== PUBLIC ========== //
    void begin() {
        status.version = COM_VERSION;
        i2c_begin();
        serial_begin();
    }

    void update() {
        serial_update();

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
                    debug(c, BIN);
                    debug(" ");

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
                debugs("...");
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
        serial_send_status();
    }
}