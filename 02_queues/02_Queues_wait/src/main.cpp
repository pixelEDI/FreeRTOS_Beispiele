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
#include <Arduino.h>
QueueHandle_t q1;
const byte led1 = D10;

void blinky1(void *parameter) {
  pinMode(led1, OUTPUT);
  while (1) {
    unsigned int buf;
    BaseType_t queueStatus;
    queueStatus = xQueueReceive(q1, &buf, (portTICK_PERIOD_MS * 500));
    if (queueStatus == pdTRUE) {
      Serial.println(buf);
      if (buf % 2 == 0) {
        digitalWrite(led1, HIGH);
      } else {
        digitalWrite(led1, LOW);
      }
    } else {
      Serial.println("no data received within Timeout");
      // noch weitere Logik - Logs etc.
    }
  }
}
void countUp(void *parameter) {
  unsigned int cnt = 0;
  while (1) {
    cnt++;
    xQueueSend(q1, &cnt, 0);
    vTaskDelay(portTICK_PERIOD_MS * 1500);
  }
}
void setup() {
  Serial.begin(115200);
  q1 = xQueueCreate(1, sizeof(unsigned int));
  xTaskCreate(blinky1, "blinky1", 2048, NULL, 1, NULL);
  xTaskCreate(countUp, "countUp", 2048, NULL, 1, NULL);
}
void loop() {
  // habe nichts zu tun :(
}
