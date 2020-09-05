/*
   This software is licensed under the MIT License. See the license file for details.
   Source: https://github.com/spacehuhntech/WiFiDuck
 */

#pragma once

#include <Arduino.h>

#ifdef ENABLE_DEBUG

#define debug_init() DEBUG_PORT.begin(DEBUG_BAUD);\
    DEBUG_PORT.setTimeout(200);

#define debug(...) DEBUG_PORT.print(__VA_ARGS__)
#define debugln(...) DEBUG_PORT.println(__VA_ARGS__)
#define debugf(...) DEBUG_PORT.printf(__VA_ARGS__)

#define debug_update()\
    if (Serial.available()) {\
        String input = Serial.readStringUntil('\n');\
        cli::parse(input.c_str(), [] (const char* str) {\
            Serial.println(str);\
        });\
    }

#else /* ifdef ENABLE_DEBUG */

#define debug_init() 0

#define debug(...) 0
#define debugln(...) 0
#define debugf(...) 0

#define debug_update() 0

#endif /* ifdef ENABLE_DEBUG */