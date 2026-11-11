"""Import pinned DSTIKE board definitions from the official Arduino packages."""

import hashlib
import json
from pathlib import Path
import urllib.request
import zipfile

ROOT = Path(__file__).resolve().parent.parent
INDEX = "https://raw.githubusercontent.com/SpacehuhnTech/arduino/main/package_spacehuhn_index.json"
VERSIONS = {"avr": "1.8.5", "esp8266": "2.7.4"}


def properties(text):
    result = {}
    for line in text.splitlines():
        if line.startswith("dstike.") and "=" in line:
            key, value = line.split("=", 1)
            result[key.removeprefix("dstike.")] = value
    return result


def save_json(path, data):
    path.parent.mkdir(parents=True, exist_ok=True)
    path.write_text(json.dumps(data, indent=2) + "\n", encoding="utf-8")


def main():
    with urllib.request.urlopen(INDEX) as response:
        index = json.load(response)
    records = {}
    for architecture, version in VERSIONS.items():
        platform = next(p for pkg in index["packages"] for p in pkg.get("platforms", [])
                        if p["architecture"] == architecture and p["version"] == version
                        and "duck" in p["name"].lower())
        cache = ROOT / ".pio/board-packages" / platform["archiveFileName"]
        cache.parent.mkdir(parents=True, exist_ok=True)
        if not cache.exists():
            print("Downloading " + platform["archiveFileName"], flush=True)
            urllib.request.urlretrieve(platform["url"], cache)
        digest = hashlib.sha256(cache.read_bytes()).hexdigest()
        if "SHA-256:" + digest != platform["checksum"]:
            raise ValueError("Board package checksum mismatch: " + cache.name)
        with zipfile.ZipFile(cache) as archive:
            p = properties(archive.read(architecture + "/boards.txt").decode("utf-8"))
            files = ["dstike/pins_arduino.h"]
            if architecture == "esp8266":
                files.append("generic/common.h")
            for filename in files:
                destination = ROOT / "boards/variants" / architecture / filename
                destination.parent.mkdir(parents=True, exist_ok=True)
                source = archive.read(architecture + "/variants/" + filename).decode("utf-8")
                # Keep the definitions and license text; normalize trailing whitespace.
                source = "\n".join(line.rstrip() for line in source.splitlines()) + "\n"
                with destination.open("w", encoding="utf-8", newline="\n") as output:
                    output.write(source)
        records[architecture] = {
            "version": version, "url": platform["url"], "sha256": digest,
            "properties": p,
        }
        upload = {
            "maximum_ram_size": int(p["upload.maximum_data_size"]),
            "require_upload_port": True,
        }
        if architecture == "avr":
            board = {
                "name": p["name"], "vendor": "Arduino", "url": "https://wifiduck.com",
                "frameworks": ["arduino"],
                "build": {
                    "core": p["build.core"], "mcu": p["build.mcu"], "f_cpu": p["build.f_cpu"],
                    "variant": p["build.variant"], "variants_dir": "boards/variants/avr",
                    "hwids": [[p["menu.keyboardid.default.build.vid"], p["menu.keyboardid.default.build.pid"]],
                              ["0x2341", "0x0036"], ["0x1B4F", "0x9205"]],
                    "usb_product": p["menu.keyboardid.default.build.usb_product"].strip('"'),
                    "extra_flags": "-DARDUINO_" + p["build.board"] + " " +
                                   p["build.protocol_flags"] + " " + p["build.led_pin"],
                },
                "upload": dict(upload, maximum_size=int(p["upload.maximum_size"]),
                               speed=int(p["upload.speed"]), protocol=p["upload.protocol"],
                               disable_flushing=True, use_1200bps_touch=True, wait_for_upload_port=True),
            }
        else:
            selection = "menu.eesz.4M3M."
            board = {
                "name": p["name"], "vendor": "DSTIKE", "url": "https://wifiduck.com",
                "frameworks": ["arduino"],
                "build": {
                    "core": p["build.core"], "mcu": p["build.mcu"], "f_cpu": p["build.f_cpu"],
                    "f_flash": p["build.flash_freq"] + "000000L", "flash_mode": p["build.flash_mode"],
                    "variant": p["build.variant"], "variants_dir": "boards/variants/esp8266",
                    "ldscript": p[selection + "build.flash_ld"],
                    "extra_flags": "-DESP8266 -DARDUINO_" + p["build.board"] + " " + p["build.protocol_flags"] +
                                   " " + p["build.flash_flags"] + " -DPIO_FRAMEWORK_ARDUINO_LWIP2_LOW_MEMORY",
                },
                "upload": dict(upload, maximum_size=int(p[selection + "upload.maximum_size"]),
                               flash_size="4MB", speed=int(p["menu.baud.115200.upload.speed"]),
                               protocol="esptool", resetmethod="nodemcu"),
            }
        save_json(ROOT / "boards" / ("dstike_wifiduck_" + architecture + ".json"), board)
        print("Imported DSTIKE " + architecture, flush=True)
    save_json(ROOT / ".pio/board-packages/arduino-package-source.json",
              {"index_url": INDEX, "packages": records})


if __name__ == "__main__":
    main()
