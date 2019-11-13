/*
   Copyright (c) 2019 Stefan Kremser
   This software is licensed under the MIT License. See the license file for details.
   Source: github.com/spacehuhn/WiFiDuck
 */

#pragma once

#include <Arduino.h> // String

namespace settings {
    void begin();
    void load();

    void reset();
    void save();

    String toString();

    const char* getSSID();
    const char* getPassword();
    const char* getChannel();
    const char* getStartup();

    int getChannelNum();

    void set(const char* name, const char* value);

    void setSSID(const char* ssid);
    void setPassword(const char* password);
    void setChannel(const char* channel);
    void setStartup(const char* startup);
}
