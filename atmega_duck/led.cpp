/*
   Copyright (c) 2019 Stefan Kremser
   This software is licensed under the MIT License. See the license file for details.
   Source: github.com/spacehuhn/WiFiDuck
 */

#include "led.h"

#include "NeoPixel.h"
#include "Adafruit_DotStar.h"
#include "config.h"

namespace led {
#if defined(NEOPIXEL)
    NeoPixel led { NEOPIXEL_NUM, LED_PIN, NEO_GRB + NEO_KHZ800 };

    void begin() {
        led.begin();
        led.show();
    }

    void setColor(int r, int g, int b) {
        for (size_t i = 0; i<led.numPixels(); i++) {
            led.setPixelColor(i, r, g, b);
        }

        led.show();
    }

#elif defined(DOTSTAR)
    Adafruit_DotStar led { DOTSTAR_NUM, DOTSTAR_DI, DOTSTAR_CI, DOTSTAR_BGR };

    void begin() {
        led.begin();
        led.show();
    }

    void setColor(int r, int g, int b) {
        for (size_t i = 0; i<led.numPixels(); i++) {
            led.setPixelColor(i, r, g, b);
        }

        led.show();
    }

#else // if defined(NEOPIXEL)
    void begin() {}

    void setColor(int r, int g, int b) {}

#endif // if defined(NEOPIXEL)
}