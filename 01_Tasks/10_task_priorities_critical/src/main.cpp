
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
const byte ledCritical = D8;    // Hohe Priorität (kritische Warnung)
const byte ledImportant = D9;   // Mittlere Priorität
const byte ledBackground = D10; // Niedrige Priorität

void taskCritical(void *pvParameters) {
  while (1) {
    Serial.println("Critical Task (High Priority)");
    digitalWrite(ledCritical, HIGH);
    vTaskDelay(500);
    digitalWrite(ledCritical, LOW);
    vTaskDelay(500);
  }
}
void taskImportant(void *pvParameters) {
  while (1) {
    Serial.println("Important Task (Medium Priority)");
    digitalWrite(ledImportant, HIGH);
    vTaskDelay(2000);
    digitalWrite(ledImportant, LOW);
    vTaskDelay(2000);
  }
}
void taskBackground(void *pvParameters) {
  while (1) {
    Serial.println("Background Task (Low Priority)");
    digitalWrite(ledBackground, HIGH);
    vTaskDelay(5000);
    digitalWrite(ledBackground, LOW);
    vTaskDelay(5000);
  }
}
void setup() {
  Serial.begin(115200);
  pinMode(ledCritical, OUTPUT);
  pinMode(ledImportant, OUTPUT);
  pinMode(ledBackground, OUTPUT);
  // Task-Erstellung mit unterschiedlichen Prioritäten
  xTaskCreate(taskCritical, "Critical Task", 1000, NULL, 3,
              NULL); // Höchste Prio
  xTaskCreate(taskImportant, "Important Task", 1000, NULL, 2,
              NULL); // Mittlere Prio
  xTaskCreate(taskBackground, "Background Task", 1000, NULL, 1,
              NULL); // Niedrigste Prio
}

void loop() {
  // Der Loop bleibt leer, da FreeRTOS alle Tasks verwaltet
}
