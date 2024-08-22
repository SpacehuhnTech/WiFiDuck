/*
   This software is licensed under the MIT License. See the license file for details.
   Source: https://github.com/spacehuhntech/WiFiDuck
 */

#include "settings.h"

#include "spiffs.h"
#include "debug.h"
#include "config.h"
#include "eeprom.h"

#define SETTINGS_ADDRES 1
#define SETTINGS_MAGIC_NUM 1234567891

namespace settings {
    // ===== PRIVATE ===== //
    typedef struct settings_t {
        uint32_t magic_num;

        char mode[11];
        char ssid[33];
        char password[65];
        char channel[5];
        char autorun[65];
    } settings_t;

    settings_t data;

    // ===== PUBLIC ====== //
    void begin() {
        eeprom::begin();
        load();
    }

    void load() {
        eeprom::getObject(SETTINGS_ADDRES, data);
        if (data.magic_num != SETTINGS_MAGIC_NUM) reset();

        if (data.mode[10] != 0) setMODE(MODE);
        if (data.ssid[32] != 0) setSSID(WIFI_SSID);
        if (data.password[64] != 0) setPassword(WIFI_PASSWORD);
        if (data.channel[4] != 0) setChannel(WIFI_CHANNEL);
        if (data.autorun[64] != 0) setAutorun("");
    }

    void reset() {
        debugln("Resetting Settings");
        data.magic_num = SETTINGS_MAGIC_NUM;
        setMODE(MODE);
        setSSID(WIFI_SSID);
        setPassword(WIFI_PASSWORD);
        setChannel(WIFI_CHANNEL);
    }

    void save() {
        debugln("Saving Settings");
        eeprom::saveObject(SETTINGS_ADDRES, data);
    }

    String toString() {
        String s;

        s += "mode=";
        s += getMODE();
        s += "\n";
        s += "ssid=";
        s += getSSID();
        s += "\n";
        s += "password=";
        s += getPassword();
        s += "\n";
        s += "channel=";
        s += getChannel();
        s += "\n";
        s += "autorun=";
        s += getAutorun();
        s += "\n";

        return s;
    }

    const char* getMODE() {
        return data.mode;
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

    const char* getAutorun() {
        return data.autorun;
    }

    void set(const char* name, const char* value) {
        if (strcmp(name, "mode") == 0) {
            setMODE(value);
        } else if (strcmp(name, "ssid") == 0) {
            setSSID(value);
        } else if (strcmp(name, "password") == 0) {
            setPassword(value);
        } else if (strcmp(name, "channel") == 0) {
            setChannel(value);
        } else if (strcmp(name, "autorun") == 0) {
            setAutorun(value);
        }
    }

    void setMODE(const char* mode) {
      if (mode) {
        memset(data.mode, 0, sizeof(data.mode));          // Clear the buffer to ensure no leftover data
        strncpy(data.mode, mode, sizeof(data.mode) - 1);  // Copy up to 10 characters, leave room for null terminator
        data.mode[sizeof(data.mode) - 1] = '\0';          // Explicitly set the last character to null terminator
        save();
      }
    }

    void setSSID(const char* ssid) {
        if (ssid) {
            memset(data.ssid, 0, 33);
            strncpy(data.ssid, ssid, 32);

            save();
        }
    }

    void setPassword(const char* password) {
        if (password && (strlen(password) >= 8)) {
            memset(data.password, 0, 65);
            strncpy(data.password, password, 64);

            save();
        }
    }

    void setChannel(const char* channel) {
        if (channel && ((strcmp(channel, "auto") == 0) || ((atoi(channel) >= 1) && (atoi(channel) <= 13)))) {
            memset(data.channel, 0, 5);
            strncpy(data.channel, channel, 4);

            save();
        }
    }

    void setAutorun(const char* autorun) {
        if (autorun) {
            memset(data.autorun, 0, 65);
            strncpy(data.autorun, autorun, 64);

            save();
        }
    }
}