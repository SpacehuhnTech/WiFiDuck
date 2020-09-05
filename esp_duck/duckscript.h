/*
   This software is licensed under the MIT License. See the license file for details.
   Source: https://github.com/spacehuhntech/WiFiDuck
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