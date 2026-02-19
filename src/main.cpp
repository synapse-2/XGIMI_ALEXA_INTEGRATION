/**
 * @file main.cpp
 * @brief Brief summary of this file.
 *
 * Detailed description of the file and its purpose.
 */


#include "AcloudIOT_Decoder.h"
#include "BLE_Remote_Decoder.h"
#include "BlueRC.h"
#include "CmdRingBuffer.h"
#include "RC_Webinterface.h"
#include "Servo_Decoder.h"
#include "Relay_Decoder.h"
#include "UtilityFunctions.h"
#include "WebLogPrint.h"
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
/**
 * @brief Brief description of static_cast<magic_enum::underlying_type_t<E>>.
 *
 * @param value Describe this parameter.
 * @return return Describe the return value.
 *
 * Algorithm:
 * - Outline the high-level algorithm or approach used.
 * - Mention important data structures or invariants.
 *
 * Loops:
 * - Describe each loop purpose and termination condition.
 * - Note whether loops are nested and their effect on complexity.
 *
 * Complexity:
 * - Time: O(...)
 * - Space: O(...)
 */
  return static_cast<magic_enum::underlying_type_t<E>>(value);
}

// Define the LED_BUILTIN pin for the ESP32
// This is typically GPIO 48 on many ESP32 boards, but can vary by board.

AcloudIOT_Decoder aIOT;
inline BLE_Remote_Decoder bleRemoteDecoder;
inline Servo_Decoder ServoRemoteDecoder;
inline Relay_Decoder RelayRemoteDecoder;

// have the wifi managwer log to the web logger
WiFiManager wm = WiFiManager(*(new WebLogPrint()));
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
/**
 * @brief Brief description of resetMills.
 *
 * @param ) Describe this parameter.
 * @return UtilityFunctions:: Describe the return value.
 *
 * Algorithm:
 * - Outline the high-level algorithm or approach used.
 * - Mention important data structures or invariants.
 *
 * Loops:
 * - Describe each loop purpose and termination condition.
 * - Note whether loops are nested and their effect on complexity.
 *
 * Complexity:
 * - Time: O(...)
 * - Space: O(...)
 */
          UtilityFunctions::resetMills());

      if (UtilityFunctions::numTimesResetPressed() < 3)
      {
/**
 * @brief Brief description of unpressRest.
 *
 * @return UtilityFunctions:: Describe the return value.
 *
 * Algorithm:
 * - Outline the high-level algorithm or approach used.
 * - Mention important data structures or invariants.
 *
 * Loops:
 * - Describe each loop purpose and termination condition.
 * - Note whether loops are nested and their effect on complexity.
 *
 * Complexity:
 * - Time: O(...)
 * - Space: O(...)
 */
        UtilityFunctions::unpressRest();
        return;
      }

      wm.resetSettings(); // Reset WiFi settings
      nvs_flash_erase();
/**
 * @brief Brief description of debugLog.
 *
 * @param settings..." Describe this parameter.
 * @return UtilityFunctions:: Describe the return value.
 *
 * Algorithm:
 * - Outline the high-level algorithm or approach used.
 * - Mention important data structures or invariants.
 *
 * Loops:
 * - Describe each loop purpose and termination condition.
 * - Note whether loops are nested and their effect on complexity.
 *
 * Complexity:
 * - Time: O(...)
 * - Space: O(...)
 */
      UtilityFunctions::debugLog("Resetting ALL NVRAM settings...");
      for (int i = 0; i < 5; i++)
      {
/**
 * @brief Brief description of ledYellow.
 *
 * @return UtilityFunctions:: Describe the return value.
 *
 * Algorithm:
 * - Outline the high-level algorithm or approach used.
 * - Mention important data structures or invariants.
 *
 * Loops:
 * - Describe each loop purpose and termination condition.
 * - Note whether loops are nested and their effect on complexity.
 *
 * Complexity:
 * - Time: O(...)
 * - Space: O(...)
 */
        UtilityFunctions::ledYellow();
/**
 * @brief Brief description of delay.
 *
 * @param 30 Describe this parameter.
 * @return UtilityFunctions:: Describe the return value.
 *
 * Algorithm:
 * - Outline the high-level algorithm or approach used.
 * - Mention important data structures or invariants.
 *
 * Loops:
 * - Describe each loop purpose and termination condition.
 * - Note whether loops are nested and their effect on complexity.
 *
 * Complexity:
 * - Time: O(...)
 * - Space: O(...)
 */
        UtilityFunctions::delay(30);
/**
 * @brief Brief description of ledStop.
 *
 * @return UtilityFunctions:: Describe the return value.
 *
 * Algorithm:
 * - Outline the high-level algorithm or approach used.
 * - Mention important data structures or invariants.
 *
 * Loops:
 * - Describe each loop purpose and termination condition.
 * - Note whether loops are nested and their effect on complexity.
 *
 * Complexity:
 * - Time: O(...)
 * - Space: O(...)
 */
        UtilityFunctions::ledStop();
/**
 * @brief Brief description of delay.
 *
 * @param 30 Describe this parameter.
 * @return UtilityFunctions:: Describe the return value.
 *
 * Algorithm:
 * - Outline the high-level algorithm or approach used.
 * - Mention important data structures or invariants.
 *
 * Loops:
 * - Describe each loop purpose and termination condition.
 * - Note whether loops are nested and their effect on complexity.
 *
 * Complexity:
 * - Time: O(...)
 * - Space: O(...)
 */
        UtilityFunctions::delay(30);
      }
      UtilityFunctions::delay(1000); // Wait for a second before restarting
/**
 * @brief Brief description of debugLog.
 *
 * @param ESP..." Describe this parameter.
 * @return UtilityFunctions:: Describe the return value.
 *
 * Algorithm:
 * - Outline the high-level algorithm or approach used.
 * - Mention important data structures or invariants.
 *
 * Loops:
 * - Describe each loop purpose and termination condition.
 * - Note whether loops are nested and their effect on complexity.
 *
 * Complexity:
 * - Time: O(...)
 * - Space: O(...)
 */
      UtilityFunctions::debugLog("Restarting ESP...");
/**
 * @brief Brief description of ESP32Restart.
 *
 * @return UtilityFunctions:: Describe the return value.
 *
 * Algorithm:
 * - Outline the high-level algorithm or approach used.
 * - Mention important data structures or invariants.
 *
 * Loops:
 * - Describe each loop purpose and termination condition.
 * - Note whether loops are nested and their effect on complexity.
 *
 * Complexity:
 * - Time: O(...)
 * - Space: O(...)
 */
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

  // also log the esp 32 errors to the log
  // esp_log_level_set("*", ESP_LOG_VERBOSE);

  // have the ESP logs go to weblog
  esp_log_set_vprintf(UtilityFunctions::webLogPrintf);

  // set the arduino cloud debug to weblogPrint stream
  Debug.setDebugOutputStream(new WebLogPrint());

  Serial.setDebugOutput(true);
/**
 * @brief Brief description of debugLog.
 *
 * @param XIGIMI..." Describe this parameter.
 * @return UtilityFunctions:: Describe the return value.
 *
 * Algorithm:
 * - Outline the high-level algorithm or approach used.
 * - Mention important data structures or invariants.
 *
 * Loops:
 * - Describe each loop purpose and termination condition.
 * - Note whether loops are nested and their effect on complexity.
 *
 * Complexity:
 * - Time: O(...)
 * - Space: O(...)
 */
  UtilityFunctions::debugLog("Initializing ALEXA XIGIMI...");
  UtilityFunctions::UtilityFunctionsInit(); // Initialize utility functions

  // Create a ring buffer of 16 bytes with no-split type
/**
 * @brief Brief description of initCmdRingBuffer.
 *
 * @return CmdRingBuffer:: Describe the return value.
 *
 * Algorithm:
 * - Outline the high-level algorithm or approach used.
 * - Mention important data structures or invariants.
 *
 * Loops:
 * - Describe each loop purpose and termination condition.
 * - Note whether loops are nested and their effect on complexity.
 *
 * Complexity:
 * - Time: O(...)
 * - Space: O(...)
 */
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
/**
 * @brief Brief description of ledRed.
 *
 * @return UtilityFunctions:: Describe the return value.
 *
 * Algorithm:
 * - Outline the high-level algorithm or approach used.
 * - Mention important data structures or invariants.
 *
 * Loops:
 * - Describe each loop purpose and termination condition.
 * - Note whether loops are nested and their effect on complexity.
 *
 * Complexity:
 * - Time: O(...)
 * - Space: O(...)
 */
    UtilityFunctions::ledRed();

/**
 * @brief Brief description of debugLog.
 *
 * @param WiFiManager..." Describe this parameter.
 * @return UtilityFunctions:: Describe the return value.
 *
 * Algorithm:
 * - Outline the high-level algorithm or approach used.
 * - Mention important data structures or invariants.
 *
 * Loops:
 * - Describe each loop purpose and termination condition.
 * - Note whether loops are nested and their effect on complexity.
 *
 * Complexity:
 * - Time: O(...)
 * - Space: O(...)
 */
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
/**
 * @brief Brief description of debugLogf.
 *
 * @param init Describe this parameter.
 * @param sec:%i\n" Describe this parameter.
 * @param AP_CONNECT_TIMEOUT Describe this parameter.
 * @return UtilityFunctions:: Describe the return value.
 *
 * Algorithm:
 * - Outline the high-level algorithm or approach used.
 * - Mention important data structures or invariants.
 *
 * Loops:
 * - Describe each loop purpose and termination condition.
 * - Note whether loops are nested and their effect on complexity.
 *
 * Complexity:
 * - Time: O(...)
 * - Space: O(...)
 */
      UtilityFunctions::debugLogf("Failed to connect to wifi in startup init, and no one connected to AP in sec:%i\n", AP_CONNECT_TIMEOUT);
/**
 * @brief Brief description of ledBlinkRedLong.
 *
 * @return UtilityFunctions:: Describe the return value.
 *
 * Algorithm:
 * - Outline the high-level algorithm or approach used.
 * - Mention important data structures or invariants.
 *
 * Loops:
 * - Describe each loop purpose and termination condition.
 * - Note whether loops are nested and their effect on complexity.
 *
 * Complexity:
 * - Time: O(...)
 * - Space: O(...)
 */
      UtilityFunctions::ledBlinkRedLong();
/**
 * @brief Brief description of debugLog.
 *
 * @param RESTARTING" Describe this parameter.
 * @return UtilityFunctions:: Describe the return value.
 *
 * Algorithm:
 * - Outline the high-level algorithm or approach used.
 * - Mention important data structures or invariants.
 *
 * Loops:
 * - Describe each loop purpose and termination condition.
 * - Note whether loops are nested and their effect on complexity.
 *
 * Complexity:
 * - Time: O(...)
 * - Space: O(...)
 */
      UtilityFunctions::debugLog("Failed to connect to wifi ssid in start up init: RESTARTING");
/**
 * @brief Brief description of ESP32Restart.
 *
 * @return UtilityFunctions:: Describe the return value.
 *
 * Algorithm:
 * - Outline the high-level algorithm or approach used.
 * - Mention important data structures or invariants.
 *
 * Loops:
 * - Describe each loop purpose and termination condition.
 * - Note whether loops are nested and their effect on complexity.
 *
 * Complexity:
 * - Time: O(...)
 * - Space: O(...)
 */
      UtilityFunctions::ESP32Restart();
    }
    else
    {
      // if you get here you have connected to the WiFi
      WiFi.setAutoReconnect(true);
/**
 * @brief Brief description of debugLog.
 *
 * @param :)" Describe this parameter.
 * @return UtilityFunctions:: Describe the return value.
 *
 * Algorithm:
 * - Outline the high-level algorithm or approach used.
 * - Mention important data structures or invariants.
 *
 * Loops:
 * - Describe each loop purpose and termination condition.
 * - Note whether loops are nested and their effect on complexity.
 *
 * Complexity:
 * - Time: O(...)
 * - Space: O(...)
 */
      UtilityFunctions::debugLog("Connected to WIFI Network...yeey :)");
/**
 * @brief Brief description of ledStop.
 *
 * @return UtilityFunctions:: Describe the return value.
 *
 * Algorithm:
 * - Outline the high-level algorithm or approach used.
 * - Mention important data structures or invariants.
 *
 * Loops:
 * - Describe each loop purpose and termination condition.
 * - Note whether loops are nested and their effect on complexity.
 *
 * Complexity:
 * - Time: O(...)
 * - Space: O(...)
 */
      UtilityFunctions::ledStop();
/**
 * @brief Brief description of ledBlinkGreenLong.
 *
 * @return UtilityFunctions:: Describe the return value.
 *
 * Algorithm:
 * - Outline the high-level algorithm or approach used.
 * - Mention important data structures or invariants.
 *
 * Loops:
 * - Describe each loop purpose and termination condition.
 * - Note whether loops are nested and their effect on complexity.
 *
 * Complexity:
 * - Time: O(...)
 * - Space: O(...)
 */
      UtilityFunctions::ledBlinkGreenLong();
    }

    // create webserver
    rc_web = new RC_WebInterface();
    rc_web->begin();
/**
 * @brief Brief description of debugLog.
 *
 * @param " Describe this parameter.
 * @return UtilityFunctions:: Describe the return value.
 *
 * Algorithm:
 * - Outline the high-level algorithm or approach used.
 * - Mention important data structures or invariants.
 *
 * Loops:
 * - Describe each loop purpose and termination condition.
 * - Note whether loops are nested and their effect on complexity.
 *
 * Complexity:
 * - Time: O(...)
 * - Space: O(...)
 */
    UtilityFunctions::debugLog("Web SERVER started ... ");

#endif
  }
}

// this shoud run on core 1
void loop()
{
  // yield(); // for the watchdog timer on core 0
  // UtilityFunctions::delay(1000);

/**
 * @brief Brief description of debugLog.
 *
 * @param Running..." Describe this parameter.
 * @return UtilityFunctions:: Describe the return value.
 *
 * Algorithm:
 * - Outline the high-level algorithm or approach used.
 * - Mention important data structures or invariants.
 *
 * Loops:
 * - Describe each loop purpose and termination condition.
 * - Note whether loops are nested and their effect on complexity.
 *
 * Complexity:
 * - Time: O(...)
 * - Space: O(...)
 */
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
/**
 * @brief Brief description of debugLog.
 *
 * @param " Describe this parameter.
 * @return UtilityFunctions:: Describe the return value.
 *
 * Algorithm:
 * - Outline the high-level algorithm or approach used.
 * - Mention important data structures or invariants.
 *
 * Loops:
 * - Describe each loop purpose and termination condition.
 * - Note whether loops are nested and their effect on complexity.
 *
 * Complexity:
 * - Time: O(...)
 * - Space: O(...)
 */
    UtilityFunctions::debugLog("AIoT SERVER started ... ");

#endif
  }
  else
  {
/**
 * @brief Brief description of debugLog.
 *
 * @param mode." Describe this parameter.
 * @return UtilityFunctions:: Describe the return value.
 *
 * Algorithm:
 * - Outline the high-level algorithm or approach used.
 * - Mention important data structures or invariants.
 *
 * Loops:
 * - Describe each loop purpose and termination condition.
 * - Note whether loops are nested and their effect on complexity.
 *
 * Complexity:
 * - Time: O(...)
 * - Space: O(...)
 */
    UtilityFunctions::debugLog("Device is in BLE_Remote_Decoder mode.");
  }

  UtilityFunctions::waitTillInitComplete(); // master core will do the init we
                                            // wait till then
/**
 * @brief Brief description of debugLog.
 *
 * @param " Describe this parameter.
 * @return UtilityFunctions:: Describe the return value.
 *
 * Algorithm:
 * - Outline the high-level algorithm or approach used.
 * - Mention important data structures or invariants.
 *
 * Loops:
 * - Describe each loop purpose and termination condition.
 * - Note whether loops are nested and their effect on complexity.
 *
 * Complexity:
 * - Time: O(...)
 * - Space: O(...)
 */
  UtilityFunctions::debugLog("BLE Init COMPLETE ");

  bleRemoteDecoder = BLE_Remote_Decoder();
  bleRemoteDecoder.start();
/**
 * @brief Brief description of debugLog.
 *
 * @param " Describe this parameter.
 * @return UtilityFunctions:: Describe the return value.
 *
 * Algorithm:
 * - Outline the high-level algorithm or approach used.
 * - Mention important data structures or invariants.
 *
 * Loops:
 * - Describe each loop purpose and termination condition.
 * - Note whether loops are nested and their effect on complexity.
 *
 * Complexity:
 * - Time: O(...)
 * - Space: O(...)
 */
  UtilityFunctions::debugLog("BLE SERVER started ... ");

  ServoRemoteDecoder = Servo_Decoder();
  ServoRemoteDecoder.start();
/**
 * @brief Brief description of debugLog.
 *
 * @param " Describe this parameter.
 * @return UtilityFunctions:: Describe the return value.
 *
 * Algorithm:
 * - Outline the high-level algorithm or approach used.
 * - Mention important data structures or invariants.
 *
 * Loops:
 * - Describe each loop purpose and termination condition.
 * - Note whether loops are nested and their effect on complexity.
 *
 * Complexity:
 * - Time: O(...)
 * - Space: O(...)
 */
  UtilityFunctions::debugLog("Servo SERVER started ... ");

  RelayRemoteDecoder = Relay_Decoder();
  RelayRemoteDecoder.start();
/**
 * @brief Brief description of debugLog.
 *
 * @param " Describe this parameter.
 * @return UtilityFunctions:: Describe the return value.
 *
 * Algorithm:
 * - Outline the high-level algorithm or approach used.
 * - Mention important data structures or invariants.
 *
 * Loops:
 * - Describe each loop purpose and termination condition.
 * - Note whether loops are nested and their effect on complexity.
 *
 * Complexity:
 * - Time: O(...)
 * - Space: O(...)
 */
  UtilityFunctions::debugLog("Relay SERVER started ... ");

  // timer for the wifi disconnect reboot.
  Wifi_Disconnect_Start_Time = 0;

  // variable to sync AIOT projector vraiable with BLE device paring to prevent when projecotr is on/off and Alexia thinks it is not and vice versa
  bool syncAIoTVariableWithBLEConnect = false;
  syncAIoTVariableWithBLEConnect = UtilityFunctions::loadSyncAIoTWithBLEDevice();
/**
 * @brief Brief description of debugLogf.
 *
 * @param to:%i\n" Describe this parameter.
 * @param syncAIoTVariableWithBLEConnect Describe this parameter.
 * @return UtilityFunctions:: Describe the return value.
 *
 * Algorithm:
 * - Outline the high-level algorithm or approach used.
 * - Mention important data structures or invariants.
 *
 * Loops:
 * - Describe each loop purpose and termination condition.
 * - Note whether loops are nested and their effect on complexity.
 *
 * Complexity:
 * - Time: O(...)
 * - Space: O(...)
 */
  UtilityFunctions::debugLogf("Sync AIOT vriable with BLE paried device is set to:%i\n", syncAIoTVariableWithBLEConnect);

  bool AIOTLogOutput = false;
  for (;;) // infinite loop
  {

    /// bluetooth handle
/**
 * @brief Brief description of delay.
 *
 * @param AIOT_POLL_TIME Describe this parameter.
 * @return UtilityFunctions:: Describe the return value.
 *
 * Algorithm:
 * - Outline the high-level algorithm or approach used.
 * - Mention important data structures or invariants.
 *
 * Loops:
 * - Describe each loop purpose and termination condition.
 * - Note whether loops are nested and their effect on complexity.
 *
 * Complexity:
 * - Time: O(...)
 * - Space: O(...)
 */
    UtilityFunctions::delay(AIOT_POLL_TIME);
/**
 * @brief Brief description of ledBlinkBlue.
 *
 * @return UtilityFunctions:: Describe the return value.
 *
 * Algorithm:
 * - Outline the high-level algorithm or approach used.
 * - Mention important data structures or invariants.
 *
 * Loops:
 * - Describe each loop purpose and termination condition.
 * - Note whether loops are nested and their effect on complexity.
 *
 * Complexity:
 * - Time: O(...)
 * - Space: O(...)
 */
    UtilityFunctions::ledBlinkBlue();

    ServerDecoder::Remote_Cmd *cmd;
    cmd = CmdRingBuffer::peekCmd();
    if (cmd != NULL)
    {
      // start of command
/**
 * @brief Brief description of ledWhite.
 *
 * @return UtilityFunctions:: Describe the return value.
 *
 * Algorithm:
 * - Outline the high-level algorithm or approach used.
 * - Mention important data structures or invariants.
 *
 * Loops:
 * - Describe each loop purpose and termination condition.
 * - Note whether loops are nested and their effect on complexity.
 *
 * Complexity:
 * - Time: O(...)
 * - Space: O(...)
 */
      UtilityFunctions::ledWhite();
#ifdef XIGIMI_DEBUG_WIFI_OFF
#else
      // for on-off toggle cmd
      aIOT.doCmd(cmd);
#endif
      bleRemoteDecoder.doCmd(cmd);
      RelayRemoteDecoder.doCmd(cmd);
      ServoRemoteDecoder.doCmd(cmd);
/**
 * @brief Brief description of ledStop.
 *
 * @return UtilityFunctions:: Describe the return value.
 *
 * Algorithm:
 * - Outline the high-level algorithm or approach used.
 * - Mention important data structures or invariants.
 *
 * Loops:
 * - Describe each loop purpose and termination condition.
 * - Note whether loops are nested and their effect on complexity.
 *
 * Complexity:
 * - Time: O(...)
 * - Space: O(...)
 */
      UtilityFunctions::ledStop();
      // end of command

      // free up the memory of the cmd in ring buffer
/**
 * @brief Brief description of dequeueCmd.
 *
 * @param cmd Describe this parameter.
 * @return CmdRingBuffer:: Describe the return value.
 *
 * Algorithm:
 * - Outline the high-level algorithm or approach used.
 * - Mention important data structures or invariants.
 *
 * Loops:
 * - Describe each loop purpose and termination condition.
 * - Note whether loops are nested and their effect on complexity.
 *
 * Complexity:
 * - Time: O(...)
 * - Space: O(...)
 */
      CmdRingBuffer::dequeueCmd(cmd);
    }

    /// TCp handle
    // If in master mode, update the properties
#ifdef XIGIMI_DEBUG_WIFI_OFF
    UtilityFunctions::debugLog(
        "WIFI is truned off for  DEBUG via #define XIGIMI_DEBUG_WIFI_OFF");
#else
/**
 * @brief Brief description of delay.
 *
 * @param AIOT_POLL_TIME Describe this parameter.
 * @return UtilityFunctions:: Describe the return value.
 *
 * Algorithm:
 * - Outline the high-level algorithm or approach used.
 * - Mention important data structures or invariants.
 *
 * Loops:
 * - Describe each loop purpose and termination condition.
 * - Note whether loops are nested and their effect on complexity.
 *
 * Complexity:
 * - Time: O(...)
 * - Space: O(...)
 */
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
/**
 * @brief Brief description of debugLogf.
 *
 * @param secs Describe this parameter.
 * @param time:%llu\n" Describe this parameter.
 * @param WIFI_DISCONNET_TIMEOUT_SEC Describe this parameter.
 * @param time_elapsed Describe this parameter.
 * @param Wifi_Disconnect_Start_Time Describe this parameter.
 * @return UtilityFunctions:: Describe the return value.
 *
 * Algorithm:
 * - Outline the high-level algorithm or approach used.
 * - Mention important data structures or invariants.
 *
 * Loops:
 * - Describe each loop purpose and termination condition.
 * - Note whether loops are nested and their effect on complexity.
 *
 * Complexity:
 * - Time: O(...)
 * - Space: O(...)
 */
          UtilityFunctions::debugLogf("Wifi is NOT CONNECTED for atleast %i secs, REBOOTING time elapsed:%llu and start time:%llu\n", WIFI_DISCONNET_TIMEOUT_SEC, time_elapsed, Wifi_Disconnect_Start_Time);
/**
 * @brief Brief description of ESP32Restart.
 *
 * @return UtilityFunctions:: Describe the return value.
 *
 * Algorithm:
 * - Outline the high-level algorithm or approach used.
 * - Mention important data structures or invariants.
 *
 * Loops:
 * - Describe each loop purpose and termination condition.
 * - Note whether loops are nested and their effect on complexity.
 *
 * Complexity:
 * - Time: O(...)
 * - Space: O(...)
 */
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
    if (syncAIoTVariableWithBLEConnect != UtilityFunctions::loadSyncAIoTWithBLEDevice())
    {
      // we do not want so many debug messages so UtilityFunctions::loadSyncAIoTWithBLEDevice does not log so we have to do this
/**
 * @brief Brief description of debugLogf.
 *
 * @param \n" Describe this parameter.
 * @param syncAIoTVariableWithBLEConnect Describe this parameter.
 * @param param Describe this parameter.
 * @return UtilityFunctions:: Describe the return value.
 *
 * Algorithm:
 * - Outline the high-level algorithm or approach used.
 * - Mention important data structures or invariants.
 *
 * Loops:
 * - Describe each loop purpose and termination condition.
 * - Note whether loops are nested and their effect on complexity.
 *
 * Complexity:
 * - Time: O(...)
 * - Space: O(...)
 */
      UtilityFunctions::debugLogf("Sync AIoT vriable flag changed from:%i to :%i \n", syncAIoTVariableWithBLEConnect, UtilityFunctions::loadSyncAIoTWithBLEDevice());
      syncAIoTVariableWithBLEConnect = UtilityFunctions::loadSyncAIoTWithBLEDevice();
    }
    if (syncAIoTVariableWithBLEConnect)
    {
      if (BLEDevice::getNumBonds() != 0)
      {

        // ok we have stored device check if connected

        BLEServer *svr = BLEDevice::getServer();

        uint8_t clients = ((svr != NULL) ? svr->getConnectedCount() : 0);
        if (clients > 0)
        {
          // we have a connected client so projecor is on
          if (!projector.getSwitch())
          {
            projector.setSwitch(true);
            aIOT.setOldProjectorSwitch(false); // make the old value differnt so that when new command comes we will action it
/**
 * @brief Brief description of debugLog.
 *
 * @param connected Describe this parameter.
 * @param TRUE" Describe this parameter.
 * @return UtilityFunctions:: Describe the return value.
 *
 * Algorithm:
 * - Outline the high-level algorithm or approach used.
 * - Mention important data structures or invariants.
 *
 * Loops:
 * - Describe each loop purpose and termination condition.
 * - Note whether loops are nested and their effect on complexity.
 *
 * Complexity:
 * - Time: O(...)
 * - Space: O(...)
 */
            UtilityFunctions::debugLog("Projector is connected, but AIOT var is false setting to TRUE");
          }
        }
        else
        {
          // we donot have a connccted client so projecor is off
          if (projector.getSwitch())
          {
            projector.setSwitch(false);
            aIOT.setOldProjectorSwitch(true); // make the old value differnt so that when new command comes we will action it
/**
 * @brief Brief description of debugLog.
 *
 * @param connected Describe this parameter.
 * @param FALSE" Describe this parameter.
 * @return UtilityFunctions:: Describe the return value.
 *
 * Algorithm:
 * - Outline the high-level algorithm or approach used.
 * - Mention important data structures or invariants.
 *
 * Loops:
 * - Describe each loop purpose and termination condition.
 * - Note whether loops are nested and their effect on complexity.
 *
 * Complexity:
 * - Time: O(...)
 * - Space: O(...)
 */
            UtilityFunctions::debugLog("Projector is NOT connected, but AIOT var is true setting to FALSE");
          }
        }
      }
      else
      {
        if (!AIOTLogOutput)
        {
/**
 * @brief Brief description of debugLogf.
 *
 * @param var Describe this parameter.
 * @param paired Describe this parameter.
 * @param bonds:%i Describe this parameter.
 * @param \n" Describe this parameter.
 * @param param Describe this parameter.
 * @return UtilityFunctions:: Describe the return value.
 *
 * Algorithm:
 * - Outline the high-level algorithm or approach used.
 * - Mention important data structures or invariants.
 *
 * Loops:
 * - Describe each loop purpose and termination condition.
 * - Note whether loops are nested and their effect on complexity.
 *
 * Complexity:
 * - Time: O(...)
 * - Space: O(...)
 */
          UtilityFunctions::debugLogf("Asked to sync BLE device connect status with AIoT cloud var, but no BLE devices paired, Stored bonds:%i, Pair a devicce to make this work \n", NimBLEDevice::getNumBonds());
          AIOTLogOutput = true; // this will prrevent more log output till we reboot
        }
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
/**
 * @brief Brief description of ledBlinkGreen.
 *
 * @return UtilityFunctions:: Describe the return value.
 *
 * Algorithm:
 * - Outline the high-level algorithm or approach used.
 * - Mention important data structures or invariants.
 *
 * Loops:
 * - Describe each loop purpose and termination condition.
 * - Note whether loops are nested and their effect on complexity.
 *
 * Complexity:
 * - Time: O(...)
 * - Space: O(...)
 */
        UtilityFunctions::ledBlinkGreen();
      }
      else
      {
/**
 * @brief Brief description of ledBlinkYellow.
 *
 * @return UtilityFunctions:: Describe the return value.
 *
 * Algorithm:
 * - Outline the high-level algorithm or approach used.
 * - Mention important data structures or invariants.
 *
 * Loops:
 * - Describe each loop purpose and termination condition.
 * - Note whether loops are nested and their effect on complexity.
 *
 * Complexity:
 * - Time: O(...)
 * - Space: O(...)
 */
        UtilityFunctions::ledBlinkYellow();
      }
    }
    else
    {
/**
 * @brief Brief description of ledBlinkRed.
 *
 * @return UtilityFunctions:: Describe the return value.
 *
 * Algorithm:
 * - Outline the high-level algorithm or approach used.
 * - Mention important data structures or invariants.
 *
 * Loops:
 * - Describe each loop purpose and termination condition.
 * - Note whether loops are nested and their effect on complexity.
 *
 * Complexity:
 * - Time: O(...)
 * - Space: O(...)
 */
      UtilityFunctions::ledBlinkRed();
    }

#endif
  }
}
