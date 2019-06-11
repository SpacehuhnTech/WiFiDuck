/*
   Copyright (c) 2019 Stefan Kremser
   This software is licensed under the MIT License. See the license file for details.
   Source: github.com/spacehuhn/WiFiDuck
 */

#ifndef SimpleMouse_h
#define SimpleMouse_h

#include <HID.h>

const uint8_t mouseDescriptor[] PROGMEM = {
    //  Mouse
    0x05, 0x01, // USAGE_PAGE (Generic Desktop)  // 54
    0x09, 0x02, // USAGE (Mouse)
    0xa1, 0x01, // COLLECTION (Application)
    0x09, 0x01, //   USAGE (Pointer)
    0xa1, 0x00, //   COLLECTION (Physical)
    0x85, 0x01, //     REPORT_ID (1)
    0x05, 0x09, //     USAGE_PAGE (Button)
    0x19, 0x01, //     USAGE_MINIMUM (Button 1)
    0x29, 0x03, //     USAGE_MAXIMUM (Button 3)
    0x15, 0x00, //     LOGICAL_MINIMUM (0)
    0x25, 0x01, //     LOGICAL_MAXIMUM (1)
    0x95, 0x03, //     REPORT_COUNT (3)
    0x75, 0x01, //     REPORT_SIZE (1)
    0x81, 0x02, //     INPUT (Data,Var,Abs)
    0x95, 0x01, //     REPORT_COUNT (1)
    0x75, 0x05, //     REPORT_SIZE (5)
    0x81, 0x03, //     INPUT (Cnst,Var,Abs)
    0x05, 0x01, //     USAGE_PAGE (Generic Desktop)
    0x09, 0x30, //     USAGE (X)
    0x09, 0x31, //     USAGE (Y)
    0x09, 0x38, //     USAGE (Wheel)
    0x15, 0x81, //     LOGICAL_MINIMUM (-127)
    0x25, 0x7f, //     LOGICAL_MAXIMUM (127)
    0x75, 0x08, //     REPORT_SIZE (8)
    0x95, 0x03, //     REPORT_COUNT (3)
    0x81, 0x06, //     INPUT (Data,Var,Rel)
    0xc0,       //   END_COLLECTION
    0xc0,       // END_COLLECTION
};

typedef struct mouse_report {
    uint8_t buttons;
    int8_t  x;
    int8_t  y;
    int8_t  scroll;
} mouse_report;

class SimpleMouse {
    private:
        mouse_report relase_mouse_report;

    public:
        SimpleMouse();

        mouse_report makeReport(uint8_t buttons = 0, int8_t x = 0, int8_t y = 0, int8_t scroll = 0);
        void move(mouse_report* m);
        void release();
};

#endif /* ifndef SimpleMouse_h */