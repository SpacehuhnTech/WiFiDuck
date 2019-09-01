/*
   Copyright (c) 2019 Stefan Kremser
   This software is licensed under the MIT License. See the license file for details.
   Source: github.com/spacehuhn/WiFiDuck
 */

#pragma once

#include <Arduino.h> // String

typedef void (* PrintFunction)(const char* s);

namespace cli {
    void begin();

    void parse(const char* input, PrintFunction printfunc);
}