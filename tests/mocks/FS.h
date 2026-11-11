#pragma once

#include "Arduino.h"

class File {
    String filename;
    String content;
    size_t position = 0;
    bool opened = false;
public:
    File() = default;
    File(String filename, String content) : filename(filename), content(content), opened(true) {}
    explicit operator bool() const { return opened; }
    bool available() const { return opened && position < content.size(); }
    uint8_t peek() const { return (uint8_t)content[position]; }
    char read() { return content[position++]; }
    void close() { opened = false; }
    const char* name() const { return filename.c_str(); }
};
