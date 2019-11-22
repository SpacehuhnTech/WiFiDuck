/*
   Copyright (c) 2019 Stefan Kremser
   This software is licensed under the MIT License. See the license file for details.
   Source: github.com/spacehuhn/SimpleCLI
 */

#include "parser.h"

#include <stdlib.h>  // malloc
#include <string.h>  // strlen
#include <stdbool.h> // bool

// My own implementation, because the default one in ctype.h make problems on older ESP8266 SDKs
char to_lower(char c) {
    if ((c >= 65) && (c <= 90)) {
        return (char)(c + 32);
    }
    return c;
}

int compare(const char* user_str, size_t user_str_len, const char* templ_str, int case_sensetive) {
    if (user_str == templ_str) return COMPARE_EQUAL;

    // null check string pointers
    if (!user_str || !templ_str) return COMPARE_UNEQUAL;

    // string lengths
    size_t str_len = user_str_len; // strlen(user_str);
    size_t key_len = strlen(templ_str);

    // when same length, it there is no need to check for slashes or commas
    if (str_len == key_len) {
        for (size_t i = 0; i < key_len; i++) {
            if (case_sensetive == COMPARE_CASE_SENSETIVE) {
                if (user_str[i] != templ_str[i]) return COMPARE_UNEQUAL;
            } else {
                if (to_lower(user_str[i]) != to_lower(templ_str[i])) return COMPARE_UNEQUAL;
            }
        }
        return COMPARE_EQUAL;
    }

    // string can't be longer than templ_str (but can be smaller because of  '/' and ',')
    if (str_len > key_len) return COMPARE_UNEQUAL;

    unsigned int res_i = 0;
    unsigned int a     = 0;
    unsigned int b     = 0;
    unsigned int res   = 1;

    while (a < str_len && b < key_len) {
        if (templ_str[b] == '/') {
            // skip slash in templ_str
            ++b;
        } else if (templ_str[b] == ',') {
            // on comma increment res_i and reset str-index
            ++b;
            a = 0;
            ++res_i;
        }

        // compare character
        if (case_sensetive == COMPARE_CASE_SENSETIVE) {
            if (user_str[a] != templ_str[b]) res = 0;
        } else {
            if (to_lower(user_str[a]) != to_lower(templ_str[b])) res = 0;
        }

        // comparison incorrect or string checked until the end and templ_str not checked until the end
        if (!res || ((a == str_len - 1) &&
                     (templ_str[b + 1] != ',') &&
                     (templ_str[b + 1] != '/') &&
                     (templ_str[b + 1] != '\0'))) {
            // fast forward to next comma
            while (b < key_len && templ_str[b] != ',') b++;
            res = 1;
        } else {
            // otherwise icrement indices
            ++a;
            ++b;
        }
    }

    // comparison correct AND string checked until the end AND templ_str checked until the end
    if (res && (a == str_len) &&
        ((templ_str[b] == ',') ||
         (templ_str[b] == '/') ||
         (templ_str[b] == '\0'))) return COMPARE_EQUAL;  // res_i

    return COMPARE_UNEQUAL;
}

// ===== Word Node ===== //
word_node* word_node_create(const char* str, size_t len) {
    word_node* n = (word_node*)malloc(sizeof(word_node));

    n->str  = str;
    n->len  = len;
    n->next = NULL;
    return n;
}

word_node* word_node_destroy(word_node* n) {
    if (n) {
        free(n);
    }
    return NULL;
}

word_node* word_node_destroy_rec(word_node* n) {
    if (n) {
        word_node_destroy_rec(n->next);
        word_node_destroy(n);
    }
    return NULL;
}

// ===== Word List ===== //
word_list* word_list_create() {
    word_list* l = (word_list*)malloc(sizeof(word_list));

    l->first = NULL;
    l->last  = NULL;
    l->size  = 0;
    return l;
}

word_list* word_list_destroy(word_list* l) {
    if (l) {
        word_node_destroy_rec(l->first);
        free(l);
    }
    return NULL;
}

void word_list_push(word_list* l, word_node* n) {
    if (l && n) {
        if (l->last) {
            l->last->next = n;
        } else {
            l->first = n;
        }

        l->last = n;
        ++l->size;
    }
}

word_node* word_list_get(word_list* l, size_t i) {
    if (!l) return NULL;

    size_t j;
    word_node* h = l->first;

    for (j = 0; j < i && h; ++j) {
        h = h->next;
    }

    return h;
}

// ===== Line Node ==== //
line_node* line_node_create(const char* str, size_t len) {
    line_node* n = (line_node*)malloc(sizeof(line_node));

    n->str   = str;
    n->len   = len;
    n->words = NULL;
    n->next  = NULL;

    return n;
}

word_node* line_node_destroy(line_node* n) {
    if (n) {
        word_list_destroy(n->words);
        free(n);
    }
    return NULL;
}

word_node* line_node_destroy_rec(line_node* n) {
    if (n) {
        line_node_destroy_rec(n->next);
        line_node_destroy(n);
    }
    return NULL;
}

// ===== Line List ===== //
line_list* line_list_create() {
    line_list* l = (line_list*)malloc(sizeof(line_list));

    l->first = NULL;
    l->last  = NULL;
    l->size  = 0;

    return l;
}

line_list* line_list_destroy(line_list* l) {
    if (l) {
        line_node_destroy_rec(l->first);
        free(l);
    }
    return NULL;
}

void line_list_push(line_list* l, line_node* n) {
    if (l && n) {
        if (l->last) {
            l->last->next = n;
        } else {
            l->first = n;
        }

        l->last = n;
        ++l->size;
    }
}

line_node* line_list_get(line_list* l, size_t i) {
    if (!l) return NULL;

    size_t j;
    line_node* h = l->first;

    for (j = 0; j < i && h; ++j) {
        h = h->next;
    }

    return h;
}

// ===== Parser ===== //
word_list* parse_words(const char* str, size_t len) {
    word_list* l = word_list_create();

    if (len == 0) return l;

    // Go through string and look for space to split it into words
    word_node* n = NULL;

    size_t i = 0; // current index
    size_t j = 0; // start index of word

    int escaped      = 0;
    int ignore_space = 0;

    for (i = 0; i <= len; ++i) {
        if ((str[i] == '\\') && (escaped == 0)) {
            escaped = 1;
        } else if ((str[i] == '"') && (escaped == 0)) {
            ignore_space = !ignore_space;
        } else if ((i == len) || ((str[i] == ' ') && (ignore_space == 0) && (escaped == 0))) {
            size_t k = i - j; // length of word

            // for every word, add to list
            if (k > 0) {
                n = word_node_create(&str[j], k);
                word_list_push(l, n);
            }

            j = i + 1; // reset start index of word
        } else if (escaped == 1) {
            escaped = 0;
        }
    }

    return l;
}

line_list* parse_lines(const char* str, size_t len) {
    line_list* l = line_list_create();

    if (len == 0) return l;

    // Go through string and look for \r and \n to split it into lines
    line_node* n = NULL;

    size_t stri = 0; // current index
    size_t ls   = 0; // start index of line

    bool escaped   = false;
    bool in_quotes = false;
    bool delimiter = false;
    bool linebreak = false;
    bool endofline = false;

    for (stri = 0; stri <= len; ++stri) {
        char prev = stri > 0 ? str[stri-1] : 0;
        char curr = str[stri];
        char next = str[stri+1];

        escaped = prev == '\\';

        // disabled because ducky script isn't using quotes
        // in_quotes = (curr == '"' && !escaped) ? !in_quotes : in_quotes;
        // delimiter = !in_quotes && !escaped && curr == ';' && next == ';';

        linebreak = !in_quotes && (curr == '\r' || curr == '\n');

        endofline = stri == len || curr == '\0';

        if (linebreak || endofline || delimiter) {
            size_t llen = stri - ls; // length of line

            // for every line, parse_words and add to list
            if (llen > 0) {
                n        = line_node_create(&str[ls], llen);
                n->words = parse_words(&str[ls], llen);
                line_list_push(l, n);
            }

            if (delimiter) ++stri;

            ls = stri+1; // reset start index of line
        }
    }

    return l;
}