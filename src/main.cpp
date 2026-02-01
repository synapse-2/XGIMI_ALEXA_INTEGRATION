
#include "AcloudIOT_Decoder.h"
#include "BLE_Remote_Decoder.h"
#include "BlueRC.h"
#include "CmdRingBuffer.h"
#include "RC_Webinterface.h"
#include "Servo_Decoder.h"
#include "Relay_Decoder.h"
#include "UtilityFunctions.h"
#include "WiFiManager.h"
#include "defaults.h"
#include "magicEnum/magic_enum.hpp"
#include "magicEnum/magic_enum_iostream.hpp"
#include "nvs_flash.h"
#include "thingProperties.h"
#include <Arduino.h>
#include <freertos/FreeRTOS.h>
#include <freertos/ringbuf.h>
#include <freertos/task.h>
#include <TimeLib.h>
#include "esp_timer.h"
#include "WiFiType.h"
#include "NimBLEClient.h"

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
inline Servo_Decoder ServoRemoteDecoder;
inline Relay_Decoder RelayRemoteDecoder;
WiFiManager wm;
RC_WebInterface *rc_web;
uint64_t Wifi_Disconnect_Start_Time = 0;

String getSSID() { return wm.getWiFiSSID(); }
String getPSK() { return wm.getWiFiPass(); }

void checkResetPressed()
{
  if (UtilityFunctions::isMaster())
  {
    // only check the boot button is pressed if we are the master

    if (UtilityFunctions::isResetPressed())
    {
      UtilityFunctions::debugLogf(
          "Boot pressed num time: %i need 3 to reset system count goees to "
          "zero after 3 secs reset detected at mills %i\n",
          UtilityFunctions::numTimesResetPressed(),
          UtilityFunctions::resetMills());

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
      UtilityFunctions::ESP32Restart();
    }
  }
}

/*
  Since Projector is READ_WRITE variable, onProjectorChange() is
  executed every time a new value is received from IoT Cloud.
*/
void onProjectorChange() { aIOT.onProjectorChange(projector); }
void onNetworkConnectMain() { aIOT.onNetworkConnect(); }

void onNetworkDisconnectMain() { aIOT.onNetworkDisconnect(); }

void onNetworkErrorMain() { aIOT.onNetworkError(); }

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
    UtilityFunctions::debugLog(
        "WIFI is truned off for  DEBUG via #define XIGIMI_DEBUG_WIFI_OFF");
#else

    UtilityFunctions::debugLog("Device is in AcloudIOT_Decoder WIFI mode. "
                               "Starting MASTER job of WIFI connect");
    // reset settings - wipe stored credentials for testing
    //  these are stored by the esp library
    //  wm.resetSettings();

    // Automatically connect using saved credentials,
    // if connection fails, it starts an access point with the specified name (
    // "AutoConnectAP"), if empty will auto generate SSID, if password is blank
    // it will be anonymous AP (wm.autoConnect()) then goes into a blocking loop
    // awaiting configuration and will return success result

    bool res;
    UtilityFunctions::ledRed();

    UtilityFunctions::debugLog("Starting WiFiManager...");
    wm.setDebugOutput(true, WIFIDEBUG);
    wm.setConfigPortalBlocking(true);
    wm.setHostname(UtilityFunctions::loadLocalHostname());
    wm.setShowInfoErase(false);  // no erase settings on info page
    wm.setDarkMode(true);        // show in black background
    wm.setShowInfoUpdate(false); // no OTA mode
    wm.setConfigPortalTimeout(
        AP_CONNECT_TIMEOUT); // Set the timeout for the configuration portal

    // AcloudIOT_Decoder::LogWifiDebugInfo();
    res = wm.autoConnect(); // auto generated AP name from chipid
    // res = wm.autoConnect("AutoConnectAP"); // anonymous ap
    // res = wm.autoConnect("AutoConnectAP","password"); // password protected
    // ap

    if (!res)
    {
      UtilityFunctions::debugLogf("Failed to connect to wifi in startup init, and no one connected to AP in sec:%i\n", AP_CONNECT_TIMEOUT);
      UtilityFunctions::ledBlinkRedLong();
      UtilityFunctions::debugLog("Failed to connect to wifi ssid in start up init: RESTARTING");
      UtilityFunctions::ESP32Restart();
    }
    else
    {
      // if you get here you have connected to the WiFi
      WiFi.setAutoReconnect(true);
      UtilityFunctions::debugLog("Connected to WIFI Network...yeey :)");
      UtilityFunctions::ledStop();
      UtilityFunctions::ledBlinkGreenLong();
    }

    // create webserver
    rc_web = new RC_WebInterface();
    rc_web->begin();
    UtilityFunctions::debugLog("Web SERVER started ... ");

#endif
  }
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
    UtilityFunctions::debugLog(
        "Device is in AcloudIOT_Decoder mode. Starting AIoT WIFI Connext ");

#ifdef XIGIMI_DEBUG_WIFI_OFF
#else
    // Create decoder task
    aIOT = AcloudIOT_Decoder();
    aIOT.setup(getSSID(), getPSK()); // Start the master functionality
    ArduinoCloud.addProperty(projector, READWRITE, ON_CHANGE,
                             onProjectorChange);
    ArduinoCloud.addCallback(ArduinoIoTCloudEvent::CONNECT,
                             onNetworkConnectMain);
    ArduinoCloud.addCallback(ArduinoIoTCloudEvent::DISCONNECT,
                             onNetworkDisconnectMain);
    // ArduinoCloud.addCallback(ArduinoIoTCloudEvent::DISCONNECT,
    // onNetworkError);
    aIOT.start();
    UtilityFunctions::debugLog("AIoT SERVER started ... ");

#endif
  }
  else
  {
    UtilityFunctions::debugLog("Device is in BLE_Remote_Decoder mode.");
  }

  UtilityFunctions::waitTillInitComplete(); // master core will do the init we
                                            // wait till then
  UtilityFunctions::debugLog("BLE Init COMPLETE ");

  bleRemoteDecoder = BLE_Remote_Decoder();
  bleRemoteDecoder.start();
  UtilityFunctions::debugLog("BLE SERVER started ... ");

  ServoRemoteDecoder = Servo_Decoder();
  ServoRemoteDecoder.start();
  UtilityFunctions::debugLog("Servo SERVER started ... ");

  RelayRemoteDecoder = Relay_Decoder();
  RelayRemoteDecoder.start();
  UtilityFunctions::debugLog("Relay SERVER started ... ");

  Wifi_Disconnect_Start_Time = 0;

  for (;;) // infinite loop
  {

    /// bluetooth handle
    UtilityFunctions::delay(AIOT_POLL_TIME);
    UtilityFunctions::ledBlinkBlue();

    ServerDecoder::Remote_Cmd *cmd;
    cmd = CmdRingBuffer::peekCmd();
    if (cmd != NULL)
    {
      // start of command
      UtilityFunctions::ledWhite();
#ifdef XIGIMI_DEBUG_WIFI_OFF
#else
      // for on-off toggle cmd
      aIOT.doCmd(cmd);
#endif
      bleRemoteDecoder.doCmd(cmd);
      RelayRemoteDecoder.doCmd(cmd);
      ServoRemoteDecoder.doCmd(cmd);
      UtilityFunctions::ledStop();
      // end of command

      // free up the memory of the cmd in ring buffer
      CmdRingBuffer::dequeueCmd(cmd);
    }

    /// TCp handle
    // If in master mode, update the properties
#ifdef XIGIMI_DEBUG_WIFI_OFF
    UtilityFunctions::debugLog(
        "WIFI is truned off for  DEBUG via #define XIGIMI_DEBUG_WIFI_OFF");
#else
    UtilityFunctions::delay(AIOT_POLL_TIME);

    // check if we are connected to Wifi
    if (WiFi.status() != WL_CONNECTED)
    {
      // we are disconnected.
      if (Wifi_Disconnect_Start_Time == 0)
      {
        // this is the first time we are disconnected
        Wifi_Disconnect_Start_Time = esp_timer_get_time(); // set this to current time
        UtilityFunctions::debugLogf("Wifi is NOT CONNECTED(State =3); current state:%i and current time:%llu\n", WiFi.status(), Wifi_Disconnect_Start_Time);
      }
      else
      {
        // we have been disconnected for some time find how long
        uint64_t time_elapsed = (esp_timer_get_time() - Wifi_Disconnect_Start_Time);
        if (time_elapsed > (WIFI_DISCONNET_TIMEOUT_SEC * 1000000))
        {
          // greater than s secs (s * 1000 * 1000)
          UtilityFunctions::debugLogf("Wifi is NOT CONNECTED for atleast %i secs, REBOOTING time elapsed:%llu and start time:%llu\n", WIFI_DISCONNET_TIMEOUT_SEC, time_elapsed, Wifi_Disconnect_Start_Time);
          UtilityFunctions::ESP32Restart();
        }
      }
    }
    else
    {
      // we are connected so reset the disconenct time
      Wifi_Disconnect_Start_Time = 0;
    }

    checkResetPressed(); // Check if the reset button has been pressed
    ArduinoCloud.update();

    // check if we need to update the cloud variable based on the BLE device connected
    if (UtilityFunctions::loadSyncAIoTWithBLEDevice())
    {
      if (BLEDevice::getNumBonds() != 0)
      {

        // ok we have stored device check if connected

        BLEServer *svr = BLEDevice::getServer();

        uint8_t clients = ((svr != NULL) ? svr->getConnectedCount() : 0);
        if (clients > 0)
        {
          // we have a connccted client so projecor is on
          if (!projector.getSwitch())
          {
            projector.setSwitch(true);
            UtilityFunctions::debugLog("Projector is connected, but AIOT var is false setting to TRUE");
          }
        }
        else
        {
          // we donot have a connccted client so projecor is off
          if (projector.getSwitch())
          {
            projector.setSwitch(false);
            UtilityFunctions::debugLog("Projector is NOT connected, but AIOT var is true setting to FALSE");
          }
        }
      }
      else
      {
        UtilityFunctions::debugLogf("Asked to sync BLE device connect status with AIoT cloud var, but no BLE devices paired, Stored bonds:{}, Pair a devicce to make this work \n", NimBLEDevice::getNumBonds());
      }
    }

    if (rc_web != NULL)
    {
      rc_web->handleClient(); // do the web serv tasks
    }

    if (aIOT.isConnectOK())
    {
      if (aIOT.hasFirstCloudSyncHasHappened())
      {
        UtilityFunctions::ledBlinkGreen();
      }
      else
      {
        UtilityFunctions::ledBlinkYellow();
      }
    }
    else
    {
      UtilityFunctions::ledBlinkRed();
    }

#endif
  }
}
