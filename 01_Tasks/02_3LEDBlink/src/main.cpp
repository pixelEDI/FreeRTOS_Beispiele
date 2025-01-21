
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
FreeRTOS LED Blink | 01.2025

*/

#include <Arduino.h>
const byte led1 = D10;
const byte led2 = D9;
const byte led3 = D8;

void blinky1(void *parameter) {
  pinMode(led1, OUTPUT);
  while (1) {
    Serial.print("blinky 1 runs on core #: ");
    Serial.println(xPortGetCoreID());
    digitalWrite(led1, HIGH);
    vTaskDelay(250 / portTICK_PERIOD_MS);
    digitalWrite(led1, LOW);
    vTaskDelay(250 / portTICK_PERIOD_MS);
  }
}
void blinky2(void *parameter) {
  pinMode(led2, OUTPUT);
  while (1) {
    Serial.print("blinky 2 runs on core #: ");
    Serial.println(xPortGetCoreID());
    digitalWrite(led2, HIGH);
    vTaskDelay(650 / portTICK_PERIOD_MS);
    digitalWrite(led2, LOW);
    vTaskDelay(650 / portTICK_PERIOD_MS);
  }
}
void setup() {
  Serial.begin(115200);
  pinMode(led3, OUTPUT);
  xTaskCreatePinnedToCore(blinky1, "blinky1", 1024, NULL, 1, NULL, 0);
  xTaskCreatePinnedToCore(blinky2, "bliny2", 1024, NULL, 1, NULL, 1);
}
void loop() {
  digitalWrite(led3, HIGH);
  delay(1500);
  digitalWrite(led3, LOW);
  delay(1500);
}
