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

#include "i2c.h"

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

            client->printf("Hello Client %u\n", client->id());
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

            // Single message
            if (info->final && (info->index == 0) && (info->len == len)) {
                debugf("Message from %u [%llu byte]\n", client->id(), info->len);

                if (info->opcode == WS_TEXT) {
                    char* msg = (char*)data;

                    msg[len] = 0;
                    debugf("%s\n", msg);

                    currentClient = client;
                    cli::parse(msg, [](const char* str) {
                        webserver::send(str);
                    });
                    currentClient = nullptr;
                } /*else {
                     for (size_t i = 0; i < info->len; ++i) {
                        debugf("%02x ", data[i]);
                     }
                     debugln();

                     client->binary("I got your binary message");
                     }*/
            }

            // Multiple packets
            else {
                if (info->index == 0) {
                    if (info->num == 0) {
                        debugf("WS Client %u message start\n", client->id());
                    }
                    debugf("WS Client %u - frame number %u start [%llu bytes]\n", client->id(), info->num, info->len);
                }

                debugf("WS Client %u - frame number %u [%llu - %llu bytes]: ", client->id(), info->num, info->index, info->index + len);

                if (info->message_opcode == WS_TEXT) {
                    data[len] = 0;
                    debugf("%s\n", (char*)data);
                } else {
                    for (size_t i = 0; i < len; ++i) {
                        debugf("%02x ", data[i]);
                    }
                    debugln();
                }

                if ((info->index + len) == info->len) {
                    debugf("WS Client %u - frame number %u end\n", client->id(), info->num);

                    if (info->final) {
                        debugf("WS Client %u message end\n", client->id());

                        /*
                           if (info->message_opcode == WS_TEXT) {
                            client->text("I got your text message");
                           } else {
                             client->binary("I got your binary message");
                             }*/
                    }
                }
            }
        }
    }

    // ===== PUBLIC ===== //
    void begin() {
        // Access Point
        WiFi.softAP(WIFI_SSID, WIFI_PASSWORD);
        debugf("Started Access Point \"%s\":\"%s\"\n", WIFI_SSID, WIFI_PASSWORD);

        // Webserver
        server.on("/", HTTP_GET, [](AsyncWebServerRequest* request) {
            request->send(200, "text/html", "<h1>Hello world!</h1>");
        });

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