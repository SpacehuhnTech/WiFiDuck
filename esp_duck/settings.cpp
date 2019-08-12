/*
   Copyright (c) 2019 Stefan Kremser
   This software is licensed under the MIT License. See the license file for details.
   Source: github.com/spacehuhn/WiFiDuck
 */

#include "settings.h"

#include "spiffs.h"

namespace settings {
    void begin() {
        if (!spiffs::exists("/settings.ini")) {
            spiffs::create("/settings.ini");
        }
    }
}