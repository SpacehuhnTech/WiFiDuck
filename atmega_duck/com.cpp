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
#define REQ_VERSION 0x02 // !< Request current version
#define REQ_EOT 0x04     // !< End of transmission

// ! Communication response codes
#define RES_OK 0x00
#define RES_PROCESSING 0x01
#define RES_REPEAT 0x02

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
     * Returns wheter or not data that is to be parsed was received.
     */
    bool receive(Stream& stream) {
        if (stream.available()) {
            debug("RECEIVED ");

            /*
                        if ((stream.available() == 1) && (stream.peek() == REQ_VERSION)) {
                            stream.read();
                            stream.print(VERSION);
                            return false;
                        }
             */
            // ! Skip bytes
            while (stream.available() && !ongoing_transmission) {
                if (stream.read() == REQ_SOT) {
                    ongoing_transmission = true;
                    debug("[SOT] ");
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

            if (!ongoing_transmission && !start_parser) debugln("DROPPED");
            else if (start_parser) debugln("[EOT]");
            else debugln();

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
        uint8_t response;

        if (hasData()) {
            uint8_t delayTime = (uint8_t)min(duckparser::getDelayTime(), 255);
            response = delayTime | (uint8_t)RES_PROCESSING;
            debug("Responding PROCESSING");
        } else if (duckparser::getRepeats()) {
            response = (uint8_t)RES_REPEAT;
            debug("Responding REPEAT");
        } else {
            response = (uint8_t)RES_OK;
            debug("Responding OK");
        }

        debug(" [");
        debug(response);
        debugln("]");

        stream.write(response);
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
    }
}