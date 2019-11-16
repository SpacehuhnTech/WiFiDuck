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
#define REQ_VERSION 0x02 // !< Request current version
#define REQ_EOT 0x04     // !< End of transmission

// ! Communication response codes
#define RES_OK 0x00
#define RES_PROCESSING 0x01
#define RES_REPEAT 0x02
#define RES_ERROR 0xFE

namespace com {
    // ===== PRIVATE ===== //
    // ! Flags for connection types
    bool serial_connection = false;
    bool i2c_connection    = false;

    // ! Callback function pointer
    com_callback callback_done   = NULL;
    com_callback callback_repeat = NULL;
    com_callback callback_error  = NULL;

    // ! Last received response code
    uint8_t response = 0x01;

    // ! Flag to indicate a changed response code
    bool response_change = false;

    // ! Timestamp of last status request
    unsigned long request_time = 0;


    // ! Internal function to start transmitting a packet
    void beginTransmission() {
#ifdef ENABLE_I2C
        Wire.beginTransmission(I2C_ADDR);
#endif // ifdef ENABLE_I2C
    }

    // ! Internal function to send a single byte
    void transmitData(char b) {
#ifdef ENABLE_I2C
        if (i2c_connection) Wire.write(b);
#endif // ifdef ENABLE_I2C

#ifdef ENABLE_SERIAL
        if (serial_connection) SERIAL_PORT.print(b);
#endif // ifdef ENABLE_SERIAL
    }

    // ! Internal function to end transmission of a packet
    void endTransmission() {
#ifdef ENABLE_I2C
        Wire.endTransmission();
#endif // ifdef ENABLE_I2C

#ifdef ENABLE_SERIAL
        SERIAL_PORT.flush();
#endif // ifdef ENABLE_I2C
    }

#ifdef ENABLE_I2C
    // ! Internal function to end transmission of a packet
    void i2c_request() {
        debug("i2c request...");

        request_time = millis();

        Wire.requestFrom(I2C_ADDR, 1);

        uint8_t prev_response = response;

        if (Wire.available()) {
            response = Wire.read();
            debugln(response);
        } else {
            i2c_connection = false;
            response       = RES_ERROR;
            debugln("ERROR");
        }

        response_change = prev_response != response;
    }

    // ! Internal function to check for response on i2c connection
    void update_i2c() {
        if (i2c_connection) {
            if ((response != RES_ERROR) &&
                (response & RES_PROCESSING == RES_PROCESSING) &&
                (millis() - request_time > response)) {
                i2c_request();
            }
        }
    }

#endif // ifdef ENABLE_I2C

#ifdef ENABLE_SERIAL
    // ! Internal function to check for response on serial connection
    void update_serial() {
        if (SERIAL_PORT.available()) {
            uint8_t prev_response = response;
            response        = Serial.read();
            response_change = (prev_response != response);
        }
    }

#endif // ifdef ENABLE_I2C

    // ===== PUBLIC ===== //
    void begin() {
        unsigned long start_time = 0;

#ifdef ENABLE_SERIAL
        SERIAL_PORT.begin(SERIAL_BAUD);

        serial_connection = true;
        send(MSG_CONNECTED);

        debug("Connecting via serial...");

        start_time = millis();

        while (response != RES_OK && millis() - start_time < 1000) {
            update();
        }
        serial_connection = (response == RES_OK);

        // debugln(serial_connection ? "OK" : "ERROR");
#endif // ifdef ENABLE_SERIAL

#ifdef ENABLE_I2C
        if (serial_connection) return;

        // ! Initialize Arduino i2c implementation
        Wire.begin(I2C_SDA, I2C_SCL);
        Wire.setClock(I2C_CLOCK_SPEED);
        i2c_connection = true;
        send(MSG_CONNECTED);

        debug("Connecting via i2c...");

        start_time = millis();

        i2c_request();

        while (response != RES_OK && millis() - start_time < 1000) {
            update();
        }
        i2c_connection = (response == RES_OK);

        while (!i2c_connection && millis() - start_time < 1000) {
            update();
        }
        i2c_connection = (response == RES_OK);

        // debugf("i2c_connection=%d,response=%u\n", i2c_connection, response);

        // debugln(i2c_connection ? "OK" : "ERROR");
#endif // ifdef ENABLE_I2C
    }

    void update() {
#ifdef ENABLE_I2C
        update_i2c();
#endif // ifdef ENABLE_I2C

#ifdef ENABLE_SERIAL
        update_serial();
#endif // ifdef ENABLE_SERIAL

        if (response_change) {
            response_change = false;

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

        beginTransmission();
        transmitData(REQ_SOT);
        ++sent;

        debug("Sending=");

        while (i < len) {
            char b = str[i];

            debug(b);
            transmitData(b);

            ++i;
            ++sent;

            if (sent % PACKET_SIZE == 0) {
                endTransmission();
                beginTransmission();
            }
        }

        transmitData(REQ_EOT);
        ++sent;
        endTransmission();

#ifdef ENABLE_I2C
        if (i2c_connection) i2c_request();
#endif // ifdef ENABLE_I2C

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
        return serial_connection || i2c_connection;
    }
}