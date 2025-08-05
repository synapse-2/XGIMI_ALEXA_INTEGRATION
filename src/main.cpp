
#include "defaults.h"
#include <Arduino.h>
#include <WiFiManager.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include "Master.h"
#include "Slave.h"
#include "UtilityFunctions.h"
#include "thingProperties.h"
#include "magicEnum/magic_enum.hpp"
#include "magicEnum/magic_enum_iostream.hpp"


template <typename E>
auto to_integer(magic_enum::Enum<E> value) -> int {
  // magic_enum::Enum<E> - C++17 Concept for enum type.
  return static_cast<magic_enum::underlying_type_t<E>>(value);
}

// Define the LED_BUILTIN pin for the ESP32
// This is typically GPIO 48 on many ESP32 boards, but can vary by board.

Master m;
Slave s;
TaskHandle_t Task0;
TaskHandle_t Task1;

void Task1code(void *pvParameters)
{
  UtilityFunctions::UtilityFunctionsInit(); // Initialize utility functions

  UtilityFunctions::debugLog("Running...");

  UtilityFunctions::ledRed(); // Turn on the LED to indicate setup is complete

  // Check if the device is in master or slave mode
  if (UtilityFunctions::isMaster())
  {
    UtilityFunctions::debugLog("Device is in Master mode. Starting MASTER job");

#ifdef XIGIMI_DEBUG_WIFI_OFF
#else
    m.start(); // Start the master functionality

    // Defined in thingProperties.h for AIot cloud

    initProperties();
    // Connect to Arduino IoT Cloud
    // initialize the WiFiConnectionHandler pointer
    iot_connector = new WiFiConnectionHandler(m.getSSID().c_str(), m.getPSK().c_str());

    ArduinoCloud.begin(*iot_connector);

    /*
       The following function allows you to obtain more information
       related to the state of network and IoT Cloud connection and errors
       the higher number the more granular information you’ll get.
       The default is 0 (only errors).
       Maximum is 4
   */
    setDebugMessageLevel(ArduinoCloudDebugLevel);
    ArduinoCloud.printDebugInfo();
#endif
  }
  else
  {
    UtilityFunctions::debugLog("Device is in Slave mode.");
    return; // exit this task as we are not the master
  }

  // this will only run if we are the master
  UtilityFunctions::debugLog("we are in the MAIN LOOP");
  for (;;) // infinite loop
  {
    UtilityFunctions::delay(AIOT_POLL_TIME);

    // If in master mode, update the properties
#ifdef XIGIMI_DEBUG_WIFI_OFF
    UtilityFunctions::debugLog("WIFI is truned off for  DEBUG via #define XIGIMI_DEBUG_WIFI_OFF");
#else
    m.checkResetPressed(); // Check if the reset button has been pressed
    ArduinoCloud.update();
#endif
  }
}

// Task0code: bluetooth server only
void Task0code(void *pvParameters)
{

  UtilityFunctions::waitTillInitComplete(); // master core will do the init we wait till then
  UtilityFunctions::debugLog("Task0 Init COMPLETE ");

  s.start();
  UtilityFunctions::debugLog("Task0 BLE SERVER started ... ");
  for (;;) // infinite loop
  {
    UtilityFunctions::ledStop();
    UtilityFunctions::delay(1000);
    UtilityFunctions::ledBlue();
  }
}

/*
  Since Projector is READ_WRITE variable, onProjectorChange() is
  executed every time a new value is received from IoT Cloud.
*/
void onProjectorChange()
{
  // Add your code here to act upon Projector change
  UtilityFunctions::debugLog("update received from cloud");
  UtilityFunctions::ledGreen(); // Turn on the LED to indicate a change has been received
  UtilityFunctions::debugLog("Projector switch changed to: " + String(projector.getSwitch()));
  UtilityFunctions::debugLogf("Projector volume changed to: %i\n", projector.getVolume());
  UtilityFunctions::debugLogf("Projector channel changed to: %i\n", projector.getChannel());
  UtilityFunctions::debugLog("Projector Mute changed to: " + String(projector.getMute()));
  String cmd = String((magic_enum::enum_name(projector.getPlaybackCommand())).data());
  UtilityFunctions::debugLogf("Projector Playback command changed to: %s\n", cmd );
  UtilityFunctions::delay(30);
  UtilityFunctions::ledStop(); // Turn off the LED after processing the change
}

void setup()
{

  // btStart(); // need to init BT stack before anything else on core 0
  xTaskCreatePinnedToCore(
      Task0code, /* Task function. */
      "Task0",   /* name of task. */
      10000,     /* Stack size of task */
      NULL,      /* parameter of the task */
      1,         /* priority of the task */
      &Task0,    /* Task handle to keep track of created task */
      0);        /* pin task to core 0 */

  // create a task that will be executed in the Task2code() function, with priority 1 and executed on core 1
  xTaskCreatePinnedToCore(
      Task1code, /* Task function. */
      "Task1",   /* name of task. */
      10000,     /* Stack size of task */
      NULL,      /* parameter of the task */
      1,         /* priority of the task */
      &Task1,    /* Task handle to keep track of created task */
      1);        /* pin task to core 1 */
}

void loop()
{
  yield(); // for the watchdog timer on core 0
}