
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
FreeRTOS Semaphores | 11.2024
*/

#include <Arduino.h>

const int ledPin = D10;
const int buttonPin1 = D2;
const int buttonPin2 = D3;
SemaphoreHandle_t binarySemaphore;

void ledTask(void *pvParameters)
{
  while (true)
  {
    // auf das Semaphore warten
    if (xSemaphoreTake(binarySemaphore, portMAX_DELAY) == pdTRUE)
    {
      digitalWrite(ledPin, HIGH);
      vTaskDelay(pdMS_TO_TICKS(500));

      digitalWrite(ledPin, LOW);
      vTaskDelay(pdMS_TO_TICKS(500));


      Serial.println("LED Taks Ende");
    }
  }
}

void triggerTask1(void *pvParameters)
{
  while (true)
  {
    if (digitalRead(buttonPin1) == HIGH)
    {
      // semaphore freigeben
      Serial.println("button 1 clicked");
      xSemaphoreGive(binarySemaphore);
      delay(500);
    }
    vTaskDelay(pdMS_TO_TICKS(500));
  }
}

void triggerTask2(void *pvParameters)
{
  while (true)
  {
    if (digitalRead(buttonPin2) == HIGH)
    {
      // semaphore freigeben
      Serial.println("button 2 clicked");
      xSemaphoreGive(binarySemaphore);
      delay(500);
    }
    vTaskDelay(pdMS_TO_TICKS(500));
  }
}

void setup()
{
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);
  pinMode(buttonPin1, INPUT_PULLUP);
  pinMode(buttonPin2, INPUT_PULLUP);

  binarySemaphore = xSemaphoreCreateBinary();

  xTaskCreate(ledTask, "LED Task", 2048, NULL, 1, NULL);
  xTaskCreate(triggerTask1, "Trigger Task1", 2048, NULL, 1, NULL);
  xTaskCreate(triggerTask2, "Trigger Task2", 2048, NULL, 1, NULL);
}

void loop()
{
  // Im Loop nichts tun, alles läuft in den Tasks
}
