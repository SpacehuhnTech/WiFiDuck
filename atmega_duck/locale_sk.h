/*
   This software is licensed under the MIT License. See the license file for details.
   Source: https://github.com/spacehuhntech/WiFiDuck
 */

#pragma once

#include "usb_hid_keys.h"

// Modifier(s), Key
const uint8_t ascii_sk[] PROGMEM = {
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
    KEY_MOD_LSHIFT, KEY_APOSTROPHE, // !
    KEY_MOD_LSHIFT, KEY_SEMICOLON,  // "
    KEY_MOD_RALT,   KEY_X,          // #

    // 36, 0x24
    KEY_MOD_RALT,   KEY_SEMICOLON,  // $
    KEY_MOD_LSHIFT, KEY_MINUS,      // %
    KEY_MOD_RALT,   KEY_C,          // &
    KEY_MOD_RALT,   KEY_P,          // '

    // 40, 0x28
    KEY_MOD_LSHIFT, KEY_RIGHTBRACE, // (
    KEY_MOD_LSHIFT, KEY_BACKSLASH,  // )
    KEY_MOD_RALT,   KEY_SLASH,      // *
    KEY_NONE,       KEY_1,          // +

    // 44, 0x2c
    KEY_NONE,       KEY_COMMA,      // ,
    KEY_NONE,       KEY_SLASH,      // -
    KEY_NONE,       KEY_DOT,        // .
    KEY_MOD_LSHIFT, KEY_LEFTBRACE,  // /

    // 48, 0x30
    KEY_MOD_LSHIFT, KEY_0,          // 0
    KEY_MOD_LSHIFT, KEY_1,          // 1
    KEY_MOD_LSHIFT, KEY_2,          // 2
    KEY_MOD_LSHIFT, KEY_3,          // 3

    // 52, 0x34
    KEY_MOD_LSHIFT, KEY_4,          // 4
    KEY_MOD_LSHIFT, KEY_5,          // 5
    KEY_MOD_LSHIFT, KEY_6,          // 6
    KEY_MOD_LSHIFT, KEY_7,          // 7

    // 56, 0x38
    KEY_MOD_LSHIFT, KEY_8,          // 8
    KEY_MOD_LSHIFT, KEY_9,          // 9
    KEY_MOD_LSHIFT, KEY_DOT,        // :
    KEY_NONE,       KEY_GRAVE,      // ;

    // 60, 0x3c
    KEY_MOD_RALT,   KEY_COMMA,      // <
    KEY_NONE,       KEY_MINUS,      // =
    KEY_MOD_RALT,   KEY_DOT,        // >
    KEY_MOD_LSHIFT, KEY_COMMA,      // ?

    // 64, 0x40
    KEY_MOD_RALT,   KEY_V,          // @
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
    KEY_MOD_RALT,   KEY_F,          // [

    // 92, 0x5c
    KEY_MOD_RALT,   KEY_Q,          // bslash
    KEY_MOD_RALT,   KEY_G,          // ]
    KEY_MOD_RALT,   KEY_3,          // ^
    KEY_MOD_LSHIFT, KEY_SLASH,      // _

    // 96, 0x60
    KEY_MOD_RALT,   KEY_7,          // `
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
    KEY_MOD_RALT,   KEY_B,          // {

    // 124, 0x7c
    KEY_MOD_RALT,   KEY_W,          // |
    KEY_MOD_RALT,   KEY_N,          // }
    KEY_MOD_RALT,   KEY_1,          // ~
    KEY_NONE,       KEY_NONE        // DEL
};

const uint8_t utf8_sk[] PROGMEM = {
    0xc2 ,0xb4, 0x00, 0x00, KEY_NONE,       KEY_EQUAL,     // ´
    0xcb ,0x87, 0x00, 0x00, KEY_MOD_LSHIFT, KEY_EQUAL,     // ˇ
    0xc3, 0xa1, 0x00, 0x00, KEY_NONE,       KEY_8,         // á
    0xc3, 0xa9, 0x00, 0x00, KEY_NONE,       KEY_0,         // é
    0xc3, 0xad, 0x00, 0x00, KEY_NONE,       KEY_9,         // í
    0xc4, 0xba, 0x00, 0x00, KEY_NONE,       KEY_L,         // ĺ   press "´" before
    0xc3, 0xb3, 0x00, 0x00, KEY_NONE,       KEY_O,         // ó   press "´" before
    0xc5, 0x95, 0x00, 0x00, KEY_NONE,       KEY_R,         // ŕ   press "´" before
    0xc3, 0xba, 0x00, 0x00, KEY_NONE,       KEY_LEFTBRACE, // ú
    0xc3, 0xbd, 0x00, 0x00, KEY_NONE,       KEY_7,         // ý
    0xc4, 0x8d, 0x00, 0x00, KEY_NONE,       KEY_4,         // č
    0xc4, 0x8f, 0x00, 0x00, KEY_NONE,       KEY_D,         // ď   press "ˇ" before
    0xc4, 0xbe, 0x00, 0x00, KEY_NONE,       KEY_2,         // ľ
    0xc5, 0x88, 0x00, 0x00, KEY_NONE,       KEY_BACKSLASH, // ň
    0xc5, 0xa1, 0x00, 0x00, KEY_NONE,       KEY_3,         // š
    0xc5, 0xa5, 0x00, 0x00, KEY_NONE,       KEY_5,         // ť
    0xc5, 0xbe, 0x00, 0x00, KEY_NONE,       KEY_6,         // ž
    0xc3, 0xa4, 0x00, 0x00, KEY_NONE,       KEY_RIGHTBRACE,// ä
    0xc3, 0xb4, 0x00, 0x00, KEY_NONE,       KEY_SEMICOLON  // ô

};

static hid_locale_t locale_sk {
    (uint8_t*)ascii_sk, 128,
    NULL, 0,
    (uint8_t*)utf8_sk, sizeof(utf8_sk) / 6
};
