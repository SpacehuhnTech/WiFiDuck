"""
   This software is licensed under the MIT License. See the license file for details.
   Source: https://github.com/spacehuhntech/WiFiDuck
"""

import os
import binascii
import gzip

def get_file_content(path):
    file = open(path,"r")
    content = file.read().encode("utf-8")
    file.close();

    gzip_content = gzip.compress(content)

    print(f"({len(content)} -> {len(gzip_content)} byte)...", end="")

    return gzip_content

def get_varname(filename):
    return filename.replace(".","_").lower()

def get_file_type(filename):
    file_ending = filename.split('.')[1]
    if file_ending == "js":
        return "application/javascript"
    elif file_ending == "css":
        return "text/css"
    elif file_ending == "html":
        return "text/html"
    else:
        return "text/plain"

def get_response_code(filename):
    if filename == "error404.html":
        return 404
    else:
        return 200

def build_hex_string(varname, content):
    hexstr = f"const uint8_t {varname}[] PROGMEM = {{ "

    for c in content:
        hexstr += f"{hex(c)},"

    hexstr = hexstr[:-1]
    hexstr += " };\n\n"

    return hexstr

def write_server_callback(filename, output):
    varname = get_varname(filename)
    filetype = get_file_type(filename)
    response_code = get_response_code(filename)

    output.write(f"\\\nserver.on(\"/{filename}\", HTTP_GET, [](AsyncWebServerRequest* request) {{")
    output.write(f"\\\n\treply(request, {response_code}, \"{filetype}\", {varname}, sizeof({varname}));")
    output.write(f"\\\n}});")

def write_hex_array(filename, output):
    print(f"Converting {filename}...", end="")

    path = f"web/{filename}"
    content = get_file_content(path)
    varname = get_varname(filename)

    hex_array = build_hex_string(varname, content)

    output.write(hex_array)

    print("OK")

def write_callbacks(files, output):
    for filename in files:
        write_server_callback(filename, output);

def write_arrays(files, output):
    for filename in files:
        write_hex_array(filename, output)

def main():
    web_files = os.listdir("web/")

    outputfile = open("esp_duck/webfiles.h", "w+")
    outputfile.write("#pragma once\n\n")

    outputfile.write(f"#define WEBSERVER_CALLBACK ")

    write_callbacks(web_files, outputfile)

    outputfile.write("\n\n")

    write_arrays(web_files, outputfile)

    outputfile.close()

if __name__== "__main__":
  main()
