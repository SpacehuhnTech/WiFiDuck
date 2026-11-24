/*
   This software is licensed under the MIT License. See the license file for details.
   Source: https://github.com/spacehuhntech/WiFiDuck
 */

#include "mouse.h"

namespace mouse {
    // Report ID 1 is separate from the keyboard's report ID 2.
    const uint8_t mouseDescriptor[] PROGMEM {
        0x05, 0x01, // USAGE_PAGE (Generic Desktop)
        0x09, 0x02, // USAGE (Mouse)
        0xa1, 0x01, // COLLECTION (Application)
        0x85, 0x01, // REPORT_ID (1)
        0x09, 0x01, // USAGE (Pointer)
        0xa1, 0x00, // COLLECTION (Physical)
        0x05, 0x09, // USAGE_PAGE (Button)
        0x19, 0x01, // USAGE_MINIMUM (Button 1)
        0x29, 0x03, // USAGE_MAXIMUM (Button 3)
        0x15, 0x00, // LOGICAL_MINIMUM (0)
        0x25, 0x01, // LOGICAL_MAXIMUM (1)
        0x95, 0x03, // REPORT_COUNT (3)
        0x75, 0x01, // REPORT_SIZE (1)
        0x81, 0x02, // INPUT (Data,Var,Abs)
        0x95, 0x01, // REPORT_COUNT (1)
        0x75, 0x05, // REPORT_SIZE (5)
        0x81, 0x03, // INPUT (Cnst,Var,Abs)
        0x05, 0x01, // USAGE_PAGE (Generic Desktop)
        0x09, 0x30, // USAGE (X)
        0x09, 0x31, // USAGE (Y)
        0x09, 0x38, // USAGE (Wheel)
        0x15, 0x81, // LOGICAL_MINIMUM (-127)
        0x25, 0x7f, // LOGICAL_MAXIMUM (127)
        0x75, 0x08, // REPORT_SIZE (8)
        0x95, 0x03, // REPORT_COUNT (3)
        0x81, 0x06, // INPUT (Data,Var,Rel)
        0xc0,       // END_COLLECTION
        0xc0,       // END_COLLECTION
    };

    uint8_t buttons = 0;

    void begin() {
        static HIDSubDescriptor node(mouseDescriptor, sizeof(mouseDescriptor));
        HID().AppendDescriptor(&node);
    }

    void move(int8_t x, int8_t y, int8_t wheel) {
        uint8_t report[] = { buttons, (uint8_t)x, (uint8_t)y, (uint8_t)wheel };
        HID().SendReport(1, report, sizeof(report));
    }

    void click(uint8_t button) {
        // Preserve other held buttons; cycle the requested button even if held.
        button &= ALL;
        uint8_t held = buttons & ~button;
        if (buttons != held) {
            buttons = held;
            move(0, 0);
        }
        buttons = held | button;
        move(0, 0);
        buttons = held;
        move(0, 0);
    }

    void press(uint8_t button) {
        buttons |= button & ALL;
        move(0, 0);
    }

    void release(uint8_t button) {
        buttons &= ~(button & ALL);
        move(0, 0);
    }
}
