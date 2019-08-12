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

    size_t size(String fileName);
    bool exists(String fileName);

    File open(String fileName);
    void create(String fileName);

    void remove(String fileName);
    void rename(String oldName, String newName);
    void write(String fileName, const uint8_t* buf, size_t len);

    String listDir(String dirName);
}