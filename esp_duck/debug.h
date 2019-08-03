/*
   Copyright (c) 2019 Stefan Kremser
   This software is licensed under the MIT License. See the license file for details.
   Source: github.com/spacehuhn/WiFiDuck
 */

#pragma once

#include <Arduino.h>

#ifdef DEBUG

#define debug(...) Serial.print(__VA_ARGS__)
#define debugln(...) Serial.println(__VA_ARGS__)
#define debugf(...) Serial.printf(__VA_ARGS__)

#else /* ifdef DEBUG */

#define debug(...) 0
#define debugln(...) 0
#define debugf(...) 0

#endif /* ifdef DEBUG */