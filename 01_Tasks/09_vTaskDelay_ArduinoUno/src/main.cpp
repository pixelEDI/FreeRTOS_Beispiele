
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

Arduino UNO
16ms = 1 Tick
1/0016s ~ 62 HZ
*/

#include <Arduino.h>
#include <Arduino_FreeRTOS.h>

void blinky1(void *parameter) {
  pinMode(LED_BUILTIN, OUTPUT);
  while (1) {
    digitalWrite(LED_BUILTIN, HIGH);
    vTaskDelay(500 / portTICK_PERIOD_MS);
    digitalWrite(LED_BUILTIN, LOW);
    vTaskDelay(500 / portTICK_PERIOD_MS);
  }
}
void showTicks(void *parameter) {
  while (1) {
    Serial.println(portTICK_PERIOD_MS);
    Serial.println(configTICK_RATE_HZ);
    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}
void setup() {
  Serial.begin(115200);
  xTaskCreate(blinky1, "blinky1", 128, NULL, 1, NULL);
  xTaskCreate(showTicks, "ticks", 128, NULL, 1, NULL);
  Serial.println("Setup finished");
}

void loop() {
  // loop wird nicht gebraucht
}
