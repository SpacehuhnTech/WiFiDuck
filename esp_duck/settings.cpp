/*
   Copyright (c) 2019 Stefan Kremser
   This software is licensed under the MIT License. See the license file for details.
   Source: github.com/spacehuhn/WiFiDuck
 */

#include "settings.h"

#include "spiffs.h"
#include "debug.h"
#include "config.h"

extern "C" {
    #include "ini.h"
}

#include <stdlib.h> // atoi

namespace settings {
    // ===== PRIVATE ===== //
    ini_file  * ini       = NULL;
    const char* FILE_NAME = "/settings.ini";

    // ===== PUBLIC ====== //
    void begin() {
        load();
    }

    void load() {
        ini = ini_file_destroy(ini);

        if (!spiffs::exists(FILE_NAME)) {
            reset();
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

        check_and_repair();
    }

    void check_and_repair() {
        if (!getSSID()) {
            setSSID(WIFI_SSID);
        }

        if (strlen(getPassword()) < 8) {
            setPassword(WIFI_PASSWORD);
        }

        if ((getChannel() == 0) || (getChannel() > 14)) {
            setChannel(WIFI_CHANNEL);
        }
    }

    void reset() {
        spiffs::remove(FILE_NAME);
        spiffs::create(FILE_NAME);
        spiffs::write(FILE_NAME, "[WiFi]\n"
                                 "SSID=\n"
                                 "password=\n"
                                 "channel=\n");
    }

    std::string toString() {
        size_t len = ini_file_strlen(ini);

        char str[len+1];

        str[len] = '\0';

        ini_file_str(ini, str);

        return std::string(str);
    }

    const char* getSSID() {
        return ini_file_get_value(ini, "SSID");
    }

    const char* getPassword() {
        return ini_file_get_value(ini, "password");
    }

    uint8_t getChannel() {
        return atoi(ini_file_get_value(ini, "channel"));
    }

    void setSSID(char* ssid) {
        ini_file_set_value(ini, "SSID", ssid);
    }

    void setPassword(char* password) {
        ini_file_set_value(ini, "password", password);
    }

    void setChannel(char* channel) {
        ini_file_set_value(ini, "channel", channel);
    }
}