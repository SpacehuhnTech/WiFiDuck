/*!
   \file atmega_duck/i2c.cpp
   \brief i2c communication module source
   \author Stefan Kremser
   \copyright MIT License
 */

#include "i2c.h"

// Arduino i2c
#include <Wire.h>

#include "debug.h"
#include "duckparser.h"

// ! Communication response codes
#define RESPONSE_OK 0x00
#define RESPONSE_PROCESSING 0x01
#define RESPONSE_REPEAT 0x02

// ! Minimum delay after request,
// ! for the master to send another request
#define MIN_DELAY 5

namespace i2c {
    // ===== PRIVATE ===== //
    buffer_t buffer;                // !< Communication buffer Instance
    bool     startParser { false }; // !< Flag to start parsing input

    /*!
     * \brief Internal i2c request event handler
     *
     * Replies to i2c master with wait time, if slave is still processing.
     * Or to repeat the last command.
     * If everything was processed and the buffer is empty, it replies with OK.
     */
    void requestEvent() {
        debugln("I2C REQUEST");

        if (buffer.len > 0) {
            unsigned int delayTime = duckparser::getDelayTime();
            if (delayTime > 255) {
                Wire.write(255);
            } else {
                Wire.write((uint8_t)delayTime | RESPONSE_PROCESSING | MIN_DELAY);
            }
            startParser = true;
            debug(".");
        } else if (duckparser::getRepeats() > 0) {
            Wire.write(RESPONSE_REPEAT);
            debugln("\nI2C REPEAT");
        } else {
            Wire.write(RESPONSE_OK);
            debugln("\nDone");
        }
    }

    /*!
     * \brief Internal i2c receive event handler
     *
     * Writes the receieved data into the buffer.
     */
    void receiveEvent(int len) {
        debugln("RECEIVE");

        if (buffer.len + (unsigned int)len <= BUFFER_SIZE) {
            debugln("Received packet");
            Wire.readBytes(&buffer.data[buffer.len], len);
            buffer.len += len;
        } else {
            debugln("!!! Buffer is full !!!");
        }
    }

    // ===== PUBLIC ===== //
    void begin() {
        Wire.begin(I2C_ADDR);         // !< Begin i2c slave on given address
        Wire.onRequest(requestEvent); // !< Set request event handler
        Wire.onReceive(receiveEvent); // !< Set receive event handler
    }

    const buffer_t& getBuffer() {
        return buffer;
    }

    bool hasData() {
        return buffer.len > 0 && startParser;
    }

    void sendACK() {
        buffer.len  = 0;
        startParser = false;
    }
}