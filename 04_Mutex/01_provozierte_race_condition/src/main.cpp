
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

void provoke_racecondition(void *pvParameters) {
  int localVar;
  while (1) {
    localVar = counter;
    localVar++;
    // Hier könnte weitere Auswertungen stattfinden
    // Glättung, Hoch- Tiefpass etc.
    vTaskDelay(random(100, 500) / portTICK_PERIOD_MS);
    counter = localVar;
    Serial.println(counter);
  }
}
void setup() {
  Serial.begin(115200);
  xTaskCreate(provoke_racecondition, "Task 1", 1024, NULL, 1, &task1Handle);
  xTaskCreate(provoke_racecondition, "Auch Task 1", 1024, NULL, 1,
              &task2Handle);
}
void loop() {
  // nix zu tun
}
