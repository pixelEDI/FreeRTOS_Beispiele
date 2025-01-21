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
FreeRTOS WatchdogTimer | 01.2025

*/

#include <Arduino.h>
hw_timer_t *timer = NULL;
byte cnt = 0;
void IRAM_ATTR onTimer() { ESP.restart(); }
void setup() {
  Serial.begin(115200);
  // Timer initialisieren: Timer 0, Prescaler 80 (es wird 1 µs pro Tick erzeugt)
  timer = timerBegin(0, 80, true);
  timerAttachInterrupt(timer, &onTimer, true);
  timerAlarmWrite(timer, (1000000 * 4), true);
  timerAlarmEnable(timer);
}
void loop() {
  cnt++;
  // feed the watchdog
  Serial.println("good boy");
  timerWrite(timer, 0);
  delay(1000);
  // Simulation Fehler
  if (cnt >= 6) {
    Serial.println("Keine Macht dem WATCHDOG");

    delay(5000);
    Serial.println("Ich werde niemals ausgegeben :(");
  }
}
