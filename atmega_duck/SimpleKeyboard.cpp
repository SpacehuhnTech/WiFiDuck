/*
   Copyright (c) 2019 Stefan Kremser
   This software is licensed under the MIT License. See the license file for details.
   Source: github.com/spacehuhn/WiFiDuck
 */

#include "SimpleKeyboard.h"

SimpleKeyboard::SimpleKeyboard() {
    static HIDSubDescriptor node(keyboardDescriptor, sizeof(keyboardDescriptor));

    HID().AppendDescriptor(&node);
}

void SimpleKeyboard::setLocale(locale_t* locale) {
    this->locale = locale;
}

key_report SimpleKeyboard::makeReport(uint8_t modifiers, uint8_t key1, uint8_t key2, uint8_t key3, uint8_t key4, uint8_t key5, uint8_t key6) {
    key_report k;

    k.modifiers = modifiers;

    k.reserved = 0x00;

    k.keys[0] = key1;
    k.keys[1] = key2;
    k.keys[2] = key3;
    k.keys[3] = key4;
    k.keys[4] = key5;
    k.keys[5] = key6;

    return k;
}

void SimpleKeyboard::send(key_report* k) {
    HID().SendReport(2, k, sizeof(key_report));
}

void SimpleKeyboard::release() {
    prev_key_report = makeReport();
    send(&prev_key_report);
}

void SimpleKeyboard::pressKey(uint8_t key, uint8_t modifiers) {
    for (uint8_t i = 0; i<6; ++i) {
        if (prev_key_report.keys[i] == KEY_NONE) {
            prev_key_report.modifiers |= modifiers;
            prev_key_report.keys[i]    = key;
            send(&prev_key_report);
            return;
        }
    }
}

void SimpleKeyboard::pressModifier(uint8_t key) {
    prev_key_report.modifiers |= key;

    send(&prev_key_report);
}

uint8_t SimpleKeyboard::press(const char* strPtr) {
    // Convert string pointer into a byte pointer
    uint8_t* b = (uint8_t*)strPtr;

    // ASCII
    if (b[0] < locale->ascii_len) {
        uint8_t modifiers = pgm_read_byte(locale->ascii + (b[0] * 2) + 0);
        uint8_t key       = pgm_read_byte(locale->ascii + (b[0] * 2) + 1);

        pressKey(key, modifiers);

        return 0;
    }

    // Extended ASCII
    for (uint8_t i = 0; i<locale->extended_ascii_len; ++i) {
        uint8_t key_code = pgm_read_byte(locale->extended_ascii + (i * 3));

        if (b[0] == key_code) {
            uint8_t modifiers = pgm_read_byte(locale->extended_ascii + (i * 3) + 1);
            uint8_t key       = pgm_read_byte(locale->extended_ascii + (i * 3) + 2);

            pressKey(key, modifiers);

            return 0;
        }
    }

    // UTF8
    for (size_t i = 0; i<locale->utf8_len; ++i) {
        uint8_t res = 0;

        // Read utf8 code and match it with the given data
        for (uint8_t j = 0; j<4; ++j) {
            uint8_t key_code = pgm_read_byte(locale->utf8 + (i * 6) + j);

            if (key_code == 0) break;

            if (key_code == b[j]) {
                ++res;
            } else {
                res = 0;
                break;
            }
        }

        // Found a match was found, read out the data and type it
        if (res > 0) {
            uint8_t modifiers = pgm_read_byte(locale->utf8 + (i * 6) + 4);
            uint8_t key       = pgm_read_byte(locale->utf8 + (i * 6) + 5);

            pressKey(key, modifiers);

            // Return the number of extra bytes we used from the string pointer
            return res-1;
        }
    }

    return 0;
}

uint8_t SimpleKeyboard::write(const char* c) {
    uint8_t res = press(c);

    release();

    return res;
}

void SimpleKeyboard::write(const char* str, size_t len) {
    for (size_t i = 0; i<len; ++i) {
        i += write(&str[i]);
    }
}