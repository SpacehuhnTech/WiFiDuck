/*
   This software is licensed under the MIT License. See the license file for details.
   Source: https://github.com/spacehuhntech/WiFiDuck
 */

#pragma once

#define VERSION "1.0.4"

/* ===== Serial Bridge ===== */
// #define BRIDGE_ENABLE
// #define BRIDGE_PORT Serial1
// #define BRIDGE_SWITCH 6
// #define BRIDGE_RST 4
// #define BRIDGE_0 5
// #define BRIDGE_0_INVERTED

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

#define BUFFER_SIZE 384
#define PACKET_SIZE 32

/*! ===== LED Settings ===== */
// #define NEOPIXEL
// #define NEOPIXEL_NUM 1
// #define LED_PIN 7

// #define DOTSTAR
// #define DOTSTAR_NUM 1
// #define DOTSTAR_DI 7
// #define DOTSTAR_CI 8

/*! ===== Parser Settings ===== */
#define CASE_SENSETIVE false
#define DEFAULT_SLEEP 5

/*! ========== Safety Checks ========= */
#if !defined(ENABLE_I2C) && !defined(ENABLE_SERIAL)
#define ENABLE_I2C
#endif /* if !defined(ENABLE_I2C) && !defined(ENABLE_SERIAL) */

#if defined(BRIDGE_ENABLE) && !defined(ENABLE_SERIAL)
#warning Serial bridge enabled, but serial communication disabled. Enabling serial again...
#define ENABLE_SERIAL
#endif /* if defined(BRIDGE_ENABLE) */

#if defined(BRIDGE_ENABLE)

  #if !defined(BRIDGE_PORT)
  #error Serial bridge port not defined!
  #endif /* if !defined(BRIDGE_PORT) */

  #if !defined(BRIDGE_SWITCH)
  #error Serial bridge button not defined!
  #endif /* if !defined(BRIDGE_SWITCH) */

  #if !defined(BRIDGE_RST)
  #error Serial bridge reset not defined!
  #endif /* if !defined(BRIDGE_RST) */

  #if !defined(BRIDGE_0)
  #error Serial bridge GPIO-0 not defined!
  #endif /* if !defined(BRIDGE_0) */

#endif   /* if defined(BRIDGE_ENABLE) */

#if defined(NEOPIXEL)

  #if defined(ENABLE_I2C) && (LED_PIN==2 || LED_PIN==3)
  #error Neopixel pin overlaps with I2C pins, disable I2C or change the LED pin!
  #endif /* if defined(ENABLE_I2C) && (LED_PIN==2 || LED_PIN==3) */

  #if defined(ENABLE_SERIAL) && (LED_PIN==0 || LED_PIN==1)
  #error Neopixel pin overlaps with serial pins, disable serial or change the LED pin!
  #endif /* if defined(ENABLE_SERIAL) && (LED_PIN==0 || LED_PIN==1) */

  #if defined(BRIDGE_ENABLE) && (LED_PIN==BRIDGE_RST || LED_PIN==BRIDGE_0 || LED_PIN==BRIDGE_SWITCH)
  #error Neopixel pin overlaps with serial bridge pins, disable serial bridge or change the LED pin!
  #endif /* if defined(BRIDGE_ENABLE) && (LED_PIN==BRIDGE_RST || LED_PIN==BRIDGE_0) */

  #if defined(NEOPIXEL) && !defined(NEOPIXEL_NUM)
  #define NEOPIXEL_NUM 1
  #endif /* if defined(NEOPIXEL) && !defined(NEOPIXEL_NUM) */

#elif defined(DOTSTAR)

  #if !defined(DOTSTAR_DI)
  #error Dotstar DI pin not set!
  #endif /* if !defined(DOTSTAR_DI) */

  #if !defined(DOTSTAR_CI)
  #error Dotstar CI pin not set!
  #endif /* if !defined(DOTSTAR_CI) */

// DI
  #if defined(ENABLE_I2C) && (DOTSTAR_DI==2 || DOTSTAR_DI==3)
  #error Dotstar DI pin overlaps with I2C pins, disable I2C or change the LED pin!
  #endif /* if defined(ENABLE_I2C) && (DOTSTAR_DI==2 || DOTSTAR_DI==3) */

  #if defined(ENABLE_SERIAL) && (DOTSTAR_DI==0 || DOTSTAR_DI==1)
  #error Dotstar DI pin overlaps with serial pins, disable serial or change the LED pin!
  #endif /* if defined(ENABLE_SERIAL) && (DOTSTAR_DI==0 || DOTSTAR_DI==1) */

  #if defined(BRIDGE_ENABLE) && (DOTSTAR_DI==BRIDGE_RST || DOTSTAR_DI==BRIDGE_0 || DOTSTAR_DI==BRIDGE_SWITCH)
  #error Dotstar DI pin overlaps with serial bridge pins, disable serial bridge or change the LED pin!
  #endif /* if defined(BRIDGE_ENABLE) && (DOTSTAR_DI==BRIDGE_RST || DOTSTAR_DI==BRIDGE_0 || DOTSTAR_DI==BRIDGE_SWITCH) */

// CI
  #if defined(ENABLE_I2C) && (DOTSTAR_CI==2 || DOTSTAR_CI==3)
  #error Dotstar CI pin overlaps with I2C pins, disable I2C or change the LED pin!
  #endif /* if defined(ENABLE_I2C) && (DOTSTAR_CI==2 || DOTSTAR_CI==3) */

  #if defined(ENABLE_SERIAL) && (DOTSTAR_CI==0 || DOTSTAR_CI==1)
  #error Dotstar CI pin overlaps with serial pins, disable serial or change the LED pin!
  #endif /* if defined(ENABLE_SERIAL) && (DOTSTAR_CI==0 || DOTSTAR_CI==1) */

  #if defined(BRIDGE_ENABLE) && (DOTSTAR_CI==BRIDGE_RST || DOTSTAR_CI==BRIDGE_0 || DOTSTAR_CI==BRIDGE_SWITCH)
  #error Dotstar CI pin overlaps with serial bridge pins, disable serial bridge or change the LED pin!
  #endif /* if defined(BRIDGE_ENABLE) && (DOTSTAR_CI==BRIDGE_RST || DOTSTAR_CI==BRIDGE_0 || DOTSTAR_CI==BRIDGE_SWITCH) */

  #if defined(DOTSTAR) && !defined(DOTSTAR_NUM)
  #define DOTSTAR_NUM 1
  #endif /* if defined(DOTSTAR) && !defined(DOTSTAR_NUM) */

#endif   /* if defined(NEOPIXEL) */