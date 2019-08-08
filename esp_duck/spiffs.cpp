/*
   Copyright (c) 2019 Stefan Kremser
   This software is licensed under the MIT License. See the license file for details.
   Source: github.com/spacehuhn/WiFiDuck
 */

#include "spiffs.h"

#include "config.h"
#include "debug.h"

namespace spiffs {
    // ===== PRIVATE ===== //
    String fixPath(const String& path) {
        if (path.startsWith("/")) {
            return path;
        } else {
            return "/" + path;
        }
    }

    // ===== PUBLIC ====== //
    void begin() {
        SPIFFS.begin();
    }

    void format() {
        debug("Formatting SPIFFS...");
        SPIFFS.format();
        debugln("OK");
    }

    size_t size() {
        FSInfo fs_info;

        SPIFFS.info(fs_info);
        return fs_info.totalBytes;
    }

    size_t usedBytes() {
        FSInfo fs_info;

        SPIFFS.info(fs_info);
        return fs_info.usedBytes;
    }

    size_t freeBytes() {
        FSInfo fs_info;

        SPIFFS.info(fs_info);
        return fs_info.totalBytes - fs_info.usedBytes;
    }

    size_t size(const String& fileName) {
        String path = fixPath(fileName);
        File   f    = SPIFFS.open(path.c_str(), "r");

        return f.size();
    }

    void remove(const String& fileName) {
        String path = fixPath(fileName);

        SPIFFS.remove(path.c_str());
    }

    void rename(const String& oldName, const String& newName) {
        String oldPath = fixPath(oldName);
        String newPath = fixPath(newName);

        SPIFFS.rename(oldPath.c_str(), newPath.c_str());
    }

    void write(const String& fileName, const uint8_t* buf, size_t len) {
        String path = fixPath(fileName);

        File f = SPIFFS.open(path, "w");

        if (f) {
            f.write(buf, len);
            f.close();
            debugln("Wrote file");
        }
    }

    void append(const String& fileName, const uint8_t* buf, size_t len) {
        String path = fixPath(fileName);

        File f = SPIFFS.open(path.c_str(), "a");

        if (f) {
            f.write(buf, len);
            f.close();
            debugln("Appended file");
        }
    }

    File openFile(const String& fileName) {
        String path = fixPath(fileName);

        return SPIFFS.open(path.c_str(), "r");
    }
}