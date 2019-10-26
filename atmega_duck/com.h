/*!
    \file atmega_duck/com.h
    \brief Communication Module header
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

/*! \namespace com
 *  \brief Communication module
 */
namespace com {
    /*! Initializes the communication module */
    void begin();

    /*! Updates the communication module */
    void update();

    /*! Returns whether or not there's data to be processed */
    bool hasData();

    /*! Returns reference to buffer */
    const buffer_t& getBuffer();

    /*! Sends acknowledgement that data was parsed and executed */
    void sendDone();
}