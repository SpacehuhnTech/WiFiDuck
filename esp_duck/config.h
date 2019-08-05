/*
   Copyright (c) 2019 Stefan Kremser
   This software is licensed under the MIT License. See the license file for details.
   Source: github.com/spacehuhn/WiFiDuck
 */

#pragma once

// DEBUG Settings
#define DEBUG
#define DEBUG_BAUD 115200

// WiFi Settings
#define WIFI_SSID "Duck"
#define WIFI_PASSWORD "quackquack"

// Connection Settings
#define NUMBER_CONNECTION_TRIES 1
#define CONNECTION_DELAY 5000

// I2C Settings
#define I2C_ADDR 0x31
#define I2C_SDA 4
#define I2C_SCL 5
#define I2C_CLOCK_SPEED 100000L

#define BUFFER_SIZE 512
#define PACKET_SIZE 32

// Test Settings
// #define ENABLE_DEFAULT_DELAY
// #define LED_TEST
// #define OPEN_NOTEPAD
// #define HELLO_WORLD
// define DELAY_TEST
// #define REPEAT_TEST
// #define US_CHAR_TEST
// #define DE_CHAR_TEST
// #define GB_CHAR_TEST
// #define OVERFLOW_TEST
// #define KEYCODE_TEST
// #define CLOSE_NOTEPAD