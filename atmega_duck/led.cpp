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

#else // Use onboard LED, or LED_PIN if defined
#include <Arduino.h>
namespace led {
  #if defined(LED_PIN)
    int ledPin = LED_PIN;
  #else
    int ledPin = LED_BUILTIN;
  #endif 
    void begin() {
      pinMode(ledPin, OUTPUT);
    }

    void setColor(int r, int g, int b) {
      // On unless all colours are below 50 (about 20%)
      if (r > 50 && g > 50 && b > 50) {
        digitalWrite(ledPin, HIGH);
      } else {
        digitalWrite(ledPin, LOW);
      }
    }
}
#endif // if defined(NEOPIXEL)