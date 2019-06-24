/*
   Copyright (c) 2019 Stefan Kremser
   This software is licensed under the MIT License. See the license file for details.
   Source: github.com/spacehuhn/WiFiDuck
 */

#ifndef DuckyTransmitter_h
#define DuckyTransmitter_h

#include <Arduino.h>
#include <Wire.h>

#define I2C_ADDR 0x31
#define I2C_SDA 5
#define I2C_SCL 4

#define BUFFER_SIZE 512
#define PACKET_SIZE 32

#define RESPONSE_OK 0x00
#define RESPONSE_PROCESSING 0x01
#define RESPONSE_REPEAT 0x02
#define RESPONSE_I2C_ERROR 0xFF

#define WAITING_TIME 5

class DuckyTransmitter {
    private:
        bool connection  = false;
        size_t bytesSent = 0;
        uint8_t response = RESPONSE_OK;

        char* prevMessage     = NULL;
        size_t prevMessageLen = 0;

        void sendRequest();
        void wait();
        void transmit(const char* buf, size_t len);

    public:
        void begin();

        void sendMessage(const String& msg);
        void sendMessage(const char* msg);

        bool connected();
};

#endif /* ifdef DuckyTransmitter_h */