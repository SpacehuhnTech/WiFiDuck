/*
   Copyright (c) 2019 Stefan Kremser
   This software is licensed under the MIT License. See the license file for details.
   Source: github.com/spacehuhn/WiFiDuck
 */

#pragma once

#include <Arduino.h>

#ifdef ENABLE_DEBUG

#define debug(...) DEBUG_PORT.print(__VA_ARGS__)
#define debugln(...) DEBUG_PORT.println(__VA_ARGS__)
#define debugf(...) DEBUG_PORT.printf(__VA_ARGS__)

#else /* ifdef ENABLE_DEBUG */

#define debug(...) 0
#define debugln(...) 0
#define debugf(...) 0

#endif /* ifdef ENABLE_DEBUG */