#ifndef locale_types_h
#define locale_types_h

typedef struct locale_t {
    uint8_t* ascii;
    uint8_t  ascii_len;

    uint8_t* extended_ascii;
    size_t   extended_ascii_len;

    uint8_t* utf8;
    size_t   utf8_len;
} locale_t;

#endif /* ifndef locale_types_h */