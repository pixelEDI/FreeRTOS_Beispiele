
/*
        _          _ ______ _____ _____
       (_)        | |  ____|  __ \_   _|
  _ __  ___  _____| | |__  | |  | || |
 | '_ \| \ \/ / _ \ |  __| | |  | || |
 | |_) | |>  <  __/ | |____| |__| || |_
 | .__/|_/_/\_\___|_|______|_____  /_____|
 | |
 |_|

https://links.pixeledi.eu
RingBuffer | 01.2025

// Ein Ringbuffer (Circular Buffer) ist eine kreisförmige Speicherstruktur,
// bei der neue Daten die ältesten überschreiben, sobald der Buffer voll ist.
// Der Ringbuffer arbeitet nach dem FIFO-Prinzip (First In, First Out),
// d. h. die ältesten Daten werden zuerst gelesen bzw. entfernt.

// Angenommen, bufferSize = 5 und writeIndex startet bei 0.
// Nach jedem Schritt wird writeIndex um 1 erhöht und der Modulo-Operator
// angewendet:

// Schritt 1: writeIndex = (0 + 1) % 5 = 1
// Schritt 2: writeIndex = (1 + 1) % 5 = 2
// Schritt 3: writeIndex = (2 + 1) % 5 = 3
// Schritt 4: writeIndex = (3 + 1) % 5 = 4
// Schritt 5: writeIndex = (4 + 1) % 5 = 0
// Buffer ist am Ende, daher Wrap-around zurück auf 0
// Schritt 6: writeIndex = (0 + 1) % 5 = 1
// Zyklus wiederholt sich

// Dieser Modulo-Mechanismus sorgt dafür, dass writeIndex immer im Bereich 0
// bis 4 bleibt.

*/

#include <Arduino.h>
const int bufferSize = 5; // Größe des Ringbuffers
int ringBuffer[bufferSize];
int writeIndex = 0; // Schreibindex
int readIndex = 0;  // Leseindex
int itemCount = 0;  // Anzahl der Elemente im Buffer

void addToBuffer(int value) {
  ringBuffer[writeIndex] = value;
  writeIndex = ((writeIndex + 1) % bufferSize);
  if (itemCount < bufferSize) {
    // Nur erhöhen, wenn noch Platz vorhanden ist
    itemCount++;
  } else {
    // Älteste Daten überschreiben, wenn Buffer voll
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
  for (int i = 1; i <= 10; i++) {
    addToBuffer(i);
    Serial.print("Hinzugefügt: ");
    Serial.println(i);
  }
  Serial.println("Lesen aus Ringbuffer:");
  while (itemCount > 0) {
    int value = readFromBuffer();
    Serial.println(value);
  }
  delay(6000);
}
