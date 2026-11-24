/*
   This software is licensed under the MIT License. See the license file for details.
   Source: https://github.com/spacehuhntech/WiFiDuck
 */

#pragma once

#include <HID.h>

namespace mouse {
    const uint8_t LEFT   = 0x01;
    const uint8_t RIGHT  = 0x02;
    const uint8_t MIDDLE = 0x04;
    const uint8_t ALL    = LEFT | RIGHT | MIDDLE;

    void begin();
    void move(int8_t x, int8_t y, int8_t wheel = 0);
    void click(uint8_t button = LEFT);
    void press(uint8_t button = LEFT);
    void release(uint8_t button = ALL);
}
