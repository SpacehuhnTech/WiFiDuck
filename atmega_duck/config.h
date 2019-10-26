/*
   Copyright (c) 2019 Stefan Kremser
   This software is licensed under the MIT License. See the license file for details.
   Source: github.com/spacehuhn/WiFiDuck
 */

#pragma once

// DEBUG Settings
#define ENABLE_DEBUG
#define DEBUG_PORT Serial
#define DEBUG_BAUD 115200

// Communication Settings
#define ENABLE_SERIAL
#define ENABLE_I2C

#define I2C_ADDR 0x31

#define SERIAL_COM Serial1
#define SERIAL_BAUD 9600

#define BUFFER_SIZE 512
#define PACKET_SIZE 32

// LED Settings
#define NEOPIXEL
#define NEOPIXEL_PIN 7
#define NEOPIXEL_NUM 1

// Parser Settings
#define CASE_SENSETIVE false
#define DEFAULT_SLEEP 5