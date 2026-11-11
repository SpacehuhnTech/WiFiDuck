"""Run host regression tests with a C++14 compiler, without attached hardware."""

import argparse
import gzip
import os
from pathlib import Path
import re
import shutil
import subprocess
import tempfile

ROOT = Path(__file__).resolve().parent.parent


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--cxx", default=os.environ.get("CXX", "clang++"))
    parser.add_argument("--sanitize", action="store_true",
                        help="Enable AddressSanitizer and UndefinedBehaviorSanitizer")
    args = parser.parse_args()
    compiler = shutil.which(args.cxx)
    if not compiler:
        parser.error("C++ compiler not found; set CXX or pass --cxx /path/to/compiler")

    with tempfile.TemporaryDirectory(prefix="wifiduck-mouse-tests-") as directory:
        build = Path(directory)
        flags = ["-std=c++14", "-I" + str(ROOT / "tests/mocks")]
        if os.name == "nt" and "clang" in Path(compiler).name:
            flags.append("-fuse-ld=lld")
        sanitizer_flags = (["-fsanitize=address,undefined", "-fno-omit-frame-pointer"]
                           if args.sanitize else [])
        flags += sanitizer_flags

        # parser.c has C linkage and must be compiled separately as C.
        obj = build / "parser.o"
        subprocess.run([compiler, "-x", "c", "-c", str(ROOT / "atmega_duck/parser.c"),
                        "-o", str(obj)] + sanitizer_flags, check=True)
        suites = {
            "mouse": ("atmega_duck", ["tests/mouse_test.cpp", "atmega_duck/mouse.cpp",
                                      "atmega_duck/keyboard.cpp", "atmega_duck/duckparser.cpp",
                                      "atmega_duck/com.cpp", str(obj)]),
            "script": ("esp_duck", ["tests/script_test.cpp", "esp_duck/duckscript.cpp"]),
            "settings": ("esp_duck", ["tests/settings_test.cpp", "esp_duck/settings.cpp",
                                        "esp_duck/eeprom.cpp"]),
        }
        for name, (include, sources) in suites.items():
            exe = build / (name + (".exe" if os.name == "nt" else ""))
            subprocess.run([compiler] + flags + ["-DESP8266", "-I" + str(ROOT / include)] +
                           [str(ROOT / source) for source in sources] + ["-o", str(exe)], check=True)
            subprocess.run([str(exe)], check=True)

    header = (ROOT / "esp_duck/webfiles.h").read_text()
    for source in (ROOT / "web").iterdir():
        variable = source.name.replace(".", "_").lower()
        array = re.search(r"const uint8_t " + variable + r"\[\] PROGMEM = \{ (.*?) \};", header).group(1)
        content = gzip.decompress(bytes(int(value, 16) for value in array.split(",")))
        assert content == source.read_text(encoding="utf-8").encode("utf-8"), source.name
    print("All embedded web assets match their source files")
    node = shutil.which("node")
    if node:
        subprocess.run([node, str(ROOT / "tests/web_version_test.js")], check=True)
    else:
        print("Node.js not found; web version display test skipped")


if __name__ == "__main__":
    main()
