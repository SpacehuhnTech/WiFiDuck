/*
   Copyright (c) 2019 Stefan Kremser
   This software is licensed under the MIT License. See the license file for details.
   Source: github.com/spacehuhn/SimpleCLI
 */

#ifndef DuckyParser_h
#define DuckyParser_h

#include "SimpleKeyboard.h"

class DuckyParser {
    private:
        bool inString  = false;
        bool inComment = false;

        int defaultDelay = 5;
        int repeatNum    = 0;

        unsigned long interpretTime  = 0;
        unsigned long sleepStartTime = 0;
        unsigned long sleepTime      = 0;

        void type(const char* str, size_t len);
        void press(const char* str, size_t len);
        void release();

        unsigned int toInt(const char* str, size_t len);

        void sleep(unsigned long delayTime);

        SimpleKeyboard keyboard;

    public:
        void parse(char* str, size_t len);
        int getRepeats();
        unsigned int getDelayTime();
};

#endif /* ifndef DuckyParser_h */