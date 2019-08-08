/*
   Copyright (c) 2019 Stefan Kremser
   This software is licensed under the MIT License. See the license file for details.
   Source: github.com/spacehuhn/WiFiDuck
 */

#include "i2c.h"

#include <Wire.h>   // i2c stuff

#include "config.h" // i2c config
#include "debug.h"  // debug output

#define RESPONSE_OK 0x00
#define RESPONSE_PROCESSING 0x01
#define RESPONSE_REPEAT 0x02
#define RESPONSE_I2C_ERROR 0xFE

namespace i2c {
    // ===== PRIVATE ===== //
    bool   connection { false };
    size_t connection_tries { 0 };

    bool responseChanged { false };
    uint8_t response   { RESPONSE_OK };

    unsigned long connectionTime { 0 };
    unsigned long requestTime { 0 };

    i2c_callback callback_ok { NULL };
    i2c_callback callback_processing { NULL };
    i2c_callback callback_repeat { NULL };
    i2c_callback callback_error { NULL };

    // ===== PUBLIC ===== //
    void begin() {
        if ((connectionTime == 0) || (millis() - connectionTime > 2000)) {
            Wire.begin(I2C_SDA, I2C_SCL);
            Wire.setClock(I2C_CLOCK_SPEED);

            connection     = true;
            connectionTime = millis();
            ++connection_tries;

            sendRequest();

            debugf("Connecting via i2c...%s\n", connection ? "OK" : "ERROR");
        }
    }

    void update() {
        if (!connection && (connection_tries < NUMBER_CONNECTION_TRIES)) {
            begin();
        }

        if ((response & 0x01) && (response != RESPONSE_I2C_ERROR)
            && (millis() - requestTime > response)) {
            sendRequest();
        }

        if (responseChanged) {
            responseChanged = false;

            debug("STATUS=");

            switch (getStatus()) {
                case status::OK:
                    debugf("OK [%u]\n", response);
                    if (callback_ok) callback_ok();
                    break;
                case status::PROCESSING:
                    debugf("PROCESSING [%u]\n", response);
                    if (callback_processing) callback_processing();
                    break;
                case status::REPEAT:
                    debugf("REPEAT [%u]\n", response);
                    if (callback_repeat) callback_repeat();
                    break;
                case status::ERROR:
                    debugf("ERROR [%u]\n", response);
                    if (callback_error) callback_error();
                    break;
                default:
                    debugf("UNKOWN [%u]\n", response);
            }
        }
    }

    bool connected() {
        return connection;
    }

    uint8_t getResponse() {
        return response;
    }

    status getStatus() {
        switch (response) {
            case RESPONSE_OK:
                return status::OK;
            case RESPONSE_REPEAT:
                return status::REPEAT;
            case RESPONSE_I2C_ERROR:
                return status::ERROR;
            default:
                return status::PROCESSING;
        }
    }

    void sendRequest() {
        if (!connection) return;

        requestTime = millis();

        Wire.requestFrom(I2C_ADDR, 1);

        uint8_t prev_response = response;

        if (Wire.available()) {
            response = Wire.read();
        } else {
            connection = false;
            response   = RESPONSE_I2C_ERROR;
            debugln("Request error :(");
        }

        responseChanged = (prev_response != response);
    }

    size_t transmit(const uint8_t* data, size_t len) {
        if (!connection) return 0;
        if (len > BUFFER_SIZE) len = BUFFER_SIZE;

        unsigned int transmissions {
            (len / PACKET_SIZE) + (len % PACKET_SIZE > 0)
        };

        size_t data_i         { 0 };
        size_t transmission_i { 0 };
        size_t packet_i       { 0 };

        bool eol { false }; // End Of Line

        for (transmission_i = 0; transmission_i < transmissions && !eol; ++transmission_i) {
            Wire.beginTransmission(I2C_ADDR);

            for (packet_i = 0; packet_i < PACKET_SIZE && data_i < len && !eol; ++packet_i) {
                uint8_t b = data[data_i];

                eol = (b == '\n');

                debug(char(b));
                Wire.write(b);

                ++data_i;
            }

            Wire.endTransmission();
        }

        sendRequest();

        return data_i;
    }

    void setOnOK(i2c_callback c) {
        callback_ok = c;
    }

    void setOnProcessing(i2c_callback c) {
        callback_processing = c;
    }

    void setOnRepeat(i2c_callback c) {
        callback_repeat = c;
    }

    void setOnError(i2c_callback c) {
        callback_error = c;
    }
}