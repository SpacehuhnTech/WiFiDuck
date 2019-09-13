/*
   Copyright (c) 2019 Stefan Kremser
   This software is licensed under the MIT License. See the license file for details.
   Source: github.com/spacehuhn/WiFiDuck
 */

#include "settings.h"

#include "spiffs.h"
#include "debug.h"

extern "C" {
    #include "ini.h"
}

namespace settings {
    // ===== PRIVATE ===== //
    ini_file* ini = NULL;

    // ===== PUBLIC ====== //
    void begin() {
        load();
    }

    void load() {
        const char* FILE_NAME = "/settings.ini";

        if (!spiffs::exists(FILE_NAME)) {
            spiffs::create(FILE_NAME);
            spiffs::write(FILE_NAME, "[WiFi]\n"
                                     "SSID=wifiduck\n"
                                     "password=wifiduck\n");
        }

        spiffs::streamOpen(FILE_NAME);

        size_t read;
        size_t buffer_size = 256;
        char   buffer[buffer_size];

        do {
            read = spiffs::streamRead(buffer, buffer_size);
            ini  = ini_parse(ini, buffer, read);
        } while (read == buffer_size);

        spiffs::streamClose();
    }

    String toString() {
        char * cstr = ini_file_str(ini);
        String str(cstr);

        free(cstr);
        return str;
    }

    const char* getSSID() {
        ini_section* section = ini_file_get_section(ini, "WiFi");
        ini_pair   * pair    = ini_section_get_pair(section, "SSID");

        if (!section) debugln("No section found");
        if (!pair) debugln("No pair found");

        if (pair) debugln(pair->value);

        if (pair) return pair->value;
        return NULL;
    }

    const char* getPassword() {
        ini_pair* pair = ini_section_get_pair(ini_file_get_section(ini, "WiFi"), "password");

        if (pair) return pair->value;
        return NULL;
    }
}