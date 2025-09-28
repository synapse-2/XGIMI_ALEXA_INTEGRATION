
#include "defaults.h"
#include <Arduino.h>
#include "WiFiManager.h"
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/ringbuf.h>
#include "AcloudIOT_Decoder.h"
#include "BLE_Remote_Decoder.h"
#include "BlueRC.h"
#include "UtilityFunctions.h"
#include "thingProperties.h"
#include "CmdRingBuffer.h"
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

AcloudIOT_Decoder m;
BLE_Remote_Decoder s;
TaskHandle_t Task0;
TaskHandle_t Task1;
WiFiManager wm;

String getSSID() { return wm.getWiFiSSID(); }
String getPSK() { return wm.getWiFiPass(); }

void LogWifiDebugInfo()
{
  // can contain gargbage on esp32 if wifi is not ready yet
  UtilityFunctions::debugLog("[WIFI] WIFI_INFO DEBUG");
  UtilityFunctions::debugLog("[WIFI] MODE: " + (String)(wm.getModeString(WiFi.getMode())));
  UtilityFunctions::debugLog("[WIFI] SAVED: " + (String)(wm.getWiFiIsSaved() ? "YES" : "NO"));
  UtilityFunctions::debugLog("[WIFI] SSID: " + (String)wm.getWiFiSSID());
  UtilityFunctions::debugLog("[WIFI] CHANNEL: " + (String)(wm.getModeString(WiFi.channel())));
  UtilityFunctions::debugLog("[WIFI] RSSI: " + (String)(wm.getModeString(WiFi.RSSI())));
  UtilityFunctions::debugLog("[WIFI] PASS: " + (String)wm.getWiFiPass());
  UtilityFunctions::debugLog("[WIFI] HOSTNAME: " + (String)WiFi.getHostname());
}

void checkResetPressed()
{
  if (UtilityFunctions::isMaster())
  {
    // only check the boot button is pressed if we are the master

    if (UtilityFunctions::isResetPressed())
    {
      UtilityFunctions::debugLogf("Boot pressed num time: %i need 3 to reset system count goees to zero after 3 secs reset detected at mills %i\n", UtilityFunctions::numTimesResetPressed(), UtilityFunctions::resetMills());

      if (UtilityFunctions::numTimesResetPressed() < 3)
      {
        UtilityFunctions::unpressRest();
        return;
      }

      wm.resetSettings(); // Reset WiFi settings
      UtilityFunctions::debugLog("Resetting WiFi settings...");
      for (int i = 0; i < 5; i++)
      {
        UtilityFunctions::ledYellow();
        UtilityFunctions::delay(30);
        UtilityFunctions::ledStop();
        UtilityFunctions::delay(30);
      }
      UtilityFunctions::delay(1000); // Wait for a second before restarting
      UtilityFunctions::debugLog("Restarting ESP...");
      ESP.restart();
    }
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

void Task1code(void *pvParameters)
{

  UtilityFunctions::debugLog("TASK 1 Running...");

  // Check if the device is in master or slave mode
  if (UtilityFunctions::isMaster())
  {
    UtilityFunctions::debugLog("Device is in AcloudIOT_Decoder mode. Starting MASTER job");

#ifdef XIGIMI_DEBUG_WIFI_OFF
#else
    m.setup(getSSID(), getPSK()); // Start the master functionality
    ArduinoCloud.addProperty(projector, READWRITE, ON_CHANGE, onProjectorChange);
    m.addCallback(NetworkConnectionEvent::CONNECTED, onNetworkConnect);
    m.addCallback(NetworkConnectionEvent::DISCONNECTED, onNetworkDisconnect);
    m.addCallback(NetworkConnectionEvent::ERROR, onNetworkError);
    m.start();

#endif
  }
  else
  {
    UtilityFunctions::debugLog("Device is in BLE_Remote_Decoder mode.");
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
    checkResetPressed(); // Check if the reset button has been pressed
    ArduinoCloud.update();
    if (m.hasFirstCloudSyncHasHappened())
    {
      UtilityFunctions::ledBlinkGreen();
    }
    else
    {
      UtilityFunctions::ledBlinkYellow();
    }
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

void setup()
{

  Serial.begin(115200);
  while (!Serial)
    ; // wait for serial attach
  Serial.setDebugOutput(true);
  UtilityFunctions::debugLog("Initializing ALEXA XIGIMI...");
  UtilityFunctions::UtilityFunctionsInit(); // Initialize utility functions

  // Create a ring buffer of 16 bytes with no-split type
  CmdRingBuffer::initCmdRingBuffer();

  // Check if the device is in master or slave mode
  if (UtilityFunctions::isMaster())
  {
    UtilityFunctions::debugLog("Device is in AcloudIOT_Decoder WIFI mode. Starting MASTER job if WIFI connect");

#ifdef XIGIMI_DEBUG_WIFI_OFF
#else
    // reset settings - wipe stored credentials for testing
    //  these are stored by the esp library
    //  wm.resetSettings();

    // Automatically connect using saved credentials,
    // if connection fails, it starts an access point with the specified name ( "AutoConnectAP"),
    // if empty will auto generate SSID, if password is blank it will be anonymous AP (wm.autoConnect())
    // then goes into a blocking loop awaiting configuration and will return success result

    bool res;
    UtilityFunctions::ledRed();

    UtilityFunctions::debugLog("Starting WiFiManager...");
    wm.setDebugOutput(true, WIFIDEBUG);
    wm.setConfigPortalBlocking(true);
    wm.setHostname(HOHSTNAME_Local);
    wm.setConfigPortalTimeout(AP_CONNECT_TIMEOUT); // Set the timeout for the configuration portal

    // AcloudIOT_Decoder::LogWifiDebugInfo();
    res = wm.autoConnect(); // auto generated AP name from chipid
    // res = wm.autoConnect("AutoConnectAP"); // anonymous ap
    // res = wm.autoConnect("AutoConnectAP","password"); // password protected ap

    if (!res)
    {
      UtilityFunctions::debugLog("Failed to connect");
      UtilityFunctions::ledBlinkRedLong();
      UtilityFunctions::debugLog("Failed to connect: RESTARTING");
      ESP.restart();
    }
    else
    {
      // if you get here you have connected to the WiFi
      UtilityFunctions::debugLog("connected to WIFI Network...yeey :)");
      LogWifiDebugInfo();
      UtilityFunctions::ledBlinkGreenLong();
      UtilityFunctions::ledYellow();
    }
#endif

    // Create decoder task
    m = AcloudIOT_Decoder();
    xTaskCreatePinnedToCore(
        Task0code,    /* Task function. */
        "BLE Core 0", /* name of task. */
        10000,        /* Stack size of task */
        NULL,         /* parameter of the task */
        1,            /* priority of the task */
        &Task0,       /* Task handle to keep track of created task */
        0);           /* pin task to core 0 */
  }

  s = BLE_Remote_Decoder();
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
