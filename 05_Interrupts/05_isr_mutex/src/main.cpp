
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

SemaphoreHandle_t xMutex;
int sharedResource = 0;
volatile bool flag = false;

IRAM_ATTR void myISR() {
  BaseType_t xHigherPriorityTaskWoken = pdFALSE;

  if(xSemaphoreTakeFromISR(xMutex, &xHigherPriorityTaskWoken) == pdTRUE){
    sharedResource++;
    flag=true;
    xSemaphoreGiveFromISR(xMutex, &xHigherPriorityTaskWoken);
  }
  portYIELD_FROM_ISR();
}

// Task 1: Arbeitet mit der Ressource und gibt ihren Wert aus
void myTask(void *pvParameters) {
  while (true) {
    if(xSemaphoreTake(xMutex, portMAX_DELAY) == pdTRUE){
      sharedResource++;
      Serial.print("inkremented: ");
      Serial.println(sharedResource);

      xSemaphoreGive(xMutex);
    }
    vTaskDelay(pdMS_TO_TICKS(3000));
  }
}

// Task 2: Zeigt alle 5 Sekunden den Wert der Ressource
void myTask2(void *pvParameters) {
  while (true) {
    if(xSemaphoreTake(xMutex, portMAX_DELAY) == pdTRUE){
      
      Serial.print("Show sharedResource: ");
      Serial.println(sharedResource);

      xSemaphoreGive(xMutex);
    }
    vTaskDelay(pdMS_TO_TICKS(5000));
  }
}

// Task 3: Dekrementiert den Wert der Ressource alle 2 Sekunden
void myTask3(void *pvParameters) {
  while (true) {
    if(xSemaphoreTake(xMutex, portMAX_DELAY) == pdTRUE){
      sharedResource--;
      Serial.print("dekremented: ");
      Serial.println(sharedResource);

      xSemaphoreGive(xMutex);
    }
    vTaskDelay(pdMS_TO_TICKS(2000));
  }
}
void setup() {
  Serial.begin(115200);

  // Mutex erstellen
  xMutex = xSemaphoreCreateMutex();
  xTaskCreate(myTask, "inkrement", 2048, NULL, 1, NULL);
  xTaskCreate(myTask2, "show", 2048, NULL, 1, NULL);
  xTaskCreate(myTask3, "dekrement", 2048, NULL, 1, NULL);
  attachInterrupt(digitalPinToInterrupt(4), myISR, RISING);

  // Hier wird der FreeRTOS-Scheduler gestartet (automatisch durch Arduino)
}

void loop() {
  // Wenn die ISR das Flag gesetzt hat, gebe eine Nachricht aus
  if (flag) {
    Serial.print("Shared Resource via ISR: ");
    Serial.println(sharedResource);
    flag = false; // Flag zurücksetzen
  }
}
