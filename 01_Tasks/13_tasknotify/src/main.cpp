
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
FreeRTOS ISR | 12.2024

// Der Code erstellt eine FreeRTOS-Task, die ununterbrochen auf eine
// Benachrichtigung wartet. Sobald die Task die Benachrichtigung über
// `ulTaskNotifyTake` empfängt, wird der Zustand des Signal-Pins (D10)
// umgeschaltet, wodurch eine LED ein- oder ausgeschaltet wird. Danach gibt die
// Task den Rückgabewert der Benachrichtigung mit `printf` aus.

// Im `setup` wird die Task mit einer festgelegten Priorität auf einem
// spezifischen CPU-Kern erstellt und gestartet. In der `loop`-Funktion wird
// alle 1 Sekunde die Funktion `xTaskNotifyGive` aufgerufen, die die
// Benachrichtigung an die Task sendet und somit die oben beschriebene Aktion
// auslöst.
//
// Unterschied zu Semaphore und Queue
//
// Benachrichtigungen (`xTaskNotifyGive`, `ulTaskNotifyTake`) sind eine einfache
// und schnelle Möglichkeit, einen Task zu benachrichtigen, ohne komplexe
// Datenstrukturen zu verwenden. Sie dienen hauptsächlich dazu, ein Ereignis
// oder Signal zwischen Tasks zu übermitteln, ohne dass dabei Daten übertragen
// werden müssen. Sie sind effizient, wenn nur das Signal "benachrichtigen" oder
// "informieren" benötigt wird.

// Im Gegensatz dazu ermöglichen **Semaphoren** eine Synchronisation und den
// Austausch von Informationen, bei denen eine Semaphore oft verwendet wird, um
// Ressourcen zu sperren oder freizugeben (z.B. zur Vermeidung von Konflikten
// zwischen Tasks). Semaphoren können entweder zählbar oder binär sein und
// erlauben die Verwaltung von Ressourcen oder die Synchronisation zwischen
// mehreren Tasks.

// **Queues** hingegen sind komplexer und erlauben es, Daten zwischen Tasks zu
// senden und zu empfangen. Eine Queue speichert Werte und ermöglicht eine
// FIFO-Datenübertragung, was bedeutet, dass sie für die Kommunikation mit Daten
// (nicht nur Signalen) zwischen Tasks verwendet wird.

*/

#include <Arduino.h>
const int signalPin = D10;
const int buttonPin = D2;
static TaskHandle_t htask1;
static TaskHandle_t htask2;

static void dowildstuff(void *arg)
{
  uint32_t rv;
  while (true)
  {
    rv = ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
    digitalWrite(signalPin, !digitalRead(signalPin));
    Serial.printf("Task notified: %u \n", unsigned(rv));
  }
}
static void buttonTask(void *arg)
{
  bool lastState = HIGH;
  bool currentState;
  while (true)
  {
    currentState = digitalRead(buttonPin);
    if (currentState == LOW && lastState == HIGH)
    {
      Serial.println("Button wurde gedrückt");
      xTaskNotifyGive(htask1);
    }
    lastState = currentState;
    vTaskDelay(50 / portTICK_PERIOD_MS);
  }
}

void setup()
{
  Serial.begin(115200);
  int espcore = 0;
  pinMode(signalPin, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP);
  digitalWrite(signalPin, LOW);
  delay(2000);
  BaseType_t taskStatus;
  espcore = xPortGetCoreID();
  taskStatus = xTaskCreatePinnedToCore(dowildstuff, "task1", 2048, nullptr, 1, &htask1, espcore);
  assert(taskStatus == pdPASS);
  taskStatus = xTaskCreatePinnedToCore(buttonTask, "task2", 2048, nullptr, 1, &htask2, espcore);
  assert(taskStatus == pdPASS);
}

void loop()
{
  delay(5000);
  Serial.println("notify via loop");
  xTaskNotifyGive(htask1);
}
