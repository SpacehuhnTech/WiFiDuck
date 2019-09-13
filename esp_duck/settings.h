/*
   Copyright (c) 2019 Stefan Kremser
   This software is licensed under the MIT License. See the license file for details.
   Source: github.com/spacehuhn/WiFiDuck
 */

#pragma once

#include <Arduino.h>

namespace settings {
    void begin();
    void load();
    String toString();
    const char* getSSID();
    const char* getPassword();
}