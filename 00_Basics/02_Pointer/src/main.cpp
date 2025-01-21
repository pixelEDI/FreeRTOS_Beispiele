
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
ESP32 Pointer | 01.2025

    // Der Pointer 'ichmagmillis' hat 4 Bytes, weil die
    // Größe eines Pointers auf dem ESP32 (der ein 32-Bit-System ist) immer 4
Bytes beträgt,
    // unabhängig davon, auf welchen Datentyp er zeigt.
    // In diesem Fall zeigt 'ichmagmillis' auf die Variable 'currentmillis',
    // die selbst 4 Bytes groß ist, da sie vom Typ 'long' ist. Daher
    // haben sowohl die Variable 'currentmillis' als auch der Pointer
    // 'ichmagmillis' in diesem Beispiel eine Größe von 4 Bytes.
    //
    // Die Verwendung von Pointern in Funktionen hat mehrere Vorteile:
    // 1. Speicherplatz: Auch wenn ein Pointer selbst 4 Bytes (auf einem
32-Bit-System) benötigt,
    // ist dies im Vergleich zu großen Datentypen oder komplexen
Datenstrukturen,
    // deren Kopien viel mehr Speicher benötigen würden, sehr wenig.
    // Wenn du also beispielsweise ein großes Array oder eine komplexe Struktur
übergibst,
    // wird nur die Adresse dieser Struktur übergeben, nicht eine vollständige
Kopie.
    // 2. Effizienz: Das Übergeben von Pointern kann die Laufzeit verbessern,
    // da weniger Speicher bewegt werden muss und die Funktion direkt auf die
ursprünglichen Daten zugreifen kann.
    // Das vermeidet zusätzliche Kopieroperationen, die Zeit kosten.
    // Daher ist das Übergeben von Pointern in vielen Fällen vorteilhafter als
das Übergeben von Kopien.
*/

#include <Arduino.h>

int findMaxValue(int *arr, int size) {
  int maxValue = arr[0];

  for (int i = 1; i < size; i++) {
    if (arr[i] > maxValue) {
      maxValue = arr[i];
    }
  }
  return maxValue;
}
void setup() {
  Serial.begin(115200);
  delay(1000);
}
void loop() {
  int eineVariable = 73;
  int *einPointer = &eineVariable;
  Serial.printf("eineVariable %d \n", eineVariable);
  Serial.printf("einPointer %d \n", *einPointer);
  Serial.printf("einPointer Speicheradresse %p \n", (void *)einPointer);
  eineVariable += 100;
  Serial.printf("------");
  Serial.printf("eineVariable %d \n", eineVariable);
  Serial.printf("einPointer %d \n", *einPointer);
  Serial.printf("######################################### \n");

  long currentMillis = millis();
  long *ichmagmillis = &currentMillis;
  Serial.printf("eineVariable %ld \n", currentMillis);
  Serial.printf("ichmagmillis %ld \n", *ichmagmillis);
  Serial.printf("sizeof currentmillis %zu \n", sizeof(currentMillis));
  Serial.printf("sizeof ichmagmillis %zu \n", sizeof(ichmagmillis));
  Serial.printf("######################################### \n");

  int messwerte[40] = {12, 87, 23, 73, 34, 90, 23, 76, 54, 8,  29, 41,  2,  68,
                       77, 3,  94, 58, 10, 33, 82, 17, 21, 11, 59, 980, 39, 73,
                       62, 30, 19, 86, 5,  95, 74, 16, 27, 84, 50, 36};
  int size = sizeof(messwerte) / sizeof(messwerte[0]);
  int maxVal = findMaxValue(messwerte, size);
  Serial.printf("maxVal= %d \n", maxVal);
  int *ichmagarrays = messwerte;
  Serial.printf("Size of messwerte: %zu bytes\n", sizeof(messwerte));
  Serial.printf("Size of ichmagarrays: %zu bytes\n", sizeof(ichmagarrays));
  Serial.printf("Element 3: %d \n", *(ichmagarrays + 2));
  delay(5000);
}
