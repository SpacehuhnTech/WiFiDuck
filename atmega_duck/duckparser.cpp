/*
   Copyright (c) 2019 Stefan Kremser
   This software is licensed under the MIT License. See the license file for details.
   Source: github.com/spacehuhn/SimpleCLI
 */

#include "duckparser.h"

#include "config.h"
// #include "debug.h"
#include "keyboard.h"
#include "led.h"

extern "C" {
 #include "parser.h" // parse_lines
}

#define CASE_INSENSETIVE 0
#define CASE_SENSETIVE 1

namespace duckparser {
    // ====== PRIVATE ===== //
    bool inString  = false;
    bool inComment = false;

    int defaultDelay = 5;
    int repeatNum    = 0;

    unsigned long interpretTime  = 0;
    unsigned long sleepStartTime = 0;
    unsigned long sleepTime      = 0;

    void type(const char* str, size_t len) {
        keyboard::write(str, len);
    }

    void press(const char* str, size_t len) {
        // character
        if (len == 1) keyboard::press(str);

        // Keys
        else if (compare(str, len, "ENTER", CASE_SENSETIVE)) keyboard::pressKey(KEY_ENTER);
        else if (compare(str, len, "MENU", CASE_SENSETIVE) || compare(str, len, "APP", CASE_SENSETIVE)) keyboard::pressKey(KEY_PROPS);
        else if (compare(str, len, "DELETE", CASE_SENSETIVE)) keyboard::pressKey(KEY_DELETE);
        else if (compare(str, len, "BACKSPACE", CASE_SENSETIVE)) keyboard::pressKey(KEY_BACKSPACE);
        else if (compare(str, len, "HOME", CASE_SENSETIVE)) keyboard::pressKey(KEY_HOME);
        else if (compare(str, len, "INSERT", CASE_SENSETIVE)) keyboard::pressKey(KEY_INSERT);
        else if (compare(str, len, "PAGEUP", CASE_SENSETIVE)) keyboard::pressKey(KEY_PAGEUP);
        else if (compare(str, len, "PAGEDOWN", CASE_SENSETIVE)) keyboard::pressKey(KEY_PAGEDOWN);
        else if (compare(str, len, "UPARROW", CASE_SENSETIVE) || compare(str, len, "UP", CASE_SENSETIVE)) keyboard::pressKey(KEY_UP);
        else if (compare(str, len, "DOWNARROW", CASE_SENSETIVE) || compare(str, len, "DOWN", CASE_SENSETIVE)) keyboard::pressKey(KEY_DOWN);
        else if (compare(str, len, "LEFTARROW", CASE_SENSETIVE) || compare(str, len, "LEFT", CASE_SENSETIVE)) keyboard::pressKey(KEY_LEFT);
        else if (compare(str, len, "RIGHTARROW", CASE_SENSETIVE) || compare(str, len, "RIGHT", CASE_SENSETIVE)) keyboard::pressKey(KEY_RIGHT);
        else if (compare(str, len, "TAB", CASE_SENSETIVE)) keyboard::pressKey(KEY_TAB);
        else if (compare(str, len, "END", CASE_SENSETIVE)) keyboard::pressKey(KEY_END);
        else if (compare(str, len, "ESC", CASE_SENSETIVE) || compare(str, len, "ESCAPE", CASE_SENSETIVE)) keyboard::pressKey(KEY_ESC);
        else if (compare(str, len, "F1", CASE_SENSETIVE)) keyboard::pressKey(KEY_F1);
        else if (compare(str, len, "F2", CASE_SENSETIVE)) keyboard::pressKey(KEY_F2);
        else if (compare(str, len, "F3", CASE_SENSETIVE)) keyboard::pressKey(KEY_F3);
        else if (compare(str, len, "F4", CASE_SENSETIVE)) keyboard::pressKey(KEY_F4);
        else if (compare(str, len, "F5", CASE_SENSETIVE)) keyboard::pressKey(KEY_F5);
        else if (compare(str, len, "F6", CASE_SENSETIVE)) keyboard::pressKey(KEY_F6);
        else if (compare(str, len, "F7", CASE_SENSETIVE)) keyboard::pressKey(KEY_F7);
        else if (compare(str, len, "F8", CASE_SENSETIVE)) keyboard::pressKey(KEY_F8);
        else if (compare(str, len, "F9", CASE_SENSETIVE)) keyboard::pressKey(KEY_F9);
        else if (compare(str, len, "F10", CASE_SENSETIVE)) keyboard::pressKey(KEY_F10);
        else if (compare(str, len, "F11", CASE_SENSETIVE)) keyboard::pressKey(KEY_F11);
        else if (compare(str, len, "F12", CASE_SENSETIVE)) keyboard::pressKey(KEY_F12);
        else if (compare(str, len, "SPACE", CASE_SENSETIVE)) keyboard::pressKey(KEY_SPACE);
        else if (compare(str, len, "PAUSE", CASE_SENSETIVE) || compare(str, len, "BREAK", CASE_SENSETIVE)) keyboard::pressKey(KEY_PAUSE);
        else if (compare(str, len, "CAPSLOCK", CASE_SENSETIVE)) keyboard::pressKey(KEY_CAPSLOCK);
        else if (compare(str, len, "NUMLOCK", CASE_SENSETIVE)) keyboard::pressKey(KEY_NUMLOCK);
        else if (compare(str, len, "PRINTSCREEN", CASE_SENSETIVE)) keyboard::pressKey(KEY_SYSRQ);
        else if (compare(str, len, "SCROLLLOCK", CASE_SENSETIVE)) keyboard::pressKey(KEY_SCROLLLOCK);

        // Modifiers
        else if (compare(str, len, "CTRL", CASE_SENSETIVE) || compare(str, len, "CONTROL", CASE_SENSETIVE)) keyboard::pressModifier(KEY_MOD_LCTRL);
        else if (compare(str, len, "SHIFT", CASE_SENSETIVE)) keyboard::pressModifier(KEY_MOD_LSHIFT);
        else if (compare(str, len, "ALT", CASE_SENSETIVE)) keyboard::pressModifier(KEY_MOD_LALT);
        else if (compare(str, len, "WINDOWS", CASE_SENSETIVE) || compare(str, len, "GUI", CASE_SENSETIVE)) keyboard::pressModifier(KEY_MOD_LMETA);

        // Utf8 character
        else keyboard::press(str);
    }

    void release() {
        keyboard::release();
    }

    unsigned int toInt(const char* str, size_t len) {
        if (!str || (len == 0)) return 0;

        unsigned int val = 0;

        // HEX
        if ((len > 2) && (str[0] == '0') && (str[1] == 'x')) {
            for (size_t i = 2; i < len; ++i) {
                uint8_t b = str[i];

                if ((b >= '0') && (b <= '9')) b = b - '0';
                else if ((b >= 'a') && (b <= 'f')) b = b - 'a' + 10;
                else if ((b >= 'A') && (b <= 'F')) b = b - 'A' + 10;

                val = (val << 4) | (b & 0xF);
            }
        }
        // DECIMAL
        else {
            for (size_t i = 0; i < len; ++i) {
                if ((str[i] >= '0') && (str[i] <= '9')) {
                    val = val * 10 + (str[i] - '0');
                }
            }
        }

        return val;
    }

    void sleep(unsigned long time) {
        unsigned long offset = millis() - interpretTime;

        if (time > offset) {
            sleepStartTime = millis();
            sleepTime      = time - offset;

            delay(sleepTime);
        }
    }

    // ====== PUBLIC ===== //

    void parse(const char* str, size_t len) {
        interpretTime = millis();

        // Split str into a list of lines
        line_list* l = parse_lines(str, len);

        // Go through all lines
        line_node* n = l->first;

        // Flag, no default delay after this command
        bool ignore_delay;

        while (n) {
            ignore_delay = false;

            word_list* wl  = n->words;
            word_node* cmd = wl->first;

            const char* line_str = cmd->str + cmd->len + 1;
            size_t line_str_len  = n->len - cmd->len - 1;

            char last_char = n->str[n->len];
            bool line_end  = last_char == '\r' || last_char == '\n';

            // REM (= Comment -> do nothing)
            if (inComment || compare(cmd->str, cmd->len, "REM", CASE_SENSETIVE)) {
                inComment    = !line_end;
                ignore_delay = true;
            }

            // LOCALE (-> change keyboard layout)
            else if (compare(cmd->str, cmd->len, "LOCALE", CASE_SENSETIVE)) {
                word_node* w = cmd->next;

                if (compare(w->str, w->len, "US", CASE_INSENSETIVE)) {
                    keyboard::setLocale(&locale_us);
                } else if (compare(w->str, w->len, "DE", CASE_INSENSETIVE)) {
                    keyboard::setLocale(&locale_de);
                } else if (compare(w->str, w->len, "RU", CASE_INSENSETIVE)) {
                    keyboard::setLocale(&locale_ru);
                } else if (compare(w->str, w->len, "GB", CASE_INSENSETIVE)) {
                    keyboard::setLocale(&locale_gb);
                } else if (compare(w->str, w->len, "ES", CASE_INSENSETIVE)) {
                    keyboard::setLocale(&locale_es);
                } else if (compare(w->str, w->len, "FR", CASE_INSENSETIVE)) {
                    keyboard::setLocale(&locale_fr);
                } else if (compare(w->str, w->len, "DK", CASE_INSENSETIVE)) {
                    keyboard::setLocale(&locale_dk);
                } else if (compare(w->str, w->len, "BE", CASE_INSENSETIVE)) {
                    keyboard::setLocale(&locale_be);
                } else if (compare(w->str, w->len, "PT", CASE_INSENSETIVE)) {
                    keyboard::setLocale(&locale_pt);
                } else if (compare(w->str, w->len, "IT", CASE_INSENSETIVE)) {
                    keyboard::setLocale(&locale_it);
                } else if (compare(w->str, w->len, "SK", CASE_INSENSETIVE)) {
                    keyboard::setLocale(&locale_sk);
                } else if (compare(w->str, w->len, "CZ", CASE_INSENSETIVE)) {
                    keyboard::setLocale(&locale_cz);
                } else if (compare(w->str, w->len, "SI", CASE_INSENSETIVE)) {
                    keyboard::setLocale(&locale_si);
                } else if (compare(w->str, w->len, "BG", CASE_INSENSETIVE)) {
                    keyboard::setLocale(&locale_bg);
                } else if (compare(w->str, w->len, "CA-FR", CASE_INSENSETIVE)) {
                    keyboard::setLocale(&locale_cafr);
                } else if (compare(w->str, w->len, "CH-DE", CASE_INSENSETIVE)) {
                    keyboard::setLocale(&locale_chde);
                } else if (compare(w->str, w->len, "CH-FR", CASE_INSENSETIVE)) {
                    keyboard::setLocale(&locale_chfr);
                } else if (compare(w->str, w->len, "HU", CASE_INSENSETIVE)) {
                    keyboard::setLocale(&locale_hu);
                }
                
                ignore_delay = true;
            }

            // DELAY (-> sleep for x ms)
            else if (compare(cmd->str, cmd->len, "DELAY", CASE_SENSETIVE)) {
                sleep(toInt(line_str, line_str_len));
                ignore_delay = true;
            }

            // DEFAULTDELAY/DEFAULT_DELAY (set default delay per command)
            else if (compare(cmd->str, cmd->len, "DEFAULTDELAY", CASE_SENSETIVE) || compare(cmd->str, cmd->len, "DEFAULT_DELAY", CASE_SENSETIVE)) {
                defaultDelay = toInt(line_str, line_str_len);
                ignore_delay = true;
            }

            // REPEAT (-> repeat last command n times)
            else if (compare(cmd->str, cmd->len, "REPEAT", CASE_SENSETIVE) || compare(cmd->str, cmd->len, "REPLAY", CASE_SENSETIVE)) {
                repeatNum    = toInt(line_str, line_str_len) + 1;
                ignore_delay = true;
            }

            // STRING (-> type each character)
            else if (inString || compare(cmd->str, cmd->len, "STRING", CASE_SENSETIVE)) {
                if (inString) {
                    type(n->str, n->len);
                } else {
                    type(line_str, line_str_len);
                }

                inString = !line_end;
            }

            // LED
            else if (compare(cmd->str, cmd->len, "LED", CASE_SENSETIVE)) {
                word_node* w = cmd->next;

                int c[3];

                for (uint8_t i = 0; i<3; ++i) {
                    if (w) {
                        c[i] = toInt(w->str, w->len);
                        w    = w->next;
                    } else {
                        c[i] = 0;
                    }
                }

                led::setColor(c[0], c[1], c[2]);
            }

            // KEYCODE
            else if (compare(cmd->str, cmd->len, "KEYCODE", CASE_SENSETIVE)) {
                word_node* w = cmd->next;
                if (w) {
                    keyboard::report k;

                    k.modifiers = (uint8_t)toInt(w->str, w->len);
                    k.reserved  = 0;
                    w           = w->next;

                    for (uint8_t i = 0; i<6; ++i) {
                        if (w) {
                            k.keys[i] = (uint8_t)toInt(w->str, w->len);
                            w         = w->next;
                        } else {
                            k.keys[i] = 0;
                        }
                    }

                    keyboard::send(&k);
                    keyboard::release();
                }
            }

            // Otherwise go through words and look for keys to press
            else {
                word_node* w = wl->first;

                while (w) {
                    press(w->str, w->len);
                    w = w->next;
                }

                if (line_end) release();
            }

            n = n->next;

            if (!inString && !inComment && !ignore_delay) sleep(defaultDelay);

            if (line_end && (repeatNum > 0)) --repeatNum;

            interpretTime = millis();
        }

        line_list_destroy(l);
    }

    int getRepeats() {
        return repeatNum;
    }

    unsigned int getDelayTime() {
        unsigned long finishTime  = sleepStartTime + sleepTime;
        unsigned long currentTime = millis();

        if (currentTime > finishTime) {
            return 0;
        } else {
            unsigned long remainingTime = finishTime - currentTime;
            return (unsigned int)remainingTime;
        }
    }
}
