#pragma once

#include "Arduino.h"

class File {
    String filename;
    String content;
    size_t cursor = 0;
    bool opened = false;
public:
    File() = default;
    File(String filename, String content) : filename(filename), content(content), opened(true) {}
    explicit operator bool() const { return opened; }
    bool available() const { return opened && cursor < content.size(); }
    uint8_t peek() const { return (uint8_t)content[cursor]; }
    char read() { return content[cursor++]; }
    uint32_t position() const { return (uint32_t)cursor; }
    bool seek(uint32_t offset) {
        if (!opened || offset > content.size()) return false;
        cursor = offset;
        return true;
    }
    void close() { opened = false; }
    const char* name() const { return filename.c_str(); }
};
