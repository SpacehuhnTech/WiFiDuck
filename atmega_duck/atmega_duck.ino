/*
   Copyright (c) 2019 Stefan Kremser
   This software is licensed under the MIT License. See the license file for details.
   Source: github.com/spacehuhn/WiFiDuck
 */

// ===== Settings ===== //
#define I2C_ADDR 0x31 // I2C address this (slave) device will listen to

#define DEBUG         // Enable serial debugging output

#define BUFFER_SIZE 256

#define ERROR_OK 0x00
#define ERROR_BUFFER 0x01

// ===== Libraries ===== //
#include <Wire.h>        // I2C
#include "DuckyParser.h" // Ducky Script language Interpreter

// ===== Types ===== //
typedef struct buffer_t {
    char   data[BUFFER_SIZE];
    size_t len;
} buffer_t;

// ===== Global Variables ===== //
DuckyParser ducky;

uint8_t error = ERROR_OK;

buffer_t mainBuffer;
buffer_t secondBuffer;

// ===== Global Functions ===== //
// I2C Request
void requestEvent() {
    Wire.write(mainBuffer.len);
    Wire.write(error);

    // Serial.println("Replied to request");
}

// I2C Receive
void receiveEvent(int len) {
    if ((unsigned int)len < BUFFER_SIZE - mainBuffer.len) {
        Wire.readBytes(&mainBuffer.data[mainBuffer.len], len);
        mainBuffer.len += len;
    } else {
        error = ERROR_BUFFER;
        // Serial.println("Buffer is full!");
    }
}

// ===== SETUP ====== //
void setup() {
    /*
       Serial.begin(115200);
       while (!Serial);
       Serial.println("Started!");
     */

    Wire.begin(I2C_ADDR); // Start I2C

    // Set I2C events
    Wire.onRequest(requestEvent);
    Wire.onReceive(receiveEvent);

    Keyboard.begin();
}

// ===== LOOOP ===== //
void loop() {
    if (secondBuffer.len > 0) {
        ducky.parse(secondBuffer.data, secondBuffer.len);
        secondBuffer.len = 0;
    }

    // Copy incoming buffer
    if (mainBuffer.len > 0) {
        memcpy(secondBuffer.data, mainBuffer.data, mainBuffer.len);
        secondBuffer.len = mainBuffer.len;
        mainBuffer.len   = 0;
    }
}