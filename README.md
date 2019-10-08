# WiFiDuck

<p align="center"><img alt="WiFi Duck Logo" src="https://raw.githubusercontent.com/spacehuhn/WiFiDuck/master/img/logo.png" width="150"></p>

<p align="center">
<b>Keystroke injection attack platform</b>
<br>
<br>
  🐦 <a href="https://twitter.com/spacehuhn">Twitter</a>
| 📺 <a href="https://www.youtube.com/spacehuhn">YouTube</a>
| 🌍 <a href="https://spacehuhn.io">spacehuhn.io</a>
<br>
<br>
Support this project and become a patron on <a href="https://patreon.com/spacehuhn">patreon.com/spacehuhn</a>.<br>
Also available: <a href="https://www.tindie.com/products/Spacehuhn/spacehuhn-stickers/">Stickers</a>
</b>!
</p>

## Overview

- [Disclaimer](#disclaimer)
- [About](#about)
- [Hardware](#hardware)
  - [Connections](#connections)
- [Flash Software](#flash-software)
  - [Requirements](#requirements)
  - [Atmega32u4](#atmega32u4)
  - [ESP8266/ESP8285](#esp8266esp8285)
- [Usage](#usage)
  - [Ducky Script](#ducky-script)
    - [Functions](#functions)
    - [Standard Keys](#standard-keys)
    - [Modifier Keys](#modifier-keys)
    - [Other Keys](#other-keys)
    - [Examples](#examples)
  - [FAQ](#faq)
- [Development](#development)
  - [Edit Web Files](#edit-web-files)
- [License](#license)

## Disclaimer

**This is a tool.**  
**It's neither good nor bad.**  
**Use it to do good, to study and to test.**  
**Never use it to do harm or create damage!**  

**The continuation of this project counts on you!**  

## About

This open source project aims to provide a user-friendly tool to learn about
keystroke injection attacks.  
A microcontroller acts as a USB keyboard that is programmable over WiFi.  
It's using the Ducky Script language that [Hak5](https://www.hak5.org/)
introduced with the [USB Rubber Ducky](https://hakshop.com/products/usb-rubber-ducky-deluxe).  

A keyboard is trusted by most operating systems by default,
which enables for a variety of attacks.  
Humans might not type very fast, but an automated device like this can.  
It can open a terminal and mess with your computer in a matter of a milliseconds!  

## Hardware

This tool requires following hardware:  
* An Atmega32u4 based board (for example: Arduino Leonardo or Arduino Pro Micro)  
* An ESP8266 or ESP8285 (for example NodeMCU or Lolin/Wemos d1 mini)  
* [Optional] A single Neopixel LED (WS2812b)  

You will also need a computer, a working USB cable, patience and common sense.  

If you're a beginner,
it's recommended you wire everything together on a breadboard first!  
So get a breadboard with a couple of jumper wires.  

If you wish to solder everything together to a small DIY gadget,
you'll obviously need soldering equipment and a bit of experience using it.  

### Connections

Here's a map of the pins that need to be connected.  

| ESP82xx | Atmega32u4 |
| ------- | ---------- |
| `GPIO 5` or `D1` | `SCL` or `D3` |
| `GPIO 4` or `D2` | `SDA` or `D2` |
| `GND` | `GND` |

| Atmega32u4 | Neopixel LED |
| ---------- | ------------ |
| `D7` | `DI`, `Data` or `In` |
| `GND` | `GND` |

## Flash Software

### Requirements

Arduino IDE:  
* [Arduino IDE](https://www.arduino.cc/en/main/software)

**Latest** ESP8266 Arduino Core:  
* [ESP8266 Arduino Core](https://github.com/esp8266/Arduino#installing-with-boards-manager)  

Arduino Libraries:  
* [SimpleCLI](https://github.com/spacehuhn/SimpleCLI#installation)
* [ESPAsyncTCP](https://github.com/me-no-dev/ESPAsyncTCP)
* [ESPAsyncWebServer](https://github.com/me-no-dev/ESPAsyncWebServer)

Installing Arduino libraries is always the same procedure:  
Download the repository, unzip it and move it inside the libraries folder.  
If you need more information, here is a
[tutorial](https://www.arduino.cc/en/Guide/Libraries) with more information.  

### Atmega32u4

1. Open `atmegaduck/atmega_duck.ino` with the Arduino IDE  
2. Under `Tools->Board` select `Arduino Leonardo`
(if you use a 3.3V Atmega32u4 select `LilyPad Arduino USB`)  
3. Plug the board in and select its port under `Tools->Port`  
4. Press the Upload button  

### ESP8266/ESP8285

1. Open `esp_duck/esp_duck.ino` with the Arduino IDE  
2. Under `Tools->Board` select `NodeMCU 1.0 (ESP-12E Module)`
(or whatever corresponds best to your ESP8266/ESP8285 based board)  
3. Under `Tools->Flash Size` select `4M (3M SPIFFS)` (or `1M (512K SPIFFS)`
if you use a ESP-07 Module or ESP8285 based board).  
4. Plug the board in and select its port under `Tools->Port`  
5. Press the Upload button  

## Usage

1. Plug it in (a blue Neopixel LED tells you that the connection is working)  
2. Connect to the WiFi network `wifiduck` with the password `wifiduck`
3. Open a browser and visit `192.168.4.1`
4. Click on Settings in the top right corner
5. Change the SSID and password

### Ducky Script

#### Functions

| Command | Description | Example |
| ------- | ----------- | ------- |
| `REM` | Comment | `REM Hello World!` |
| `DEFAULTDELAY` or `DEFAULT_DELAY` | Time in ms between every command | `DEFAULTDELAY 200` |
| `DELAY` | Delay in ms | `DELAY 1000` |
| `STRING` | Types the following string | `STRING Hello World!` |
| `REPEAT` or `REPLAY` | Repeats the last command n times | `REPEAT 3` |
| `LOCALE` | Sets the keyboard layout. Currently supported: `DE`, `GB`, `US` | `LOCALE DE` |
| `KEYCODE` | Type specific key code (modifier, key1[, ..., key6]) in decimal or hexadecimal | `KEYCODE 0x02 0x04` |

#### Standard Keys

| Key |
| --- |
| `a` - `z` |
| `A` - `Z` |
| `0` - `9` |
| `F1` - `F12` |

#### Modifier Keys

| Key |
| --- |
| `CTRL` or `CONTROL` |
| `SHIFT` |
| `ALT` |
| `WINDOWS` or `GUI` |

#### Other Keys

| Key |
| --- |
| `ENTER` |
| `MENU` or `APP`|
| `DELETE` |
| `HOME` |
| `INSERT` |
| `PAGEUP` |
| `PAGEDOWN` |
| `UP` or `UPARROW` |
| `DOWN` or `DOWNARROW` |
| `LEFT` or `LEFTARROW` |
| `RIGHT` or `RIGHTARROW` |
| `TAB` |
| `END` |
| `ESC` or `ESCAPE` |
| `SPACE` |
| `PAUSE or BREAK` |
| `CAPSLOCK` |
| `NUMLOCK` |
| `PRINTSCREEN` |
| `SCROLLLOCK` |

#### Examples

```
REM Hello World for Windows PCs
DEFAULTDELAY 200
GUI r
STRING notepad
ENTER
STRING Hello World!
```

### FAQ

#### I forgot the password

Flash the ESP8266 again,
but make sure that you select `Erase Flash: Sketch + WiFi Settings`
under Tools in the Arduino IDE.  

## Development

### Edit Web Files

If you would like to modify the web interface, you can!  
The `web/` folder contains all `.html`, `.css`, `.js` files.  
You can edit and test them locally as long as you're connected to the WiFi Duck
network thanks to the websocket connection handled by JavaScript in the background.  

To get the new files onto the ESP8266, run `python3 webconverter.py` in the
repository folder.  
It gzips all files inside `web/`, converts them into a hex array
and saves it in `esp_duck/webfiles.h`.  
Now you just need to [flash](#flash-software) the ESP8266 again.  

## License

This software is licensed under the MIT License.
See the [license file](LICENSE) for details.  
