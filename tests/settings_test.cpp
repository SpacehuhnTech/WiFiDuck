#include "settings.h"
#include <EEPROM.h>
#include <cassert>
#include <iostream>

unsigned long testTime = 1000;
TestEEPROM EEPROM;

// The old fork's persisted settings have the same layout as upstream autorun.
struct ForkSettings {
    uint32_t magic_num;
    char ssid[33];
    char password[65];
    char channel[5];
    char startup[65];
};

int main() {
    ForkSettings legacy { 1234567891, "My WiFi Duck", "my password", "6", "/old.duck" };
    EEPROM.put(1, legacy);
    settings::begin();
    assert(std::string(settings::getSSID()) == "My WiFi Duck");
    assert(std::string(settings::getPassword()) == "my password");
    assert(settings::getChannelNum() == 6);
    assert(std::string(settings::getStartup()) == "/old.duck");
    assert(std::string(settings::getAutorun()) == "/old.duck");

    settings::set("startup", "/next.duck");
    settings::load();
    assert(std::string(settings::getAutorun()) == "/next.duck");
    settings::set("autorun", "/latest.duck");
    settings::load();
    assert(std::string(settings::getStartup()) == "/latest.duck");
    settings::setStartup("/api.duck");
    settings::load();
    assert(std::string(settings::getAutorun()) == "/api.duck");
    assert(settings::toString().find("autorun=/api.duck\n") != std::string::npos);

    settings::set("ssid", "SSID with spaces");
    settings::set("password", "password with spaces");
    settings::load();
    assert(std::string(settings::getSSID()) == "SSID with spaces");
    assert(std::string(settings::getPassword()) == "password with spaces");
    settings::reset();
    settings::load();
    assert(std::string(settings::getAutorun()).empty());
    assert(std::string(settings::getSSID()) == "wifiduck");

    std::cout << "Fork settings migration, startup alias, and persistence tests passed\n";
}
