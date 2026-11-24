# DSTIKE WiFi Duck with PlatformIO

The PlatformIO profiles use the official DSTIKE pin definitions and defaults
from the WiFi Duck Arduino packages. The Arduino sketches remain available;
both build systems compile the same firmware sources.

Install PlatformIO Core or the PlatformIO extension for VS Code, then run
commands from the repository root.

## Build both chips

```
pio run
```

The default environments build these two files:

| Chip | Environment | Output |
| --- | --- | --- |
| ATmega32u4 | `dstike_avr` | `.pio/build/dstike_avr/firmware.hex` |
| ESP8266 | `dstike_esp8266` | `.pio/build/dstike_esp8266/firmware.bin` |

The ESP build embeds the current web sources automatically. Package versions
and library versions are pinned in `platformio.ini`.

## Upload the ATmega32u4 over USB

Set the eight DIP switches to **10101010** (positions 1 through 8; 1 is ON),
the ATmega flashing mode documented for DSTIKE WiFi Duck. Connect USB and run:

```
pio run -e dstike_avr -t upload --upload-port COM7
```

Replace `COM7` with the device's current port. PlatformIO resets it with a
1200-baud touch and waits for its bootloader port, then uploads using AVR109
at 57600 baud. Normal sketch uploads preserve the existing bootloader.

## Upload the ESP8266 over WiFi

Return the switches to normal mode, **10101101**, and power-cycle the device.
Connect to its WiFi network, then run:

```
pio run -e dstike_esp8266_ota -t upload
```

This uses Arduino OTA at `192.168.4.1`. The already installed ESP firmware must
support Arduino OTA. The web interface offers another route: open Settings,
choose `.pio/build/dstike_esp8266/firmware.bin`, and press Upload.

Only the ESP8266 can be updated over WiFi. Update both chips when changing
communication protocol versions, including the initial mouse-support update.

## Upload the ESP8266 over USB

Set switches to **01011101**, the ESP flashing mode, and run:

```
pio run -e dstike_esp8266 -t upload --upload-port COM7
```

After updating, return switches to **10101101** and power-cycle the device.
Use the switch numbering printed on the board rather than reversing the bit
sequence based on viewing direction.

## Board settings and USB identity

| Setting | Imported default |
| --- | --- |
| AVR clock | 16 MHz |
| AVR application flash limit | 28672 bytes |
| AVR USB VID/PID | `2341:8036` |
| AVR USB product | `Leonardo` (official default USB Device ID menu selection) |
| AVR NeoPixel | One WS2812b on pin 7 |
| Communication | I2C, ESP SDA GPIO 4 / SCL GPIO 5, AVR SDA pin 2 / SCL pin 3 |
| ESP CPU / flash clock | 160 MHz / 40 MHz |
| ESP flash mode | DIO |
| ESP flash capacity / filesystem | 4 MB / 3 MB SPIFFS (`eagle.flash.4m3m.ld`) |
| ESP USB upload speed | 115200 baud |

The PlatformIO board label is **DSTIKE WiFi Duck**. The USB product string
is a separate setting. To display `WiFi Duck` instead of `Leonardo` in the
running firmware, add this under `[env:dstike_avr]`:

```
board_build.usb_product = WiFi Duck
```

That changes the application product string while preserving the VID/PID and
bootloader upload behavior. The bootloader can appear under its own identity.

The flash layout matches the official package's first/default 4M3M selection.
Match your existing layout when retaining stored scripts; changing SPIFFS
boundaries can require restoring the scripts from a backup. Other official
layouts can be selected with `board_build.ldscript`, for example
`eagle.flash.4m1m.ld` for the 4 MB / 1 MB SPIFFS option.

## Re-import the official definitions

```
python tools/import_dstike_boards.py
```

The importer reads the official package index, downloads pinned AVR **1.8.5**
and ESP8266 **2.7.4** archives, verifies their published SHA-256 checksums,
then imports DSTIKE `boards.txt` properties and pin variants. The source
URLs, checksums, and properties are recorded in
`.pio/board-packages/arduino-package-source.json`, alongside the downloaded
archives. This provenance record and the download cache are ignored by Git;
only the board descriptors, required pin headers, and license are committed.

The PlatformIO AVR Arduino core is provided by the pinned Atmel AVR platform.
The ESP platform uses Arduino core 2.7.4, matching the official ESP package.
The board importer does not install or modify Arduino IDE board packages.
