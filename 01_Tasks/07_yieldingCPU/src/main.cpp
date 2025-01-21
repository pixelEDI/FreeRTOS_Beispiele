
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
FreeRTOS | 01.2025

*/

#include <Arduino.h>
const byte gpiotoggle = D1;

static void gpiotoggle_on(void *parameter) {
  while (1) {
    digitalWrite(gpiotoggle, HIGH);
    taskYIELD();
  }
}
static void gpiotoggle_off(void *parameter) {
  while (1) {
    digitalWrite(gpiotoggle, LOW);
  }
}
void setup() {
  pinMode(gpiotoggle, OUTPUT);
  xTaskCreate(gpiotoggle_on, "gpiotoggle_on", 1024, NULL, 1, NULL);
  xTaskCreate(gpiotoggle_off, "gpiotoggle_off", 1024, NULL, 1, NULL);
}

void loop() { vTaskDelete(xTaskGetCurrentTaskHandle()); }
