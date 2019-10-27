/*
   Copyright (c) 2019 Stefan Kremser
   This software is licensed under the MIT License. See the license file for details.
   Source: github.com/spacehuhn/WiFiDuck
 */

#pragma once

/*! DEBUG Settings */
#define ENABLE_DEBUG
#define DEBUG_PORT Serial
#define DEBUG_BAUD 115200

/*! Communication Settings */
// #define ENABLE_SERIAL
#define SERIAL_PORT Serial
#define SERIAL_BAUD 9600

#define ENABLE_I2C
#define I2C_ADDR 0x31
#define I2C_SDA 4
#define I2C_SCL 5
#define I2C_CLOCK_SPEED 100000L

#define BUFFER_SIZE 512
#define PACKET_SIZE 32

/*! EEPROM Settings */
#define EEPROM_SIZE       4095
#define EEPROM_BOOT_ADDR  3210
#define BOOT_MAGIC_NUM    1234567890

/*! WiFi Settings */
#define WIFI_SSID "wifiduck"
#define WIFI_PASSWORD "wifiduck"
#define WIFI_CHANNEL "1"

/*! Connection Settings */
#define NUMBER_CONNECTION_TRIES 1
#define CONNECTION_DELAY 5000

/*! Safty checks */
#if defined(ENABLE_DEBUG) && defined(ENABLE_SERIAL) && DEBUG_PORT == SERIAL_PORT
#error Using same serial port for debugging and Communication!\
    Comment out ENABLE_DEBUG or ENABLE_SERIAL in esp_duck/config.h!
#endif /* if DEBUG_PORT == SERIAL_PORT */