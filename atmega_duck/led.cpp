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

#else // if defined(NEOPIXEL)

namespace led {
    void begin() {}

    void setColor(int r, int g, int b) {}
}

#endif // if defined(NEOPIXEL)