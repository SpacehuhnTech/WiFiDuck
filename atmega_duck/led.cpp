/*
   This software is licensed under the MIT License. See the license file for details.
   Source: https://github.com/spacehuhntech/WiFiDuck
 */

#include "led.h"

#include "config.h"

#if defined(NEOPIXEL)

#include "NeoPixel.h"

namespace led {
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
}

#elif defined(DOTSTAR)

#include "Adafruit_DotStar.h"

namespace led {
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
}

#elif defined(LED_RGB)

namespace led {
    #include <Arduino.h>
    
    void begin() {
        pinMode(LED_R, OUTPUT);
        pinMode(LED_G, OUTPUT);
        pinMode(LED_B, OUTPUT);
    }

    void setColor(int r, int g, int b) {
#ifdef LED_ANODE
        r = 255 - r;
        g = 255 - g;
        b = 255 - b;
#endif

        analogWrite(LED_R, r);
        analogWrite(LED_G, g);
        analogWrite(LED_B, b);
    }
}

#else // if defined(NEOPIXEL)

namespace led {
    void begin() {}

    void setColor(int r, int g, int b) {}
}

#endif // if defined(NEOPIXEL)