/*
   Copyright (c) 2019 Stefan Kremser
   This software is licensed under the MIT License. See the license file for details.
   Source: github.com/spacehuhn/WiFiDuck
 */

#include "cli.h"

#include "spiffs.h"
#include "duckscript.h"

#include <SimpleCLI.h>

namespace cli {
    // ===== PRIVATE ===== //
    SimpleCLI cli;

    PrintFunction printfunc;

    // Internal print functions
    void print(const char* s) {
        if (printfunc) printfunc(s);
    }

    void println(const char* s) {
        print(s);
        print("\n");
    }

    // ===== PUBLIC ===== //
    void begin() {
        cli.setOnError([](cmd_error* e) {
            CommandError cmdError(e); // Create wrapper object

            print("ERROR: ");
            println(cmdError.toString().c_str());

            if (cmdError.hasCommand()) {
                print("Did you mean \"");
                print(cmdError.getCommand().toString().c_str());
                println("\"?");
            }
        });

        cli.addCommand("help", [](cmd* c) {
            println(cli.toString().c_str());
        });

        cli.addSingleArgCmd("ls", [](cmd* c) {
            Command  cmd { c };
            Argument arg { cmd.getArg(0) };

            println(spiffs::listDir(arg.getValue()).c_str());
        });

        cli.addCommand("mem", [](cmd* c) {
            String s { NULL };
            s.reserve(64);

            s  = String(spiffs::size());
            s += " byte\n";
            s += String(spiffs::usedBytes());
            s += " byte used\n";
            s += String(spiffs::freeBytes());
            s += " byte free\n";

            print(s.c_str());
        });

        cli.addSingleArgCmd("cat", [](cmd* c) {
            Command  cmd { c };
            Argument arg { cmd.getArg(0) };

            File f = spiffs::open(arg.getValue());

            char buffer[32];

            while (f && f.available()) {
                for (size_t i = 0; i<32; ++i) {
                    if (!f.available() || (i == 31)) {
                        buffer[i] = '\0';
                        i         = 32;
                    } else {
                        buffer[i] = f.read();
                    }
                }
                print(buffer);
            }
            println("");
        });

        cli.addSingleArgCmd("run", [](cmd* c) {
            Command  cmd { c };
            Argument arg { cmd.getArg(0) };

            duckscript::run(arg.getValue());
        });

        cli.addSingleArgCmd("create", [](cmd* c) {
            Command  cmd { c };
            Argument arg { cmd.getArg(0) };

            spiffs::create(arg.getValue());
        });

        cli.addSingleArgCmd("remove", [](cmd* c) {
            Command  cmd { c };
            Argument arg { cmd.getArg(0) };

            spiffs::remove(arg.getValue());
        });

        Command cmdRename {
            cli.addCommand("rename", [](cmd* c) {
                Command  cmd { c };

                Argument argA { cmd.getArg(0) };
                Argument argB { cmd.getArg(1) };

                String fileA { argA.getValue() };
                String fileB { argB.getValue() };

                spiffs::rename(fileA, fileB);
            })
        };
        cmdRename.addPosArg("fileA,a");
        cmdRename.addPosArg("fileB,b");

        Command cmdWrite {
            cli.addCommand("write", [](cmd* c) {
                Command  cmd { c };

                Argument argFileName { cmd.getArg(0) };
                Argument argContent { cmd.getArg(1) };

                String fileName { argFileName.getValue() };
                String content { argContent.getValue() };

                spiffs::write(fileName, (uint8_t*)content.c_str(), content.length());
            })
        };
        cmdWrite.addPosArg("f/ile");
        cmdWrite.addPosArg("c/ontent");
    }

    void parse(const char* input, PrintFunction printfunc) {
        cli::printfunc = printfunc;

        print("# ");
        println(input);

        cli.parse(input);
    }
}