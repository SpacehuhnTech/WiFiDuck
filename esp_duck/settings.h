/*
   Copyright (c) 2019 Stefan Kremser
   This software is licensed under the MIT License. See the license file for details.
   Source: github.com/spacehuhn/WiFiDuck
 */

#pragma once

#include <string>

namespace settings {
    void begin();
    void load();

    void check_and_repair();
    void reset();

    std::string toString();

    const char* getSSID();
    const char* getPassword();
    uint8_t getChannel();

    void setSSID(char* ssid);
    void setPassword(char* password);
    void setChannel(char* channel);
}