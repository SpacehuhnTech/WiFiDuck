/*
   This software is licensed under the MIT License. See the license file for details.
   Source: https://github.com/spacehuhntech/WiFiDuck
 */

#pragma once

#include "usb_hid_keys.h"

// Modifier(s), Key
const uint8_t ascii_es[] PROGMEM = {
    KEY_NONE,       KEY_NONE,       // NUL
    KEY_NONE,       KEY_NONE,       // SOH
    KEY_NONE,       KEY_NONE,       // STX
    KEY_NONE,       KEY_NONE,       // ETX
    KEY_NONE,       KEY_NONE,       // EOT
    KEY_NONE,       KEY_NONE,       // ENQ
    KEY_NONE,       KEY_NONE,       // ACK
    KEY_NONE,       KEY_NONE,       // BEL

    // 8, 0x08
    KEY_NONE,       KEY_BACKSPACE,  // BS	Backspace
    KEY_NONE,       KEY_TAB,        // TAB	Tab
    KEY_NONE,       KEY_ENTER,      // LF	Enter

    KEY_NONE,       KEY_NONE,       // VT
    KEY_NONE,       KEY_NONE,       // FF
    KEY_NONE,       KEY_NONE,       // CR
    KEY_NONE,       KEY_NONE,       // SO
    KEY_NONE,       KEY_NONE,       // SI
    KEY_NONE,       KEY_NONE,       // DEL
    KEY_NONE,       KEY_NONE,       // DC1
    KEY_NONE,       KEY_NONE,       // DC2
    KEY_NONE,       KEY_NONE,       // DC3
    KEY_NONE,       KEY_NONE,       // DC4
    KEY_NONE,       KEY_NONE,       // NAK
    KEY_NONE,       KEY_NONE,       // SYN
    KEY_NONE,       KEY_NONE,       // ETB
    KEY_NONE,       KEY_NONE,       // CAN
    KEY_NONE,       KEY_NONE,       // EM
    KEY_NONE,       KEY_NONE,       // SUB
    KEY_NONE,       KEY_NONE,       // ESC
    KEY_NONE,       KEY_NONE,       // FS
    KEY_NONE,       KEY_NONE,       // GS
    KEY_NONE,       KEY_NONE,       // RS
    KEY_NONE,       KEY_NONE,       // US

    // 32, 0x20
    KEY_NONE,       KEY_SPACE,      // ' '
    KEY_MOD_LSHIFT, KEY_1,          // !
    KEY_MOD_LSHIFT, KEY_2,          // "
    KEY_MOD_RALT,       KEY_3,      // #

    // 36, 0x24
    KEY_MOD_LSHIFT, KEY_4,          // $
    KEY_MOD_LSHIFT, KEY_5,          // %
    KEY_MOD_LSHIFT, KEY_6,          // &
    KEY_NONE,       KEY_MINUS,      // '

    // 40, 0x28
    KEY_MOD_LSHIFT, KEY_8,          // (
    KEY_MOD_LSHIFT, KEY_9,          // )
    KEY_MOD_LSHIFT, KEY_RIGHTBRACE, // *
    KEY_NONE,       KEY_RIGHTBRACE, // +

    // 44, 0x2c
    KEY_NONE,       KEY_COMMA,      // ,
    KEY_NONE,       KEY_SLASH,      // -
    KEY_NONE,       KEY_DOT,        // .
    KEY_MOD_LSHIFT, KEY_7,          // /

    // 48, 0x30
    KEY_NONE,       KEY_0,          // 0
    KEY_NONE,       KEY_1,          // 1
    KEY_NONE,       KEY_2,          // 2
    KEY_NONE,       KEY_3,          // 3

    // 52, 0x34
    KEY_NONE,       KEY_4,          // 4
    KEY_NONE,       KEY_5,          // 5
    KEY_NONE,       KEY_6,          // 6
    KEY_NONE,       KEY_7,          // 7

    // 56, 0x38
    KEY_NONE,       KEY_8,          // 8
    KEY_NONE,       KEY_9,          // 9
    KEY_MOD_LSHIFT, KEY_DOT,        // :
    KEY_MOD_LSHIFT, KEY_COMMA,      // ;

    // 60, 0x3c
    KEY_NONE,       KEY_102ND,      // <
    KEY_MOD_LSHIFT, KEY_0,          // =
    KEY_MOD_LSHIFT, KEY_102ND,      // >
    KEY_MOD_LSHIFT, KEY_MINUS,      // ?

    // 64, 0x40
    KEY_MOD_RALT,   KEY_2,          // @
    KEY_MOD_LSHIFT, KEY_A,          // A
    KEY_MOD_LSHIFT, KEY_B,          // B
    KEY_MOD_LSHIFT, KEY_C,          // C

    // 68, 0x44
    KEY_MOD_LSHIFT, KEY_D,          // D
    KEY_MOD_LSHIFT, KEY_E,          // E
    KEY_MOD_LSHIFT, KEY_F,          // F
    KEY_MOD_LSHIFT, KEY_G,          // G

    // 72, 0x48
    KEY_MOD_LSHIFT, KEY_H,          // H
    KEY_MOD_LSHIFT, KEY_I,          // I
    KEY_MOD_LSHIFT, KEY_J,          // J
    KEY_MOD_LSHIFT, KEY_K,          // K

    // 76, 0x4c
    KEY_MOD_LSHIFT, KEY_L,          // L
    KEY_MOD_LSHIFT, KEY_M,          // M
    KEY_MOD_LSHIFT, KEY_N,          // N
    KEY_MOD_LSHIFT, KEY_O,          // O

    // 80, 0x50
    KEY_MOD_LSHIFT, KEY_P,          // P
    KEY_MOD_LSHIFT, KEY_Q,          // Q
    KEY_MOD_LSHIFT, KEY_R,          // R
    KEY_MOD_LSHIFT, KEY_S,          // S

    // 84, 0x54
    KEY_MOD_LSHIFT, KEY_T,          // T
    KEY_MOD_LSHIFT, KEY_U,          // U
    KEY_MOD_LSHIFT, KEY_V,          // V
    KEY_MOD_LSHIFT, KEY_W,          // W

    // 88, 0x58
    KEY_MOD_LSHIFT, KEY_X,          // X
    KEY_MOD_LSHIFT, KEY_Y,          // Y
    KEY_MOD_LSHIFT, KEY_Z,          // Z
    KEY_MOD_RALT,   KEY_LEFTBRACE,  // [

    // 92, 0x5c
    KEY_MOD_RALT,   KEY_GRAVE,      // bslash
    KEY_MOD_RALT,   KEY_RIGHTBRACE, // ]
    KEY_MOD_LSHIFT, KEY_LEFTBRACE,  // ^
    KEY_MOD_LSHIFT, KEY_SLASH,      // _

    // 96, 0x60
    KEY_NONE,       KEY_LEFTBRACE,  // `
    KEY_NONE,       KEY_A,          // a
    KEY_NONE,       KEY_B,          // b
    KEY_NONE,       KEY_C,          // c

    // 100, 0x64
    KEY_NONE,       KEY_D,          // d
    KEY_NONE,       KEY_E,          // e
    KEY_NONE,       KEY_F,          // f
    KEY_NONE,       KEY_G,          // g

    // 104, 0x68
    KEY_NONE,       KEY_H,          // h
    KEY_NONE,       KEY_I,          // i
    KEY_NONE,       KEY_J,          // j
    KEY_NONE,       KEY_K,          // k

    // 108, 0x6c
    KEY_NONE,       KEY_L,          // l
    KEY_NONE,       KEY_M,          // m
    KEY_NONE,       KEY_N,          // n
    KEY_NONE,       KEY_O,          // o

    // 112, 0x70
    KEY_NONE,       KEY_P,          // p
    KEY_NONE,       KEY_Q,          // q
    KEY_NONE,       KEY_R,          // r
    KEY_NONE,       KEY_S,          // s

    // 116, 0x74
    KEY_NONE,       KEY_T,          // t
    KEY_NONE,       KEY_U,          // u
    KEY_NONE,       KEY_V,          // v
    KEY_NONE,       KEY_W,          // w

    // 120, 0x78
    KEY_NONE,       KEY_X,          // x
    KEY_NONE,       KEY_Y,          // y
    KEY_NONE,       KEY_Z,          // z
    KEY_MOD_RALT,   KEY_APOSTROPHE, // {

    // 124, 0x7c
    KEY_MOD_RALT,   KEY_1,          // |
    KEY_MOD_RALT,   KEY_BACKSLASH,  // }
    KEY_MOD_RALT,   KEY_4,          // ~
    KEY_NONE,       KEY_NONE        // DEL
};

const uint8_t extended_ascii_es[] PROGMEM = {
    0xa4, KEY_NONE,       KEY_SEMICOLON,  // ñ
    0xa5, KEY_MOD_LSHIFT, KEY_SEMICOLON,  // Ñ
    0xad, KEY_NONE,       KEY_EQUAL,      // ¡
    0xa8, KEY_MOD_LSHIFT, KEY_EQUAL,      // ¿
    0x87, KEY_NONE,       KEY_BACKSLASH,  // ç
    0x80, KEY_MOD_LSHIFT, KEY_BACKSLASH,  // Ç
    0xa7, KEY_NONE,       KEY_GRAVE,      // º
    0xa6, KEY_MOD_LSHIFT, KEY_GRAVE,      // ª
    0xfa, KEY_MOD_LSHIFT, KEY_3,          // ·
    0xf9, KEY_MOD_LSHIFT, KEY_APOSTROPHE, // ¨
    0xef, KEY_NONE,       KEY_APOSTROPHE  // ´
};

const uint8_t utf8_es[] PROGMEM = {
    0xc3, 0xb1, 0x00, 0x00, KEY_NONE,       KEY_SEMICOLON,  // ñ
    0xc3, 0x91, 0x00, 0x00, KEY_MOD_LSHIFT, KEY_SEMICOLON,  // Ñ
    0xc2, 0xa1, 0x00, 0x00, KEY_NONE,       KEY_EQUAL,      // ¡
    0xc2, 0xbf, 0x00, 0x00, KEY_MOD_LSHIFT, KEY_EQUAL,      // ¿
    0xc3, 0xa7, 0x00, 0x00, KEY_NONE,       KEY_BACKSLASH,  // ç
    0xc3, 0x87, 0x00, 0x00, KEY_MOD_LSHIFT, KEY_BACKSLASH,  // Ç
    0xc2, 0xba, 0x00, 0x00, KEY_NONE,       KEY_GRAVE,      // º
    0xc2, 0xaa, 0x00, 0x00, KEY_MOD_LSHIFT, KEY_GRAVE,      // ª
    0xe2, 0x82, 0xac, 0x00, KEY_MOD_RALT,   KEY_E,          // €
    0xc2, 0xb7, 0x00, 0x00, KEY_MOD_LSHIFT, KEY_3,          // ·
    0xc2, 0xa8, 0x00, 0x00, KEY_MOD_LSHIFT, KEY_APOSTROPHE, // ¨
    0xc2, 0xb4, 0x00, 0x00, KEY_NONE,       KEY_APOSTROPHE  // ´
};

static hid_locale_t locale_es {
    (uint8_t*)ascii_es, 128,
    (uint8_t*)extended_ascii_es,  sizeof(extended_ascii_es) / 3,
    (uint8_t*)utf8_es, sizeof(utf8_es) / 6
};
