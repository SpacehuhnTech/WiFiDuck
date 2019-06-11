/*
   Copyright (c) 2019 Stefan Kremser
   This software is licensed under the MIT License. See the license file for details.
   Source: github.com/spacehuhn/WiFiDuck
 */

#include "SimpleMouse.h"

SimpleMouse::SimpleMouse() {
    static HIDSubDescriptor node(mouseDescriptor, sizeof(mouseDescriptor));

    HID().AppendDescriptor(&node);

    relase_mouse_report = makeReport();
}

mouse_report SimpleMouse::makeReport(uint8_t buttons, int8_t x, int8_t y, int8_t scroll) {
    mouse_report m;

    m.buttons = buttons;
    m.x       = x;
    m.y       = y;
    m.scroll  = scroll;

    return m;
}

void SimpleMouse::move(mouse_report* m) {
    HID().SendReport(1, m, sizeof(mouse_report));
}

void SimpleMouse::release() {
    move(&relase_mouse_report);
}