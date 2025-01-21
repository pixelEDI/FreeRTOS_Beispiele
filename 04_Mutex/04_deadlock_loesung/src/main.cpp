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
FreeRTOS Mutex | 01.2025

*/

#include <Arduino.h>
TaskHandle_t task1;
TaskHandle_t task2;
SemaphoreHandle_t mutex1;
SemaphoreHandle_t mutex2;

void task1Code(void *parameter) {
  while (true) {
    if (xSemaphoreTake(mutex1, pdMS_TO_TICKS(573))) {
      Serial.println("Task 1 hat Mutex1");
      delay(273);
      // Simuliert Aufgabe
      if (xSemaphoreTake(mutex2, pdMS_TO_TICKS(573))) {
        Serial.println("Task1 hat Mutex 2");
        delay(173);
        xSemaphoreGive(mutex2);
      } else {
        Serial.println("Achtung - Task 1 kein Zugriff auf Mutex 2");
      }
      xSemaphoreGive(mutex1);
    }
    delay(73);
  }
}
void task2Code(void *parameter) {
  while (true) {
    // Task 2 nimmt mutex2 und wartet dann auf mutex1
    if (xSemaphoreTake(mutex2, pdMS_TO_TICKS(373))) {
      Serial.println("Task 2 hat Mutex 2");
      delay(100); // Simuliere Arbeit

      // Task 2 wartet jetzt auf Mutex 1, was Task 1 blockiert
      if (xSemaphoreTake(mutex1, pdMS_TO_TICKS(373))) {
        Serial.println("Task 2 hat Mutex 1");
        delay(373);
        xSemaphoreGive(mutex1);
      } else {
        Serial.println("Achtung - Task 1 kein Zugriff auf Mutex 2");
      }
      xSemaphoreGive(mutex2);
    }
    delay(10);
  }
}
void setup() {
  Serial.begin(115200);
  mutex1 = xSemaphoreCreateMutex();
  mutex2 = xSemaphoreCreateMutex();
  while (!Serial) {
    delay(1000);
    Serial.println("Warte auf Serial Verbindung...");
  }
  xTaskCreate(task1Code, "Task 1", 1000, NULL, 1, &task1);
  xTaskCreate(task2Code, "Task 2", 1000, NULL, 1, &task2);
  Serial.println("Setup Ende");
}
void loop() {
  // nix zu tun
}
