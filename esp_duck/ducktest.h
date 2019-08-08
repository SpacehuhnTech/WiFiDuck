/*
   Copyright (c) 2019 Stefan Kremser
   This software is licensed under the MIT License. See the license file for details.
   Source: github.com/spacehuhn/WiFiDuck
 */

#pragma once

#include <Arduino.h> // String

namespace ducktest {
    void run();
    void run(const String& fileName);

    void update();
    void repeat();
    void stop();
};