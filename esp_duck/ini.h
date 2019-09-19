/*
   Copyright (c) 2019 Stefan Kremser
   This software is licensed under the MIT License. See the license file for details.
   Source: github.com/spacehuhn/WiFiDuck
 */

#ifndef ini_h
#define ini_h

#include <stdlib.h>

typedef void (* ini_print)(const char* str);

// ===== Types ==== //
typedef struct ini_pair {
    char           * key;
    char           * value;
    char           * comment;
    struct ini_pair* next;
} ini_pair;

typedef struct ini_section {
    char              * name;
    char              * comment;
    struct ini_pair   * pairs;
    struct ini_section* next;
} ini_section;

typedef struct ini_file {
    struct ini_section* default_section;
    struct ini_section* sections;
} ini_file;

// ===== ini_pair ===== //
ini_pair* ini_pair_create(const char* key, size_t key_len, const char* value, size_t value_len, const char* comment, size_t comment_len);
ini_pair* ini_pair_copy(ini_pair* pair);
ini_pair* ini_pair_destroy(ini_pair* pair);
void ini_pair_str(ini_pair* pair, char* str);
int ini_pair_equals(ini_pair* a, ini_pair* b);
void ini_pair_set_value(ini_pair* pair, char* value);
void ini_pair_print(ini_pair* pair, ini_print print);
size_t ini_pair_strlen(ini_pair* pair);

// ===== ini_section ===== //
ini_section* ini_section_create(const char* name, size_t name_len, const char* comment, size_t comment_len);
ini_section* ini_section_copy(ini_section* section);
ini_section* ini_section_destroy(ini_section* section);
ini_pair* ini_section_get_pair(ini_section* section, const char* key);
void ini_section_str(ini_section* section, char* str);
ini_section* ini_section_add_pair(ini_section* section, ini_pair* pair);
int ini_section_equals(ini_section* a, ini_section* b);
void ini_section_print(ini_section* section, ini_print print);
size_t ini_section_strlen(ini_section* section);

// ===== ini_file ===== //
ini_file* ini_file_create();
ini_file* ini_file_copy(ini_file* file);
ini_file* ini_file_destroy(ini_file* file);
ini_section* ini_file_get_section(ini_file* file, const char* section_name);
void ini_file_str(ini_file* file, char* str);
ini_file* ini_file_add_section(ini_file* file, ini_section* section);
void ini_file_print(ini_file* file, ini_print print);
size_t ini_file_strlen(ini_file* file);

// ===== Parser ===== //
ini_file* ini_parse(ini_file* file, char* str, size_t len);

#endif /* ifndef ini_h */