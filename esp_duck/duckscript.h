/*
   Copyright (c) 2019 Stefan Kremser
   This software is licensed under the MIT License. See the license file for details.
   Source: github.com/spacehuhn/WiFiDuck
 */

#pragma once

#include <Arduino.h> // String

namespace duckscript {
    void runTest();
    void run(String fileName);

    void nextLine();
    void repeat();
    void stopAll();
    void stop(String fileName);

    bool isRunning();
    String currentScript();
};