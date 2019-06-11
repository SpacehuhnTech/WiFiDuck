/*
   Copyright (c) 2019 Stefan Kremser
   This software is licensed under the MIT License. See the license file for details.
   Source: github.com/spacehuhn/WiFiDuck
 */

// ===== Settings ===== //
#define I2C_ADDR 0x31 // I2C address this (slave) device will listen to

// #define DEBUG         // Enable serial debugging output

#define BUFFER_SIZE 512

#define RES_CODE_OK 0x00
#define RES_CODE_ERROR 0x01

// ===== Libraries ===== //
#include <Wire.h>     // I2C
#include <Keyboard.h> // USB HID Keyboard

// ===== Types ===== //
typedef struct word_t {
    char  * begin;
    char  * end;
    uint8_t len;
} word_t;

// ===== Global Variables ===== //
uint8_t error = RES_CODE_OK;

uint8_t mainBuffer[BUFFER_SIZE];
uint8_t secondBuffer[BUFFER_SIZE];

unsigned int mainBufferSize   = 0;
unsigned int secondBufferSize = 0;

bool inString     = false;
bool inComment    = false;
int  defaultDelay = 5;
int  repeatNum    = 0;

// ===== Global Functions ===== //
// I2C Request
void requestEvent() {
    Wire.write(mainBufferSize);
    Wire.write(error);
    Wire.write(repeatNum);

    /*
       Wire.write(error); // Reply with error code
       error = 0x00;      // Reset error code
     */

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

    if ((unsigned int)len < BUFFER_SIZE - mainBufferSize) {
        Wire.readBytes(&mainBuffer[mainBufferSize], len);
        mainBufferSize += len;
    } else {
        error = RES_CODE_ERROR;

        #ifdef DEBUG
        Serial.println("Buffer is full!");
        #endif // ifdef DEBUG
    }
}

// ===== SETUP ====== //
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

// ===== LOOOP ===== //
void loop() {
    if (secondBufferSize > 0) {
        // Serial.println();

        for (unsigned int i = 0; i<secondBufferSize; i++) {
            Serial.print((char)secondBuffer[i]);
        }
        // Serial.println();

        secondBufferSize = 0;
    }

    // Copy incoming buffer
    if (mainBufferSize > 0) {
        memcpy(secondBuffer, mainBuffer, mainBufferSize);
        secondBufferSize = mainBufferSize;
        mainBufferSize   = 0;
    }
}