
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
FreeRTOS tasks | 01.2025

*/

#include <Arduino.h>
#define MAX_TASKS 10
const byte led1 = 10;
const byte led2 = 9;
const byte btn = 4;
bool showList = false;
TaskStatus_t taskStatusArray[MAX_TASKS];

void blinky1(void *parameter) {
  pinMode(led1, OUTPUT);
  while (1) {
    digitalWrite(led1, HIGH);
    vTaskDelay(250 / portTICK_PERIOD_MS);
    digitalWrite(led1, LOW);
    vTaskDelay(250 / portTICK_PERIOD_MS);
  }
}
void blinky2(void *parameter) {
  pinMode(led2, OUTPUT);
  while (1) {
    digitalWrite(led2, HIGH);
    vTaskDelay(350 / portTICK_PERIOD_MS);
    digitalWrite(led2, LOW);
    vTaskDelay(350 / portTICK_PERIOD_MS);
  }
}
static void btninterrupt() {
  // kein Code nur flags!
  showList = true;
}
void setup() {
  Serial.begin(115200);
  attachInterrupt(digitalPinToInterrupt(btn), btninterrupt, FALLING);
  xTaskCreate(blinky1, "blinky1", 1024, NULL, 1, NULL);
  xTaskCreate(blinky2, "blinky2", 1024, NULL, 2, NULL);
}

void loop() {
  if (showList) {
    showList = false;
    UBaseType_t numTasks =
        uxTaskGetSystemState(taskStatusArray, MAX_TASKS, NULL);
    Serial.println(F("Task | Stack | Prio | Num"));
    Serial.println(F("##########################################"));
    for (UBaseType_t i = 0; i < numTasks; i++) {
      Serial.print(taskStatusArray[i].pcTaskName);
      Serial.print("            | \t");
      Serial.print(taskStatusArray[i].usStackHighWaterMark);
      Serial.print("| \t");
      Serial.print(taskStatusArray[i].uxCurrentPriority);
      Serial.print("| \t");
      Serial.println(taskStatusArray[i].xTaskNumber);
    }
    Serial.println(F("##########################################"));
  }
}
