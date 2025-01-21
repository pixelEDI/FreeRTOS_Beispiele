
/*
        _          _ ______ _____ _____
       (_)        | |  ____|  __ \_   _|
  _ __  ___  _____| | |__  | |  | || |
 | '_ \| \ \/ / _ \ |  __| | |  | || |
 | |_) | |>  <  __/ | |____| |__| || |_
 | .__/|_/_/\_\___|_|______|_____/_____|
 | |
 |_|

https://links.pixeledi.eu
RingBuffer | 01.2025
*/

#include <Arduino.h>

const int bufferSize = 5; // Größe des Ringbuffers
int ringBuffer[bufferSize];
int writeIndex = 0; // Schreibindex
int readIndex = 0;  // Leseindex
int itemCount = 0;  // Anzahl der Elemente im Buffer
int analogSensorPin = A2;
unsigned long lastReadTimeSensor = 0;
unsigned long lastReadTimeBuffer = 0;

void addToBuffer(int value) {
  ringBuffer[writeIndex] = value;
  writeIndex = ((writeIndex + 1) % bufferSize);
  if (itemCount < bufferSize) {
    // Nur erhöhen, wenn noch Platz vorhanden ist
    itemCount++;
  } else {
    readIndex = (readIndex + 1) % bufferSize;
  }
}
int readFromBuffer() {
  if (itemCount == 0) {
    return -1; // Buffer ist leer
  }
  int value = ringBuffer[readIndex];
  readIndex = (readIndex + 1) % bufferSize;
  itemCount--; // Elementanzahl im Buffer reduzieren
  return value;
}
void setup() {
  Serial.begin(115200);
  delay(1000);
}

void loop() {
  unsigned long currentMillis = millis();
  if (currentMillis - lastReadTimeSensor >= 100) {
    lastReadTimeSensor = currentMillis;
    int value = analogRead(analogSensorPin);
    // Serial.println(value);
    addToBuffer(value);
  }
  if (currentMillis - lastReadTimeBuffer >= 1000) {
    lastReadTimeBuffer = currentMillis;

    Serial.println("Lese aus Ringbuffer...");
    while (itemCount > 0) {
      int value = readFromBuffer();
      Serial.println(value);
    }
  }
}
