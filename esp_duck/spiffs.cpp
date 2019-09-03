/*
   Copyright (c) 2019 Stefan Kremser
   This software is licensed under the MIT License. See the license file for details.
   Source: github.com/spacehuhn/WiFiDuck
 */

#include "spiffs.h"

#include "config.h"
#include "debug.h"

namespace spiffs {
    File streamFile;

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
        if (!exists("/settings.ini")) {
            format();
            File f { SPIFFS.open("/settings.ini", "w") };
            f.close();
        }
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

    size_t size(String fileName) {
        String path { fixPath(fileName) };
        File   f { SPIFFS.open(path.c_str(), "r") };

        return f.size();
    }

    bool exists(String fileName) {
        return SPIFFS.exists(fileName);
    }

    File open(String fileName) {
        String path { fixPath(fileName) };

        return SPIFFS.open(path.c_str(), "a+");
    }

    void create(String fileName) {
        String path { fixPath(fileName) };
        File   f { SPIFFS.open(path.c_str(), "a+") };

        f.close();
    }

    void remove(String fileName) {
        String path { fixPath(fileName) };

        SPIFFS.remove(path.c_str());
    }

    void rename(String oldName, String newName) {
        String oldPath { fixPath(oldName) };
        String newPath { fixPath(newName) };

        SPIFFS.rename(oldPath.c_str(), newPath.c_str());
    }

    void write(String fileName, const uint8_t* buf, size_t len) {
        File f { open(fileName) };

        if (f) {
            f.write(buf, len);
            f.close();
            debugln("Wrote file");
        } else {
            debugln("File error");
        }
    }

    String listDir(String dirName) {
        String res;

        String path { fixPath(dirName) };

        Dir dir { SPIFFS.openDir(path.c_str()) };

        while (dir.next()) {
            res += dir.fileName();
            res += ' ';
            res += size(dir.fileName());
            res += '\n';
        }

        return res;
    }

    void streamOpen(String fileName) {
        streamClose();
        streamFile = open(fileName);
    }

    void streamWrite(const char* buf, size_t len) {
        if (streamFile) streamFile.write(buf, len);
        else debugln("ERROR: No stream file open");
    }

    size_t streamRead(char* buf, size_t len) {
        if (streamFile) {
            size_t i;

            for (i = 0; i<len; ++i) {
                if (!streamFile.available() || (i == len-1)) {
                    buf[i] = '\0';
                    if (!streamFile.available()) break;
                } else {
                    buf[i] = streamFile.read();
                }
            }

            return i;
        } else {
            debugln("ERROR: No stream file open");
            return 0;
        }
    }

    void streamClose() {
        streamFile.close();
    }

    bool streaming() {
        return streamFile;
    }
}