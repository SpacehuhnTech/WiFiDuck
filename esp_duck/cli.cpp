/*
   Copyright (c) 2019 Stefan Kremser
   This software is licensed under the MIT License. See the license file for details.
   Source: github.com/spacehuhn/WiFiDuck
 */

#include "cli.h"

#include "spiffs.h"

#include <SimpleCLI.h>

namespace cli {
    SimpleCLI cli;
    Command   cmd_ls;

    void error(cmd_error* e) {
        CommandError cmdError(e); // Create wrapper object

        Serial.print("ERROR: ");
        Serial.println(cmdError.toString());

        if (cmdError.hasCommand()) {
            Serial.print("Did you mean \"");
            Serial.print(cmdError.getCommand().toString());
            Serial.println("\"?");
        }
    }

    void ls(cmd* c) {
        Command  cmd { c };
        Argument arg { cmd.getArg(0) };

        Serial.print("ls ");
        Serial.print(arg.getValue());
        Serial.println(":");

        Serial.println(spiffs::listDir(arg.getValue()));
    }

    void begin() {
        cli.setOnError(error); // Set error Callback
        cmd_ls = cli.addSingleArgCmd("ls", ls);
    }

    void parse(String input /*, printfunc*/) {
        cli.parse(input);
    }
}