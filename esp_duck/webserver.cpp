/*
   Copyright (c) 2019 Stefan Kremser
   This software is licensed under the MIT License. See the license file for details.
   Source: github.com/spacehuhn/WiFiDuck
 */

#include "webserver.h"

#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>

#include "config.h"
#include "debug.h"
#include "cli.h"
#include "spiffs.h"
#include "i2c.h"
#include "settings.h"

// Minified using: https://www.willpeavy.com/tools/minifier
// And escaped using: http://easyonlineconverter.com/converters/cpp-string-escape.html
const char index_html[] PROGMEM = "<!DOCTYPE html><html><head> <meta charset=\"UTF-8\"> <meta name=\"viewport\" content=\"width=device-width, initial-scale=1\"> <title>WiFi Duck</title> <style>/* Copyright 2014 Owen Versteeg; MIT licensed */ body, textarea, input, select{background: 0; border-radius: 0; font: 16px sans-serif; margin: 0}.smooth{transition: all .2s}.btn, .nav a{text-decoration: none}.container{margin: 0 20px; width: auto}label>*{display: inline}form>*{display: block; margin-bottom: 10px}.btn{background: #999; border-radius: 6px; border: 0; color: #fff; cursor: pointer; display: inline-block; margin: 2px 0; padding: 12px 30px 14px}.btn:hover{background: #888}.btn:active, .btn:focus{background: #777}.btn-a{background: #0ae}.btn-a:hover{background: #09d}.btn-a:active, .btn-a:focus{background: #08b}.btn-b{background: #3c5}.btn-b:hover{background: #2b4}.btn-b:active, .btn-b:focus{background: #2a4}.btn-c{background: #d33}.btn-c:hover{background: #c22}.btn-c:active, .btn-c:focus{background: #b22}.btn-sm{border-radius: 4px; padding: 10px 14px 11px}.row{margin: 1% 0; overflow: auto}.col{float: left}.table, .c12{width: 100%}.c11{width: 91.66%}.c10{width: 83.33%}.c9{width: 75%}.c8{width: 66.66%}.c7{width: 58.33%}.c6{width: 50%}.c5{width: 41.66%}.c4{width: 33.33%}.c3{width: 25%}.c2{width: 16.66%}.c1{width: 8.33%}h1{font-size: 3em}.btn, h2{font-size: 2em}.ico{font: 33px Arial Unicode MS, Lucida Sans Unicode}.addon, .btn-sm, .nav, textarea, input, select{outline: 0; font-size: 14px}textarea, input, select{padding: 8px; border: 1px solid #ccc}textarea:focus, input:focus, select:focus{border-color: #5ab}textarea, input[type=text]{-webkit-appearance: none; width: 13em}.addon{padding: 8px 12px; box-shadow: 0 0 0 1px #ccc}.nav, .nav .current, .nav a:hover{background: #000; color: #fff}.nav{height: 24px; padding: 11px 0 15px}.nav a{color: #aaa; padding-right: 1em; position: relative; top: -1px}.nav .pagename{font-size: 22px; top: 1px}.btn.btn-close{background: #000; float: right; font-size: 25px; margin: -54px 7px; display: none}@media(min-width:1310px){.container{margin: auto; width: 1270px}}@media(max-width:870px){.row .col{width: 100%}}@media(max-width:500px){.btn.btn-close{display: block}.nav{overflow: hidden}.pagename{margin-top: -11px}.nav:active, .nav:focus{height: auto}.nav div:before{background: #000; border-bottom: 10px double; border-top: 3px solid; content: ''; float: right; height: 4px; position: relative; right: 3px; top: 14px; width: 20px}.nav a{padding: .5em 0; display: block; width: 50%}}.table th, .table td{padding: .5em; text-align: left}.table tbody>:nth-child(2n-1){background: #f2f2f2}.msg{padding: 1.5em; background: #def; border-left: 5px solid #59d}#status{text-align: center; padding: 0.5rem; background: #20C20E; color: #fff; position: sticky; top: 0;}textarea{border-radius: 4px; background: #ddd; border: none; width: 100%; height: 10rem; box-sizing: border-box; -moz-box-sizing: border-box; -webkit-box-sizing: border-box; margin: .5rem 0;}input[type=\"text\"]{border-radius: 4px; background: #ddd; border: none;}</style></head><body> <nav class=\"nav\" tabindex=\"-1\" onclick=\"this.focus()\"> <div class=\"container\"> <a class=\"pagename current\" href=\"#\">WiFi Duck</a><!-- <a href=\"#\">One</a> <a href=\"#\">Two</a> <a href=\"#\">Three</a>--> </div></nav> <button class=\"btn-close btn btn-sm\">×</button> <div id=\"status\">Connected</div><div class=\"container\"> <div class=\"row\"> <div class=\"col c12\"> <h2>Status</h2> <p><b>i2c connection: </b><span>OK</span></p><p><b>Free memory: </b><span>x Bytes</span></p><a class=\"btn btn-c btn-sm smooth\">STOP</a> </div></div><div class=\"row\"> <div class=\"col c12\"> <h2>Scripts</h2> <table class=\"table\"> <thead> <tr> <th>File</th> <th>Actions</th> </tr></thead> <tbody> <tr> <td>/test.script</td><td> <a class=\"btn btn-a btn-sm smooth\">edit</a> <a class=\"btn btn-c btn-sm smooth\">run</a> </td></tr><tr> <td>/tmp.script</td><td> <a class=\"btn btn-a btn-sm smooth\">edit</a> <a class=\"btn btn-c btn-sm smooth\">run</a> </td></tr><tr> <td><input type=\"text\" class=\"smooth\" value=\"/test.script\"/></td><td> <a class=\"btn btn-b btn-sm smooth\">create</a> </td></tr></tbody> </table> </div></div><div class=\"row\"> <div class=\"col c12\"> <h2>Editor</h2> <input type=\"text\" class=\"smooth\" value=\"/test.script\"/> <textarea class=\"smooth\">DELAY 2000GUI r </textarea> <a class=\"btn btn-a btn-sm smooth\">save</a> <a class=\"btn btn-c btn-sm smooth\">run</a> </div></div><div class=\"row\"> <div class=\"col c12\"> <hr> <p>WiFi Duck version -1 | Copyleft myself</p></div></div></div></body></html>";

void sendIndex(AsyncWebServerRequest* request) {
    request->send_P(200, "text/html", index_html);
}

namespace webserver {
    // ===== PRIVATE ===== //
    AsyncWebServer server(80);
    AsyncWebSocket ws("/ws");

    AsyncWebSocketClient* currentClient { nullptr };

    void notFound(AsyncWebServerRequest* request) {
        request->send(404, "text/plain", "Not found");
    }

    void wsEvent(AsyncWebSocket* server, AsyncWebSocketClient* client, AwsEventType type, void* arg, uint8_t* data, size_t len) {
        if (type == WS_EVT_CONNECT) {
            debugf("WS Client connected %u\n", client->id());

            // client->printf("%u", client->id());
        }

        else if (type == WS_EVT_DISCONNECT) {
            debugf("WS Client disconnected %u\n", client->id());
        }

        else if (type == WS_EVT_ERROR) {
            debugf("WS Client %u error(%u): %s\n", client->id(), *((uint16_t*)arg), (char*)data);
        }

        else if (type == WS_EVT_PONG) {
            debugf("PONG %u\n", client->id());
        }

        else if (type == WS_EVT_DATA) {
            AwsFrameInfo* info = (AwsFrameInfo*)arg;

            if (info->opcode == WS_TEXT) {
                debugf("Message from %u [%llu byte]\n", client->id(), info->len);

                char* msg = (char*)data;

                msg[len] = 0;
                debugf("%s\n", msg);

                if (spiffs::streaming() && (strcmp(msg, "close") != 0) && (strcmp(msg, "read") != 0)) {
                    spiffs::streamWrite(msg, len);
                    debugln("Written data to file");
                    client->text("> Written data to file");
                } else {
                    currentClient = client;
                    cli::parse(msg, [](const char* str) {
                        webserver::send(str);
                        Serial.print(str);
                    }, false);
                    currentClient = nullptr;
                }
            }
        }
    }

    // ===== PUBLIC ===== //
    void begin() {
        // Access Point
        const char* ssid     = settings::getSSID();
        const char* password = settings::getPassword();

        WiFi.softAP(ssid ? ssid : WIFI_SSID, password ? password : WIFI_PASSWORD);
        debugf("Started Access Point \"%s\":\"%s\"\n", ssid ? ssid : WIFI_SSID, password ? password : WIFI_PASSWORD);

        // Webserver
        server.on("/", HTTP_GET, sendIndex);
        server.on("/index.html", HTTP_GET, sendIndex);

        server.onNotFound(notFound);

        // Websocket
        ws.onEvent(wsEvent);
        server.addHandler(&ws);

        // Start Server
        server.begin();
        debugln("Started Webserver");
    }

    void send(const char* str) {
        if (currentClient) currentClient->text(str);
    }
}