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
        ini = ini_file_destroy(ini);

        const char* FILE_NAME = "/settings.ini";

        if (!spiffs::exists(FILE_NAME)) {
            spiffs::create(FILE_NAME);
            spiffs::write(FILE_NAME, "[WiFi]\n"
                                     "SSID=wifiduck\n"
                                     "password=wifiduck\n");
        }

        spiffs::streamOpen(FILE_NAME);

        size_t read;
        size_t buffer_size = 1024;
        char   buffer[buffer_size];

        do {
            read = spiffs::streamReadUntil(buffer, '\n', buffer_size);
            ini  = ini_parse(ini, buffer, read);
        } while (read > 0 && spiffs::streaming());

        spiffs::streamClose();
    }

    String toString() {
        size_t len = ini_file_strlen(ini);

        char str[len+1];

        str[len] = '\0';

        ini_file_str(ini, str);

        return String(str);
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