
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
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Arduino.h>
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
Button btn1 = D2;
const byte led1 = D10;
static QueueHandle_t queue;

struct SensorInfos {
  bool btnState;
  int tempValue;
  int humValue;
};
void initOled() {
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;)
      ;
  }
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println(F("Ich mag FreeRTOS und Queues!"));
  display.display();
  delay(2000);
}
static void debounceButton(void *argp) {
  SensorInfos inf;

  while (1) {
    switch (btn1.checkBTN()) {
    case 1:
      inf.btnState = true;
      inf.tempValue = random(10, 60);
      inf.humValue = random(3, 100);
      xQueueSendToBack(queue, &inf, 0);
      break;
    case 2:
      inf.btnState = false;
      xQueueSendToBack(queue, &inf, 0);
      break;
    }
  }
}
static void oled_task(void *argp) {
  BaseType_t queueStatus;
  SensorInfos infos;
  while (1) {
    queueStatus = xQueueReceive(queue, &infos, portMAX_DELAY);
    assert(queueStatus == pdPASS);
    if (infos.btnState) {
      display.clearDisplay();
      display.setTextSize(1);
      display.setTextColor(SSD1306_WHITE);
      display.setCursor(0, 0);
      display.println("My random Values");
      display.setCursor(0, 10);
      display.println("Temp");
      display.setCursor(0, 20);
      display.println(String(infos.tempValue));
      display.setCursor(0, 40);
      display.println("Hum");
      display.setCursor(0, 50);
      display.println(String(infos.humValue));
      display.display();
    } else if (!infos.btnState) {
      display.clearDisplay();
      display.display();
    }
  }
}
void setup() {
  int cpucore = xPortGetCoreID();
  TaskHandle_t h;
  BaseType_t taskStatus;
  pinMode(led1, OUTPUT);
  initOled();
  queue = xQueueCreate(1, sizeof(SensorInfos));
  assert(queue);
  taskStatus = xTaskCreatePinnedToCore(debounceButton, "debounce", 2048,
                                       nullptr, 1, &h, cpucore);
  assert(taskStatus == pdPASS);
  assert(h);
  taskStatus =
      xTaskCreatePinnedToCore(oled_task, "led", 2048, nullptr, 1, &h, cpucore);
  assert(taskStatus == pdPASS);
  assert(h);
}

void loop() {
  // weg damit
  vTaskDelete(nullptr);
}
