#pragma once

#include <cstddef>
#include <cstdint>
#include <cstring>
#include <string>

#define PROGMEM
#define pgm_read_byte(address) (*(const uint8_t*)(address))
#define _min(a, b) ((a) < (b) ? (a) : (b))

using String = std::string;
extern unsigned long testTime;
inline unsigned long millis() { return testTime; }
inline void delay(unsigned long time) { testTime += time; }
inline void noInterrupts() {}
inline void interrupts() {}
