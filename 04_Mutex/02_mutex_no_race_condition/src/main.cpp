
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
static int counter = 0;
TaskHandle_t task1Handle = NULL;
TaskHandle_t task2Handle = NULL;
// Mutex wird FreeRTOS mit SemaphoreHandle initialisiert
static SemaphoreHandle_t mutex;

void provoke_racecondition(void *pvParameters) {
  int localVar;
  while (1) {
    if (xSemaphoreTake(mutex, 0) == pdTRUE) {
      localVar = counter;
      localVar++;
      // Hier könnte weitere Auswertungen stattfinden
      // Glättung, Hoch- Tiefpass etc.
      vTaskDelay(random(100, 500) / portTICK_PERIOD_MS);
      counter = localVar;
      Serial.println(counter);
      xSemaphoreGive(mutex);
    } else {
      // falls kein Zugriff auf mutex möglich ist
      // kann dieser Fall hier abgefangen werden
    }
  }
}
void setup() {
  Serial.begin(115200);
  mutex = xSemaphoreCreateMutex();
  xTaskCreate(provoke_racecondition, "Task 1", 1024, NULL, 1, &task1Handle);
  xTaskCreate(provoke_racecondition, "Auch Task 1", 1024, NULL, 1,
              &task2Handle);
}

void loop() {
  // nix zu tun
}
