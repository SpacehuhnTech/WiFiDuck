/*!
   \file esp_duck/cli.h
   \brief Command line interface header
   \author Stefan Kremser
   \copyright MIT License
 */

#pragma once

#include <Arduino.h> // String, bool

/*! \typedef PrintFunction
 *  \brief A function that outputs a given string, for example to std::out.
 *  \param s String to be printed
 */
typedef void (* PrintFunction)(const char* s);

/*! \namespace CLI
 *  \brief Command line interface module
 */
namespace cli {
    /*! Initializes the CLI module */
    void begin();

    /*!
     * \brief Processes user input as a command
     *
     * Analyzes the input string if it matches a command name and its arguments.
     * Resulting output, from a command or an error message,
     * will be passed to printfunc.
     * If echo is true, "# <input>" will be passed to printfunc first.
     *
     * \param input     String to be parsed
     * \param printfunc Function that prints the result
     * \param echo      Flag to enable echo of input
     */
    void parse(const char* input, PrintFunction printfunc, bool echo = true);
}