/*
   Copyright (c) 2019 Stefan Kremser
   This software is licensed under the MIT License. See the license file for details.
   Source: github.com/spacehuhn/WiFiDuck
 */

// ===== Settings ===== //
#define DEBUG         // Enable serial debugging output

#define I2C_ADDR 0x31 // I2C address this (slave) device will listen to

#define BUFFER_SIZE 512

#define RESPONSE_OK 0x00
#define RESPONSE_PROCESSING 0x01
#define RESPONSE_REPEAT 0x02

// ===== Libraries ===== //
#include <Wire.h>        // I2C
#include "DuckyParser.h" // Ducky Script language Interpreter
#include "locales.h"

// ===== Types ===== //
typedef struct buffer_t {
    char   data[BUFFER_SIZE];
    size_t len;
} buffer_t;

// ===== Global Variables ===== //
DuckyParser ducky;

bool processing = false;

buffer_t mainBuffer;

// ===== Global Functions ===== //
// I2C Request
void requestEvent() {
#ifdef DEBUG
    Serial.println("I2C REQUEST");
#endif // ifdef DEBUG
    if (processing) {
        Wire.write(ducky.getDelayTime() | RESPONSE_PROCESSING);
    } else {
        processing = mainBuffer.len > 0;

        if (processing) {
            Wire.write(ducky.getDelayTime() | RESPONSE_PROCESSING);
        } else if (ducky.getRepeats() > 0) {
            Wire.write(RESPONSE_REPEAT);
#ifdef DEBUG
            Serial.println("I2C REPEAT");
#endif // ifdef DEBUG
        } else {
            Wire.write(RESPONSE_OK);
#ifdef DEBUG
            Serial.println("Done");
#endif // ifdef DEBUG
        }
    }
}

// I2C Receive
void receiveEvent(int len) {
#ifdef DEBUG
    Serial.println("RECEIVE");
#endif // ifdef DEBUG
    if (mainBuffer.len + (unsigned int)len <= BUFFER_SIZE) {
#ifdef DEBUG
        Serial.println("Received packet");
#endif // ifdef DEBUG
        Wire.readBytes(&mainBuffer.data[mainBuffer.len], len);
        mainBuffer.len += len;
    } else {
#ifdef DEBUG
        Serial.println("!!! Buffer is full !!!");
#endif // ifdef DEBUG
    }
}

// ===== SETUP ====== //
void setup() {
#ifdef DEBUG
    Serial.begin(115200);

    while (!Serial);
    Serial.println("Started!");
#endif // ifdef DEBUG

    Wire.begin(I2C_ADDR); // Start I2C

    // Set I2C events
    Wire.onRequest(requestEvent);
    Wire.onReceive(receiveEvent);
}

// ===== LOOOP ===== //
void loop() {
    if (processing) {
#ifdef DEBUG
        Serial.print("Interpreting: ");

        for (size_t i = 0; i<mainBuffer.len; i++) Serial.print(mainBuffer.data[i]);
#endif // ifdef DEBUG

        ducky.parse(mainBuffer.data, mainBuffer.len);
        mainBuffer.len = 0;
        processing     = false;
    }
}