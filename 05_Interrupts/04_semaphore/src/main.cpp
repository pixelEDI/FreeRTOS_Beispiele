
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
FreeRTOS ISR | 01.2025

*/

#include <Arduino.h>
SemaphoreHandle_t semaphore;
int led = D10;
int button = D2;

void IRAM_ATTR isr(){
  BaseType_t higherPrio = pdFALSE;
  xSemaphoreGiveFromISR(semaphore, &higherPrio);
  portYIELD_FROM_ISR();
}

void task1(void *pvParameters) {
  while (true) {
    if (xSemaphoreTake(semaphore, portMAX_DELAY) == pdTRUE) {
      // Semaphore erhalten, ISR wurde ausgelöst
      Serial.println("Interrupt wurde ausgelöst!");
      for (int i = 0; i < 6; i++) {
        digitalWrite(led, !digitalRead(led));
        delay(173);
      }
    }
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(led, OUTPUT);
  pinMode(button, INPUT_PULLUP);
  semaphore = xSemaphoreCreateBinary();

  if (semaphore == NULL) {
    Serial.println("Fehler beim Erstellen des Semaphores");
    return;
  }
  attachInterrupt(digitalPinToInterrupt(button), isr, RISING);
  xTaskCreate(task1, "Task1", 1000, NULL, 1, NULL);
}

void loop() {
  // nix zu tun hier
}
