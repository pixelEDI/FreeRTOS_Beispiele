
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
const byte button1 = D2;
const byte button2 = D3;
TaskHandle_t taskBlinky1;
TaskHandle_t taskBlinky2;

void blinky1(void *parameter) {
  pinMode(led1, OUTPUT);
  while (1) {
    digitalWrite(led1, HIGH);
    vTaskDelay(250 / portTICK_PERIOD_MS);
    digitalWrite(led1, LOW);
    vTaskDelay(250 / portTICK_PERIOD_MS);
  }
}
void blinky2(void *parameter) {
  pinMode(led2, OUTPUT);
  while (1) {
    digitalWrite(led2, HIGH);
    vTaskDelay(650 / portTICK_PERIOD_MS);
    digitalWrite(led2, LOW);
    vTaskDelay(650 / portTICK_PERIOD_MS);
  }
}
void setup() {
  Serial.begin(115200);
  pinMode(button1, INPUT_PULLUP);
  pinMode(button2, INPUT_PULLUP);
  xTaskCreatePinnedToCore(blinky1, "blinky1", 1024, NULL, 1, &taskBlinky1, 0);
  xTaskCreatePinnedToCore(blinky2, "blinky2", 1024, NULL, 1, &taskBlinky2, 0);
}

void loop() {
  if (digitalRead(button1) == HIGH) {
    vTaskResume(taskBlinky1);
    vTaskResume(taskBlinky2);
    Serial.println("Tasks resumed");
    delay(250);
  }

  if (digitalRead(button2) == HIGH) {
    vTaskSuspend(taskBlinky1);
    vTaskSuspend(taskBlinky2);
    digitalWrite(led1, LOW);
    digitalWrite(led2, LOW);
    Serial.println("Tasks suspended");
    delay(250);
  }
}
