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

void SimpleKeyboard::setLocale(uint8_t* locale) {
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

void SimpleKeyboard::pressKey(uint8_t key) {
    for (uint8_t i = 0; i<6; ++i) {
        if (prev_key_report.keys[i] == KEY_NONE) {
            prev_key_report.keys[i] = key;
            send(&prev_key_report);
            return;
        }
    }
}

void SimpleKeyboard::pressModifier(uint8_t key) {
    prev_key_report.modifiers |= key;

    send(&prev_key_report);
}

void SimpleKeyboard::press(char c) {
    for (uint8_t i = 0; i<6; ++i) {
        if (prev_key_report.keys[i] == KEY_NONE) {
            prev_key_report.modifiers |= pgm_read_byte(locale + ((int)c * 2));
            prev_key_report.keys[i]    = pgm_read_byte(locale + ((int)c * 2) + 1);
            send(&prev_key_report);
            return;
        }
    }
}

void SimpleKeyboard::write(char c) {
    press(c);
    release();
}

void SimpleKeyboard::write(const char* str, size_t len) {
    for (size_t i = 0; i<len; ++i) {
        write(str[i]);
    }
}