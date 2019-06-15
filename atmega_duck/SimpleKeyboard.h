/*
   Copyright (c) 2019 Stefan Kremser
   This software is licensed under the MIT License. See the license file for details.
   Source: github.com/spacehuhn/WiFiDuck
 */

#ifndef SimpleKeyboard_h
#define SimpleKeyboard_h

#include <HID.h>
#include "locales.h"

const uint8_t keyboardDescriptor[] PROGMEM = {
    //  Keyboard
    0x05, 0x01, //   USAGE_PAGE (Generic Desktop)  // 47
    0x09, 0x06, //   USAGE (Keyboard)
    0xa1, 0x01, //   COLLECTION (Application)
    0x85, 0x02, //   REPORT_ID (2)
    0x05, 0x07, //   USAGE_PAGE (Keyboard)

    0x19, 0xe0, //   USAGE_MINIMUM (Keyboard LeftControl)
    0x29, 0xe7, //   USAGE_MAXIMUM (Keyboard Right GUI)
    0x15, 0x00, //   LOGICAL_MINIMUM (0)
    0x25, 0x01, //   LOGICAL_MAXIMUM (1)
    0x75, 0x01, //   REPORT_SIZE (1)

    0x95, 0x08, //   REPORT_COUNT (8)
    0x81, 0x02, //   INPUT (Data,Var,Abs)
    0x95, 0x01, //   REPORT_COUNT (1)
    0x75, 0x08, //   REPORT_SIZE (8)
    0x81, 0x03, //   INPUT (Cnst,Var,Abs)

    0x95, 0x06, //   REPORT_COUNT (6)
    0x75, 0x08, //   REPORT_SIZE (8)
    0x15, 0x00, //   LOGICAL_MINIMUM (0)
    0x25, 0x73, //   LOGICAL_MAXIMUM (115)
    0x05, 0x07, //   USAGE_PAGE (Keyboard)

    0x19, 0x00, //   USAGE_MINIMUM (Reserved (no event indicated))
    0x29, 0x73, //   USAGE_MAXIMUM (Keyboard Application)
    0x81, 0x00, //   INPUT (Data,Ary,Abs)
    0xc0,       //   END_COLLECTION
};

typedef struct key_report {
    uint8_t modifiers;
    uint8_t reserved;
    uint8_t keys[6];
} key_report;

class SimpleKeyboard {
    private:
        uint8_t  * locale          = (uint8_t*)locale_us;
        key_report prev_key_report = key_report { KEY_NONE, KEY_NONE, { KEY_NONE, KEY_NONE, KEY_NONE, KEY_NONE, KEY_NONE, KEY_NONE } };

        key_report makeReport(uint8_t modifiers = KEY_NONE, uint8_t key1 = KEY_NONE, uint8_t key2 = KEY_NONE, uint8_t key3 = KEY_NONE, uint8_t key4 = KEY_NONE, uint8_t key5 = KEY_NONE, uint8_t key6 = KEY_NONE);

    public:
        SimpleKeyboard();

        void setLocale(uint8_t* locale);

        void send(key_report* k);
        void release();

        void pressKey(uint8_t key);
        void pressModifier(uint8_t key);

        void press(char c);

        void write(char c);
        void write(const char* str, size_t len);
};

#endif /* ifndef SimpleKeyboard_h */