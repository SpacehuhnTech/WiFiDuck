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

#define COM_VERSION 4

typedef struct status_t {
    unsigned int version : 8;
    unsigned int wait    : 16;
    unsigned int repeat  : 8;
} status_t;

namespace com {
    // ========== PRIVATE ========== //
    bool connection = false;

    com_callback callback_done   = NULL;
    com_callback callback_repeat = NULL;
    com_callback callback_error  = NULL;

    bool react_on_status  = false;
    bool new_transmission = false;

    status_t status;

    uint8_t transm_tries = 0;

    // ========= PRIVATE I2C ========= //

#ifdef ENABLE_I2C
    unsigned long request_time = 0;

    void i2c_start_transmission() {
        Wire.beginTransmission(I2C_ADDR);
        debug("Transmitting '");
    }

    void i2c_stop_transmission() {
        Wire.endTransmission();
        debugln("' ");
        delay(1);
    }

    void i2c_transmit(char b) {
        Wire.write(b);
    }

    void i2c_request() {
        debug("I2C Request");

        uint16_t prev_wait = status.wait;

        Wire.requestFrom(I2C_ADDR, sizeof(status_t));

        if (Wire.available() == sizeof(status_t)) {
            status.version = Wire.read();

            status.wait  = Wire.read();
            status.wait |= uint16_t(Wire.read()) << 8;

            status.repeat = Wire.read();

            debugf(" %u", status.wait);
        } else {
            connection = false;
            debug(" ERROR");
        }

        react_on_status = status.wait == 0 ||
                          status.repeat > 0 ||
                          ((prev_wait&1) ^ (status.wait&1));

        debugln();

        if (!react_on_status && (status.wait == prev_wait)) {
            debug("Last message was not processed");

            if (transm_tries > 3) {
                connection = false;
                debugln("...LOOP ERROR");
            } else {
                debugln("...repeating last line");

                status.repeat = 1;

                react_on_status = true;

                ++transm_tries;
            }
        } else {
            transm_tries = 0;
        }

        request_time = millis();
    }

    void i2c_begin() {
        unsigned long start_time = millis();

        Wire.begin(I2C_SDA, I2C_SCL);
        Wire.setClock(I2C_CLOCK_SPEED);

        while (Wire.available()) Wire.read();

        debugln("Connecting via i2c");

        connection = true;

        send(MSG_CONNECTED);

        update();

        debug("I2C Connection ");
        debugln(connection ? "OK" : "ERROR");
    }

    void i2c_update() {
        if (!connection) return;

        bool processing = status.wait > 0;
        bool delay_over = request_time + status.wait < millis();

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

    // ========= PRIVATE I2C ========= //

#ifdef ENABLE_SERIAL
    bool ongoing_transmission = false;

    void serial_start_transmission() {
        debug("Transmitting '");
    }

    void serial_stop_transmission() {
        SERIAL_PORT.flush();
        debugln("' ");
    }

    void serial_transmit(char b) {
        SERIAL_PORT.write(b);
    }

    void serial_begin() {
        SERIAL_PORT.begin(SERIAL_BAUD);

        while (SERIAL_PORT.available()) SERIAL_PORT.read();

        debug("Connecting via serial");

        connection = true;

        send(MSG_CONNECTED);

        update();

        debug("Serial Connection ");
        debugln(connection ? "OK" : "ERROR");
    }

    void serial_update() {
        if (SERIAL_PORT.available() >= sizeof(status_t)+2) {
            if (SERIAL_PORT.read() == REQ_SOT) {
                uint16_t prev_wait = status.wait;

                status.version = SERIAL_PORT.read();

                status.wait  = SERIAL_PORT.read();
                status.wait |= uint16_t(SERIAL_PORT.read()) << 8;

                status.repeat = SERIAL_PORT.read();

                react_on_status = status.wait == 0 ||
                                  status.repeat > 0 ||
                                  ((prev_wait&1) ^ (status.wait&1));

                while (SERIAL_PORT.available() && SERIAL_PORT.read() != REQ_EOT) {}
            }
        }
    }

#else // ifdef ENABLE_SERIAL
    void serial_start_transmission() {}

    void serial_stop_transmission() {}

    void serial_transmit(char b) {}

    void serial_begin() {}

    void serial_update() {}

#endif // ifdef ENABLE_SERIAL

    void start_transmission() {
        i2c_start_transmission();
        serial_start_transmission();
    }

    void stop_transmission() {
        i2c_stop_transmission();
        serial_stop_transmission();
    }

    void transmit(char b) {
        i2c_transmit(b);
        serial_transmit(b);
    }

    // ===== PUBLIC ===== //
    void begin() {
        status.version = 0;
        status.wait    = 0;
        status.repeat  = 0;

        i2c_begin();
        serial_begin();
    }

    void update() {
        i2c_update();
        serial_update();

        if (react_on_status) {
            react_on_status = false;

            debug("Com. status ");

            if (status.version != COM_VERSION) {
                debugf("ERROR %u\n", status.version);
                connection = false;
                if (callback_error) callback_error();
            } else if (status.wait > 0) {
                debugf("PROCESSING %u\n", status.wait);
            } else if (status.repeat > 0) {
                debugf("REPEAT %u\n", status.repeat);
                if (callback_repeat) callback_repeat();
            } else if ((status.wait == 0) && (status.repeat == 0)) {
                debugln("DONE");
                if (callback_done) callback_done();
            } else {
                debugln("idk");
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

        size_t sent = 0; // byte sent overall
        size_t i    = 0; // index of string
        size_t j    = 0; // byte sent for current packet

        start_transmission();

        transmit(REQ_SOT);

        ++sent;
        ++j;

        while (i < len) {
            char b = str[i];
            
            if ((b != '\n') && (b != '\n')) debug(b);
            transmit(b);

            ++i;
            ++j;
            ++sent;

            if (j == PACKET_SIZE/*sent % PACKET_SIZE == 0*/) {
                stop_transmission();
                start_transmission();
                j = 0;
            }
        }

        transmit(REQ_EOT);

        ++sent;

        stop_transmission();

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

    int getVersion() {
        return status.version;
    }
}