#ifndef locale_us_h
#define locale_us_h

#include "usb_hid_keys.h"

// Modifier(s), Key
const uint8_t locale_us[256*2] PROGMEM = {
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
    KEY_MOD_LSHIFT, KEY_APOSTROPHE, // "
    KEY_MOD_LSHIFT, KEY_3,          // #

    // 36, 0x24
    KEY_MOD_LSHIFT, KEY_4,          // $
    KEY_MOD_LSHIFT, KEY_5,          // %
    KEY_MOD_LSHIFT, KEY_7,          // &
    KEY_NONE,       KEY_APOSTROPHE, // '

    // 40, 0x28
    KEY_MOD_LSHIFT, KEY_9,          // (
    KEY_MOD_LSHIFT, KEY_0,          // )
    KEY_MOD_LSHIFT, KEY_8,          // *
    KEY_MOD_LSHIFT, KEY_EQUAL,      // +

    // 44, 0x2c
    KEY_NONE,       KEY_COMMA,      // ,
    KEY_NONE,       KEY_MINUS,      // -
    KEY_NONE,       KEY_DOT,        // .
    KEY_NONE,       KEY_SLASH,      // /

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
    KEY_MOD_LSHIFT, KEY_SEMICOLON,  // :
    KEY_NONE,       KEY_SEMICOLON,  // ;

    // 60, 0x3c
    KEY_MOD_LSHIFT, KEY_COMMA,      // <
    KEY_NONE,       KEY_EQUAL,      // =
    KEY_MOD_LSHIFT, KEY_DOT,        // >
    KEY_MOD_LSHIFT, KEY_SLASH,      // ?

    // 64, 0x40
    KEY_MOD_LSHIFT, KEY_2,          // @
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
    KEY_NONE,       KEY_LEFTBRACE,  // [

    // 92, 0x5c
    KEY_NONE,       KEY_BACKSLASH,  // bslash
    KEY_NONE,       KEY_RIGHTBRACE, // ]
    KEY_MOD_LSHIFT, KEY_6,          // ^
    KEY_MOD_LSHIFT, KEY_MINUS,      // _

    // 96, 0x60
    KEY_NONE,       KEY_GRAVE,      // `
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
    KEY_MOD_LSHIFT, KEY_LEFTBRACE,  // {

    // 124, 0x7c
    KEY_MOD_LSHIFT, KEY_BACKSLASH,  // |
    KEY_MOD_LSHIFT, KEY_RIGHTBRACE, // }
    KEY_MOD_LSHIFT, KEY_GRAVE,      // ~
    KEY_NONE,       KEY_NONE,       // DEL


    // ===== Extended ASCII ===== //

    // 128, 0x80
    KEY_NONE,       KEY_NONE, // Ç
    KEY_NONE,       KEY_NONE, // ü
    KEY_NONE,       KEY_NONE, // â
    KEY_NONE,       KEY_NONE, // ä

    // 132, 0x84
    KEY_NONE,       KEY_NONE, // à
    KEY_NONE,       KEY_NONE, // å
    KEY_NONE,       KEY_NONE, // ç
    KEY_NONE,       KEY_NONE, // ê

    // 136, 0x88
    KEY_NONE,       KEY_NONE, // ë
    KEY_NONE,       KEY_NONE, // è
    KEY_NONE,       KEY_NONE, // ï
    KEY_NONE,       KEY_NONE, // î

    // 140, 0x8c
    KEY_NONE,       KEY_NONE, // ì
    KEY_NONE,       KEY_NONE, // Ä
    KEY_NONE,       KEY_NONE, // Å
    KEY_NONE,       KEY_NONE, // É

    // 144, 0x90
    KEY_NONE,       KEY_NONE, // æ
    KEY_NONE,       KEY_NONE, // Æ
    KEY_NONE,       KEY_NONE, // ô
    KEY_NONE,       KEY_NONE, // ö

    // 148, 0x94
    KEY_NONE,       KEY_NONE, // ò
    KEY_NONE,       KEY_NONE, // û
    KEY_NONE,       KEY_NONE, // ù
    KEY_NONE,       KEY_NONE, // ÿ

    // 152, 0x98
    KEY_NONE,       KEY_NONE, // Ö
    KEY_NONE,       KEY_NONE, // Ü
    KEY_NONE,       KEY_NONE, // ¢
    KEY_NONE,       KEY_NONE, // £

    // 156, 0x9c
    KEY_NONE,       KEY_NONE, // ¥
    KEY_NONE,       KEY_NONE, // ₧
    KEY_NONE,       KEY_NONE, // ƒ
    KEY_NONE,       KEY_NONE, // á

    // 160, 0xa0
    KEY_NONE,       KEY_NONE, // í
    KEY_NONE,       KEY_NONE, // ó
    KEY_NONE,       KEY_NONE, // ú
    KEY_NONE,       KEY_NONE, // ñ

    // 164, 0xa4
    KEY_NONE,       KEY_NONE, // Ñ
    KEY_NONE,       KEY_NONE, // ª
    KEY_NONE,       KEY_NONE, // º
    KEY_NONE,       KEY_NONE, // ¿

    // 168, 0xa8
    KEY_NONE,       KEY_NONE, // ⌐
    KEY_NONE,       KEY_NONE, // ¬
    KEY_NONE,       KEY_NONE, // ½
    KEY_NONE,       KEY_NONE, // ¼

    // 172, 0xac
    KEY_NONE,       KEY_NONE, // ¡
    KEY_NONE,       KEY_NONE, // «
    KEY_NONE,       KEY_NONE, // »
    KEY_NONE,       KEY_NONE, // ░

    // 176, 0xb0
    KEY_NONE,       KEY_NONE, // ▒
    KEY_NONE,       KEY_NONE, // ▒
    KEY_NONE,       KEY_NONE, // ▓
    KEY_NONE,       KEY_NONE, // │

    // 180, 0xb4
    KEY_NONE,       KEY_NONE, // ┤
    KEY_NONE,       KEY_NONE, // ╡
    KEY_NONE,       KEY_NONE, // ╢
    KEY_NONE,       KEY_NONE, // ╖

    // 184, 0xb8
    KEY_NONE,       KEY_NONE, // ╕
    KEY_NONE,       KEY_NONE, // ╣
    KEY_NONE,       KEY_NONE, // ║
    KEY_NONE,       KEY_NONE, // ╗

    // 188, 0xbc
    KEY_NONE,       KEY_NONE, // ╝
    KEY_NONE,       KEY_NONE, // ╜
    KEY_NONE,       KEY_NONE, // ╛
    KEY_NONE,       KEY_NONE, // ┐

    // 192, 0xc0
    KEY_NONE,       KEY_NONE, // └
    KEY_NONE,       KEY_NONE, // ┴
    KEY_NONE,       KEY_NONE, // ┬
    KEY_NONE,       KEY_NONE, // ├

    // 196, 0xc4
    KEY_NONE,       KEY_NONE, // ─
    KEY_NONE,       KEY_NONE, // ┼
    KEY_NONE,       KEY_NONE, // ╞
    KEY_NONE,       KEY_NONE, // ╟

    // 200, 0xc8
    KEY_NONE,       KEY_NONE, // ╚
    KEY_NONE,       KEY_NONE, // ╔
    KEY_NONE,       KEY_NONE, // ╩
    KEY_NONE,       KEY_NONE, // ╦

    // 204, 0xcc
    KEY_NONE,       KEY_NONE, // ╠
    KEY_NONE,       KEY_NONE, // ═
    KEY_NONE,       KEY_NONE, // ╬
    KEY_NONE,       KEY_NONE, // ╧

    // 208, 0xd0
    KEY_NONE,       KEY_NONE, // ╨
    KEY_NONE,       KEY_NONE, // ╤
    KEY_NONE,       KEY_NONE, // ╥
    KEY_NONE,       KEY_NONE, // ╙

    // 212, 0xd4
    KEY_NONE,       KEY_NONE, // ╘
    KEY_NONE,       KEY_NONE, // ╒
    KEY_NONE,       KEY_NONE, // ╓
    KEY_NONE,       KEY_NONE, // ╫

    // 216, 0xd8
    KEY_NONE,       KEY_NONE, // ╪
    KEY_NONE,       KEY_NONE, // ┘
    KEY_NONE,       KEY_NONE, // ┌
    KEY_NONE,       KEY_NONE, // █

    // 220, 0xdc
    KEY_NONE,       KEY_NONE, // ▄
    KEY_NONE,       KEY_NONE, // ▌
    KEY_NONE,       KEY_NONE, // ▐
    KEY_NONE,       KEY_NONE, // ▀

    // 224, 0xe0
    KEY_NONE,       KEY_NONE, // α
    KEY_NONE,       KEY_NONE, // ß
    KEY_NONE,       KEY_NONE, // Γ
    KEY_NONE,       KEY_NONE, // π

    // 228, 0xe4
    KEY_NONE,       KEY_NONE, // Σ
    KEY_NONE,       KEY_NONE, // σ
    KEY_NONE,       KEY_NONE, // µ
    KEY_NONE,       KEY_NONE, // τ

    // 232, 0xe8
    KEY_NONE,       KEY_NONE, // Φ
    KEY_NONE,       KEY_NONE, // Θ
    KEY_NONE,       KEY_NONE, // Ω
    KEY_NONE,       KEY_NONE, // δ

    // 236, 0xec
    KEY_NONE,       KEY_NONE, // ∞
    KEY_NONE,       KEY_NONE, // φ
    KEY_NONE,       KEY_NONE, // ε
    KEY_NONE,       KEY_NONE, // ∩

    // 240, 0xf0
    KEY_NONE,       KEY_NONE, // ≡
    KEY_NONE,       KEY_NONE, // ±
    KEY_NONE,       KEY_NONE, // ≥
    KEY_NONE,       KEY_NONE, // ≤

    // 244, 0xf4
    KEY_NONE,       KEY_NONE, // ⌠
    KEY_NONE,       KEY_NONE, // ⌡
    KEY_NONE,       KEY_NONE, // ÷
    KEY_NONE,       KEY_NONE, // ≈

    // 248, 0xf8
    KEY_NONE,       KEY_NONE, // °
    KEY_NONE,       KEY_NONE, // ∙
    KEY_NONE,       KEY_NONE, // ·
    KEY_NONE,       KEY_NONE, // √

    // 252, 0xfc
    KEY_NONE,       KEY_NONE, // ⁿ
    KEY_NONE,       KEY_NONE, // ²
    KEY_NONE,       KEY_NONE, // ■
    KEY_NONE,       KEY_NONE  //
};

const uint8_t utf8_de[0*6] PROGMEM = {};

#endif /* ifndef locale_us_h */