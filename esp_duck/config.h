/*
   This software is licensed under the MIT License. See the license file for details.
   Source: https://github.com/spacehuhntech/WiFiDuck
 */

#pragma once

#define VERSION "1.0.4"

/*! ===== DEBUG Settings ===== */
// #define ENABLE_DEBUG
// #define DEBUG_PORT Serial
// #define DEBUG_BAUD 115200

/*! ===== Communication Settings ===== */
// #define ENABLE_SERIAL
#define SERIAL_PORT Serial
#define SERIAL_BAUD 9600

// #define ENABLE_I2C
#define I2C_ADDR 0x31
// #define I2C_SDA 4
// #define I2C_SCL 5
#define I2C_CLOCK_SPEED 100000L

#define BUFFER_SIZE 384
#define PACKET_SIZE 32

#define MSG_CONNECTED "LED 0 0 25\n"
#define MSG_STARTED "LED 0 25 0\n"

/*! ======EEPROM Settings ===== */
#define EEPROM_SIZE       4095
#define EEPROM_BOOT_ADDR  3210
#define BOOT_MAGIC_NUM    1234567890

/*! ===== WiFi Settings ===== */
#define WIFI_SSID "wifiduck"
#define WIFI_PASSWORD "wifiduck"
#define WIFI_CHANNEL "1"

#define HOSTNAME "wifiduck"
#define URL "wifi.duck"

/*! ========== Safty checks ========== */
#if !defined(ENABLE_I2C) && !defined(ENABLE_SERIAL)
  #define ENABLE_I2C
  #define I2C_SDA 4
  #define I2C_SCL 5
#endif /* if !defined(ENABLE_I2C) || !defined(ENABLE_SERIAL) */

#if !defined(ESP8266)
#error You are compiling for the wrong board, mate! Select something with an ESP8266.
#endif /* ifdef DUCKMCU && DUCKMCU!="ATMEGA32U4" */

#if defined(ENABLE_DEBUG) && defined(ENABLE_SERIAL) && DEBUG_PORT == SERIAL_PORT
#error Using same serial port for debugging and Communication!\
    Use I2C instead or disable debug.
#endif /* if DEBUG_PORT == SERIAL_PORT */

#if defined(ENABLE_I2C) && I2C_SDA==I2C_SCL
#error SDA pin equals to SCL pin
#endif /* if !defined(ENABLE_I2C) && !defined(ENABLE_I2C) */

#if defined(ENABLE_I2C) && defined(ENABLE_SERIAL) && (I2C_SDA==1 || I2C_SDA==3 || I2C_SCL==1 || I2C_SCL==3)
#error I2C pins overlap with RX and TX pins. Disable serial debugging or change the I2C pins.
#endif /* if !defined(ENABLE_I2C) && !defined(ENABLE_I2C) */