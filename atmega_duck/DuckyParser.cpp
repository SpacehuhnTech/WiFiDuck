/*
   Copyright (c) 2019 Stefan Kremser
   This software is licensed under the MIT License. See the license file for details.
   Source: github.com/spacehuhn/SimpleCLI
 */

#include "DuckyParser.h"

#define CASE_SENSETIVE false
#define DEFAULT_SLEEP 5

extern "C" {
 #include "parser.h"     // parse_lines
 #include "comparator.h" // compare
}

void DuckyParser::type(const char* str, size_t len) {
    keyboard.write(str, len);
}

void DuckyParser::press(const char* str, size_t len) {
    // character
    if (len == 1) keyboard.press(str);

    // Keys
    else if (compare(str, len, "ENTER", CASE_SENSETIVE)) keyboard.pressKey(KEY_ENTER);
    else if (compare(str, len, "MENU", CASE_SENSETIVE) || compare(str, len, "APP", CASE_SENSETIVE)) keyboard.pressKey(KEY_PROPS);
    else if (compare(str, len, "DELETE", CASE_SENSETIVE)) keyboard.pressKey(KEY_BACKSPACE);
    else if (compare(str, len, "HOME", CASE_SENSETIVE)) keyboard.pressKey(KEY_HOME);
    else if (compare(str, len, "INSERT", CASE_SENSETIVE)) keyboard.pressKey(KEY_INSERT);
    else if (compare(str, len, "PAGEUP", CASE_SENSETIVE)) keyboard.pressKey(KEY_PAGEUP);
    else if (compare(str, len, "PAGEDOWN", CASE_SENSETIVE)) keyboard.pressKey(KEY_PAGEDOWN);
    else if (compare(str, len, "UPARROW", CASE_SENSETIVE) || compare(str, len, "UP", CASE_SENSETIVE)) keyboard.pressKey(KEY_UP);
    else if (compare(str, len, "DOWNARROW", CASE_SENSETIVE) || compare(str, len, "DOWN", CASE_SENSETIVE)) keyboard.pressKey(KEY_DOWN);
    else if (compare(str, len, "LEFTARROW", CASE_SENSETIVE) || compare(str, len, "LEFT", CASE_SENSETIVE)) keyboard.pressKey(KEY_LEFT);
    else if (compare(str, len, "RIGHTARROW", CASE_SENSETIVE) || compare(str, len, "RIGHT", CASE_SENSETIVE)) keyboard.pressKey(KEY_RIGHT);
    else if (compare(str, len, "TAB", CASE_SENSETIVE)) keyboard.pressKey(KEY_TAB);
    else if (compare(str, len, "END", CASE_SENSETIVE)) keyboard.pressKey(KEY_END);
    else if (compare(str, len, "ESC", CASE_SENSETIVE) || compare(str, len, "ESCAPE", CASE_SENSETIVE)) keyboard.pressKey(KEY_ESC);
    else if (compare(str, len, "F1", CASE_SENSETIVE)) keyboard.pressKey(KEY_F1);
    else if (compare(str, len, "F2", CASE_SENSETIVE)) keyboard.pressKey(KEY_F2);
    else if (compare(str, len, "F3", CASE_SENSETIVE)) keyboard.pressKey(KEY_F3);
    else if (compare(str, len, "F4", CASE_SENSETIVE)) keyboard.pressKey(KEY_F4);
    else if (compare(str, len, "F5", CASE_SENSETIVE)) keyboard.pressKey(KEY_F5);
    else if (compare(str, len, "F6", CASE_SENSETIVE)) keyboard.pressKey(KEY_F6);
    else if (compare(str, len, "F7", CASE_SENSETIVE)) keyboard.pressKey(KEY_F7);
    else if (compare(str, len, "F8", CASE_SENSETIVE)) keyboard.pressKey(KEY_F8);
    else if (compare(str, len, "F9", CASE_SENSETIVE)) keyboard.pressKey(KEY_F9);
    else if (compare(str, len, "F10", CASE_SENSETIVE)) keyboard.pressKey(KEY_F10);
    else if (compare(str, len, "F11", CASE_SENSETIVE)) keyboard.pressKey(KEY_F11);
    else if (compare(str, len, "F12", CASE_SENSETIVE)) keyboard.pressKey(KEY_F12);
    else if (compare(str, len, "SPACE", CASE_SENSETIVE)) keyboard.pressKey(KEY_SPACE);
    else if (compare(str, len, "PAUSE", CASE_SENSETIVE) || compare(str, len, "BREAK", CASE_SENSETIVE)) keyboard.pressKey(KEY_PAUSE);
    else if (compare(str, len, "CAPSLOCK", CASE_SENSETIVE)) keyboard.pressKey(KEY_CAPSLOCK);
    else if (compare(str, len, "NUMLOCK", CASE_SENSETIVE)) keyboard.pressKey(KEY_NUMLOCK);
    else if (compare(str, len, "PRINTSCREEN", CASE_SENSETIVE)) keyboard.pressKey(KEY_SYSRQ);
    else if (compare(str, len, "SCROLLLOCK", CASE_SENSETIVE)) keyboard.pressKey(KEY_SCROLLLOCK);

    // Modifiers
    else if (compare(str, len, "CTRL", CASE_SENSETIVE) || compare(str, len, "CONTROL", CASE_SENSETIVE)) keyboard.pressModifier(KEY_MOD_LCTRL);
    else if (compare(str, len, "SHIFT", CASE_SENSETIVE)) keyboard.pressModifier(KEY_MOD_LSHIFT);
    else if (compare(str, len, "ALT", CASE_SENSETIVE)) keyboard.pressModifier(KEY_MOD_LALT);
    else if (compare(str, len, "WINDOWS", CASE_SENSETIVE) || compare(str, len, "GUI", CASE_SENSETIVE)) keyboard.pressModifier(KEY_MOD_LMETA);

    // Utf8 character
    else keyboard.press(str);
}

void DuckyParser::release() {
    keyboard.release();
}

int DuckyParser::toInt(const char* str, size_t len) {
    if (!str || (len == 0)) return 0;

    int val = 0;

    for (size_t i = 0; i < len; ++i) {
        if ((str[i] >= '0') && (str[i] <= '9')) {
            val = val * 10 + (str[i] - '0');
        }
    }

    return val;
}

void DuckyParser::sleep(unsigned long time) {
    unsigned long offset = millis() - interpretTime;

    if (time > offset) {
        sleepStartTime = millis();
        sleepTime      = time - offset;

        delay(sleepTime);
    }
}

void DuckyParser::parse(char* str, size_t len) {
    interpretTime = millis();

    // Split str into a list of lines
    line_list* l = parse_lines(str, len);

    // Go through all lines
    line_node* n = l->first;

    while (n) {
        word_list* wl  = n->words;
        word_node* cmd = wl->first;

        const char* line_str = cmd->str + cmd->len + 1;
        size_t line_str_len  = n->len - cmd->len - 1;

        char last_char = n->str[n->len];
        bool line_end  = last_char == '\r' || last_char == '\n';

        // REM (= Comment -> do nothing)
        if (inComment || compare(cmd->str, cmd->len, "REM", CASE_SENSETIVE)) {
            inComment = !line_end;
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

        // LOCALE (-> change keyboard layout)
        else if (compare(cmd->str, cmd->len, "LOCALE", CASE_SENSETIVE)) {
            word_node* w = cmd->next;

            if (compare(w->str, w->len, "US", CASE_SENSETIVE)) {
                keyboard.setLocale(&locale_us);
            } else if (compare(w->str, w->len, "DE", CASE_SENSETIVE)) {
                keyboard.setLocale(&locale_de);
            } else if (compare(w->str, w->len, "GB", CASE_SENSETIVE)) {
                keyboard.setLocale(&locale_gb);
            }
        }

        // DELAY (-> sleep for x ms)
        else if (compare(cmd->str, cmd->len, "DELAY", CASE_SENSETIVE)) {
            sleep(toInt(line_str, line_str_len));
        }

        // DEFAULTDELAY/DEFAULT_DELAY (set default delay per command)
        else if (compare(cmd->str, cmd->len, "DEFAULTDELAY", CASE_SENSETIVE) || compare(cmd->str, cmd->len, "DEFAULT_DELAY", CASE_SENSETIVE)) {
            defaultDelay = toInt(line_str, line_str_len);
        }

        // REPEAT (-> repeat last command n times)
        else if (compare(cmd->str, cmd->len, "REPEAT", CASE_SENSETIVE)) {
            repeatNum = toInt(line_str, line_str_len) + 1;
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

        if (!inString && !inComment) sleep(defaultDelay);

        if (line_end && (repeatNum > 0)) --repeatNum;

        interpretTime = millis();
    }

    line_list_destroy(l);
}

int DuckyParser::getRepeats() {
    return repeatNum;
}

unsigned int DuckyParser::getDelayTime() {
    unsigned long finishTime  = sleepStartTime + sleepTime;
    unsigned long currentTime = millis();

    if (currentTime > finishTime) {
        return DEFAULT_SLEEP;
    } else {
        unsigned long remainingTime = finishTime - currentTime;
        return remainingTime | DEFAULT_SLEEP;
    }
}