#include "RC_WebInterface.h"
#include "BLE_Remote_Decoder.h"
#include "BlueRC.h"
#include "CmdRingBuffer.h"
#include "FFat.h"
#include "FS.h"
#include "UtilityFunctions.h"
#include "WString.h"
#include "defaults.h"
#include "esp32-hal.h"
#include <BlueXGIMI_RC.h>
#include <ESPmDNS.h>
#include <Preferences.h>
#include <WiFi.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

extern BLE_Remote_Decoder bleRemoteDecoder;

// Constructor
RC_WebInterface::RC_WebInterface() : _server(80)
{

  // Initialize mDNS
  // - first argument is the domain name, in this example .local is NOT
  // required.
  //   the fully-qualified domain name is "esp32.local"
  // - second argument is the IP address to advertise
  //   we send our IP address on the WiFi network

  if (!MDNS.begin(
          UtilityFunctions::loadLocalHostname()))
  { // Set the hostname to
    // "alexa_esp32.local"
    UtilityFunctions::debugLog("Error setting up MDNS responder!");
    while (1)
    {
      delay(1000);
    }
  }

  // Add service to MDNS-SD
  MDNS.addService("http", "tcp", 80);
  UtilityFunctions::debugLogf(
      "Webserver: mDNS responder started Local name:%s and IP:%s\n",
      UtilityFunctions::loadLocalHostname(), WiFi.localIP().toString().c_str());
}
String RC_WebInterface::wifiSignalStrengthDecoder(int8_t rssi)
{
  if (rssi == 0)
  {
    return "wifi-none";
  }
  if (rssi > -55)
  {
    return "wifi-high";
  }

  if (rssi > -67)
  {
    return "wifi-medium";
  }

  if (rssi > -75)
  {
    return "wifi-low";
  }

  return "wifi-none";
}
void RC_WebInterface::refreshGlobalJS()
{
  globalJS = "//start of file \n";
  globalJS = globalJS + "const projectorList = [ \n";
  for (size_t i = 0; i < projectorWakeList.size(); ++i)
  {
    globalJS = globalJS + "[ \"" +
               String(projectorWakeList[i].projName.c_str()) + "\"," +
               String(i) + " ]" +
               ((i < (projectorWakeList.size() - 1)) ? ",\n" : "\n");
  }
  globalJS = globalJS + "]; \n";

  globalJS = globalJS + "const headingTxt = '" +
             UtilityFunctions::loadBlueToothName() + "';\n";
  int8_t rssi = WiFi.RSSI();
  globalJS = globalJS + "const wifiStrength =  \"" +
             wifiSignalStrengthDecoder(rssi) + "\";\n";
  globalJS = globalJS + "const bluetoothStatus = \"" +
             ((bleRemoteDecoder.getConnectedCount() != 0) ? "bt-connected"
                                                          : "bt-advertising") +
             "\";\n";
  globalJS = globalJS + "const WifiRssi = " + String(rssi) + ";\n";
  globalJS = globalJS + "const projSelected =  " +
             String(UtilityFunctions::loadWakePacketNum()) + ";\n";
  globalJS = globalJS + "const devHostname =  \"" +
             UtilityFunctions::loadLocalHostname() + "\";\n";
  globalJS = globalJS + "const devBlueName =  \"" +
             UtilityFunctions::loadBlueToothName() + "\";\n";

  globalJS = globalJS + "const deviceID =  \"" +
             UtilityFunctions::loadAIoTDeviceID() + "\";\n";

  globalJS = globalJS + "const secretID =  \"" +
             UtilityFunctions::loadAIoTDeviceSECRET() + "\";\n";

  globalJS = globalJS +
             "const servoIOPIN =  " + UtilityFunctions::loadServoIOPin() +
             ";\n";
  globalJS = globalJS +
             "const servoMAXAngle =  " + UtilityFunctions::loadServoMaxAngle() +
             ";\n";
  globalJS = globalJS + "const servoRESTAngle =  " +
             UtilityFunctions::loadServoRestAngle() + ";\n";
  globalJS = globalJS + "const servoActionAngle =  " +
             UtilityFunctions::loadServoActionAngle() + ";\n";
  globalJS = globalJS + "const servoActionDelay =  " +
             UtilityFunctions::loadServoActionHold() + ";\n";

  String chipInfo = UtilityFunctions::chipInfo();
  chipInfo.replace("\n", "<br>\\\n");
  String taskInfo = UtilityFunctions::taskInfo();
  taskInfo.replace("\n", "<br>\\\n");
  String ledCInfo = UtilityFunctions::ledCInfo();
  ledCInfo.replace("\n", "<br>\\\n");
  String webLog = UtilityFunctions::webLog();
  webLog.replace("\n", "<br>\\\n");

  globalJS = globalJS + "const statusTxt =  \"" + chipInfo + "<br>\\\n" +
             ((UtilityFunctions::loadAIoTDeviceID().length() != NVRAM_PERFS_AIoT_DEVICE_ID_LEN) ? "<span style=\\\"color: red;\\\">AIoT DEVICE ID NOT CORRECT set in setting page</span><br>\\\n" : "") +
             ((UtilityFunctions::loadAIoTDeviceSECRET().length() != NVRAM_PERFS_AIoT_DEVICE_SECRET_LEN) ? "<span style=\\\"color: red;\\\">AIoT DEVICE SECRET NOT CORRECT set in setting page</span><br>\\\n" : "") +
             "<br>\\\n" + taskInfo + "<br>\\\n" +
             ledCInfo + "<br>\\\n" +
             "Console log: <br> \\\n" +
             webLog + "\";\n";
}

// Public begin method to start the web interface
void RC_WebInterface::begin()
{

  if (!FFat.begin(true))
  {
    UtilityFunctions::debugLog(
        "Webserver: An Error has occurred while mounting FFat");
    return;
  }

  UtilityFunctions::debugLog("Webserver: Mounted FFat OK");
  loadAdminPassword();

  setupRoutes();
  _server.begin();
}

// Public handleClient method to be called in the main loop
void RC_WebInterface::handleClient() { _server.handleClient(); }

// Load password from NVRAM
void RC_WebInterface::loadAdminPassword()
{
  _preferences.begin(NVRAM_PERFS, false);
  _adminPassword = _preferences.getString(NVRAM_PERFS_ADMIN_PASS_PROP,
                                          NVRAM_PERFS_ADMIN_PASS_DEFAULT);
  _preferences.end();
  UtilityFunctions::debugLogf(
      "Webserver: Admin password loaded from NVRAM. %s\n",
      _adminPassword.c_str());
}

// Save password to NVRAM
void RC_WebInterface::saveAdminPassword(String newPassword)
{
  _preferences.begin(NVRAM_PERFS, false);
  _preferences.putString(NVRAM_PERFS_ADMIN_PASS_PROP, newPassword);
  _preferences.end();
  _adminPassword = newPassword;
  UtilityFunctions::debugLog(
      "Webserver: Admin password updated and saved to NVRAM.");
}

// Check for HTTP basic authentication
bool RC_WebInterface::checkAdminAuth()
{
  if (!_server.authenticate(NVRAM_PERFS_ADMIN_NAME, _adminPassword.c_str()))
  {
    _server.requestAuthentication(
        BASIC_AUTH, NULL,
        (String("Default username ") + String(NVRAM_PERFS_ADMIN_NAME) +
         String(" password ") + String(NVRAM_PERFS_ADMIN_PASS_DEFAULT))
            .c_str());
    UtilityFunctions::debugLog("Webserver: auth FAILED");
    return false;
  }
  return true;
}
void RC_WebInterface::enQueueCmd(ServerDecoder::Remote_Cmd cmd)
{
  CmdRingBuffer::enQueueCmd(cmd);
}
// Handle remote control button presses by sending to ring buffer
void RC_WebInterface::handleRemotePress()
{
  if (!checkAdminAuth())
  {
    _server.send(200, "plain/txt",
                 "{ \"success\": false, \"message\": \"NOT Authenticated\" }");
    return;
  }

  ServerDecoder::Remote_Cmd rcCmd;
  bool enQueuedCmd = false;
  String action;

  if (_server.hasArg("cmd"))
  {
    action = _server.arg("cmd");
    UtilityFunctions::debugLog("Webserver: Received guest command: " + action);

    if (action.equals("power"))
    {

      // add to the ring buffer
      rcCmd.cmds.cmd = ServerDecoder::RC_Cmd_Action::On_OFF_Btn;
      enQueueCmd(rcCmd);
      enQueuedCmd = true;
    }
    if (action.equals("setting"))
    {

      // add to the ring buffer
      rcCmd.cmds.cmd = ServerDecoder::RC_Cmd_Action::Settings_Btn;
      enQueueCmd(rcCmd);
      enQueuedCmd = true;
    }

    if (action.equals("nav_up"))
    {

      // add to the ring buffer
      rcCmd.cmds.cmd = ServerDecoder::RC_Cmd_Action::Up_Btn;
      enQueueCmd(rcCmd);
      enQueuedCmd = true;
    }

    if (action.equals("nav_left"))
    {

      // add to the ring buffer
      rcCmd.cmds.cmd = ServerDecoder::RC_Cmd_Action::Left_Btn;
      enQueueCmd(rcCmd);
      enQueuedCmd = true;
    }

    if (action.equals("nav_ok"))
    {

      // add to the ring buffer
      rcCmd.cmds.cmd = ServerDecoder::RC_Cmd_Action::Ok_Btn;
      enQueueCmd(rcCmd);
      enQueuedCmd = true;
    }

    if (action.equals("nav_right"))
    {

      // add to the ring buffer
      rcCmd.cmds.cmd = ServerDecoder::RC_Cmd_Action::Right_Btn;
      enQueueCmd(rcCmd);
      enQueuedCmd = true;
    }

    if (action.equals("nav_down"))
    {

      // add to the ring buffer
      rcCmd.cmds.cmd = ServerDecoder::RC_Cmd_Action::Down_Btn;
      enQueueCmd(rcCmd);
      enQueuedCmd = true;
    }

    if (action.equals("channel_up"))
    {

      // add to the ring buffer
      rcCmd.cmds.cmd = ServerDecoder::RC_Cmd_Action::Channel_Up_Btn;
      enQueueCmd(rcCmd);
      enQueuedCmd = true;
    }

    if (action.equals("channel_down"))
    {

      // add to the ring buffer
      rcCmd.cmds.cmd = ServerDecoder::RC_Cmd_Action::Channel_Dn_Btn;
      enQueueCmd(rcCmd);
      enQueuedCmd = true;
    }

    if (action.equals("volume_up"))
    {

      // add to the ring buffer
      rcCmd.cmds.cmd = ServerDecoder::RC_Cmd_Action::Vol_Up_Btn;
      enQueueCmd(rcCmd);
      enQueuedCmd = true;
    }

    if (action.equals("volume_down"))
    {

      // add to the ring buffer
      rcCmd.cmds.cmd = ServerDecoder::RC_Cmd_Action::Vol_Dn_Btn;
      enQueueCmd(rcCmd);
      enQueuedCmd = true;
    }

    if (action.equals("back"))
    {

      // add to the ring buffer
      rcCmd.cmds.cmd = ServerDecoder::RC_Cmd_Action::Previous;
      enQueueCmd(rcCmd);
      enQueuedCmd = true;
    }

    if (action.equals("menu"))
    {

      // add to the ring buffer
      rcCmd.cmds.cmd = ServerDecoder::RC_Cmd_Action::Menu_Btn;
      enQueueCmd(rcCmd);
      enQueuedCmd = true;
    }

    if (action.equals("home"))
    {

      // add to the ring buffer
      rcCmd.cmds.cmd = ServerDecoder::RC_Cmd_Action::Home_Btn;
      enQueueCmd(rcCmd);
      enQueuedCmd = true;
    }

    if (action.equals("projector_settings"))
    {

      // add to the ring buffer
      rcCmd.cmds.cmd = ServerDecoder::RC_Cmd_Action::Projector_Setting_Btn;
      enQueueCmd(rcCmd);
      enQueuedCmd = true;
    }
  }

  if (enQueuedCmd)
  {
    _server.send(200, "plain/txt", "{ \"success\": true }");

    UtilityFunctions::debugLog("Webserver: SUCESS guest command: " + action);
  }
  else
  {
    _server.send(200, "plain/txt",
                 "{ \"success\": false, \"message\": \"command not found-" +
                     action + "\" }");
    UtilityFunctions::debugLog("Webserver: ERROR guest command: " + action);
  }
}

// Handle admin password change requests
void RC_WebInterface::handleUpdatePassword()
{
  if (!checkAdminAuth())
    return;
  if (_server.hasArg("newPassword"))
  {
    String newPassword = _server.arg("newPassword");
    if (newPassword.length() > 0)
    {
      saveAdminPassword(newPassword);
      _server.send(200, "text/plain", "Password updated successfully!");
    }
    else
    {
      _server.send(400, "text/plain", "Password cannot be empty.");
    }
  }
  else
  {
    _server.send(400, "text/plain", "Invalid request.");
  }
}

// Set up web server routes
void RC_WebInterface::setupRoutes()
{
  // Guest remote page
  _server.on("/", HTTP_GET, [this]()
             {
    File file = FFat.open("/index.htm", "r");
    if (file) {
      _server.streamFile(file, "text/html");
      file.close();
    } else {
      _server.send(404, "text/plain", "File not found");
    } });

  _server.on("/remote.css", HTTP_GET, [this]()
             {
    File file = FFat.open("/remote.css", "r");
    if (file) {
      _server.streamFile(file, "text/css");
      file.close();
    } else {
      _server.send(404, "text/plain", "File not found");
    } });

  _server.on("/script.js", HTTP_GET, [this]()
             {
    File file = FFat.open("/script.js", "r");
    if (file) {
      _server.streamFile(file, "text/javascript");
      file.close();
    } else {
      _server.send(404, "text/plain", "File not found");
    } });

  _server.on("/globals.js", HTTP_GET, [this]()
             {
    refreshGlobalJS();
    _server.send(200, "text/html", globalJS.c_str()); });

  // Handle button presses
  _server.on("/remote-action", HTTP_GET,
             std::bind(&RC_WebInterface::handleRemotePress, this));

  // Admin settings page (requires auth)
  _server.on("/settings.htm", HTTP_GET, [this]()
             {
    if (!checkAdminAuth())
      return;
    File file = FFat.open("/settings.htm", "r");
    if (file) {
      _server.streamFile(file, "text/html");
      file.close();
    } else {
      _server.send(404, "text/plain", "File not found");
    } });

  // Admin settings page (requires auth)
  _server.on("/status.htm", HTTP_GET, [this]()
             {
    if (!checkAdminAuth())
      return;
    File file = FFat.open("/status.htm", "r");
    if (file) {
      _server.streamFile(file, "text/html");
      file.close();
    } else {
      _server.send(404, "text/plain", "File not found");
    } });

  // Handle password change (requires auth)
  _server.on("/change-password", HTTP_POST,
             std::bind(&RC_WebInterface::handleUpdatePassword, this));

  // Handle forms
  _server.on("/wake-packet", HTTP_POST, [this]()
             {
               if (!checkAdminAuth())
                 return;
               long projector = _server.arg("projector").toInt();
               // Add logic here to process wake packet for the selected projector
               UtilityFunctions::debugLogf("Wake packet index update sent for: % \n",
                                           projector);
               if ((projector >= 0) && (projector < projectorWakeList.size()))
               {

                 UtilityFunctions::saveWakePacketNum(projector);
               }

               File file = FFat.open("/settings.htm", "r");
               if (file)
               {
                 _server.streamFile(file, "text/html");
                 file.close();
               }
               else
               {
                 _server.send(404, "text/plain", "File not found");
               }
               //_server.send(200, "text/plain", "Wake packet sent!");
             });

  _server.on("/send-2byte", HTTP_POST, [this]()
             {
               if (!checkAdminAuth())
                 return;
               String hex2byte = _server.arg("hex2byte");
               // Add logic here to process the 2-byte hex value
               UtilityFunctions::debugLog("Received 2-byte hex: " + hex2byte);
               if (hex2byte.length() == 4)
               {
                  ServerDecoder::Remote_Cmd rcCmd;
                  // add to the ring buffer
                  rcCmd.cmds.cmd = ServerDecoder::RC_Cmd_Action::Raw_2Byte;
                  for(int f=0;f<2;f++){
                  sscanf(hex2byte.substring(f*2,(f*2)+2).c_str(), "%2x",&rcCmd.cmds.codeData[f]);
                  }
                  enQueueCmd(rcCmd);
                  UtilityFunctions::debugLogf("Hex 2-byte packet sent with bytes %02x,%02x\n",rcCmd.cmds.codeData[0],rcCmd.cmds.codeData[1]);
                 _server.send(200, "text/plain", "2-byte hex received and queeued!");
               }
               else
               {
                 _server.send(200, "text/plain", "2-byte hex vaalue not CORRECT");
               } });

  _server.on("/send-8byte", HTTP_POST, [this]()
             {
               if (!checkAdminAuth())
                 return;
               String hex8byte = _server.arg("hex8byte");
               // Add logic here to process the 8-byte hex value
               UtilityFunctions::debugLog("Received 8-byte hex: " + hex8byte);
               if (hex8byte.length() == 16)
               {
                 ServerDecoder::Remote_Cmd rcCmd;
                 // add to the ring buffer
                 rcCmd.cmds.cmd = ServerDecoder::RC_Cmd_Action::Raw_8Byte;
                 for (int f = 0; f < 8; f++)
                 {
                   sscanf(hex8byte.substring(f * 2, (f * 2) + 2).c_str(), "%2x", &rcCmd.cmds.codeData[f]);
                 }
                 enQueueCmd(rcCmd);
                 UtilityFunctions::debugLogf("Hex 8-byte packet sent with bytes %02x,%02x,%02x,%02x,%02x,%02x,%02x,%02x\n", rcCmd.cmds.codeData[0], rcCmd.cmds.codeData[1], rcCmd.cmds.codeData[2], rcCmd.cmds.codeData[3], rcCmd.cmds.codeData[4], rcCmd.cmds.codeData[5], rcCmd.cmds.codeData[6], rcCmd.cmds.codeData[7]);
                 _server.send(200, "text/plain", "8-byte hex received and queeued!");
               }
               else
               {
                 _server.send(200, "text/plain", "8-byte hex vaalue not CORRECT");
               } });

  _server.on("/change-hostname", HTTP_POST, [this]()
             {
    if (!checkAdminAuth())
      return;
    String newHostname = _server.arg("newHostname");
    // Add logic here to change the hostname and restart
    UtilityFunctions::debugLog("Hostname change requested to: " + newHostname);
    String sucess = UtilityFunctions::saveLocalHostname(newHostname);
    if (sucess.isEmpty()) {
      _server.send(200, "text/plain", "Hostname updated! Restarting...");
      UtilityFunctions::delay(WEB_ESP_RESTART_DELAY);
      ESP.restart();
    } else {
      _server.send(200, "text/plain", "Hostname FAILED to update:" + sucess);
    } });

  _server.on("/change-bluename", HTTP_POST, [this]()
             {
    if (!checkAdminAuth())
      return;
    String newBlueName = _server.arg("newBlueName");
    // Add logic here to change the hostname and restart
    UtilityFunctions::debugLog("BlueTooth name change requested to: " +
                               newBlueName);
    String sucess = UtilityFunctions::saveBlueToothName(newBlueName);
    if (sucess.isEmpty()) {
      _server.send(200, "text/plain", "BlueTooth name updated! Restarting...");
      UtilityFunctions::delay(WEB_ESP_RESTART_DELAY);
      ESP.restart();
    } else {
      _server.send(200, "text/plain", "BlueTooth FAILED to update:" + sucess);
    } });

  _server.on("/change-servoIOPIN", HTTP_POST, [this]()
             {
    if (!checkAdminAuth())
      return;
    String arg = _server.arg("newservoIOPIN");
    // Add logic here to change the hostname and restart
    UtilityFunctions::debugLog("Servo IO PIN  change requested to: " + arg);
    int num = arg.toInt();
    if (num == 0) {
      UtilityFunctions::debugLog(
          "Servo IO PIN change is not anumber or zero: " + arg);
          _server.send(200, "text/plain", "Servo IO PIN FAILED change is not anumber or zero: " + arg);
    } else {
      String sucess = UtilityFunctions::saveServoIOPin(num);
      if (sucess.isEmpty()) {
        _server.send(200, "text/plain", "Servo IO PIN Updated...");

      } else {
        _server.send(200, "text/plain",
                     "Servo IO PIN FAILED to update:" + sucess);
      }
    } });

  _server.on("/change-servoMAXAngle", HTTP_POST, [this]()
             {
    if (!checkAdminAuth())
      return;
    String arg = _server.arg("newservoMAXAngle");
    // Add logic here to change the hostname and restart
    UtilityFunctions::debugLog("Servo Max Angle change requested to: " + arg);
    int num = arg.toInt();
    if (num == 0) {
      UtilityFunctions::debugLog(
          "Servo Max Angle change is not anumber or zero: " + arg);
          _server.send(200, "text/plain", "Servo Max Angle change FAILED is not anumber or zero: " + arg);
    } else {
      String sucess = UtilityFunctions::saveServoMaxAngle(num);
      if (sucess.isEmpty()) {
        _server.send(200, "text/plain", "Servo Max Angle Updated...");

      } else {
        _server.send(200, "text/plain",
                     "Servo Max Angle FAILED to update:" + sucess);
      }
    } });

  _server.on("/change-servoRESTAngle", HTTP_POST, [this]()
             {
    if (!checkAdminAuth())
      return;
    String arg = _server.arg("newservoRESTAngle");
    // Add logic here to change the hostname and restart
    UtilityFunctions::debugLog("Servo REST Angle change requested to: " + arg);
    int num = arg.toInt();
    if (num == 0) {
      UtilityFunctions::debugLog(
          "Servo REST Angle change is not anumber or zero: " + arg);
          _server.send(200, "text/plain", "Servo REST Angle change FAILED is not anumber or zero: " + arg);
    } else {
      String sucess = UtilityFunctions::saveServoRestAngle(num);

      if (sucess.isEmpty()) {
        _server.send(200, "text/plain", "Servo Rest Angle Updated...");

      } else {
        _server.send(200, "text/plain",
                     "Servo Rest Angle FAILED to update:" + sucess);
      }
    } });

  _server.on("/change-servoActionAngle", HTTP_POST, [this]()
             {
    if (!checkAdminAuth())
      return;
    String arg = _server.arg("newservoActionAngle");
    // Add logic here to change the hostname and restart
    UtilityFunctions::debugLog("Servo ACTION Angle change requested to: " +
                               arg);
    int num = arg.toInt();
    if (num == 0) {
      UtilityFunctions::debugLog(
          "Servo new ACTION Angle change is not anumber or zero: " + arg);
          _server.send(200, "text/plain",
                     "Servo new ACTION Angle change FAILED is not anumber or zero: " + arg);
    } else {
      String sucess = UtilityFunctions::saveServoActionAngle(num);

      if (sucess.isEmpty()) {
        _server.send(200, "text/plain", "Servo ACTION Angle Updated...");

      } else {
        _server.send(200, "text/plain",
                     "Servo ACTION FAILED to update:" + sucess);
      }
    } });

  _server.on("/change-servoActionDelay", HTTP_POST, [this]()
             {
    if (!checkAdminAuth())
      return;
    String arg = _server.arg("newservoActionDelay");
    // Add logic here to change the hostname and restart
    UtilityFunctions::debugLog("Servo ACTION Delay change requested to: " +
                               arg);
    int num = arg.toInt();
    if (num == 0) {
      UtilityFunctions::debugLog(
          "Servo new ACTION Delay change is not anumber or zero: " + arg);
          _server.send(200, "text/plain",
                     "Servo new ACTION Delay change FAILED is not anumber or zero: " + arg);
    } else {
      String sucess = UtilityFunctions::saveServoActionHold(num);

      if (sucess.isEmpty()) {
        _server.send(200, "text/plain", "Servo ACTION Delay Updated...");

      } else {
        _server.send(200, "text/plain",
                     "Servo ACTION Delay FAILED to update:" + sucess);
      }
    } });

  _server.on("/change-deviceID", HTTP_POST, [this]()
             {
    if (!checkAdminAuth())
      return;
    String newDevID = _server.arg("newdeviceID");
    // Add logic here to change the hostname and restart
    UtilityFunctions::debugLog("Device ID change requested to: " + newDevID);
    String sucess = UtilityFunctions::saveAIoTDeviceID(newDevID);
    if (sucess.isEmpty()) {
      _server.send(200, "text/plain", "DEVICE ID updated! Restarting...");
      delay(WEB_ESP_RESTART_DELAY);
      ESP.restart();
    } else {
      _server.send(200, "text/plain", "DEVICE ID update FAILED ..." + sucess);
    } });

  _server.on("/change-secretID", HTTP_POST, [this]()
             {
    if (!checkAdminAuth())
      return;
    String newsecretID = _server.arg("newsecretID");
    // Add logic here to change the hostname and restart
    UtilityFunctions::debugLog("Device SECRET ID change requested to: " +
                               newsecretID);
    String sucess = UtilityFunctions::saveAIoTDeviceSECRET(newsecretID);

    if (sucess.isEmpty()) {
      _server.send(200, "text/plain",
                   "DEVICE SECRET ID updated! Restarting...");
      delay(WEB_ESP_RESTART_DELAY);
      ESP.restart();
    } else {
      _server.send(200, "text/plain",
                   "DEVICE SECRET ID update FAILED ..." + sucess);
    } });
}
