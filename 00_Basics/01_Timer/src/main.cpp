
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
ESP32 Timer | 01.2025

  Ein Hardware-Timer läuft unabhängig von der CPU und FreeRTOS. Er arbeitet direkt 
  auf der Hardware des ESP32 und löst präzise Interrupts aus, 
  selbst wenn die CPU ausgelastet ist. Er eignet sich für zeitkritische 
  Aufgaben mit geringer Latenz.

  Der FreeRTOS-Scheduler hingegen steuert die Ausführung von Tasks 
  im Betriebssystem. Er basiert auf einem internen Tick-Timer, der 
  typischerweise alle 1 ms (1 kHz) einen Interrupt auslöst. FreeRTOS-Timer
  sind Software-Timer, die innerhalb dieses Ticks laufen, was weniger präzise
  und abhängig von der CPU-Auslastung ist.

  Der Timer wird auf 1 MHz eingestellt, damit er genau 1 Schritt pro Mikrosekunde zählt. 
  Das erleichtert die Zeitberechnung, da Intervalle direkt in Mikrosekunden angegeben 
  werden können. 

  Der Prescaler teilt dazu die ursprüngliche Eingangsfrequenz 
  des ESP32 von 80 MHz (80 Millionen Schritte pro Sekunde) durch 80. 
  So läuft der Timer langsamer, bleibt aber präzise und einfach zu konfigurieren.

*/
#include <Arduino.h>
#define LED_PIN 10        
hw_timer_t *timer=NULL;

void IRAM_ATTR Timer0_ISR0(){
  digitalWrite(LED_PIN, !digitalRead(LED_PIN));
}
void setup() {
  pinMode(LED_PIN, OUTPUT); 
  timer=timerBegin(1000000);
  timerAttachInterrupt(timer, &Timer0_ISR0);
  timerAlarm(timer, 350000, true, 0);
}
void loop() {
// Timer übernimmt die Arbeit  
}