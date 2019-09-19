/*
   Copyright (c) 2019 Stefan Kremser
   This software is licensed under the MIT License. See the license file for details.
   Source: github.com/spacehuhn/WiFiDuck
 */

#include "ini.h"

#include <string.h>

// ===== Internal ===== //
size_t sum = 0;

char * cstr   = NULL;
size_t cstr_i = 0;

void print_to_sum(const char* str) {
    if (str) {
        sum += strlen(str);
    }
}

void print_to_str(const char* str) {
    if (str && cstr) {
        size_t len = strlen(str);

        memcpy(&cstr[cstr_i], str, len);
        cstr_i += len;
    }
}

// ===== ini_pair ===== //
ini_pair* ini_pair_create(const char* key, size_t key_len, const char* value, size_t value_len, const char* comment, size_t comment_len) {
    ini_pair* pair = (ini_pair*)malloc(sizeof(ini_pair));

    if (key && (key_len > 0)) {
        pair->key = (char*)malloc(key_len+1);
        strncpy(pair->key, key, key_len);
        pair->key[key_len] = '\0';
    } else {
        pair->key = NULL;
    }

    if (value && (value_len > 0)) {
        pair->value = (char*)malloc(value_len+1);
        strncpy(pair->value, value, value_len);
        pair->value[value_len] = '\0';
    } else {
        pair->value = NULL;
    }

    if (comment && (comment_len > 0)) {
        pair->comment = (char*)malloc(comment_len+1);
        strncpy(pair->comment, comment, comment_len);
        pair->comment[comment_len] = '\0';
    } else {
        pair->comment = NULL;
    }

    pair->next = NULL;

    return pair;
}

ini_pair* ini_pair_copy(ini_pair* pair) {
    if (!pair) return NULL;

    return ini_pair_create(pair->key, strlen(pair->key), pair->value, strlen(pair->value), pair->comment, strlen(pair->comment));
}

ini_pair* ini_pair_destroy(ini_pair* pair) {
    if (pair) {
        if (pair->key) free(pair->key);
        if (pair->value) free(pair->value);
        if (pair->comment) free(pair->comment);

        pair->key     = NULL;
        pair->value   = NULL;
        pair->comment = NULL;

        free(pair);
    }

    return NULL;
}

void ini_pair_str(ini_pair* pair, char* str) {
    cstr   = str;
    cstr_i = 0;

    ini_pair_print(pair, print_to_str);

    cstr   = NULL;
    cstr_i = 0;
}

int ini_pair_equals(ini_pair* a, ini_pair* b) {
    if (!a || !b) return 0;

    return strcmp(a->key, b->key) == 0;
}

void ini_pair_set_value(ini_pair* pair, char* value) {
    if (pair) {
        if (pair->value) {
            free(pair->value);
            pair->value = NULL;
        }

        if (value) {
            pair->value = (char*)malloc(strlen(value+1));
            strcpy(pair->value, value);
        }
    }
}

void ini_pair_print(ini_pair* pair, ini_print print) {
    if (pair) {
        if (pair->key) {
            print(pair->key);
            print("=");
        }

        if (pair->value) {
            print(pair->value);
        }

        if (pair->comment) {
            print(";");
            print(pair->comment);
        }

        print("\n");
    }
}

size_t ini_pair_strlen(ini_pair* pair) {
    sum = 0;
    ini_pair_print(pair, print_to_sum);
    return sum;
}

// ===== ini_section ===== //
ini_section* ini_section_create(const char* name, size_t name_len, const char* comment, size_t comment_len) {
    ini_section* section = (ini_section*)malloc(sizeof(ini_section));

    if (name && (name_len > 0)) {
        section->name = (char*)malloc(name_len+1);
        strncpy(section->name, name, name_len);
        section->name[name_len] = '\0';
    } else {
        section->name = NULL;
    }

    if (comment && (comment_len > 0)) {
        section->comment = (char*)malloc(comment_len+1);
        strncpy(section->comment, comment, comment_len);
        section->comment[comment_len] = '\0';
    } else {
        section->comment = NULL;
    }

    section->pairs = NULL;
    section->next  = NULL;

    return section;
}

ini_section* ini_section_copy(ini_section* section) {
    if (!section) return NULL;

    return ini_section_create(section->name, strlen(section->name), section->comment, strlen(section->comment));
}

ini_section* ini_section_destroy(ini_section* section) {
    if (section) {
        if (section->name) free(section->name);
        if (section->comment) free(section->comment);

        section->name    = NULL;
        section->comment = NULL;

        ini_pair* h = section->pairs;
        ini_pair* to_delete;

        while (h) {
            to_delete = h;
            h         = h->next;
            ini_pair_destroy(to_delete);
        }

        free(section);
    }
    return NULL;
}

ini_pair* ini_section_get_pair(ini_section* section, const char* key) {
    if (section) {
        ini_pair* h = section->pairs;

        while (h) {
            if (strcmp(h->key, key) == 0) return h;
            h = h->next;
        }
    }

    return NULL;
}

void ini_section_str(ini_section* section, char* str) {
    cstr   = str;
    cstr_i = 0;

    ini_section_print(section, print_to_str);

    cstr   = NULL;
    cstr_i = 0;
}

ini_section* ini_section_add_pair(ini_section* section, ini_pair* pair) {
    if (!section) return NULL;

    if (pair) {
        if (section->pairs) {
            ini_pair* h = section->pairs;

            while (h->next) {
                h = h->next;
            }

            h->next = pair;
        } else {
            section->pairs = pair;
        }
    }

    return section;
}

int ini_section_equals(ini_section* a, ini_section* b) {
    if (a && b) {
        return strcmp(a->name, b->name) == 0;
    }

    return 0;
}

void ini_section_print(ini_section* section, ini_print print) {
    if (section) {
        if (section->name || section->comment) {
            if (section->name) {
                print("[");
                print(section->name);
                print("]");
            }

            if (section->comment) {
                print(";");
                print(section->comment);
            }

            print("\n");
        }

        ini_pair* h = section->pairs;

        while (h) {
            ini_pair_print(h, print);
            h = h->next;
        }
    }
}

size_t ini_section_strlen(ini_section* section) {
    sum = 0;
    ini_section_print(section, print_to_sum);
    return sum;
}

// ===== ini_file ===== //
ini_file* ini_file_create() {
    ini_file* file = (ini_file*)malloc(sizeof(ini_file));

    file->default_section = ini_section_create(NULL, 0, NULL, 0);
    file->sections        = NULL;

    return file;
}

ini_file* ini_file_copy(ini_file* file) {
    ini_file* new_file = ini_file_create();

    if (file) {
        ini_section* h = file->sections;

        while (h) {
            ini_file_add_section(new_file, ini_section_copy(h));
            h = h->next;
        }
    }

    return new_file;
}

ini_file* ini_file_destroy(ini_file* file) {
    if (file) {
        ini_section_destroy(file->default_section);

        ini_section* h = file->sections;
        ini_section* to_delete;

        while (h) {
            to_delete = h;
            h         = h->next;
            ini_section_destroy(to_delete);
        }

        file->sections = NULL;

        free(file);
    }
    return NULL;
}

ini_section* ini_file_get_section(ini_file* file, const char* section_name) {
    if (file) {
        ini_section* h = file->sections;

        while (h) {
            if (strcmp(h->name, section_name) == 0) return h;
            h = h->next;
        }
    }

    return NULL;
}

void ini_file_str(ini_file* file, char* str) {
    cstr   = str;
    cstr_i = 0;

    ini_file_print(file, print_to_str);

    cstr   = NULL;
    cstr_i = 0;
}

ini_file* ini_file_add_section(ini_file* file, ini_section* section) {
    if (file) {
        ini_section* h = file->sections;

        if (h) {
            while (h->next) {
                h = h->next;
            }

            h->next = section;
        } else {
            file->sections = section;
        }
    }

    return file;
}

void ini_file_print(ini_file* file, ini_print print) {
    if (file) {
        ini_section_print(file->default_section, print);

        ini_section* h = file->sections;

        while (h) {
            ini_section_print(h, print);
            h = h->next;
        }
    }
}

size_t ini_file_strlen(ini_file* file) {
    sum = 0;
    ini_file_print(file, print_to_sum);
    return sum;
}

// ===== Parser ===== //
ini_file* ini_parse(ini_file* file, char* str, size_t len) {
    if (!file) file = ini_file_create();

    if (!str || (len == 0)) return file;

    ini_section* current_section = file->default_section;

    if (file->sections) {
        current_section = file->sections;

        while (current_section->next) current_section = current_section->next;
    }

    // Go through string and look for /r and /n to split it into lines
    size_t i = 0; // current index
    size_t j = 0; // start index of line

    int linebreak = 0;
    int endofline = 0;

    for (i = 0; i <= len; ++i) {
        endofline = (i == len || str[i] == '\0');
        linebreak = ((str[i] == '\r') || (str[i] == '\n'));

        if ((linebreak || endofline) && (i - j > 0)) {
            // for every line to file
            char * line     = &str[j];
            size_t line_len = i - j;

            size_t begin = 0;
            size_t end   = line_len-1;

            // skip spaces
            while (line[begin] == ' ' && begin < end) ++begin;

            while (line[end] == ' ' && end > begin) --end;

            // skip carriage returns
            while (line[begin] == '\r' && begin < end) ++begin;

            while (line[end] == '\r' && end > begin) --end;

            char * comment     = NULL;
            size_t comment_len = 0;

            // ;Comment
            if (line[begin] == ';') {
                comment     = &line[begin+1];
                comment_len = end-begin;
            }
            // *;something with a comment
            else {
                size_t k;
                int    inquotes = 0;

                for (k = begin; k<end; ++k) {
                    if (line[k] == '"') {
                        inquotes = !inquotes;
                    } else if ((line[k] == ';') && !inquotes) {
                        comment     = &line[k+1];
                        comment_len = end - k;
                        end         = k-1;
                        k           = end;
                    }
                }
            }

            // [Section]
            if ((line[begin] == '[') && (line[end] == ']')) {
                current_section = ini_section_create(&line[begin+1], end-begin-1, comment, comment_len);
                ini_file_add_section(file, current_section);
            }
            // PairKey=PairValue
            else {
                char * key       = NULL;
                char * value     = NULL;
                size_t key_len   = 0;
                size_t value_len = 0;

                // search for equals sign
                size_t equals = begin;

                while (line[equals] != '=' && equals < end) ++equals;

                if (line[equals] == '=') {
                    key       = &line[begin];
                    value     = &line[equals+1];
                    key_len   = equals - begin;
                    value_len = end - equals;

                    // skip quotes
                    while (key[0] == '"' && key[key_len-1] == '"') {
                        ++key;
                        key_len -= 2;
                    }

                    while (value[0] == '"' && value[value_len-1] == '"') {
                        ++value;
                        value_len -= 2;
                    }
                }

                ini_section_add_pair(current_section, ini_pair_create(key, key_len, value, value_len, comment, comment_len));
            }

            j = i+1; // reset start index of line
        }
    }

    return file;
}