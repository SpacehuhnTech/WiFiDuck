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

#include "webfiles.h"

void reply(AsyncWebServerRequest* request, const char* type, const uint8_t* data, size_t len) {
    AsyncWebServerResponse* response = request->beginResponse_P(200, type, data, len);

    response->addHeader("Content-Encoding", "gzip");
    request->send(response);
}

namespace webserver {
    // ===== PRIVATE ===== //
    AsyncWebServer server(80);
    AsyncWebSocket ws("/ws");

    AsyncWebSocketClient* currentClient { nullptr };

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
        WiFi.softAP(settings::getSSID(), settings::getPassword(), settings::getChannelNum());
        debugf("Started Access Point \"%s\":\"%s\"\n", settings::getSSID(), settings::getPassword());

        // Webserver
        server.on("/", HTTP_GET, [](AsyncWebServerRequest* request) {
            reply(request, "text/html", index_html, sizeof(index_html));
        });

        server.on("/index.html", HTTP_GET, [](AsyncWebServerRequest* request) {
            reply(request, "text/html", index_html, sizeof(index_html));
        });
        server.on("/credits.html", HTTP_GET, [](AsyncWebServerRequest* request) {
            reply(request, "text/html", credits_html, sizeof(credits_html));
        });
        server.on("/settings.html", HTTP_GET, [](AsyncWebServerRequest* request) {
            reply(request, "text/html", settings_html, sizeof(settings_html));
        });
        server.on("/terminal.html", HTTP_GET, [](AsyncWebServerRequest* request) {
            reply(request, "text/html", terminal_html, sizeof(terminal_html));
        });
        server.on("/index.js", HTTP_GET, [](AsyncWebServerRequest* request) {
            reply(request, "application/javascript", index_js, sizeof(index_js));
        });
        server.on("/script.js", HTTP_GET, [](AsyncWebServerRequest* request) {
            reply(request, "application/javascript", script_js, sizeof(script_js));
        });
        server.on("/settings.js", HTTP_GET, [](AsyncWebServerRequest* request) {
            reply(request, "application/javascript", settings_js, sizeof(settings_js));
        });
        server.on("/terminal.js", HTTP_GET, [](AsyncWebServerRequest* request) {
            reply(request, "application/javascript", terminal_js, sizeof(terminal_js));
        });
        server.on("/style.css", HTTP_GET, [](AsyncWebServerRequest* request) {
            reply(request, "text/css", style_css, sizeof(style_css));
        });

        server.onNotFound([](AsyncWebServerRequest* request) {
            request->send(404, "text/plain", "Not found");
        });

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