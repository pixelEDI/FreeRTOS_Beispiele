
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
  xTaskCreatePinnedToCore(blinky1, "blinky1", 1024, NULL, 1, NULL, 0);
  xTaskCreatePinnedToCore(blinky2, "blinky2", 1024, NULL, 1, NULL, 1);
}

void loop() {
  // hier gibt es nichts zu tun
}
