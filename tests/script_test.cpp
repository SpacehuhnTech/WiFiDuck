#include "duckscript.h"
#include "spiffs.h"
#include "com.h"
#include <cassert>
#include <iostream>
#include <vector>

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

int main() {
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

    std::cout << "Script completion, stop, and repeat tests passed\n";
}
