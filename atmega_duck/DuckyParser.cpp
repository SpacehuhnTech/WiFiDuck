/*
   Copyright (c) 2019 Stefan Kremser
   This software is licensed under the MIT License. See the license file for details.
   Source: github.com/spacehuhn/SimpleCLI
 */

#include "DuckyParser.h"

extern "C" {
 #include "parser.h"     // parse_lines
 #include "comparator.h" // compare
}

void DuckyParser::type(const char* str, size_t len) {
    Keyboard.write(str, len);
}

void DuckyParser::press(const char* str, size_t len) {
    if (len == 1) Keyboard.press(str[0]);
    // else if (strcmp(str, "GUI") == 0) Keyboard.press(KEY_WINDOWS);
}

void DuckyParser::release() {
    Keyboard.releaseAll();
}

void DuckyParser::parse(char* str, size_t len) {
    // Split str into a list of lines
    line_list* l = parse_lines(str, len);

    // Go through all lines
    line_node* n = l->first;

    while (n) {
        word_list* wl        = n->words;
        word_node* cmd_name  = wl->first;
        word_node* first_arg = cmd_name->next;

        const char* line_str = cmd_name->str + cmd_name->len + 1;
        size_t line_str_len  = n->len - cmd_name->len - 1;

        char last_char = n->str[n->len];
        bool line_end  = last_char == '\r' || last_char == '\n';

        // REM (= Comment -> do nothing)
        if (inComment || compare(cmd_name->str, cmd_name->len, "REM", true)) {
            // for(size_t i=0; i<line_str_len; ++i) Serial.print(line_str[i]);
            // if (line_end) Serial.println();

            inComment = !line_end;
        }

        // STRING (-> type each character)
        else if (inString || compare(cmd_name->str, cmd_name->len, "STRING", true)) {
            if (inString) {
                type(n->str, n->len);
            } else {
                type(line_str, line_str_len);
            }

            inString = !line_end;
        }

        // DELAY (-> sleep for x ms)
        else if (compare(cmd_name->str, cmd_name->len, "DELAY", true)) {
            // Serial.print("Delay(");
            // print(first_arg->str, first_arg->len);
            // Serial.println("ms)");
        }

        // DEFAULTDELAY/DEFAULT_DELAY (set default delay per command)
        else if (compare(cmd_name->str, cmd_name->len, "DEFAULTDELAY", true) || compare(cmd_name->str, cmd_name->len, "DEFAULT_DELAY", true)) {
            // Serial.print("DEFAULT_DELAY=");
            // print(first_arg->str, first_arg->len);
            // Serial.println("ms");
        }

        // REPEAT (-> repeat last command n times)
        else if (compare(cmd_name->str, cmd_name->len, "REPEAT", true)) {
            // Serial.print("REPEAT ");
            // print(first_arg->str, first_arg->len);
            // Serial.println(" times");
        }

        // Otherwise go through words and look for keys to press
        else {
            word_node* w = wl->first;

            while (w) {
                // Serial.print("Press '");
                // print(w->str, w->len);
                // Serial.println("'");
                w = w->next;
            }

            if (line_end) release();
        }

        n = n->next;
    }

    line_list_destroy(l);
}