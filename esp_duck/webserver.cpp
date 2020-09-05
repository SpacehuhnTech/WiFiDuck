/*
   This software is licensed under the MIT License. See the license file for details.
   Source: https://github.com/spacehuhntech/WiFiDuck
 */

#include "webserver.h"

#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <DNSServer.h>
#include <ArduinoOTA.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>

#include "config.h"
#include "debug.h"
#include "cli.h"
#include "spiffs.h"
#include "settings.h"

#include "webfiles.h"

void reply(AsyncWebServerRequest* request, int code, const char* type, const uint8_t* data, size_t len) {
    AsyncWebServerResponse* response =
        request->beginResponse_P(code, type, data, len);

    response->addHeader("Content-Encoding", "gzip");
    request->send(response);
}

namespace webserver {
    // ===== PRIVATE ===== //
    AsyncWebServer   server(80);
    AsyncWebSocket   ws("/ws");
    AsyncEventSource events("/events");

    AsyncWebSocketClient* currentClient { nullptr };

    DNSServer dnsServer;

    bool reboot = false;

    void wsEvent(AsyncWebSocket* server, AsyncWebSocketClient* client, AwsEventType type, void* arg, uint8_t* data, size_t len) {
        if (type == WS_EVT_CONNECT) {
            debugf("WS Client connected %u\n", client->id());
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
                char* msg = (char*)data;
                msg[len] = 0;

                debugf("Message from %u [%llu byte]=%s", client->id(), info->len, msg);

                currentClient = client;
                cli::parse(msg, [](const char* str) {
                    webserver::send(str);
                    debugf("%s\n", str);
                }, false);
                currentClient = nullptr;
            }
        }
    }

    // ===== PUBLIC ===== //
    void begin() {
        // Access Point
        WiFi.hostname(HOSTNAME);

        // WiFi.mode(WIFI_AP_STA);
        WiFi.softAP(settings::getSSID(), settings::getPassword(), settings::getChannelNum());
        debugf("Started Access Point \"%s\":\"%s\"\n", settings::getSSID(), settings::getPassword());

        // Webserver
        server.on("/", HTTP_GET, [](AsyncWebServerRequest* request) {
            request->redirect("/index.html");
        });

        server.onNotFound([](AsyncWebServerRequest* request) {
            request->redirect("/error404.html");
        });

        server.on("/run", [](AsyncWebServerRequest* request) {
            String message;

            if (request->hasParam("cmd")) {
                message = request->getParam("cmd")->value();
            }

            request->send(200, "text/plain", "Run: " + message);

            cli::parse(message.c_str(), [](const char* str) {
                debugf("%s\n", str);
            }, false);
        });

        WEBSERVER_CALLBACK;

        // Arduino OTA Update
        ArduinoOTA.onStart([]() {
            events.send("Update Start", "ota");
        });
        ArduinoOTA.onEnd([]() {
            events.send("Update End", "ota");
        });
        ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
            char p[32];
            sprintf(p, "Progress: %u%%\n", (progress/(total/100)));
            events.send(p, "ota");
        });
        ArduinoOTA.onError([](ota_error_t error) {
            if (error == OTA_AUTH_ERROR) events.send("Auth Failed", "ota");
            else if (error == OTA_BEGIN_ERROR) events.send("Begin Failed", "ota");
            else if (error == OTA_CONNECT_ERROR) events.send("Connect Failed", "ota");
            else if (error == OTA_RECEIVE_ERROR) events.send("Recieve Failed", "ota");
            else if (error == OTA_END_ERROR) events.send("End Failed", "ota");
        });
        ArduinoOTA.setHostname(HOSTNAME);
        ArduinoOTA.begin();

        events.onConnect([](AsyncEventSourceClient* client) {
            client->send("hello!", NULL, millis(), 1000);
        });
        server.addHandler(&events);

        // Web OTA
        server.on("/update", HTTP_POST, [](AsyncWebServerRequest* request) {
            reboot = !Update.hasError();

            AsyncWebServerResponse* response;
            response = request->beginResponse(200, "text/plain", reboot ? "OK" : "FAIL");
            response->addHeader("Connection", "close");

            request->send(response);
        }, [](AsyncWebServerRequest* request, String filename, size_t index, uint8_t* data, size_t len, bool final) {
            if (!index) {
                debugf("Update Start: %s\n", filename.c_str());
                Update.runAsync(true);
                if (!Update.begin((ESP.getFreeSketchSpace() - 0x1000) & 0xFFFFF000)) {
                    Update.printError(Serial);
                }
            }
            if (!Update.hasError()) {
                if (Update.write(data, len) != len) {
                    Update.printError(Serial);
                }
            }
            if (final) {
                if (Update.end(true)) {
                    debugf("Update Success: %uB\n", index+len);
                } else {
                    Update.printError(Serial);
                }
            }
        });

        MDNS.addService("http", "tcp", 80);

        dnsServer.start(53, URL, IPAddress(192, 168, 4, 1));

        // Websocket
        ws.onEvent(wsEvent);
        server.addHandler(&ws);

        // Start Server
        server.begin();
        debugln("Started Webserver");
    }

    void update() {
        ArduinoOTA.handle();
        if (reboot) ESP.restart();
        dnsServer.processNextRequest();
    }

    void send(const char* str) {
        if (currentClient) currentClient->text(str);
    }
}