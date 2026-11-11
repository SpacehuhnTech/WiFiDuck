#include "keyboard.h"
#include "mouse.h"
#include "duckparser.h"
#include "com.h"
#include <Wire.h>
#include <cassert>
#include <iostream>
#include <initializer_list>

unsigned long testTime = 1000;
TestHID testHID;
TestWire Wire;
int ledR = 0, ledG = 0, ledB = 0;
namespace led { void setColor(int r, int g, int b) { ledR = r; ledG = g; ledB = b; } }

void parse(const std::string& text) {
    // Pass a buffer with no terminator to exercise the real transport contract.
    std::vector<char> buffer(text.begin(), text.end());
    duckparser::parse(buffer.data(), buffer.size());
}

void report(size_t index, std::initializer_list<uint8_t> data, uint8_t id = 1) {
    assert(index < testHID.reports.size());
    assert(testHID.reports[index].id == id);
    assert(testHID.reports[index].data == std::vector<uint8_t>(data));
}

void clear() {
    duckparser::reset();
    testHID.reports.clear();
}

void frame(const std::string& text) {
    std::vector<uint8_t> bytes { 0x01 };
    bytes.insert(bytes.end(), text.begin(), text.end());
    bytes.push_back(0x04);
    Wire.deliver(bytes);
    com::update();
    if (com::hasData()) {
        const buffer_t& buffer = com::getBuffer();
        duckparser::parse(buffer.data, buffer.len);
        com::sendDone();
    }
}

int main() {
    keyboard::begin();
    mouse::begin();
    com::begin();
    assert(testHID.descriptors.size() == 2);
    assert(testHID.descriptors[0]->data[7] == 2);
    assert(testHID.descriptors[1]->data[7] == 1);

    // Signed values, optional wheel, multiple lines, CRLF, and an unterminated line.
    clear();
    parse("MOUSEMOVE -127 +127 -3\r\nMOUSEMOVE 0 -0\nMOUSESCROLL 127");
    assert(testHID.reports.size() == 3);
    report(0, { 0, 129, 127, 253 });
    report(1, { 0, 0, 0, 0 });
    report(2, { 0, 0, 0, 127 });

    // No invalid argument may wrap around or become a keyboard keystroke.
    clear();
    for (const char* line : {
        "MOUSEMOVE", "MOUSEMOVE 1", "MOUSEMOVE 128 0", "MOUSEMOVE -128 0",
        "MOUSEMOVE 1 2 3 4", "MOUSEMOVE 1 2 128", "MOUSEMOVE --1 0",
        "MOUSEMOVE - 0", "MOUSEMOVE 1.5 0", "MOUSEMOVE 0x10 0",
        "MOUSEMOVE 9999999999999999999999 0", "MOUSEMOVE nope 0",
        "MOUSESCROLL", "MOUSESCROLL 1 2", "MOUSESCROLL 128",
        "MOUSECLICK invalid", "MOUSEPRESS LEFT RIGHT", "MOUSERELEASE invalid"
    }) parse(std::string(line) + "\n");
    assert(testHID.reports.empty());

    clear();
    parse("MOUSECLICK\nMOUSECLICK right\nMOUSECLICK MIDDLE\n");
    assert(testHID.reports.size() == 6);
    report(0, { 1, 0, 0, 0 }); report(1, { 0, 0, 0, 0 });
    report(2, { 2, 0, 0, 0 }); report(3, { 0, 0, 0, 0 });
    report(4, { 4, 0, 0, 0 }); report(5, { 0, 0, 0, 0 });

    // Drag across parse calls, preserve other buttons during clicks, release one/all.
    clear();
    parse("MOUSEPRESS\n");
    parse("MOUSEMOVE 30 0\nMOUSECLICK RIGHT\nMOUSEPRESS MIDDLE\nMOUSERELEASE LEFT\nMOUSERELEASE\n");
    assert(testHID.reports.size() == 7);
    report(0, { 1, 0, 0, 0 }); report(1, { 1, 30, 0, 0 });
    report(2, { 3, 0, 0, 0 }); report(3, { 1, 0, 0, 0 });
    report(4, { 5, 0, 0, 0 }); report(5, { 4, 0, 0, 0 });
    report(6, { 0, 0, 0, 0 });

    clear();
    parse("MOUSEPRESS LEFT\nMOUSECLICK LEFT\nMOUSEMOVE 1 0\n");
    assert(testHID.reports.size() == 5);
    report(1, { 0, 0, 0, 0 }); report(2, { 1, 0, 0, 0 });
    report(3, { 0, 0, 0, 0 }); report(4, { 0, 1, 0, 0 });

    // Mouse commands get default delays and repeat counts like existing commands.
    clear();
    unsigned long start = testTime;
    parse("DEFAULTDELAY 25\nMOUSEMOVE 1 2\nREPEAT 2\n");
    assert(testTime == start + 25);
    assert(duckparser::getRepeats() == 2);
    parse("MOUSEMOVE 1 2\nMOUSEMOVE 1 2\n");
    assert(duckparser::getRepeats() == 0);
    assert(testTime == start + 75);
    assert(testHID.reports.size() == 3);

    // Keyboard chords and strings retain keyboard report ID 2.
    clear();
    parse("GUI r\n");
    assert(testHID.reports.size() == 3);
    report(1, { KEY_MOD_LMETA, 0, KEY_R, 0, 0, 0, 0, 0 }, 2);
    report(2, { 0, 0, 0, 0, 0, 0, 0, 0 }, 2);
    clear();
    parse("STRING MOUSECLICK LEFT\n");
    assert(testHID.reports.size() == 30);
    for (const auto& item : testHID.reports) assert(item.id == 2);
    clear();
    parse("REM MOUSECLICK LEFT\n   \nLOCALE\n");
    assert(testHID.reports.empty());

    // Reset is a transport control frame, including during STRING/REM continuation.
    for (const char* continuation : { "STRING unfinished", "REM unfinished", "GUI" }) {
        clear();
        frame("MOUSEPRESS LEFT\nREPEAT 2\n");
        frame(continuation);
        testHID.reports.clear();
        frame(std::string(1, '\x03'));
        assert(!com::hasData());
        assert(duckparser::getRepeats() == 0);
        assert(testHID.reports.size() == 2);
        report(0, { 0, 0, 0, 0, 0, 0, 0, 0 }, 2);
        report(1, { 0, 0, 0, 0 });
        frame("MOUSECLICK\n");
        assert(testHID.reports.size() == 4);
        report(2, { 1, 0, 0, 0 }); report(3, { 0, 0, 0, 0 });
    }

    // Back-to-back frames can arrive while a command is running. Keep the reset
    // and the following command queued instead of dropping either at EOT.
    clear();
    const std::string queued = std::string("\x01MOUSEPRESS LEFT\n\x04") +
                               "\x01\x03\x04" + "\x01MOUSEMOVE 2 0\n\x04";
    Wire.deliver(std::vector<uint8_t>(queued.begin(), queued.end()));
    com::update();
    assert(com::hasData());
    const buffer_t& held = com::getBuffer();
    duckparser::parse(held.data, held.len);
    com::sendDone();
    com::update(); // Reset frame.
    assert(!com::hasData());
    com::update(); // Next command is still queued.
    assert(com::hasData());
    const buffer_t& next = com::getBuffer();
    duckparser::parse(next.data, next.len);
    com::sendDone();
    assert(testHID.reports.size() == 4);
    report(0, { 1, 0, 0, 0 });
    report(1, { 0, 0, 0, 0, 0, 0, 0, 0 }, 2);
    report(2, { 0, 0, 0, 0 });
    report(3, { 0, 2, 0, 0 });

    // An ESP ONSTOP cleanup queues LED restoration behind the reset. Both
    // must reach the AVR even when the previous command was a held input.
    clear();
    frame("LED 0 0 25\nMOUSEPRESS LEFT\n");
    const std::string cleanup = std::string("\x01\x03\x04") + "\x01LED 0 25 0\n\x04";
    Wire.deliver(std::vector<uint8_t>(cleanup.begin(), cleanup.end()));
    com::update();
    assert(!com::hasData());
    assert(ledB == 25); // Reset retains ordinary scripts' LED behavior.
    com::update();
    assert(com::hasData());
    const buffer_t& color = com::getBuffer();
    duckparser::parse(color.data, color.len);
    com::sendDone();
    assert(ledR == 0 && ledG == 25 && ledB == 0);

    std::cout << "Mouse HID, parser, keyboard compatibility, and reset tests passed\n";
}
