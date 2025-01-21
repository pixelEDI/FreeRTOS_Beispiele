
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
void setup() {
  Serial.begin(115200);
  while (!Serial) {
    delay(10);
  }
  Serial.println("SystemTick Auslesen gestartet!");
}
void loop() {
  // Hole die aktuelle Tick-Zahl
  TickType_t ticks = xTaskGetTickCount();
  // Tick-Zahl in Millisekunden umrechnen
  uint32_t millis = ticks * portTICK_PERIOD_MS;
  // Ausgabe der Werte
  Serial.print("Ticks: ");
  Serial.print(ticks);
  Serial.print(" | Millis: ");
  Serial.println(millis);
  vTaskDelay(pdMS_TO_TICKS(1000));
}
