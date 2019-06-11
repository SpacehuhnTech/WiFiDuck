/*
   Copyright (c) 2019 Stefan Kremser
   This software is licensed under the MIT License. See the license file for details.
   Source: github.com/spacehuhn/WiFiDuck
 */

#include "SimpleKeyboard.h"

SimpleKeyboard::SimpleKeyboard() {
    static HIDSubDescriptor node(keyboardDescriptor, sizeof(keyboardDescriptor));

    HID().AppendDescriptor(&node);

    release_key_report = makeReport();
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

void SimpleKeyboard::press(key_report* k) {
    HID().SendReport(2, k, sizeof(key_report));
}

void SimpleKeyboard::release() {
    press(&release_key_report);
}