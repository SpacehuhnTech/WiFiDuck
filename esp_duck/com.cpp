/*!
    \file esp_duck/com.cpp
    \brief Communication Module source
    \author Stefan Kremser
    \copyright MIT License
 */

#include "com.h"

#include <Wire.h> // Arduino i2c

#include "config.h"
#include "debug.h"

// ! Communication request codes
#define REQ_SOT 0x01     // !< Start of transmission
#define REQ_EOT 0x04     // !< End of transmission
#define REQ_VERSION 0x02 // !< Request current version

// ! Communication response codes
#define RES_OK 0x00
#define RES_PROCESSING 0x01
#define RES_REPEAT 0x02
#define RES_ERROR 0xFE

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
    // ========== PRIVATE ========== //
    bool connection = false;

    com_callback callback_done   = NULL;
    com_callback callback_repeat = NULL;
    com_callback callback_error  = NULL;

    uint8_t response = RES_PROCESSING;

    bool react_on_response = false;
    bool new_transmission  = false;

    unsigned long request_time = 0;

    status_t status;

    // ========= PRIVATE I2C ========= //

#ifdef ENABLE_I2C
    void i2c_start_transmission() {
        Wire.beginTransmission(I2C_ADDR);
        debug("Transmitting '");
    }

    void i2c_stop_transmission() {
        Wire.endTransmission();
        debugln("' ");
    }

    void i2c_transmit(char b) {
        Wire.write(b);
    }

    void i2c_request() {
        debug("I2C-Req...");

        uint8_t prev_response = response;

        Wire.requestFrom(I2C_ADDR, sizeof(status_t));

        if (Wire.available()) {
            status.legacy_response = Wire.read();

            if (Wire.available() == sizeof(status_t)-1) {
                status.version_major    = Wire.read();
                status.version_minor    = Wire.read();
                status.version_revision = Wire.read();

                status.repeat = Wire.read();

                status.buffer  = Wire.read();
                status.buffer |= uint16_t(Wire.read()) << 8;

                status.delay  = Wire.read();
                status.delay |= uint16_t(Wire.read()) << 8;

                // debugf("Version=(%1u,%1u,%1u) repeat=%4u buffer=%4u delay=%4u response=", status.version_major, status.version_minor, status.version_revision, status.repeat, status.buffer, status.delay);
            }

            response = status.legacy_response;

            // debugf("%3u", response);
        } else {
            connection = false;
            response   = RES_ERROR;
            debug("ERROR");
        }

        react_on_response = response == RES_OK ||
                            response == RES_REPEAT ||
                            (prev_response != response &&
                             ((prev_response & RES_PROCESSING) != (response & RES_PROCESSING)));

        debugln();

        request_time = millis();
    }

    void i2c_begin() {
        unsigned long start_time = millis();

        connection = false;

        Wire.begin(I2C_SDA, I2C_SCL);
        Wire.setClock(I2C_CLOCK_SPEED);

        while (Wire.available()) Wire.read();

        debugln("Connecting via i2c");

        send(MSG_CONNECTED);

        while (start_time + 5000 > millis()) {
            if (response == RES_OK) {
                connection = true;
                break;
            }
            delay(response);
            i2c_request();
        }

        debug("I2C Connection ");
        debugln(connection ? "OK" : "ERROR");
    }

    void i2c_update() {
        if (!connection) return;

        bool processing = response & RES_PROCESSING == RES_PROCESSING;
        bool delay_over = request_time + response < millis();

        if (new_transmission || (processing && delay_over)) {
            new_transmission = false;
            i2c_request();
        }
    }

#else // ifdef ENABLE_I2C
    void i2c_start_transmission() {}

    void i2c_stop_transmission() {}

    void i2c_transmit(char b) {}

    void i2c_request() {}

    void i2c_begin() {}

    void i2c_update() {}

#endif // ifdef ENABLE_I2C

    // ===== PUBLIC ===== //
    void begin() {
        status.legacy_response  = 0;
        status.version_major    = 0;
        status.version_minor    = 0;
        status.version_revision = 0;
        status.repeat           = 0;
        status.buffer           = 0;
        status.delay            = 0;

        i2c_begin();
    }

    void update() {
        i2c_update();

        if (react_on_response) {
            react_on_response = false;

            debugf("NEW STATUS [%u] = ", response);

            if (response & RES_PROCESSING) {
                debugln("PROCESSING");
            } else if (response == RES_OK) {
                debugln("DONE");
                if (callback_done) callback_done();
            } else if (response == RES_REPEAT) {
                debugln("REPEAT");
                if (callback_repeat) callback_repeat();
            } else if (response == RES_ERROR) {
                debugln("ERROR");
                if (callback_error) callback_error();
            } else {
                debugln("UNKOWN");
            }
        }
    }

    unsigned int send(char str) {
        return send(&str, 1);
    }

    unsigned int send(const char* str) {
        return send(str, strlen(str));
    }

    unsigned int send(const char* str, size_t len) {
        // ! Truncate string to fit into buffer
        if (len > BUFFER_SIZE) len = BUFFER_SIZE;

        size_t sent = 0;
        size_t i    = 0;

        i2c_start_transmission();
        i2c_transmit(REQ_SOT);
        ++sent;

        while (i < len) {
            char b = str[i];

            if ((b != '\n') && (b != '\n')) debug(b);
            i2c_transmit(b);

            ++i;
            ++sent;

            if (sent % PACKET_SIZE == 0) {
                i2c_stop_transmission();
                i2c_start_transmission();
            }
        }

        i2c_transmit(REQ_EOT);
        ++sent;
        i2c_stop_transmission();

        new_transmission = true;

        // ! Return number of characters sent, minus 2 due to the signals
        return sent-2;
    }

    void onDone(com_callback c) {
        callback_done = c;
    }

    void onRepeat(com_callback c) {
        callback_repeat = c;
    }

    void onError(com_callback c) {
        callback_error = c;
    }

    bool connected() {
        return connection;
    }
}