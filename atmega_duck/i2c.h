/*!
    \file atmega_duck/i2c.h
    \brief i2c communication Module header
    \author Stefan Kremser
    \copyright MIT License
 */

#pragma once

#include <stddef.h> // size_t
#include "config.h" // BUFFER_SIZE

/*! \typedef buffer_t
 *  \brief A structure to buffer data and simplify access for the communication
 */
typedef struct buffer_t {
    char   data[BUFFER_SIZE]; // !< Array to buffer incoming bytes
    size_t len;               // !< How many bytes are currently in the buffer
} buffer_t;

/*! \namespace i2c
 *  \brief i2c communication module
 */
namespace i2c {
    /*! Initializes the CLI module */
    void begin();

    /*! Returns reference to buffer */
    const buffer_t& getBuffer();

    /*! Returns whether or not there's data to be processed */
    bool hasData();

    /*! Sends back acknowledgement that command was receieved and executed */
    void sendACK();
}