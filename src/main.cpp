#/*******************************************************************************
#  Overview:
#  This program runs on a small computer (ESP32). Think of it as a BluetoothESP32 device.
#  - It connects to the home WiFi so it can talk to the cloud (like the internet).
#  - It listens to a tiny remote (Bluetooth), a web page, and the cloud (Alexa).
#  - When someone presses buttons or tells Alexa, this BluetoothESP32 device tells the projector,
#    servos, and relays what to do (turn on/off, move, etc.).
#
#  The comments below explain each part in plain words and give a short note
#  about how each function works so anyone (even a kid) can get the idea.
#*******************************************************************************/

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

/**
 * @brief Check whether the physical reset/boot button was pressed.
 *
 * Plain words: if the BluetoothESP32 device is the boss (master) and someone presses the
 * tiny boot button 3 times, the BluetoothESP32 device will forget saved WiFi and settings
 * and then restart itself.  This is useful when you want to make it like
 * new again.
 *
 * Algorithm (simple):
 * - If we are the master device, check the reset button state.
 * - If pressed, count how many times it was pressed recently.
 * - If fewer than 3 presses: ignore (clear the press). If 3 or more:
 *   - Erase WiFi settings and NVRAM, blink an LED a few times, wait, then
 *     restart the board.
 *
 * Loops: a small for-loop blinks an LED 5 times to show the reset action.
 */
void checkResetPressed()
{
  // If this device is configured as the master, check whether the
  // physical reset/boot button was pressed and handle a factory reset
  // sequence (erase settings, blink LED, restart).
  if (UtilityFunctions::isMaster())
  {
    // only check the boot button if we are the master device

    if (UtilityFunctions::isResetPressed())
    {
      UtilityFunctions::debugLogf(
          "Boot pressed num time: %i need 3 to reset system count goees to "
          "zero after 3 secs reset detected at mills %i\n",
          UtilityFunctions::numTimesResetPressed(),
          UtilityFunctions::resetMills());

      if (UtilityFunctions::numTimesResetPressed() < 3)
      {
        // Not enough presses yet: clear and wait for more
        UtilityFunctions::unpressRest();
        return;
      }

      // Enough presses: erase settings and restart to factory-like state
      wm.resetSettings(); // Reset WiFi settings
      nvs_flash_erase();
      UtilityFunctions::debugLog("Resetting ALL NVRAM settings...");
      // Blink the yellow LED 5 times to indicate an imminent full reset.
      // Purpose: provide a visible warning before erasing settings.
      // Exit condition: loop ends after 5 iterations.
      for (int i = 0; i < 5; i++)
      {
        UtilityFunctions::ledYellow();
        UtilityFunctions::delay(30);
        UtilityFunctions::ledStop();
        UtilityFunctions::delay(30);
      }
      UtilityFunctions::delay(1000); // short pause before restart
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
  // Wait for the serial console to be ready. This is a blocking spin-wait
  // that exits once `Serial` becomes available (host opens serial terminal).
  // Exit condition: `Serial` evaluates true.
  while (!Serial)
    ; // wait for serial attach

  // also log the esp 32 errors to the log
  // esp_log_level_set("*", ESP_LOG_VERBOSE);

  // have the ESP logs go to weblog
  esp_log_set_vprintf(UtilityFunctions::webLogPrintf);

  // set the arduino cloud debug to weblogPrint stream
  Debug.setDebugOutputStream(new WebLogPrint());

  Serial.setDebugOutput(true);
  UtilityFunctions::debugLog("Initializing ALEXA XIGIMI...");
  UtilityFunctions::UtilityFunctionsInit(); // Initialize utility functions

  // Create a ring buffer of 16 bytes with no-split type
  CmdRingBuffer::initCmdRingBuffer();

  // Check if the device is in master or slave mode
  // If device is master: initialize cloud/WiFi functionality, otherwise
  // run in BLE-only (slave) mode. Exit from this block when setup
  // completes or after a restart is triggered on failure.
  if (UtilityFunctions::isMaster())
  {

    /**
     * @brief Setup (what happens once when the BluetoothESP32 device wakes up)
     *
     * Plain words: This function runs one time when the BluetoothESP32 device starts. It
     * turns on the console (so we can see messages), sets up WiFi (if we are
     * the boss/master), starts the little web server that helps configure
     * the BluetoothESP32 device, and gets everything ready for the repeating work in
     * `loop()`.
     *
     * Important steps:
     * - Start serial console for debug messages
     * - Redirect ESP logs to the web logger so logs are viewable remotely
     * - Initialize utility code and the command ring buffer
     * - If master: start WiFiManager to connect to WiFi or create an AP
     * - Create the web server so users can interact through a browser
     *
     * Loops: this function does not contain repeated loops except possible
     * short LED blink loops to show activity.
     */

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

    // If connection to WiFi failed after the config portal timeout,
    // indicate failure (long red blink) and restart to retry the
    // initialization flow. Else, continue normal startup.
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
    if (rc_web->begin())
    {
      UtilityFunctions::debugLog("Web SERVER started ... ");
    }
    else
    {
      UtilityFunctions::debugLog("Web SERVER DID NOT START ... ");
    }

#endif
  }
}

// this shoud run on core 1
void loop()
{
  // yield(); // for the watchdog timer on core 0
  // UtilityFunctions::delay(1000);

  /**
   * @brief Main loop (this runs forever)
   *
   * Plain words: This is the BluetoothESP32 device's heartbeat — it keeps running again and
   * again. The loop checks for Bluetooth remote commands, handles cloud and
   * WiFi status, updates the web server, and makes sure the projector and
   * other parts are kept in sync. Think of it as "look, listen, and act" in
   * a small repeating cycle.
   *
   * Algorithm (high-level):
   * - Wait a short time (so we don't use too much energy)
   * - Blink a blue LED to show we're alive
   * - Check if any remote (BLE) command is waiting and, if so, process it
   * - If master: handle AIoT/cloud tasks and keep WiFi alive
   * - Keep the web server running and update LEDs to show status
   *
   * Loops: The function contains an infinite `for(;;)` loop which repeatedly
   * polls for commands and handles timeouts. Inner loops are small and used
   * for retrying or blinking LEDs.
   */

  UtilityFunctions::debugLog("LOOP TASK Running...");

  // Check if the device is in master or slave mode
  // If this device is configured as master, start AIoT cloud services and
  // register callbacks; otherwise operate in BLE-only mode.
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

  // timer for the wifi disconnect reboot.
  Wifi_Disconnect_Start_Time = 0;

  // variable to sync AIOT projector vraiable with BLE device paring to prevent when projecotr is on/off and Alexia thinks it is not and vice versa
  bool syncAIoTVariableWithBLEConnect = false;
  syncAIoTVariableWithBLEConnect = UtilityFunctions::loadSyncAIoTWithBLEDevice();
  UtilityFunctions::debugLogf("Sync AIOT vriable with BLE paried device is set to:%i\n", syncAIoTVariableWithBLEConnect);

  bool AIOTLogOutput = false;
  // Main worker loop: continuously polls for BLE commands, cloud and
  // WiFi status, and handles timeouts. Intended to run indefinitely.
  // Exit condition: only stops when the device is reset or powered off.
  for (;;) // infinite loop
  {

    /// bluetooth handle
    UtilityFunctions::delay(AIOT_POLL_TIME);
    UtilityFunctions::ledBlinkBlue();

    ServerDecoder::Remote_Cmd *cmd;
    cmd = CmdRingBuffer::peekCmd();
    // If a remote (BLE) command is waiting in the ring buffer, process it
    // by dispatching to AIoT, BLE handlers, relay, and servo decoders.
    // Exit: once processed, the command is dequeued and memory freed.
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

    // Check WiFi connection: if disconnected, start/track a disconnect
    // timer and reboot the device if it remains disconnected longer than
    // `WIFI_DISCONNET_TIMEOUT_SEC`. Exit: when connected the timer resets.
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

    checkResetPressed();   // Check if the reset button has been pressed
    ArduinoCloud.update(); // check we have updates from AIoT cloud, this will triggger call backs to be called on projector variable update

    // Detect changes to the "sync AIoT with BLE" configuration flag and
    // update the local copy. Purpose: avoid spamming logs inside the called
    // helper which suppresses its own logs.
    if (syncAIoTVariableWithBLEConnect != UtilityFunctions::loadSyncAIoTWithBLEDevice())
    {
      // we do not want so many debug messages so UtilityFunctions::loadSyncAIoTWithBLEDevice does not log so we have to do this
      UtilityFunctions::debugLogf("Sync AIoT vriable flag changed from:%i to :%i \n", syncAIoTVariableWithBLEConnect, UtilityFunctions::loadSyncAIoTWithBLEDevice());
      syncAIoTVariableWithBLEConnect = UtilityFunctions::loadSyncAIoTWithBLEDevice();
    }
    // If user enabled syncing AIoT cloud state with BLE connection status,
    // check whether any bonded BLE device is currently connected. If one or
    // more clients are connected, set the `projector` cloud variable to
    // ON; otherwise set it to OFF. If no bonded devices exist, log a single
    // message asking the user to pair a device.
    // we check only if the the ring buffer is empty else we process ALL of the messages first BEFORE syncing the status
    if ((syncAIoTVariableWithBLEConnect) && (CmdRingBuffer::isEmpty()))
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
            aIOT.setOldProjectorSwitch(true); // make the old value same so that when new command comes we will action it
            UtilityFunctions::debugLog("Projector is connected, but AIOT var is false setting to TRUE");
          }
        }
        else
        {
          // we donot have a connccted client so projecor is off
          if (projector.getSwitch())
          {
            projector.setSwitch(false);
            aIOT.setOldProjectorSwitch(false); // make the old value same so that when new command comes we will action it
            UtilityFunctions::debugLog("Projector is NOT connected, but AIOT var is true setting to FALSE");
          }
        }
      }
      else
      {
        if (!AIOTLogOutput)
        {
          UtilityFunctions::debugLogf("Asked to sync BLE device connect status with AIoT cloud var, but no BLE devices paired, Stored bonds:%i, Pair a devicce to make this work \n", NimBLEDevice::getNumBonds());
          AIOTLogOutput = true; // this will prrevent more log output till we reboot
        }
      }
    }

    if (rc_web != NULL)
    {
      rc_web->handleClient(); // do the web serv tasks
    }

    // Update status LEDs based on AIoT connection and whether the first
    // cloud synchronization has completed. Green = connected and synced,
    // Yellow = connected but first sync pending, Red = not connected.
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
