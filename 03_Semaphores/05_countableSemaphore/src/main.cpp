
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
FreeRTOS Semaphores | 01.2025
*/
#include <Arduino.h>
const int LED1 = D10;
const int LED2 = D9;
const int LED3 = D8;
const int LED4 = D7;
SemaphoreHandle_t countingSem;

void blink1(void *parameter) {
  pinMode(LED1, OUTPUT);
  while (1) {
    xSemaphoreTake(countingSem, portMAX_DELAY);
    for (int i = 0; i < 10; i++) {
      digitalWrite(LED1, HIGH);
      vTaskDelay(pdMS_TO_TICKS(273));
      digitalWrite(LED1, LOW);
      vTaskDelay(pdMS_TO_TICKS(273));
    }
    xSemaphoreGive(countingSem);
    vTaskDelay(pdMS_TO_TICKS(200));
  }
}
void blink2(void *parameter) {
  pinMode(LED2, OUTPUT);
  while (1) {
    xSemaphoreTake(countingSem, portMAX_DELAY);
    for (int i = 0; i < 10; i++) {
      digitalWrite(LED2, HIGH);
      vTaskDelay(pdMS_TO_TICKS(373));
      digitalWrite(LED2, LOW);
      vTaskDelay(pdMS_TO_TICKS(373));
    }
    xSemaphoreGive(countingSem);
    vTaskDelay(pdMS_TO_TICKS(200));
  }
}

void blink3(void *parameter) {
  pinMode(LED3, OUTPUT);
  while (1) {
    xSemaphoreTake(countingSem, portMAX_DELAY);
    for (int i = 0; i < 10; i++) {
      digitalWrite(LED3, HIGH);
      vTaskDelay(pdMS_TO_TICKS(73));
      digitalWrite(LED3, LOW);
      vTaskDelay(pdMS_TO_TICKS(73));
    }
    xSemaphoreGive(countingSem);
    vTaskDelay(pdMS_TO_TICKS(200));
  }
}
void blink4(void *parameter) {
  pinMode(LED4, OUTPUT);
  while (1) {
    xSemaphoreTake(countingSem, portMAX_DELAY);
    for (int i = 0; i < 10; i++) {
      digitalWrite(LED4, HIGH);
      vTaskDelay(pdMS_TO_TICKS(473));
      digitalWrite(LED4, LOW);
      vTaskDelay(pdMS_TO_TICKS(473));
    }
    xSemaphoreGive(countingSem);
    vTaskDelay(pdMS_TO_TICKS(200));
  }
}
void setup() {
  countingSem = xSemaphoreCreateCounting(2, 2);
  xTaskCreate(blink1, "Blink 1", 2048, NULL, 1, NULL);
  xTaskCreate(blink2, "Blink 2", 2048, NULL, 1, NULL);
  xTaskCreate(blink3, "Blink 3", 2048, NULL, 1, NULL);
  xTaskCreate(blink4, "Blink 4", 2048, NULL, 1, NULL);
}

void loop() {}
