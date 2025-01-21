
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
FreeRTOS Queues | 01.2025

*/
#include "button.h"
#include <Arduino.h>

Button btn1 = D2;
const byte led1 = D10;
static QueueHandle_t queue;

static void debounceButton(void *argp) {
  bool event;
  while (1) {
    switch (btn1.checkBTN()) {
    case 1:
      event = true;
      break;
    case 2:
      event = false;
      break;
    }
    xQueueSendToBack(queue, &event, 0);
  }
}
static void led_task(void *argp) {
  BaseType_t queueStatus;
  bool event, led = false;
  while (1) {
    queueStatus = xQueueReceive(queue, &event, portMAX_DELAY);
    assert(queueStatus == pdPASS);
    if (event) {
      digitalWrite(led1, HIGH);
    } else if (!event) {
      digitalWrite(led1, LOW);
    }
  }
}
void setup() {
  int cpucore = xPortGetCoreID();
  TaskHandle_t h;
  BaseType_t taskStatus;
  pinMode(led1, OUTPUT);
  queue = xQueueCreate(1, sizeof(bool));
  assert(queue);
  taskStatus = xTaskCreatePinnedToCore(debounceButton, "debounce", 2048,
                                       nullptr, 1, &h, cpucore);
  assert(taskStatus == pdPASS);
  assert(h);
  taskStatus =
      xTaskCreatePinnedToCore(led_task, "led", 2048, nullptr, 1, &h, cpucore);
  assert(taskStatus == pdPASS);
  assert(h);
}
void loop() {
  // weg damit
  vTaskDelete(nullptr);
}
