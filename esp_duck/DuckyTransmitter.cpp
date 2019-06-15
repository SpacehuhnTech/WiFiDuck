/*
   Copyright (c) 2019 Stefan Kremser
   This software is licensed under the MIT License. See the license file for details.
   Source: github.com/spacehuhn/WiFiDuck
 */

 #include "DuckyTransmitter.h"

// ===== PRIVATE ===== //
void DuckyTransmitter::sendRequest() {
    Wire.requestFrom(I2C_ADDR, 1);

    if (Wire.available()) {
        response = Wire.read();
    } else {
        // Serial.println("Request error :(");
        connection = false;
        response   = RESPONSE_I2C_ERROR;
    }
}

void DuckyTransmitter::wait() {
    sendRequest();

    while (response == RESPONSE_PROCESSING) {
        delay(WAITING_TIME);
        Serial.print('.');
        sendRequest();
    }

    bytesSent = 0;
}

void DuckyTransmitter::transmit(const char* buf, size_t len) {
    if (!connection) return;

    unsigned int transmissions = (len / PACKET_SIZE) + (len % PACKET_SIZE > 0);
    unsigned int buffer_i      = 0;
    unsigned int transmission_i;
    unsigned int packet_i;

    for (transmission_i = 0; transmission_i < transmissions; ++transmission_i) {
        Wire.beginTransmission(I2C_ADDR);

        for (packet_i = 0; packet_i < PACKET_SIZE && buffer_i < len; ++packet_i) {
            Serial.print(buf[buffer_i]);
            Wire.write(buf[buffer_i++]);
            if (++bytesSent == BUFFER_SIZE) wait();
        }

        Wire.endTransmission();
    }

    wait();
}

// ===== PUBLIC ===== //

void DuckyTransmitter::begin() {
    Wire.begin(I2C_SDA, I2C_SCL);
    connection = true;
    sendRequest();
}

void DuckyTransmitter::sendMessage(const String& msg) {
    sendMessage(msg.c_str());
}

void DuckyTransmitter::sendMessage(const char* msg) {
    Serial.print("Sending message: ");
    // Serial.print(msg);

    size_t msg_len = strlen(msg);

    transmit(msg, msg_len);

    // Remember this message if transmission was succesful
    if (response == RESPONSE_OK) {
        if (prevMessage) free(prevMessage);
        prevMessageLen = msg_len;
        prevMessage    = (char*)malloc(prevMessageLen + 1);
        strcpy(prevMessage, msg);
    }

    // Repeat last message as long as requested
    while (response == RESPONSE_REPEAT) {
        transmit(prevMessage, prevMessageLen);
    }

    Serial.println("Done");
}

bool DuckyTransmitter::connected() {
    return connection;
}