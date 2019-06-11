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

extern "C" {
#include "parser.h"     // parse_lines
#include "comparator.h" // compare
}

// ===== Global Variables ===== //
uint8_t error = RES_CODE_OK;

char mainBuffer[BUFFER_SIZE];
char secondBuffer[BUFFER_SIZE];

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

void print(const char* str, size_t len) {
    for (size_t i = 0; i<len; ++i) Serial.print(str[i]);
}

void println(const char* str, size_t len) {
    print(str, len);
    Serial.println();
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
        // Split str into a list of lines
        line_list* l = parse_lines(secondBuffer, secondBufferSize);

        // Serial.print("Processing '");
        // print(secondBuffer, secondBufferSize);
        // Serial.println("'");

        // Go through all lines and try to find a matching command
        line_node* n = l->first;

        while (n) {
            word_list* wl        = n->words;
            word_node* cmd_name  = wl->first;
            word_node* first_arg = cmd_name->next;

            const char* line_str = cmd_name->str + cmd_name->len + 1;
            size_t line_str_len  = n->len - cmd_name->len - 1;

            char last_char = secondBuffer[secondBufferSize-1];
            bool line_end  = last_char == '\r' || last_char == '\n';

            // REM
            if (inComment || compare(cmd_name->str, cmd_name->len, "REM", true)) {
                if (inComment) {
                    print(n->str, n->len);
                } else {
                    print(line_str, line_str_len);
                }

                if (line_end) {
                    Serial.println();
                    inComment = false;
                } else {
                    inComment = true;
                }
            }

            // STRING
            else if (inString || compare(cmd_name->str, cmd_name->len, "STRING", true)) {
                if (inString) {
                    print(n->str, n->len);
                } else {
                    Serial.print("Type: '");
                    print(line_str, line_str_len);
                }

                if (line_end) {
                    Serial.println("'");
                    inString = false;
                } else {
                    inString = true;
                }
            }

            // DELAY
            else if (compare(cmd_name->str, cmd_name->len, "DELAY", true)) {
                Serial.print("Delay(");
                print(first_arg->str, first_arg->len);
                Serial.println("ms)");
            }

            // DEFAULTDELAY/DEFAULT_DELAY
            else if (compare(cmd_name->str, cmd_name->len, "DEFAULTDELAY", true) || compare(cmd_name->str, cmd_name->len, "DEFAULT_DELAY", true)) {
                Serial.print("DEFAULT_DELAY=");
                print(first_arg->str, first_arg->len);
                Serial.println("ms");
            }

            // REPEAT
            else if (compare(cmd_name->str, cmd_name->len, "REPEAT", true)) {
                Serial.print("REPEAT ");
                print(first_arg->str, first_arg->len);
                Serial.println(" times");
            }

            else {
                word_node* w = wl->first;

                while (w) {
                    Serial.print("Press '");
                    print(w->str, w->len);
                    Serial.println("'");
                    w = w->next;
                }

                if (line_end) Serial.println("Release");
            }

            n = n->next;
        }

        line_list_destroy(l);
        secondBufferSize = 0;
    }

    // Copy incoming buffer
    if (mainBufferSize > 0) {
        memcpy(secondBuffer, mainBuffer, mainBufferSize);
        secondBufferSize = mainBufferSize;
        mainBufferSize   = 0;
    }
}