/*
   Copyright (c) 2019 Stefan Kremser
   This software is licensed under the MIT License. See the license file for details.
   Source: github.com/spacehuhn/WiFiDuck
 */

#include "educk.h"

#include <Wire.h>   // i2c stuff

#include "config.h" // i2c config
#include "debug.h"  // debug output

#define RESPONSE_OK 0x00
#define RESPONSE_PROCESSING 0x01
#define RESPONSE_REPEAT 0x02
#define RESPONSE_I2C_ERROR 0xFF

namespace educk {
    // ===== PRIVATE ===== //
    bool    connection = false;
    size_t  bytesSent  = 0;
    uint8_t response   = RESPONSE_OK;

    char * prevMessage    = NULL;
    size_t prevMessageLen = 0;

    void sendRequest() {
        Wire.requestFrom(I2C_ADDR, 1);

        if (Wire.available()) {
            response = Wire.read();
        } else {
            // debugln("Request error :(");
            connection = false;
            response   = RESPONSE_I2C_ERROR;
        }
    }

    void wait() {
        sendRequest();

        while (response & 0x01 == RESPONSE_PROCESSING) {
            delay(response);
            debug('.');
            sendRequest();
        }

        bytesSent = 0;
    }

    void transmit(const char* buf, size_t len) {
        if (!connection) return;

        unsigned int transmissions = (len / PACKET_SIZE) + (len % PACKET_SIZE > 0);
        unsigned int buffer_i      = 0;
        unsigned int transmission_i;
        unsigned int packet_i;

        for (transmission_i = 0; transmission_i < transmissions; ++transmission_i) {
            Wire.beginTransmission(I2C_ADDR);

            for (packet_i = 0; packet_i < PACKET_SIZE && buffer_i < len; ++packet_i) {
                debug(buf[buffer_i]);
                Wire.write(buf[buffer_i++]);
                if (++bytesSent == BUFFER_SIZE) wait();
            }

            Wire.endTransmission();
        }

        wait();
    }

    // ===== PUBLIC ===== //

    void begin() {
        Wire.begin(I2C_SDA, I2C_SCL);
        Wire.setClock(I2C_CLOCK_SPEED);
        connection = true;
        sendRequest();
    }

    void sendMessage(const String& msg) {
        sendMessage(msg.c_str());
    }

    void sendMessage(const char* msg) {
        debug("Sending message: ");
        // debug(msg);

        size_t msg_len = strlen(msg);

        transmit(msg, msg_len);

        // Remember this message if transmission was succesful
        if (response == RESPONSE_OK) {
            if (prevMessage) free(prevMessage);
            prevMessageLen = msg_len;
            prevMessage    = (char*)malloc(prevMessageLen + 1);
            strcpy(prevMessage, msg);
        }

        // Repeat last message as long as requested
        while (response == RESPONSE_REPEAT) {
            transmit(prevMessage, prevMessageLen);
        }

        debugln("Done");
    }

    bool connected() {
        return connection;
    }
}