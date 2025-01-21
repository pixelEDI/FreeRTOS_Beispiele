
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

void IRAM_ATTR onPulse()
{
  pulseCount++;
}

void setup()
{
  Serial.begin(115200);
  pinMode(signalPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(signalPin), onPulse, RISING);
}

void loop()
{
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
