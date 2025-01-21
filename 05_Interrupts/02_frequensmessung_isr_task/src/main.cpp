
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

// Versuchsaufbau
// Funktionsgenerator Rechtecksignal 20Hz mit 3.3V
// ESP32 misst frequenz via ISR
//
//
// Bei 20 Hz mit 50 % Duty-Cycle:
//
// Das Signal ist 25 Millisekunden "ON" (High).
// Das Signal ist 25 Millisekunden "OFF" (Low).
//

*/

#include <Arduino.h>
const int signalPin = D1;
volatile unsigned long pulseCount = 0;
unsigned long lastTime = 0;
unsigned int frequency = 0;
TaskHandle_t frequencyTaskHandle = NULL;

void IRAM_ATTR onPulse()
{
  pulseCount++;
  xTaskNotifyGive(frequencyTaskHandle);
}

void frequencyTask(void *pvParameters)
{
  uint32_t rv;

  while (true)
  {
    ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
    unsigned long currentTime = millis();
    if (currentTime - lastTime >= 1000)
    {
      detachInterrupt(digitalPinToInterrupt(signalPin));
      frequency = pulseCount;
      pulseCount = 0;
      lastTime = currentTime;
      attachInterrupt(digitalPinToInterrupt(signalPin), onPulse, RISING);

      Serial.print("Frequenz: ");
      Serial.print(frequency);
      Serial.println(" Hz");
    }
  }
}



void setup()
{
  Serial.begin(115200);
  pinMode(signalPin, INPUT_PULLUP);
  BaseType_t rc;
  int espcore = xPortGetCoreID();
  rc = xTaskCreatePinnedToCore(frequencyTask, "frequency1", 3000, nullptr, 1, &frequencyTaskHandle, espcore);
  assert(rc = pdPASS);
  assert(frequencyTaskHandle);

  attachInterrupt(digitalPinToInterrupt(signalPin), onPulse, RISING);
}

void loop()
{
  // hier gibts nix zu tun
}
