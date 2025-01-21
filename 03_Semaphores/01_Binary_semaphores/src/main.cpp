
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
FreeRTOS Semaphores | 11.2024

// Im Setup wird die Semaphore einmal freigegeben, wodurch sie sofort verfügbar
// ist. Der Scheduler wählt aus, welche Task zuerst gestartet wird. Wenn die
// ledTask gestartet wird, wartet sie auf die Semaphore. Da die Semaphore beim
// Start freigegeben wurde, kann die ledTask sie sofort übernehmen (take), die
// LED ein- und ausschalten. Der triggerTask gibt alle 3 Sekunden die Semaphore
// erneut frei (give), damit die ledTask die Möglichkeit hat, die LED zu
// steuern, wann immer der triggerTask die Semaphore freigibt.
*/

#include <Arduino.h>

const int ledPin = D10;
SemaphoreHandle_t binarySemaphore;

void ledTask(void *pvParameters) {
  while (true) {
    //auf das Semaphore warten
    if(xSemaphoreTake(binarySemaphore, portMAX_DELAY) == pdTRUE){
      digitalWrite(ledPin, HIGH);
      vTaskDelay(pdMS_TO_TICKS(500));

      digitalWrite(ledPin, LOW);
      vTaskDelay(pdMS_TO_TICKS(500));
    }
  }
}

void triggerTask(void *pvParameters) {
  while (true) {
    // semaphore freigeben
    xSemaphoreGive(binarySemaphore);
    vTaskDelay(pdMS_TO_TICKS(2000));
  }
}

void setup() {
  pinMode(ledPin, OUTPUT);
  binarySemaphore = xSemaphoreCreateBinary();

  xTaskCreate(ledTask, "LED Task", 2048, NULL, 1, NULL);
  xTaskCreate(triggerTask, "Trigger Task", 2048, NULL, 1, NULL);
}

void loop() {
  // Im Loop nichts tun, alles läuft in den Tasks
}
