
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
Synchronisation von Tasks
*/

#include <Arduino.h>

#include "DFRobot_SHTC3.h"
#include "ccs811.h" // by Maarten Pennings
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Wire.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// CCS811 Sensor
CCS811 ccs811(-1);
uint16_t eco2, etvoc, errstat, raw;
/*unsigned long previousMillis = 0;*/

DFRobot_SHTC3 SHTC3;
uint32_t id = 0;
float temperature, humidity;

SemaphoreHandle_t binarySemaphore;

/*void getSHTC3Data() {*/
static void getSHTValues(void *argp)
{

  while (true)
  {
    if (xSemaphoreTake(binarySemaphore, portMAX_DELAY) == pdTRUE)
    {
      temperature =
          SHTC3.getTemperature(/*mode = */ PRECISION_HIGH_CLKSTRETCH_ON);

      humidity = SHTC3.getHumidity(/*mode = */ PRECISION_HIGH_CLKSTRETCH_OFF);
      if (temperature == MODE_ERR)
      {
        Serial.println("Incorrect mode configuration to get temperature");
      }
      else
      {
        Serial.print("Temperature :");
        Serial.print(temperature);
        Serial.println(" C");
      }
      if (humidity == MODE_ERR)
      {
        Serial.println("The mode for getting humidity was misconfigured");
      }
      else
      {
        Serial.print("Humidity :");
        Serial.print(humidity);
        Serial.println(" %RH");
      }
      xSemaphoreGive(binarySemaphore);
    }
    vTaskDelay(pdMS_TO_TICKS(1500));
  }
}

void setupSHTC3()
{
  SHTC3.begin();
  SHTC3.wakeup();

  while ((id = SHTC3.getDeviceID()) == 0)
  {
    Serial.println("ID retrieval error, please check whether the device is "
                   "connected correctly!!!");
    delay(1000);
  }
}

static void oled_task(void *argp)
{
  while (true)
  {
    if (xSemaphoreTake(binarySemaphore, portMAX_DELAY) == pdTRUE)
    {
      display.clearDisplay();
      display.setTextSize(1);
      display.setTextColor(WHITE);
      // eco2 data.
      display.setCursor(0, 2);
      display.setTextSize(2);
      display.print("eco2");

      display.setCursor(10, 35);
      display.setTextSize(1);
      display.print(eco2);
      display.print(" ppm ");

      display.drawLine(SCREEN_WIDTH / 2, 30, SCREEN_WIDTH / 2, SCREEN_HEIGHT,
                       WHITE);

      byte halfscreen = SCREEN_WIDTH / 2;
      display.setTextSize(2);
      display.setCursor(halfscreen, 2);
      display.print("Temp");

      display.setCursor(halfscreen + 20, 35);
      display.setTextSize(1);
      display.print(temperature);
      display.print(" C");

      display.display();
      xSemaphoreGive(binarySemaphore);
    }
    vTaskDelay(pdMS_TO_TICKS(500));
  }
}

static void getCCS811Values(void *argp)
{
  while (true)
  {
    ccs811.read(&eco2, &etvoc, &errstat, &raw);

    if (errstat == CCS811_ERRSTAT_OK)
    {
      Serial.println(eco2);
      Serial.println(etvoc);
      Serial.println("--------");
    }
    else if (errstat == CCS811_ERRSTAT_OK_NODATA)
    {
      Serial.println("CCS811: waiting for (new) data");
    }
    else if (errstat & CCS811_ERRSTAT_I2CFAIL)
    {
      Serial.println("CCS811: I2C error");
    }
    else
    {
      Serial.print("CCS811: errstat=");
      Serial.print(errstat, HEX);
      Serial.print("=");
      Serial.println(ccs811.errstat_str(errstat));
    }

    xSemaphoreGive(binarySemaphore);
    vTaskDelay(pdMS_TO_TICKS(2000));
  }
}

void setup()
{
  Serial.begin(115200);

  setupSHTC3();
  delay(1000);

  Wire.begin();

  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  delay(100);

  ccs811.set_i2cdelay(50);
  bool ok = ccs811.begin();
  if (!ok)
    Serial.println("setup: CCS811 begin FAILED");

  ok = ccs811.start(CCS811_MODE_1SEC);
  if (!ok)
    Serial.println("setup: CCS811 start FAILED");



  int cpucore = xPortGetCoreID();
  TaskHandle_t h1;
  TaskHandle_t h2;
  TaskHandle_t h3;
  BaseType_t taskStatus;

  binarySemaphore = xSemaphoreCreateBinary();

  taskStatus = xTaskCreatePinnedToCore(getCCS811Values, "ccs811 values",
                                       4096,    // Stack size
                                       nullptr, // No args
                                       1,       // Priority
                                       &h1,     // Task handle
                                       cpucore  // cpu Core
  );

  assert(taskStatus == pdPASS);
  assert(h1);

  taskStatus = xTaskCreatePinnedToCore(oled_task, "oled",
                                       4096,    // Stack size
                                       nullptr, // Not used
                                       3,       // Priority
                                       &h2,     // Task handle
                                       cpucore  // CPU
  );
  assert(taskStatus == pdPASS);
  assert(h2);

  taskStatus = xTaskCreatePinnedToCore(getSHTValues, "sht3 values",
                                       4096,    // Stack size
                                       nullptr, // No args
                                       2,       // Priority
                                       &h3,     // Task handle
                                       cpucore  // cpu Core
  );

  assert(taskStatus == pdPASS);
  assert(h3);

  Serial.println("Setup finished");
}

void loop()
{
  // sowas von langweilig hier
}