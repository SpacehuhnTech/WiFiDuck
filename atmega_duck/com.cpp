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
#define REQ_SOT 0x01 // !< Start of transmission
#define REQ_EOT 0x04 // !< End of transmission

// ! Communication response codes
#define RES_OK 0x00
#define RES_PROCESSING 0x01
#define RES_REPEAT 0x02

// ! Minimum delay after request,
// ! for the master to send another request
#define MIN_DELAY 5

namespace com {
    // ===== PRIVATE ===== //
    buffer_t buffer; // !< Communication buffer Instance

    bool start_parser         = false;
    bool i2c_active           = false;
    bool serial_active        = false;
    bool ongoing_transmission = false;

    /*!
     * \brief Internal function to buffer received data
     *
     * Writes the receieved data into the buffer.
     * Returns wheter or not data was received.
     */
    bool receive(Stream& stream) {
        if (stream.available()) {
            debug("RECEIVE ");

            // ! Skip bytes
            while (stream.available() && !ongoing_transmission) {
                if (stream.read() == REQ_SOT) {
                    ongoing_transmission = true;
                    debug("SOT ");
                }
            }

            if (stream.available()) {
                debug("'");

                while (stream.available() && ongoing_transmission) {
                    char c = stream.read();

                    if (c == REQ_EOT) {
                        start_parser         = true;
                        ongoing_transmission = false;
                    } else {
                        debug(c);
                        buffer.data[buffer.len] = c;
                        ++buffer.len;
                    }

                    if (buffer.len == BUFFER_SIZE) {
                        start_parser         = true;
                        ongoing_transmission = false;
                    }
                }
                debug("' ");
            }

            if (!ongoing_transmission && !start_parser) debug("DROPPED");
            debugln();

            return start_parser;
        }

        return false;
    }

    /*!
     * \brief Internal function to send response
     *
     * Replies with wait time, if slave is still processing.
     * Replies repeat if duckparsers repeat counter > 0.
     * If everything was processed and the buffer is empty, it replies with OK.
     */
    void respond(Stream& stream) {
        if (hasData()) {
            uint8_t delayTime = min(duckparser::getDelayTime(), 255);
            uint8_t response  = delayTime | RES_PROCESSING | MIN_DELAY;
            stream.print(response);
            debug("Responding PROCESSING ");
            debug(response);
            debugln("ms");
        } else if (duckparser::getRepeats()) {
            stream.print(RES_REPEAT);
            debugln("Responding REPEAT");
        } else {
            stream.print(RES_OK);
            debugln("Responding OK");
        }
    }

    /*!
     * \brief Internal i2c request event handler
     */
    void requestEvent() {
        debugln("I2C REQUEST");
        respond(Wire);
    }

    /*!
     * \brief Internal i2c receive event handler
     */
    void receiveEvent(int len) {
        receive(Wire);
    }

    // ===== PUBLIC ===== //
    void begin() {
#ifdef ENABLE_SERIAL
        debugln("ENABLED SERIAL");
        SERIAL_COM.begin(SERIAL_BAUD);
#endif // ifdef ENABLE_SERIAL

#ifdef ENABLE_I2C
        debugln("ENABLED I2C");
        Wire.begin(I2C_ADDR);         // !< Begin i2c slave on given address
        Wire.onRequest(requestEvent); // !< Set request event handler
        Wire.onReceive(receiveEvent); // !< Set receive event handler
#endif // ifdef ENABLE_I2C
    }

    void update() {
#ifdef ENABLE_SERIAL
        if (receive(SERIAL_COM)) {
            serial_active = true;
            respond(SERIAL_COM);
        }
#endif // ifdef ENABLE_SERIAL
    }

    bool hasData() {
        return buffer.len > 0 && start_parser;
    }

    const buffer_t& getBuffer() {
        return buffer;
    }

    void sendDone() {
        buffer.len   = 0;
        start_parser = false;

#ifdef ENABLE_SERIAL
        if (serial_active) respond(SERIAL_COM);
#endif // ifdef ENABLE_SERIAL

#ifdef ENABLE_I2C
        if (i2c_active) respond(Wire);
#endif // ifdef ENABLE_I2C
    }
}