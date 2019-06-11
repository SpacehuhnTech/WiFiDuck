/*
   Copyright (c) 2019 Stefan Kremser
   This software is licensed under the MIT License. See the license file for details.
   Source: github.com/spacehuhn/WiFiDuck
 */

// ===== Settings ===== //
#define I2C_ADDR 0x31 // I2C address this (slave) device will listen to

#define DEBUG         // Enable serial debugging output

#define MODE_MASK      0x78
#define MODE_KEYBOARD  0x00
#define MODE_MOUSE     0x10

// ===== Libraries ===== //
#include <Wire.h>           // I2C
#include "SimpleKeyboard.h" // USB HID Keyboard
#include "SimpleMouse.h"    // USB HID Mouse

// ===== Global Variables ===== //
uint8_t error = 0x00;       // Error Code

key_report   kr;            // Keyboard state
mouse_report mr;            // Mouse state

SimpleKeyboard keyboard;    // keyboard instance
SimpleMouse    mouse;       // Mouse instance

// ===== Global Functions ===== //
// I2C Request
void requestEvent() {
    Wire.write(error); // Reply with error code
    error = 0x00;      // Reset error code

    #ifdef DEBUG
    Serial.println("Replied to request");
    #endif // ifdef DEBUG
}

// I2C Receive
void receiveEvent(int len) {
    #ifdef DEBUG
    Serial.print("Received packet with length ");
    Serial.println(len);
    #endif // ifdef DEBUG

    // Write byte stream into a structure
    uint8_t mode = Wire.read();

    if (((mode & MODE_MASK) == MODE_KEYBOARD) && (len == 8)) {
        #ifdef DEBUG
        Serial.println("Mode = Keyboard");
        #endif // ifdef DEBUG

        // Copy data into keyboard report structure
        kr.modifiers = Wire.read();
        kr.keys[0]   = Wire.read();
        kr.keys[1]   = Wire.read();
        kr.keys[2]   = Wire.read();
        kr.keys[3]   = Wire.read();
        kr.keys[4]   = Wire.read();
        kr.keys[5]   = Wire.read();

        // Run it!
        keyboard.press(&kr);
        if (mode & 0x01) keyboard.release();
    } else if (((mode & MODE_MASK) == MODE_MOUSE) && (len == 5)) {
        #ifdef DEBUG
        Serial.println("Mode = Mouse");
        #endif // ifdef DEBUG

        // Copy data into mouse report structure
        mr.buttons = Wire.read();
        mr.x       = (int8_t)Wire.read();
        mr.y       = (int8_t)Wire.read();
        mr.scroll  = (int8_t)Wire.read();

        // Run it!
        mouse.move(&mr);
        if (mode & 0x01) mouse.release();
    } else {
        #ifdef DEBUG
        Serial.print("Unknown Mode ");
        Serial.println(mode, BIN);
        #endif // ifdef DEBUG
    }
}

void setup() {
    Serial.begin(115200);

    while (!Serial);

    #ifdef DEBUG
    Serial.println("Started!");
    #endif // ifdef DEBUG

    Wire.begin(I2C_ADDR); // Start I2C

    // Set I2C events
    Wire.onRequest(requestEvent);
    Wire.onReceive(receiveEvent);
}

void loop() {}