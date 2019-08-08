/*
   Copyright (c) 2019 Stefan Kremser
   This software is licensed under the MIT License. See the license file for details.
   Source: github.com/spacehuhn/WiFiDuck
 */

#pragma once

#include <Arduino.h> // String
#include <FS.h>      // File

namespace spiffs {
    void begin();
    void format();

    size_t size();
    size_t usedBytes();
    size_t freeBytes();

    size_t size(const String& fileName);

    void remove(const String& fileName);
    void rename(const String& oldName, const String& newName);
    void write(const String& fileName, const uint8_t* buf, size_t len);
    void append(const String& fileName, const uint8_t* buf, size_t len);

    File openFile(const String& fileName);
}