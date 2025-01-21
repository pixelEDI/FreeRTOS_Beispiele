
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
FreeRTOS LED Blink | 01.2025

*/
#include <Arduino.h>
const byte led1=D10;
const byte led2=D9;

void blinky1(void *parameter){
  pinMode(led1, OUTPUT);
  while(1){
    digitalWrite(led1, HIGH);
    delay(250);
    digitalWrite(led1, LOW);
    delay(250);
  }
}
void blinky2(void *parameter){
  pinMode(led2, OUTPUT);
  while(1){
    digitalWrite(led2, HIGH);
    delay(650);
    digitalWrite(led2, LOW);
    delay(650);
  }
}
void setup()
{
  Serial.begin(115200);

  xTaskCreate(
    blinky1, // Funktion
    "blinky1", // für debugging
    1024, // Stack size in word(!)
    NULL, // Parameter
    1, // Task priorität
    NULL // task handle
  );

 xTaskCreate(blinky2, "bliny2", 1024, NULL, 1, NULL);
}

void loop()
{
 // hier gibt es nichts zu tun
}
