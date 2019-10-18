/*
   Copyright (c) 2019 Stefan Kremser
   This software is licensed under the MIT License. See the license file for details.
   Source: github.com/spacehuhn/SimpleCLI
 */

#pragma once

#include <stddef.h> // size_t

namespace duckparser {
    void parse(const char* str, size_t len);
    int getRepeats();
    unsigned int getDelayTime();
};