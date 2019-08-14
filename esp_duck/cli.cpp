/*
   Copyright (c) 2019 Stefan Kremser
   This software is licensed under the MIT License. See the license file for details.
   Source: github.com/spacehuhn/WiFiDuck
 */

#include "cli.h"

#include "spiffs.h"

#include <SimpleCLI.h>

typedef void (* PrintFunction)(const char* s);

namespace cli {
    // ===== PRIVATE ===== //
    SimpleCLI cli;
    Command   cmd_ls;

    PrintFunction printfunc;

    // PrintFunction for Serial and Web Socket
    void printSerial(const char* s) {
        Serial.print(s);
    }

    void printWS(const char* s) {}

    // Internal print functions
    void print(const char* s) {
        if (printfunc) printfunc(s);
    }

    void println(const char* s) {
        print(s);
        print("\n");
    }

    // CLI callbacks
    void error(cmd_error* e) {
        CommandError cmdError(e); // Create wrapper object

        print("ERROR: ");
        println(cmdError.toString().c_str());

        if (cmdError.hasCommand()) {
            print("Did you mean \"");
            print(cmdError.getCommand().toString().c_str());
            println("\"?");
        }
    }

    void ls(cmd* c) {
        Command  cmd { c };
        Argument arg { cmd.getArg(0) };

        println(spiffs::listDir(arg.getValue()).c_str());
    }

    // ===== PUBLIC ===== //
    void begin() {
        cli.setOnError(error); // Set error Callback
        cmd_ls = cli.addSingleArgCmd("ls", ls);
    }

    void execSerial(const char* input) {
        printfunc = printSerial;

        print("# ");
        println(input);

        cli.parse(input);
    }

    void execWS(const char* input) {
        printfunc = printWS;

        cli.parse(input);
    }
}