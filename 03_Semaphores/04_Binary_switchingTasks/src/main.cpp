
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
FreeRTOS Semaphores | 01.2025
*/
#include <Arduino.h>
const int ledPin1 = D10;
const int ledPin2 = D9;

SemaphoreHandle_t semphr;

void blink1(void *pvParameters)
{
  pinMode(ledPin1, OUTPUT);
  while (1)
  {
    for (int i = 0; i < 5; i++)
    {
      digitalWrite(ledPin1, HIGH);
      vTaskDelay(pdMS_TO_TICKS(573));
      digitalWrite(ledPin1, LOW);
      vTaskDelay(pdMS_TO_TICKS(573));
    }

    xSemaphoreGive(semphr);

    for (int i = 0; i < 4; i++)
    {
      digitalWrite(ledPin1, HIGH);
      vTaskDelay(pdMS_TO_TICKS(273));
      digitalWrite(ledPin1, LOW);
      vTaskDelay(pdMS_TO_TICKS(273));
    }

    delay(2000);
  }
}
void blink2(void *pvParameters)
{
  pinMode(ledPin2, OUTPUT);
  while (1)
  {
    xSemaphoreTake(semphr, portMAX_DELAY);
    for (int i = 0; i < 10; i++)
    {
      digitalWrite(ledPin2, HIGH);
      vTaskDelay(pdMS_TO_TICKS(73));
      digitalWrite(ledPin2, LOW);
      vTaskDelay(pdMS_TO_TICKS(73));
    }
  }
}

void setup()
{
  semphr = xSemaphoreCreateBinary();

  xTaskCreate(blink1, "Blink 1", 2048, NULL, 1, NULL);
  xTaskCreate(blink2, "Blink 2", 2048, NULL, 1, NULL);
}
void loop() {}
