
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
#include "RC_Webinterface.h"
#include "nvs_flash.h"

template <typename E>
auto to_integer(magic_enum::Enum<E> value) -> int
{
  // magic_enum::Enum<E> - C++17 Concept for enum type.
  return static_cast<magic_enum::underlying_type_t<E>>(value);
}

// Define the LED_BUILTIN pin for the ESP32
// This is typically GPIO 48 on many ESP32 boards, but can vary by board.

AcloudIOT_Decoder aIOT;
inline BLE_Remote_Decoder bleRemoteDecoder;
//TaskHandle_t Task0;
//TaskHandle_t Task1;
WiFiManager wm;
RC_WebInterface *rc_web;

String getSSID() { return wm.getWiFiSSID(); }
String getPSK() { return wm.getWiFiPass(); }

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
      nvs_flash_erase();
      UtilityFunctions::debugLog("Resetting ALL NVRAM settings...");
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

  aIOT.onProjectorChange(projector);
}
void onNetworkConnect()
{
  aIOT.onNetworkConnect();
}

void onNetworkDisconnect()
{

  aIOT.onNetworkDisconnect();
}

void onNetworkError()
{
  aIOT.onNetworkError();
}

// Arduino IOT task
void Task1code(void *pvParameters)
{
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

#ifdef XIGIMI_DEBUG_WIFI_OFF
    UtilityFunctions::debugLog("WIFI is truned off for  DEBUG via #define XIGIMI_DEBUG_WIFI_OFF");
#else

    UtilityFunctions::debugLog("Device is in AcloudIOT_Decoder WIFI mode. Starting MASTER job if WIFI connect");
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
    wm.setHostname(UtilityFunctions::loadLocalHostname());
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
      UtilityFunctions::ledBlinkGreenLong();
      UtilityFunctions::ledYellow();
    }

    // create webserver
    rc_web = new RC_WebInterface();
    rc_web->begin();

    // Create decoder task
    aIOT = AcloudIOT_Decoder();
    // xTaskCreatePinnedToCore(
    //     Task1code,          /* Task function. */
    //     "Wifi AIoT Core 1", /* name of task. */
    //     10000,              /* Stack size of task */
    //     NULL,               /* parameter of the task */
    //     1,                  /* priority of the task */
    //     &Task1,             /* Task handle to keep track of created task */
    //     1);                 /* pin task to core 1 */

#endif
  }

  // xTaskCreatePinnedToCore(
  //     Task0code,    /* Task function. */
  //     "BLE Core 0", /* name of task. */
  //     10000,        /* Stack size of task */
  //     NULL,         /* parameter of the task */
  //     1,            /* priority of the task */
  //     &Task0,       /* Task handle to keep track of created task */
  //     0);           /* pin task to core 0 */
}

// this shoud run on core 1
void loop()
{
  // yield(); // for the watchdog timer on core 0
  // UtilityFunctions::delay(1000);

  UtilityFunctions::debugLog("LOOP TASK Running...");

  // Check if the device is in master or slave mode
  if (UtilityFunctions::isMaster())
  {
    UtilityFunctions::debugLog("Device is in AcloudIOT_Decoder mode. Starting AIT WIFI Connext ");

#ifdef XIGIMI_DEBUG_WIFI_OFF
#else
    aIOT.setup(getSSID(), getPSK()); // Start the master functionality
    ArduinoCloud.addProperty(projector, READWRITE, ON_CHANGE, onProjectorChange);
    aIOT.addCallback(NetworkConnectionEvent::CONNECTED, onNetworkConnect);
    aIOT.addCallback(NetworkConnectionEvent::DISCONNECTED, onNetworkDisconnect);
    aIOT.addCallback(NetworkConnectionEvent::ERROR, onNetworkError);
    aIOT.start();

#endif
  }
  else
  {
    UtilityFunctions::debugLog("Device is in BLE_Remote_Decoder mode.");
  }

  UtilityFunctions::waitTillInitComplete(); // master core will do the init we wait till then
  UtilityFunctions::debugLog("BLE Init COMPLETE ");

  bleRemoteDecoder = BLE_Remote_Decoder();
  bleRemoteDecoder.start();
  UtilityFunctions::debugLog("BLE SERVER started ... ");
  for (;;) // infinite loop
  {
    /// bluetooth handle
    UtilityFunctions::delay(AIOT_POLL_TIME);
    UtilityFunctions::ledBlinkBlue();
    bleRemoteDecoder.dequeueCmd(); // see if we have a commmand

    /// TCp handle
    // If in master mode, update the properties
#ifdef XIGIMI_DEBUG_WIFI_OFF
    UtilityFunctions::debugLog("WIFI is truned off for  DEBUG via #define XIGIMI_DEBUG_WIFI_OFF");
#else
    UtilityFunctions::delay(AIOT_POLL_TIME);
    checkResetPressed(); // Check if the reset button has been pressed
    ArduinoCloud.update();
    if (rc_web != NULL)
    {
      rc_web->handleClient(); // do the web serv tasks
    }
    if (aIOT.hasFirstCloudSyncHasHappened())
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
