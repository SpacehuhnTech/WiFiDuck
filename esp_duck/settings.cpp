/*
   Copyright (c) 2019 Stefan Kremser
   This software is licensed under the MIT License. See the license file for details.
   Source: github.com/spacehuhn/WiFiDuck
 */

#include "settings.h"


#include "spiffs.h"
#include "eeprom.h"
#include "config.h"
#include "debug.h"

#define SETTINGS_ADDRES 1
#define SETTINGS_MAGIC_NUM 1234567891

namespace settings {
    // ===== PRIVATE ===== //
    typedef struct settings_t {
        uint32_t magic_num;

        char ssid[33];
        char password[65];
        char channel[5];
        char startup[65];
    } settings_t;

    settings_t data;

    // ===== PUBLIC ====== //
    void begin() {
        load();
    }

    void load() {        
        eeprom::getObject(SETTINGS_ADDRES, data);        
        if (data.magic_num != SETTINGS_MAGIC_NUM) reset();
    }

    void reset() {
        debugln("Resetting Settings");
        data.magic_num = SETTINGS_MAGIC_NUM;
        setSSID(WIFI_SSID);
        setPassword(WIFI_PASSWORD);
        setChannel(WIFI_CHANNEL);
        setStartup("");
        save();
    }

    void save() {
        debugln("Saving Settings");
        eeprom::saveObject(SETTINGS_ADDRES, data);
    }

    String toString() {
        String s;

        s += "ssid=";
        s += getSSID();
        s += "\n";
        s += "password=";
        s += getPassword();
        s += "\n";
        s += "channel=";
        s += getChannel();
        s += "\n";
        s += "startup=";
        s += getStartup();
        s += "\n";

        return s;
    }

    const char* getSSID() {
        return data.ssid;
    }

    const char* getPassword() {
        return data.password;
    }

    const char* getChannel() {
        return data.channel;
    }

    int getChannelNum() {
        if (strcmp(data.channel, "auto") != 0) return atoi(data.channel);
        return 1;
    }

    const char* getStartup() {
      return data.startup;
    }

    void set(const char* name, const char* value) {
        if (strcmp(name, "ssid") == 0) {
            setSSID(value);
        } else if (strcmp(name, "password") == 0) {
            setPassword(value);
        } else if (strcmp(name, "channel") == 0) {
            setChannel(value);
        } else if (strcmp(name, "startup") == 0) {
            setStartup(value);
        }
    }

    void setSSID(const char* ssid) {
        if (ssid && (strlen(ssid) <= 32)) {
            size_t ssid_len = strlen(ssid);

            for (uint8_t i = 0; i<33; ++i) {
                if (i < ssid_len) data.ssid[i] = ssid[i];
                else data.ssid[i] = '\0';
            }
        }
    }

    void setPassword(const char* password) {
        if (password && (strlen(password) >= 8) && (strlen(password) <= 64)) {
            size_t password_len = strlen(password);

            for (uint8_t i = 0; i<65; ++i) {
                if (i < password_len) data.password[i] = password[i];
                else data.password[i] = '\0';
            }
        }
    }

    void setChannel(const char* channel) {
        if (channel && ((strcmp(channel, "auto") == 0) || ((atoi(channel) >= 1) && (atoi(channel) <= 13)))) {
            size_t channel_len = strlen(channel);

            for (uint8_t i = 0; i<5; ++i) {
                if (i < channel_len) data.channel[i] = channel[i];
                else data.channel[i] = '\0';
            }
        }
    }

    void setStartup(const char* startup) {
        if (startup && (strlen(startup) <= 64)) {
            size_t startup_len = strlen(startup);

            for (uint8_t i = 0; i<65; ++i) {
                if (i < startup_len) data.startup[i] = startup[i];
                else data.startup[i] = '\0';
            }
        }
    }
}
