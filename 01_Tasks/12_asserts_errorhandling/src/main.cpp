
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
FreeRTOS asserts und errorhandling | 01.2025

*/
#include <Arduino.h>
const byte led1 = D10;
const byte led2 = D9;
int cnt = 0;
TaskHandle_t h1;
TaskHandle_t h2;

void blinky1(void *parameter) {
  pinMode(led1, OUTPUT);
  while (1) {
    digitalWrite(led1, HIGH);
    delay(250);
    digitalWrite(led1, LOW);
    delay(250);
    cnt++;
    Serial.println(cnt);
    if (cnt > 3) {
      Serial.println("h1 null");
      h1 = NULL;
      assert(h1);
    }
  }
}
void blinky2(void *parameter) {
  pinMode(led2, OUTPUT);
  while (1) {
    digitalWrite(led2, HIGH);
    delay(650);
    digitalWrite(led2, LOW);
    delay(650);
  }
}
void setup() {
  Serial.begin(115200);
  BaseType_t taskStatus;
  delay(2000);
  Serial.println("Setup startet");
  taskStatus = xTaskCreate(blinky1, "blinky1", 1024, NULL, 1, &h1);
  assert(taskStatus == pdPASS); // pdFAIL
  // h1=NULL; // provozierter Fehler
  assert(h1);
  taskStatus = xTaskCreate(blinky2, "bliny2", 1024, NULL, 1, &h2);
  assert(taskStatus == pdPASS);
  assert(h2);
  Serial.println("Setup abgeschlossen");
}

void loop() {
  // hier gibt es nichts zu tun
}
