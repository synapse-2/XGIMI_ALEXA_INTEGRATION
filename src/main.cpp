#include <Arduino.h>

// New feature! Overclocking WS2812
// #define FASTLED_OVERCLOCK 1.2 // 20% overclock ~ 960 khz.
#include <FastLED.h>
#include <WiFiManager.h>
#include "UtilityFunctions.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "Master.h"
#include "thingProperties.h"

// Define the LED_BUILTIN pin for the ESP32
// This is typically GPIO 48 on many ESP32 boards, but can vary by board.

Master m;

void setup()
{
  Serial.begin(115200);
  while (!Serial)
    ; // wait for serial attach

  Serial.println();
  Serial.println("Initializing...");
  Serial.flush();

  Serial.println();
  Serial.println("Running...");

  /* Print chip information */
  esp_chip_info_t chip_info;
  uint32_t flash_size;
  esp_chip_info(&chip_info);
  printf("This is %s chip with %d CPU core(s), %s%s%s%s, ",
         CONFIG_IDF_TARGET,
         chip_info.cores,
         (chip_info.features & CHIP_FEATURE_WIFI_BGN) ? "WiFi/" : "",
         (chip_info.features & CHIP_FEATURE_BT) ? "BT" : "",
         (chip_info.features & CHIP_FEATURE_BLE) ? "BLE" : "",
         (chip_info.features & CHIP_FEATURE_IEEE802154) ? ", 802.15.4 (Zigbee/Thread)" : "");

  unsigned major_rev = chip_info.revision / 100;
  unsigned minor_rev = chip_info.revision % 100;
  printf("silicon revision v%d.%d, ", major_rev, minor_rev);
  if (esp_flash_get_size(NULL, &flash_size) != ESP_OK)
  {
    printf("Get flash size failed");
    return;
  }

  printf("%" PRIu32 "MB %s flash\n", flash_size / (uint32_t)(1024 * 1024),
         (chip_info.features & CHIP_FEATURE_EMB_FLASH) ? "embedded" : "external");

  printf("Minimum free heap size: %" PRIu32 " bytes\n", esp_get_minimum_free_heap_size());

  UtilityFunctions::UtilityFunctionsInit(); // Initialize utility functions
  UtilityFunctions::ledRed();               // Turn on the LED to indicate setup is complete

  // Check if the device is in master or slave mode
  if (UtilityFunctions::MasterOrSlave())
  {
    Serial.println("Device is in Master mode.");

    m.start(); // Start the master functionality

    // Defined in thingProperties.h
    initProperties();
  }
  else
  {
    Serial.println("Device is in Slave mode.");
  }
}

void loop()
{

  delay(1000);
  Serial.println("we are in the LOOP");
  m.checkResetPressed(); // Check if the reset button has been pressed
  if (UtilityFunctions::MasterOrSlave())
  {
    // If in master mode, update the properties
    ArduinoCloud.update();
  }
}

// Connect to Arduino IoT Cloud
ArduinoCloud.begin(ArduinoIoTPreferredConnection)
{

  /*
     The following function allows you to obtain more information
     related to the state of network and IoT Cloud connection and errors
     the higher number the more granular information you’ll get.
     The default is 0 (only errors).
     Maximum is 4
  */
  setDebugMessageLevel(2);
  ArduinoCloud.printDebugInfo();
}

/*
  Since Projector is READ_WRITE variable, onProjectorChange() is
  executed every time a new value is received from IoT Cloud.
*/
void onProjectorChange()
{
  // Add your code here to act upon Projector change
}
