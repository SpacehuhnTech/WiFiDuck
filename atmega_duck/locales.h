#ifndef locales_h
#define locales_h

#include "usb_hid_keys.h"

// Modifier(s), Key
const uint8_t locale_us[256*2] PROGMEM = {
    KEY_NONE,       KEY_NONE, // NUL
    KEY_NONE,       KEY_NONE, // SOH
    KEY_NONE,       KEY_NONE, // STX
    KEY_NONE,       KEY_NONE, // ETX
    KEY_NONE,       KEY_NONE, // EOT
    KEY_NONE,       KEY_NONE, // ENQ
    KEY_NONE,       KEY_NONE, // ACK
    KEY_NONE,       KEY_NONE, // BEL

    KEY_NONE,       0x2a,     // BS	Backspace
    KEY_NONE,       0x2b,     // TAB	Tab
    KEY_NONE,       0x28,     // LF	Enter

    KEY_NONE,       KEY_NONE, // VT
    KEY_NONE,       KEY_NONE, // FF
    KEY_NONE,       KEY_NONE, // CR
    KEY_NONE,       KEY_NONE, // SO
    KEY_NONE,       KEY_NONE, // SI
    KEY_NONE,       KEY_NONE, // DEL
    KEY_NONE,       KEY_NONE, // DC1
    KEY_NONE,       KEY_NONE, // DC2
    KEY_NONE,       KEY_NONE, // DC3
    KEY_NONE,       KEY_NONE, // DC4
    KEY_NONE,       KEY_NONE, // NAK
    KEY_NONE,       KEY_NONE, // SYN
    KEY_NONE,       KEY_NONE, // ETB
    KEY_NONE,       KEY_NONE, // CAN
    KEY_NONE,       KEY_NONE, // EM
    KEY_NONE,       KEY_NONE, // SUB
    KEY_NONE,       KEY_NONE, // ESC
    KEY_NONE,       KEY_NONE, // FS
    KEY_NONE,       KEY_NONE, // GS
    KEY_NONE,       KEY_NONE, // RS
    KEY_NONE,       KEY_NONE, // US

    KEY_NONE,       0x2c,     // ' '
    KEY_MOD_LSHIFT, 0x1e,     // !
    KEY_MOD_LSHIFT, 0x34,     // "
    KEY_MOD_LSHIFT, 0x20,     // #

    KEY_MOD_LSHIFT, 0x21,     // $
    KEY_MOD_LSHIFT, 0x22,     // %
    KEY_MOD_LSHIFT, 0x24,     // &
    KEY_NONE,       0x34,     // '

    KEY_MOD_LSHIFT, 0x26,     // (
    KEY_MOD_LSHIFT, 0x27,     // )
    KEY_MOD_LSHIFT, 0x25,     // *
    KEY_MOD_LSHIFT, 0x2e,     // +

    KEY_NONE,       0x36,     // ,
    KEY_NONE,       0x2d,     // -
    KEY_NONE,       0x37,     // .
    KEY_NONE,       0x38,     // /

    KEY_NONE,       0x27,     // 0
    KEY_NONE,       0x1e,     // 1
    KEY_NONE,       0x1f,     // 2
    KEY_NONE,       0x20,     // 3

    KEY_NONE,       0x21,     // 4
    KEY_NONE,       0x22,     // 5
    KEY_NONE,       0x23,     // 6
    KEY_NONE,       0x24,     // 7

    KEY_NONE,       0x25,     // 8
    KEY_NONE,       0x26,     // 9
    KEY_MOD_LSHIFT, 0x33,     // :
    KEY_NONE,       0x33,     // ;

    KEY_MOD_LSHIFT, 0x36,     // <
    KEY_NONE,       0x2e,     // =
    KEY_MOD_LSHIFT, 0x37,     // >
    KEY_MOD_LSHIFT, 0x38,     // ?

    KEY_MOD_LSHIFT, 0x1f,     // @
    KEY_MOD_LSHIFT, 0x04,     // A
    KEY_MOD_LSHIFT, 0x05,     // B
    KEY_MOD_LSHIFT, 0x06,     // C

    KEY_MOD_LSHIFT, 0x07,     // D
    KEY_MOD_LSHIFT, 0x08,     // E
    KEY_MOD_LSHIFT, 0x09,     // F
    KEY_MOD_LSHIFT, 0x0a,     // G

    KEY_MOD_LSHIFT, 0x0b,     // H
    KEY_MOD_LSHIFT, 0x0c,     // I
    KEY_MOD_LSHIFT, 0x0d,     // J
    KEY_MOD_LSHIFT, 0x0e,     // K

    KEY_MOD_LSHIFT, 0x0f,     // L
    KEY_MOD_LSHIFT, 0x10,     // M
    KEY_MOD_LSHIFT, 0x11,     // N
    KEY_MOD_LSHIFT, 0x12,     // O

    KEY_MOD_LSHIFT, 0x13,     // P
    KEY_MOD_LSHIFT, 0x14,     // Q
    KEY_MOD_LSHIFT, 0x15,     // R
    KEY_MOD_LSHIFT, 0x16,     // S

    KEY_MOD_LSHIFT, 0x17,     // T
    KEY_MOD_LSHIFT, 0x18,     // U
    KEY_MOD_LSHIFT, 0x19,     // V
    KEY_MOD_LSHIFT, 0x1a,     // W

    KEY_MOD_LSHIFT, 0x1b,     // X
    KEY_MOD_LSHIFT, 0x1c,     // Y
    KEY_MOD_LSHIFT, 0x1d,     // Z
    KEY_NONE,       0x2f,     // [

    KEY_NONE,       0x31,     // bslash
    KEY_NONE,       0x30,     // ]
    KEY_MOD_LSHIFT, 0x23,     // ^
    KEY_MOD_LSHIFT, 0x2d,     // _

    KEY_NONE,       0x35,     // `
    KEY_NONE,       0x04,     // a
    KEY_NONE,       0x05,     // b
    KEY_NONE,       0x06,     // c

    KEY_NONE,       0x07,     // d
    KEY_NONE,       0x08,     // e
    KEY_NONE,       0x09,     // f
    KEY_NONE,       0x0a,     // g

    KEY_NONE,       0x0b,     // h
    KEY_NONE,       0x0c,     // i
    KEY_NONE,       0x0d,     // j
    KEY_NONE,       0x0e,     // k

    KEY_NONE,       0x0f,     // l
    KEY_NONE,       0x10,     // m
    KEY_NONE,       0x11,     // n
    KEY_NONE,       0x12,     // o

    KEY_NONE,       0x13,     // p
    KEY_NONE,       0x14,     // q
    KEY_NONE,       0x15,     // r
    KEY_NONE,       0x16,     // s

    KEY_NONE,       0x17,     // t
    KEY_NONE,       0x18,     // u
    KEY_NONE,       0x19,     // v
    KEY_NONE,       0x1a,     // w

    KEY_NONE,       0x1b,     // x
    KEY_NONE,       0x1c,     // y
    KEY_NONE,       0x1d,     // z
    KEY_MOD_LSHIFT, 0x2f,     // {

    KEY_MOD_LSHIFT, 0x31,     // |
    KEY_MOD_LSHIFT, 0x30,     // }
    KEY_MOD_LSHIFT, 0x35,     // ~
    KEY_NONE,       KEY_NONE, // DEL


    // ===== Extended ASCII ===== //

    KEY_NONE,       KEY_NONE, // Ç
    KEY_NONE,       KEY_NONE, // ü
    KEY_NONE,       KEY_NONE, // â
    KEY_NONE,       KEY_NONE, // ä

    KEY_NONE,       KEY_NONE, // à
    KEY_NONE,       KEY_NONE, // å
    KEY_NONE,       KEY_NONE, // ç
    KEY_NONE,       KEY_NONE, // ê

    KEY_NONE,       KEY_NONE, // ë
    KEY_NONE,       KEY_NONE, // è
    KEY_NONE,       KEY_NONE, // ï
    KEY_NONE,       KEY_NONE, // î

    KEY_NONE,       KEY_NONE, // ì
    KEY_NONE,       KEY_NONE, // Ä
    KEY_NONE,       KEY_NONE, // Å
    KEY_NONE,       KEY_NONE, // É

    KEY_NONE,       KEY_NONE, // æ
    KEY_NONE,       KEY_NONE, // Æ
    KEY_NONE,       KEY_NONE, // ô
    KEY_NONE,       KEY_NONE, // ö

    KEY_NONE,       KEY_NONE, // ò
    KEY_NONE,       KEY_NONE, // û
    KEY_NONE,       KEY_NONE, // ù
    KEY_NONE,       KEY_NONE, // ÿ

    KEY_NONE,       KEY_NONE, // Ö
    KEY_NONE,       KEY_NONE, // Ü
    KEY_NONE,       KEY_NONE, // ¢
    KEY_NONE,       KEY_NONE, // £

    KEY_NONE,       KEY_NONE, // ¥
    KEY_NONE,       KEY_NONE, // ₧
    KEY_NONE,       KEY_NONE, // ƒ
    KEY_NONE,       KEY_NONE, // á

    KEY_NONE,       KEY_NONE, // í
    KEY_NONE,       KEY_NONE, // ó
    KEY_NONE,       KEY_NONE, // ú
    KEY_NONE,       KEY_NONE, // ñ

    KEY_NONE,       KEY_NONE, // Ñ
    KEY_NONE,       KEY_NONE, // ª
    KEY_NONE,       KEY_NONE, // º
    KEY_NONE,       KEY_NONE, // ¿

    KEY_NONE,       KEY_NONE, // ⌐
    KEY_NONE,       KEY_NONE, // ¬
    KEY_NONE,       KEY_NONE, // ½
    KEY_NONE,       KEY_NONE, // ¼

    KEY_NONE,       KEY_NONE, // ¡
    KEY_NONE,       KEY_NONE, // «
    KEY_NONE,       KEY_NONE, // »
    KEY_NONE,       KEY_NONE, // ░

    KEY_NONE,       KEY_NONE, // ▒
    KEY_NONE,       KEY_NONE, // ▒
    KEY_NONE,       KEY_NONE, // ▓
    KEY_NONE,       KEY_NONE, // │

    KEY_NONE,       KEY_NONE, // ┤
    KEY_NONE,       KEY_NONE, // ╡
    KEY_NONE,       KEY_NONE, // ╢
    KEY_NONE,       KEY_NONE, // ╖

    KEY_NONE,       KEY_NONE, // ╕
    KEY_NONE,       KEY_NONE, // ╣
    KEY_NONE,       KEY_NONE, // ║
    KEY_NONE,       KEY_NONE, // ╗

    KEY_NONE,       KEY_NONE, // ╝
    KEY_NONE,       KEY_NONE, // ╜
    KEY_NONE,       KEY_NONE, // ╛
    KEY_NONE,       KEY_NONE, // ┐

    KEY_NONE,       KEY_NONE, // └
    KEY_NONE,       KEY_NONE, // ┴
    KEY_NONE,       KEY_NONE, // ┬
    KEY_NONE,       KEY_NONE, // ├

    KEY_NONE,       KEY_NONE, // ─
    KEY_NONE,       KEY_NONE, // ┼
    KEY_NONE,       KEY_NONE, // ╞
    KEY_NONE,       KEY_NONE, // ╟

    KEY_NONE,       KEY_NONE, // ╚
    KEY_NONE,       KEY_NONE, // ╔
    KEY_NONE,       KEY_NONE, // ╩
    KEY_NONE,       KEY_NONE, // ╦

    KEY_NONE,       KEY_NONE, // ╠
    KEY_NONE,       KEY_NONE, // ═
    KEY_NONE,       KEY_NONE, // ╬
    KEY_NONE,       KEY_NONE, // ╧

    KEY_NONE,       KEY_NONE, // ╨
    KEY_NONE,       KEY_NONE, // ╤
    KEY_NONE,       KEY_NONE, // ╥
    KEY_NONE,       KEY_NONE, // ╙

    KEY_NONE,       KEY_NONE, // ╘
    KEY_NONE,       KEY_NONE, // ╒
    KEY_NONE,       KEY_NONE, // ╓
    KEY_NONE,       KEY_NONE, // ╫

    KEY_NONE,       KEY_NONE, // ╪
    KEY_NONE,       KEY_NONE, // ┘
    KEY_NONE,       KEY_NONE, // ┌
    KEY_NONE,       KEY_NONE, // █

    KEY_NONE,       KEY_NONE, // ▄
    KEY_NONE,       KEY_NONE, // ▌
    KEY_NONE,       KEY_NONE, // ▐
    KEY_NONE,       KEY_NONE, // ▀

    KEY_NONE,       KEY_NONE, // α
    KEY_NONE,       KEY_NONE, // ß
    KEY_NONE,       KEY_NONE, // Γ
    KEY_NONE,       KEY_NONE, // π

    KEY_NONE,       KEY_NONE, // Σ
    KEY_NONE,       KEY_NONE, // σ
    KEY_NONE,       KEY_NONE, // µ
    KEY_NONE,       KEY_NONE, // τ

    KEY_NONE,       KEY_NONE, // Φ
    KEY_NONE,       KEY_NONE, // Θ
    KEY_NONE,       KEY_NONE, // Ω
    KEY_NONE,       KEY_NONE, // δ

    KEY_NONE,       KEY_NONE, // ∞
    KEY_NONE,       KEY_NONE, // φ
    KEY_NONE,       KEY_NONE, // ε
    KEY_NONE,       KEY_NONE, // ∩

    KEY_NONE,       KEY_NONE, // ≡
    KEY_NONE,       KEY_NONE, // ±
    KEY_NONE,       KEY_NONE, // ≥
    KEY_NONE,       KEY_NONE, // ≤

    KEY_NONE,       KEY_NONE, // ⌠
    KEY_NONE,       KEY_NONE, // ⌡
    KEY_NONE,       KEY_NONE, // ÷
    KEY_NONE,       KEY_NONE, // ≈

    KEY_NONE,       KEY_NONE, // °
    KEY_NONE,       KEY_NONE, // ∙
    KEY_NONE,       KEY_NONE, // ·
    KEY_NONE,       KEY_NONE, // √

    KEY_NONE,       KEY_NONE, // ⁿ
    KEY_NONE,       KEY_NONE, // ²
    KEY_NONE,       KEY_NONE, // ■
    KEY_NONE,       KEY_NONE  //
};

#endif /* ifndef locales_h */