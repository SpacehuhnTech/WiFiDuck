#include <Wire.h>

#define MODE_KEYBOARD  0x00
#define MODE_MOUSE     0x10

void setup() {
  Serial.begin(115200);
  Serial.println();
  Wire.begin(5, 4); // join i2c bus (address optional for master)

  Wire.requestFrom(0x31, 1);
  if (Wire.available()) {
    Serial.println("Connected");
    Wire.read();
  } else {
    Serial.println("Connection error");
  }

  Serial.print(".");
  delay(1000);

  Serial.print(".");
  delay(1000);

  Serial.println(".");
  delay(1000);

  for (int j = 0; j < 50; j++) {
    for (int k = 0; k < 20; k++) {
      for (int i = 0; i < 10; i++) {
        writeChar(0x00, 0x1e + i, true);
      }

      writeChar(0x00, 0x2c, true);
    }

    writeChar(0x00, 0x28, true);
  }
/*
  moveMouse(1, 0, 0, 0, true);
  moveMouse(1, 0, 0, 0, true);
  
  Serial.print(".");
  delay(1000);
  Serial.print(".");
  delay(1000);
  Serial.println(".");
  delay(1000);
  
  moveMouse(0, 100, 100, 0, true);

  Serial.print(".");
  delay(1000);
  Serial.print(".");
  delay(1000);
  Serial.println(".");
  delay(1000);
  
  moveMouse(0, 0, 0, 100, true);

  Serial.print(".");
  delay(1000);
  Serial.print(".");
  delay(1000);
  Serial.println(".");
  delay(1000);
  
  moveMouse(0, 0, 0, -100, true);
  
  Serial.print(".");
  delay(1000);
  Serial.print(".");
  delay(1000);
  Serial.println(".");
  delay(1000);
  
  moveMouse(0, -100, -100, 0, true);*/
}

void writeChar(uint8_t modifier, uint8_t key, bool release) {
  //Serial.println("Sent packet with ");
  //Serial.print(modifier, HEX);
  //Serial.print(" ");
  //Serial.print(key, HEX);
  //Serial.print(" ");
  //Serial.println(release, HEX);

  Wire.beginTransmission(0x31);
  Wire.write(MODE_KEYBOARD | release);
  Wire.write(modifier);
  Wire.write(key);
  Wire.write(0);
  Wire.write(0);
  Wire.write(0);
  Wire.write(0);
  Wire.write(0);
  Wire.endTransmission();

  delay(2);
  /*
  Wire.requestFrom(0x31, 1);
  if (Wire.available()) {
    uint8_t response = Wire.read();
    if (response != 0x00) {
      Serial.print("ERROR ");
      Serial.println((modifier + key) | release, HEX);
    }
  } else {
    Serial.println("No reply");
  }*/
}

void moveMouse(uint8_t buttons, int8_t x, int8_t y, int8_t scroll, bool release) {
  uint8_t header = 0x00;
  
  Wire.beginTransmission(0x31);
  Wire.write(MODE_MOUSE | release);
  Wire.write(buttons);
  Wire.write((uint8_t)x);
  Wire.write((uint8_t)y);
  Wire.write((uint8_t)scroll);
  Wire.endTransmission();

  delay(2);
  /*
  Wire.requestFrom(0x31, 1);
  if (Wire.available()) {
    uint8_t response = Wire.read();
    if (response != 0x00) {
      Serial.print("ERROR ");
      Serial.println((modifier + key) | release, HEX);
    }
  } else {
    Serial.println("No reply");
  }*/
}

void loop() {}
