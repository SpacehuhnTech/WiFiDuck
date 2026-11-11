#include "duckscript.h"
#include "spiffs.h"
#include "com.h"
#include <cassert>
#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>
#include <iterator>

unsigned long testTime = 1000;
std::vector<std::string> messages;
int resets = 0;
std::string contents;

namespace spiffs {
    File open(String name) { return File(name, contents); }
}
namespace com {
    unsigned int send(const char* data, unsigned int length) {
        messages.emplace_back(data, length);
        return length;
    }
    void reset() { ++resets; }
}

void finish(size_t limit = 1000) {
    while (duckscript::isRunning() && limit--) duckscript::nextLine();
    assert(!duckscript::isRunning());
}

size_t count(const std::string& command) {
    return std::count(messages.begin(), messages.end(), command);
}

int main(int argc, char* argv[]) {
    contents = "MOUSEPRESS LEFT\n";
    duckscript::run("/mouse.duck");
    assert(duckscript::isRunning());
    assert(messages.back() == contents);
    duckscript::nextLine(); // End of file releases held inputs.
    assert(!duckscript::isRunning());
    assert(resets == 1);
    size_t sent = messages.size();
    duckscript::repeat();
    assert(messages.size() == sent);
    duckscript::stopAll();
    assert(resets == 1);

    contents = "MOUSEPRESS LEFT\nDELAY 1000\n";
    duckscript::run("/mouse.duck");
    duckscript::stop("/other.duck");
    assert(duckscript::isRunning());
    assert(resets == 1);
    duckscript::stop("/mouse.duck");
    assert(!duckscript::isRunning());
    assert(resets == 2);
    sent = messages.size();
    duckscript::repeat();
    duckscript::nextLine();
    assert(messages.size() == sent);

    duckscript::run("/mouse.duck");
    duckscript::stop("");
    assert(!duckscript::isRunning());
    assert(resets == 3);

    contents = "MOUSEMOVE 30 0\nREPEAT 3\n";
    duckscript::run("/mouse.duck");
    duckscript::nextLine();
    assert(messages.back() == "REPEAT 3\n");
    duckscript::repeat();
    assert(messages.back() == "MOUSEMOVE 30 0\n");
    duckscript::stopAll();

    // Finite counts are total iterations, and nested blocks rewind correctly.
    messages.clear();
    contents = "LOOP 3\nMOUSEMOVE 1 0\nMOUSEMOVE -1 0\nENDLOOP\nLED 0 25 0\n";
    duckscript::run("/loop.duck");
    finish();
    assert(count("MOUSEMOVE 1 0\n") == 3);
    assert(count("MOUSEMOVE -1 0\n") == 3);
    assert(count("LED 0 25 0\n") == 1);
    assert(!duckscript::getError());
    messages.clear();
    contents = "LOOP 2\nLOOP 3\nMOUSEMOVE 2 0\nENDLOOP\nMOUSEMOVE -6 0\nENDLOOP\n";
    duckscript::run("/nested.duck");
    finish();
    assert(count("MOUSEMOVE 2 0\n") == 6);
    assert(count("MOUSEMOVE -6 0\n") == 2);

    // Infinite loops stay running and stop without another movement. Cleanup
    // restores the normal LED after the reset that releases keys and buttons.
    messages.clear();
    contents = "ONSTOP LED 0 25 0\nLOOP 0\nLED 0 0 25\nMOUSEMOVE 1 0\nLED 0 25 0\nDELAY 55000\nENDLOOP\n";
    duckscript::run("/infinite.duck");
    for (int i = 0; i < 50; ++i) duckscript::nextLine();
    assert(duckscript::isRunning());
    assert(count("MOUSEMOVE 1 0\n") > 1);
    assert(count("LED 0 0 25\n") == count("MOUSEMOVE 1 0\n"));
    sent = messages.size();
    int oldResets = resets;
    duckscript::stopAll();
    assert(resets == oldResets + 1);
    assert(messages.back() == "LED 0 25 0\n");
    assert(messages.size() == sent + 1);
    duckscript::repeat();
    duckscript::nextLine();
    assert(messages.size() == sent + 1);

    // Cleanup also runs at EOF, errors, and when another script starts. It
    // remains opt-in, and a subsequent script can't inherit old repeat state.
    contents = "ONSTOP LED 7 8 9\nLED 20 0 0\n";
    duckscript::run("/cleanup.duck");
    finish();
    assert(messages.back() == "LED 7 8 9\n");
    contents = "ONSTOP LED 7 8 9\nLOOP 0\nDELAY 100\nENDLOOP\n";
    duckscript::run("/cleanup.duck");
    contents = "MOUSEMOVE 1 0\n";
    duckscript::run("/replacement.duck");
    assert(messages[messages.size() - 2] == "LED 7 8 9\n");
    finish();
    assert(messages.back() == "MOUSEMOVE 1 0\n");
    contents = "REPEAT 3\n";
    duckscript::run("/repeat-only.duck");
    duckscript::repeat();
    assert(!duckscript::isRunning());
    assert(duckscript::getError());

    // REPLAY is the documented REPEAT alias; controls don't replace its target.
    contents = "LOOP 1\nMOUSEMOVE 3 0\nREPLAY 2\nENDLOOP\n";
    duckscript::run("/replay.duck");
    duckscript::nextLine();
    duckscript::nextLine();
    assert(messages.back() == "REPLAY 2\n");
    duckscript::repeat();
    assert(messages.back() == "MOUSEMOVE 3 0\n");
    duckscript::stopAll();

    // CRLF, whitespace, and an unterminated ENDLOOP are supported.
    messages.clear();
    contents = "  LOOP 2\r\nMOUSEMOVE 4 0\r\n ENDLOOP";
    duckscript::run("/crlf.duck");
    finish();
    assert(count("MOUSEMOVE 4 0\r\n") == 2);
    assert(!duckscript::getError());

    // Invalid control syntax stops rather than becoming keyboard input.
    for (const char* invalid : {
        "LOOP\n", "LOOP -1\n", "LOOP 4294967296\n", "LOOP 1 extra\n",
        "ENDLOOP\n", "LOOP 1\nENDLOOP 2\n", "LOOP 1\nREM no end\n",
        "LOOP 0\nENDLOOP\n", "ONSTOP LED 256 0 0\n", "ONSTOP LED 1 2\n",
        "ONSTOP LED 1 2 3 4\n", "ONSTOP STRING text\n"
    }) {
        messages.clear();
        contents = invalid;
        duckscript::run("/invalid.duck");
        finish();
        assert(duckscript::getError());
        for (const auto& msg : messages) assert(msg == "REM\n" || msg == "REM no end\n");
    }
    contents = "ONSTOP LED 0 25 0\nENDLOOP\n";
    duckscript::run("/invalid-cleanup.duck");
    finish();
    assert(duckscript::getError());
    assert(messages.back() == "LED 0 25 0\n");
    contents.clear();
    for (int i = 0; i < 9; ++i) contents += "LOOP 1\n";
    duckscript::run("/too-deep.duck");
    finish();
    assert(duckscript::getError());

    // Control-like text in STRING/REM and continuation chunks stays literal.
    messages.clear();
    contents = "STRING LOOP 0\nREM ENDLOOP\n";
    duckscript::run("/literal.duck");
    finish();
    assert(messages[0] == "STRING LOOP 0\n");
    assert(messages[1] == "REM ENDLOOP\n");
    contents = "STRING " + std::string(249, 'a') + "LOOP 0\n";
    messages.clear();
    duckscript::run("/long.duck");
    finish();
    assert(messages.size() == 2);
    assert(messages[1] == "LOOP 0\n");
    assert(!duckscript::getError());

    // Exercise the shipped example through the real script engine, including
    // the first rewind, its timing, and cleanup during the next active burst.
    if (argc > 1) {
        std::ifstream example(argv[1]);
        assert(example.good());
        contents.assign(std::istreambuf_iterator<char>(example), std::istreambuf_iterator<char>());
        messages.clear();
        duckscript::run("/mouse_wiggle");
        size_t limit = 100;
        while (count("LED 0 0 25\n") < 2 && limit--) {
            duckscript::nextLine();
            // Supply the AVR's repeat callbacks as it finishes each delay.
            if (messages.back().find("REPEAT ") == 0) {
                unsigned int repeats = (unsigned int)std::stoul(messages.back().substr(7));
                for (unsigned int i = 0; i < repeats; ++i) duckscript::repeat();
            }
        }
        assert(limit && duckscript::isRunning());
        unsigned int active = 0, rest = 0;
        int dx = 0, dy = 0;
        bool moving = false;
        for (const auto& msg : messages) {
            if (msg == "LED 0 0 25\n") moving = true;
            else if (msg == "LED 0 25 0\n") moving = false;
            else if (msg.find("DELAY ") == 0) {
                unsigned int delay = (unsigned int)std::stoul(msg.substr(6));
                if (moving) active += delay;
                else rest += delay;
            } else if (msg.find("MOUSEMOVE ") == 0) {
                int x, y;
                assert(sscanf(msg.c_str(), "MOUSEMOVE %d %d", &x, &y) == 2);
                dx += x;
                dy += y;
            }
            assert(msg.find("LOOP ") != 0 && msg.find("ENDLOOP") != 0 && msg.find("ONSTOP") != 0);
        }
        assert(active == 5000 && rest == 55000);
        assert(dx == 0 && dy == 0);
        duckscript::stopAll();
        assert(messages.back() == "LED 0 25 0\n");
    }

    std::cout << "Script loops, cleanup, completion, stop, and repeat tests passed\n";
}
