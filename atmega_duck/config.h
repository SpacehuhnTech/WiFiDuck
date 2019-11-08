/*
   Copyright (c) 2019 Stefan Kremser
   This software is licensed under the MIT License. See the license file for details.
   Source: github.com/spacehuhn/WiFiDuck
 */

#pragma once

/* ===== Serial Bridge ===== */
// #define BRIDGE_ENABLE
// #define BRIDGE_PORT Serial1
// #define BRIDGE_SWITCH 6
// #define BRIDGE_RST 4
// #define BRIDGE_0 5

/*! DEBUG Settings */
// #define ENABLE_DEBUG
// #define DEBUG_PORT Serial
// #define DEBUG_BAUD 115200

/*! ===== Communication Settings ===== */
// #define ENABLE_SERIAL
#define SERIAL_COM Serial1
#define SERIAL_BAUD 9600

// #define ENABLE_I2C
#define I2C_ADDR 0x31

#define BUFFER_SIZE 512
#define PACKET_SIZE 32

/*! ===== LED Settings ===== */
// #define ENABLE_LED
// #define LED_PIN 7
// #define NEOPIXEL
#define NEOPIXEL_NUM 1

/*! ===== Parser Settings ===== */
#define CASE_SENSETIVE false
#define DEFAULT_SLEEP 5

/*! ========== Safety Checks ========= */
#if defined(DUCKMCU) && !(DUCKMCU==ATMEGA32U4)
#error You're compiling for the wrong board, mate!\
    Select something with an ATmega32u4 or SAMD21.
#endif /* ifdef DUCKMCU && DUCKMCU!="ATMEGA32U4" */

#if !defined(ENABLE_I2C) && !defined(ENABLE_SERIAL)
#define ENABLE_I2C
#endif /* if !defined(ENABLE_I2C) && !defined(ENABLE_SERIAL) */

#if defined(BRIDGE_ENABLE) && !defined(ENABLE_SERIAL)
#warning Serial bridge enabled, but serial communication disabled. Enabling serial again...
#define ENABLE_SERIAL
#endif /* if defined(BRIDGE_ENABLE) */

#if defined(ENABLE_LED)

#if defined(ENABLE_I2C) && (LED_PIN==2 || LED_PIN==3)
#error LED pin overlaps with I2C pins, disable I2C or change the LED pin!
#endif /* if defined(ENABLE_I2C) && (LED_PIN==2 || LED_PIN==3) */

#if defined(ENABLE_SERIAL) && (LED_PIN==0 || LED_PIN==1)
#error LED pin overlaps with serial pins, disable serial or change the LED pin!
#endif /* if defined(ENABLE_SERIAL) && (LED_PIN==0 || LED_PIN==1) */

#if defined(BRIDGE_ENABLE) && (LED_PIN==BRIDGE_RST || LED_PIN==BRIDGE_0 || LED_PIN==BRIDGE_SWITCH)
#error LED pin overlaps with serial bridge pins, disable serial bridge or change the LED pin!
#endif /* if defined(BRIDGE_ENABLE) && (LED_PIN==BRIDGE_RST || LED_PIN==BRIDGE_0) */

#endif /* if defined(NEOPIXEL) */
