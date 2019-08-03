/*
   Copyright (c) 2019 Stefan Kremser
   This software is licensed under the MIT License. See the license file for details.
   Source: github.com/spacehuhn/WiFiDuck
 */

#pragma once

#include <Arduino.h> // String

namespace educk {
    void begin();

    void sendMessage(const String& msg);
    void sendMessage(const char* msg);

    bool connected();
};