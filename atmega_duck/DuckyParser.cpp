/*
   Copyright (c) 2019 Stefan Kremser
   This software is licensed under the MIT License. See the license file for details.
   Source: github.com/spacehuhn/SimpleCLI
 */

#include "DuckyParser.h"

#define CASE_SENSETIVE false

#define KEY_MENU 0xED
#define KEY_PAUSE 0xD4
#define KEY_NUM_LOCK 0xDB
#define KEY_PRINTSCREEN 0xCE
#define KEY_SCROLL_LOCK 0xCF

extern "C" {
 #include "parser.h"     // parse_lines
 #include "comparator.h" // compare
}

void DuckyParser::type(const char* str, size_t len) {
    Keyboard.write((uint8_t*)str, len);
}

void DuckyParser::press(const char* str, size_t len) {
    if (len == 1) Keyboard.press(str[0]);
    else if (compare(str, len, "ENTER", CASE_SENSETIVE)) Keyboard.press(KEY_RETURN);
    else if (compare(str, len, "WINDOWS", CASE_SENSETIVE) || compare(str, len, "GUI", CASE_SENSETIVE)) Keyboard.press(KEY_LEFT_GUI);
    else if (compare(str, len, "MENU", CASE_SENSETIVE) || compare(str, len, "APP", CASE_SENSETIVE)) Keyboard.press(KEY_MENU);
    else if (compare(str, len, "SHIFT", CASE_SENSETIVE)) Keyboard.press(KEY_LEFT_SHIFT);
    else if (compare(str, len, "DELETE", CASE_SENSETIVE)) Keyboard.press(KEY_DELETE);
    else if (compare(str, len, "HOME", CASE_SENSETIVE)) Keyboard.press(KEY_HOME);
    else if (compare(str, len, "INSERT", CASE_SENSETIVE)) Keyboard.press(KEY_INSERT);
    else if (compare(str, len, "PAGEUP", CASE_SENSETIVE)) Keyboard.press(KEY_PAGE_UP);
    else if (compare(str, len, "PAGEDOWN", CASE_SENSETIVE)) Keyboard.press(KEY_PAGE_DOWN);
    else if (compare(str, len, "UPARROW", CASE_SENSETIVE) || compare(str, len, "UP", CASE_SENSETIVE)) Keyboard.press(KEY_UP_ARROW);
    else if (compare(str, len, "DOWNARROW", CASE_SENSETIVE) || compare(str, len, "DOWN", CASE_SENSETIVE)) Keyboard.press(KEY_DOWN_ARROW);
    else if (compare(str, len, "LEFTARROW", CASE_SENSETIVE) || compare(str, len, "LEFT", CASE_SENSETIVE)) Keyboard.press(KEY_LEFT_ARROW);
    else if (compare(str, len, "RIGHTARROW", CASE_SENSETIVE) || compare(str, len, "RIGHT", CASE_SENSETIVE)) Keyboard.press(KEY_RIGHT_ARROW);
    else if (compare(str, len, "TAB", CASE_SENSETIVE)) Keyboard.press(KEY_TAB);
    else if (compare(str, len, "ALT", CASE_SENSETIVE)) Keyboard.press(KEY_LEFT_ALT);
    else if (compare(str, len, "END", CASE_SENSETIVE)) Keyboard.press(KEY_END);
    else if (compare(str, len, "ESC", CASE_SENSETIVE) || compare(str, len, "ESCAPE", CASE_SENSETIVE)) Keyboard.press(KEY_ESC);
    else if (compare(str, len, "F1", CASE_SENSETIVE)) Keyboard.press(KEY_F1);
    else if (compare(str, len, "F2", CASE_SENSETIVE)) Keyboard.press(KEY_F2);
    else if (compare(str, len, "F3", CASE_SENSETIVE)) Keyboard.press(KEY_F3);
    else if (compare(str, len, "F4", CASE_SENSETIVE)) Keyboard.press(KEY_F4);
    else if (compare(str, len, "F5", CASE_SENSETIVE)) Keyboard.press(KEY_F5);
    else if (compare(str, len, "F6", CASE_SENSETIVE)) Keyboard.press(KEY_F6);
    else if (compare(str, len, "F7", CASE_SENSETIVE)) Keyboard.press(KEY_F7);
    else if (compare(str, len, "F8", CASE_SENSETIVE)) Keyboard.press(KEY_F8);
    else if (compare(str, len, "F9", CASE_SENSETIVE)) Keyboard.press(KEY_F9);
    else if (compare(str, len, "F10", CASE_SENSETIVE)) Keyboard.press(KEY_F10);
    else if (compare(str, len, "F11", CASE_SENSETIVE)) Keyboard.press(KEY_F11);
    else if (compare(str, len, "F12", CASE_SENSETIVE)) Keyboard.press(KEY_F12);
    else if (compare(str, len, "SPACE", CASE_SENSETIVE)) Keyboard.press(' ');
    else if (compare(str, len, "CTRL", CASE_SENSETIVE) || compare(str, len, "CONTROL", CASE_SENSETIVE)) Keyboard.press(KEY_LEFT_CTRL);
    else if (compare(str, len, "PAUSE", CASE_SENSETIVE) || compare(str, len, "BREAK", CASE_SENSETIVE)) Keyboard.press(KEY_PAUSE);
    else if (compare(str, len, "CAPSLOCK", CASE_SENSETIVE)) Keyboard.press(KEY_CAPS_LOCK);
    else if (compare(str, len, "NUMLOCK", CASE_SENSETIVE)) Keyboard.press(KEY_NUM_LOCK);
    else if (compare(str, len, "PRINTSCREEN", CASE_SENSETIVE)) Keyboard.press(KEY_PRINTSCREEN);
    else if (compare(str, len, "SCROLLLOCK", CASE_SENSETIVE)) Keyboard.press(KEY_SCROLL_LOCK);
}

void DuckyParser::release() {
    Keyboard.releaseAll();
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

void DuckyParser::parse(char* str, size_t len) {
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

        // DELAY (-> sleep for x ms)
        else if (compare(cmd->str, cmd->len, "DELAY", CASE_SENSETIVE)) {
            delay(toInt(line_str, line_str_len));
        }

        // DEFAULTDELAY/DEFAULT_DELAY (set default delay per command)
        else if (compare(cmd->str, cmd->len, "DEFAULTDELAY", CASE_SENSETIVE) || compare(cmd->str, cmd->len, "DEFAULT_DELAY", CASE_SENSETIVE)) {
            defaultDelay = toInt(line_str, line_str_len);
            Serial.println("DEFAULTDELAY="+String(defaultDelay));
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

        if (!inString && !inComment) delay(defaultDelay);
    }

    line_list_destroy(l);

    if (repeatNum > 0) --repeatNum;
}

int DuckyParser::getRepeats() {
    return repeatNum;
}