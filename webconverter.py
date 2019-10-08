"""
   Copyright (c) 2019 Stefan Kremser
   This software is licensed under the MIT License. See the license file for details.
   Source: github.com/spacehuhn/WiFiDuck
"""

import os
import binascii
import gzip

web_files = os.listdir("web/")

outputfile = open("esp_duck/webfiles.h", "w+")
outputfile.write("#pragma once\n\n")

def get_file_content(path):
    file = open(path,"r")
    content = file.read().encode("utf-8")
    file.close();

    gzip_content = gzip.compress(content)

    print(f"({len(gzip_content)} / {len(content)})", end="")

    return gzip_content

def filename_to_varname(filename):
    return filename.replace(".","_").lower()

def build_hex_string(varname, content):
    hexstr = f"const uint8_t {varname}[] PROGMEM = {{ "

    for c in content:
        hexstr += f"{hex(c)},"

    hexstr = hexstr[:-1]
    hexstr += " };\n\n"

    return hexstr

def main():
    for filename in web_files:
        print(f"Converting {filename}...", end="")

        path = f"web/{filename}"
        content = get_file_content(path)
        varname = filename_to_varname(filename)

        output = build_hex_string(varname, content)

        outputfile.write(output)
        outputfile.flush()
        """
        print()
        print()
        print(content)
        print()
        print()
        print(output)
        print()
        print()
        """
        print("OK")

    outputfile.close()

if __name__== "__main__":
  main()
