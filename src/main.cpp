
#include "defaults.h"
#include <Arduino.h>
#include <WiFiManager.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/ringbuf.h>
#include "Master.h"
#include "Slave.h"
#include "BlueRC.h"
#include "UtilityFunctions.h"
#include "thingProperties.h"
#include "magicEnum/magic_enum.hpp"
#include "magicEnum/magic_enum_iostream.hpp"

template <typename E>
auto to_integer(magic_enum::Enum<E> value) -> int
{
  // magic_enum::Enum<E> - C++17 Concept for enum type.
  return static_cast<magic_enum::underlying_type_t<E>>(value);
}

// Define the LED_BUILTIN pin for the ESP32
// This is typically GPIO 48 on many ESP32 boards, but can vary by board.

Master m;
Slave s;
TaskHandle_t Task0;
TaskHandle_t Task1;
RingbufHandle_t buf_handle;

void Task1code(void *pvParameters)
{

  UtilityFunctions::debugLog("TASK 1 Running...");

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
    ArduinoCloud.addProperty(projector, READWRITE, ON_CHANGE, onProjectorChange);
    // when we get here we should have wi fi connected to the internet

    /*
      The following function allows you to obtain more information
      related to the state of network and IoT Cloud connection and errors
      the higher number the more granular information you’ll get.
      The default is 0 (only errors).
      Maximum is 4
  */
    setDebugMessageLevel(ArduinoCloudDebugLevel);
    ArduinoCloud.printDebugInfo();
    // Connect to Arduino IoT Cloud
    // initialize the WiFiConnectionHandler pointer
    iot_connector = new WiFiConnectionHandler(m.getSSID().c_str(), m.getPSK().c_str());
    // iot_connector->addCallback(NetworkConnectionEvent::CONNECTED, onNetworkConnect);
    // iot_connector->addCallback(NetworkConnectionEvent::DISCONNECTED, onNetworkDisconnect);
    // iot_connector->addCallback(NetworkConnectionEvent::ERROR, onNetworkError);

    ArduinoCloud.begin(*iot_connector);

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
    UtilityFunctions::ledBlinkBlue();
    s.dequeueCmd(); // see if we have a commmand
    UtilityFunctions::delay(AIOT_POLL_TIME);
  }
}

/*
  Since Projector is READ_WRITE variable, onProjectorChange() is
  executed every time a new value is received from IoT Cloud.
*/
void onProjectorChange()
{

  m.onProjectorChange(projector);
}
void onNetworkConnect()
{
  m.onNetworkConnect();
}

void onNetworkDisconnect()
{

  m.onNetworkDisconnect();
}

void onNetworkError()
{
  m.onNetworkError();
}

void setup()
{

  Serial.begin(115200);
  while (!Serial)
    ; // wait for serial attach
  Serial.setDebugOutput(true);
  UtilityFunctions::debugLog("Initializing ALEXA XIGIMI...");
  UtilityFunctions::UtilityFunctionsInit(); // Initialize utility functions

  // Create a ring buffer of 16 bytes with no-split type
  buf_handle = xRingbufferCreate(256, RINGBUF_TYPE_NOSPLIT);
  m = Master(buf_handle);
  s = Slave(buf_handle);

  if (buf_handle == NULL)
  {
    UtilityFunctions::debugLog("Failed to create ring buffer\n");
  }
  else
  {
    UtilityFunctions::debugLog("SUCESScreate ring buffer\n");
  }
  xTaskCreatePinnedToCore(
      Task0code,    /* Task function. */
      "BLE Core 0", /* name of task. */
      10000,        /* Stack size of task */
      NULL,         /* parameter of the task */
      1,            /* priority of the task */
      &Task0,       /* Task handle to keep track of created task */
      0);           /* pin task to core 0 */

  // create a task that will be executed in the Task2code() function, with priority 1 and executed on core 1
  xTaskCreatePinnedToCore(
      Task1code,          /* Task function. */
      "Wifi AIoT Core 1", /* name of task. */
      10000,              /* Stack size of task */
      NULL,               /* parameter of the task */
      1,                  /* priority of the task */
      &Task1,             /* Task handle to keep track of created task */
      1);                 /* pin task to core 1 */
}

// this shoud run on core 1
void loop()
{
  yield(); // for the watchdog timer on core 0
  UtilityFunctions::delay(1000);
}
