
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

// Beispielpuffer
#define BUFFER_SIZE 10
int dataBuffer[BUFFER_SIZE];
int writeIndex = 0;

SemaphoreHandle_t xSemaphore;

void addToBuffer(int data) {
  if (writeIndex < BUFFER_SIZE) {
    dataBuffer[writeIndex++] = data;
    Serial.print("Daten hinzugefügt: ");
    Serial.println(data);
  } else {
    Serial.println("Puffer ist voll! Lösche Inhalt...");
    // Puffer löschen
    for (int i = 0; i < BUFFER_SIZE; i++) {
      dataBuffer[i] = 0;
    }
    writeIndex = 0; // Setze den Index zurück
    Serial.println("Puffer wurde gelöscht!");
  }
}

void dataProducerTask1(void *pvParameters) {
  while (true) {
    if (xSemaphoreTake(xSemaphore, portMAX_DELAY) == pdTRUE) {
      // Schreibe in Buffer
      int data = random(20, 1000);
      addToBuffer(data);
      Serial.println("Dataproduce 1");
      xSemaphoreGive(xSemaphore);
    } else {
      Serial.println("Fehler: Semaphore konnte nicht übernommen werden");
    }
    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}

void dataProducerTask2(void *pvParameters) {
  while (true) {
    if (xSemaphoreTake(xSemaphore, portMAX_DELAY) == pdTRUE) {
      // Schreibe in Buffer
      int data = random(300, 3000);
      addToBuffer(data);
      Serial.println("Dataproduce 2");
      xSemaphoreGive(xSemaphore);
    } else {
      Serial.println("Fehler: Semaphore konnte nicht übernommen werden");
    }
    vTaskDelay(500 / portTICK_PERIOD_MS);
  }
}

void setup() {
  Serial.begin(115200);
  xSemaphore = xSemaphoreCreateBinary();

  if (xSemaphore == NULL) {
    Serial.println("FEHLER im Setup bei Semaphore-Erstellung");
    return;
  }

  xSemaphoreGive(xSemaphore);

  xTaskCreate(dataProducerTask1, "Datenproduzent 1", 1024, NULL, 1, NULL);
  xTaskCreate(dataProducerTask2, "Datenproduzent 2", 1024, NULL, 1, NULL);
}

void loop() {
  // Der Code in loop() wird nicht benötigt, da Tasks verwendet werden
}
