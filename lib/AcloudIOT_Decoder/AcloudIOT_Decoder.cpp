#include "AcloudIOT_Decoder.h" // Master class for I2C master functionality
#include "BlueRC.h"
#include "CmdRingBuffer.h"
#include "ConnectionHandlerDefinitions.h"
#include "UtilityFunctions.h" // Custom utility functions
#include "magicEnum/magic_enum.hpp"
#include "magicEnum/magic_enum_iostream.hpp"
#include <WiFiManager.h> // https://github.com/tzapu/WiFiManager
#include "thingProperties.h"

extern void onNetworkErrorMain();
extern void onNetworkDisconnectMain();

AcloudIOT_Decoder::AcloudIOT_Decoder() {}

bool AcloudIOT_Decoder::isConnectOK()
{
  return isConnectOkState;
}

bool AcloudIOT_Decoder::hasFirstCloudSyncHasHappened()
{
  return firstCloudSyncHasHappened;
}

void AcloudIOT_Decoder::enQueueCmd(ServerDecoder::Remote_Cmd cmd)
{
  CmdRingBuffer::enQueueCmd(cmd);
}

void AcloudIOT_Decoder::setup(String ssid, String psk)
{

  // update the IOT device to conenct to
  ArduinoCloud.setBoardId(UtilityFunctions::loadAIoTDeviceID());
  ArduinoCloud.setSecretDeviceKey(UtilityFunctions::loadAIoTDeviceSECRET());

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
  iot_connector = new WiFiConnectionHandler(ssid.c_str(), psk.c_str());
  iot_connector->addCallback(NetworkConnectionEvent::ERROR, onNetworkErrorMain);
  iot_connector->addCallback(NetworkConnectionEvent::DISCONNECTED,
                             onNetworkDisconnectMain);
}

void AcloudIOT_Decoder::start() { ArduinoCloud.begin(*iot_connector); }

void AcloudIOT_Decoder::onNetworkConnect()
{
  UtilityFunctions::debugLog(">>>> CONNECTED to ArduinoIoT network");
  isConnectOkState = true;
}

void AcloudIOT_Decoder::onNetworkDisconnect()
{

  firstCloudSyncHasHappened = false; // reset the tag
  UtilityFunctions::debugLog(
      ">>>> DISCONNECTED from ArduinoIoT network. setting first sync to false");
  isConnectOkState = false;
}

void AcloudIOT_Decoder::onNetworkError()
{
  Serial.println(">>>> ERROR FROM AIoT cloud");
  isConnectOkState = false;
}

void AcloudIOT_Decoder::onProjectorChange(CloudTelevision newPrj)
{
  ServerDecoder::Remote_Cmd rcCmd;
  bool enQueuedCmd = false;

  if (!firstCloudSyncHasHappened)
  {
    // we have connected and we are getting old value no coomand was sent
    oldProjector_value = newPrj;
    firstCloudSyncHasHappened = true;
    UtilityFunctions::debugLog("update received from cloud but this is the "
                               "FIRST sync no action taken");
    return;
  }
  UtilityFunctions::debugLogf(" firstCloudSyncHasHappened %i \n",
                              firstCloudSyncHasHappened);
  UtilityFunctions::debugLog("update received from cloud");

  if (oldProjector_value.getSwitch() != newPrj.getSwitch())
  {
    // switch value has changed
    // for the switch change event in ring buffer
    if (newPrj.getSwitch())
    {
      // true is on
      rcCmd.cmds.cmd = ServerDecoder::RC_Cmd_Action::On_Btn;
    }
    else
    {
      rcCmd.cmds.cmd = ServerDecoder::RC_Cmd_Action::Off_Btn;
    }
    enQueueCmd(rcCmd);
    enQueuedCmd = true;
    UtilityFunctions::debugLog("Projector switch changed to: " +
                               String(newPrj.getSwitch()));
  }

  if (oldProjector_value.getVolume() != newPrj.getVolume())
  {
    // volume value has changed
    // for the volume change event in ring buffer
    rcCmd.cmds.cmd = ServerDecoder::RC_Cmd_Action::Volume;
    rcCmd.cmds.fromVal = oldProjector_value.getVolume();
    rcCmd.cmds.toVal = newPrj.getVolume();
    enQueueCmd(rcCmd);
    enQueuedCmd = true;
    UtilityFunctions::debugLogf("Projector volume changed to: %i from %i\n",
                                newPrj.getVolume(),
                                oldProjector_value.getVolume());
  }

  if (oldProjector_value.getChannel() != newPrj.getChannel())
  {
    // channel value has changed
    // for the channel change event in ring buffer
    rcCmd.cmds.cmd = ServerDecoder::RC_Cmd_Action::Channel;
    rcCmd.cmds.fromVal = oldProjector_value.getChannel();
    rcCmd.cmds.toVal = newPrj.getChannel();
    enQueueCmd(rcCmd);
    enQueuedCmd = true;
    UtilityFunctions::debugLogf(
        "Projector channel changed to: %i from old channel %i\n",
        newPrj.getChannel(), oldProjector_value.getChannel());
  }

  if (oldProjector_value.getMute() != newPrj.getMute())
  {
    // Mute value has changed
    // for the mute change event in ring buffer
    rcCmd.cmds.cmd = ServerDecoder::RC_Cmd_Action::Mute;
    enQueueCmd(rcCmd);
    enQueuedCmd = true;
    UtilityFunctions::debugLog("Projector Mute changed to: " +
                               String(newPrj.getMute()));
  }

  if (oldProjector_value.getInputValue() != newPrj.getInputValue())
  {
    // Input value has changed
    // for the Input change event in ring buffer
    String cmd = String((magic_enum::enum_name(newPrj.getInputValue())).data());
    rcCmd.cmds.cmd = ServerDecoder::RC_Cmd_Action::ChangeInput;
    rcCmd.cmds.toVal = (uint8_t)newPrj.getInputValue();
    enQueueCmd(rcCmd);
    enQueuedCmd = true;
    UtilityFunctions::debugLogf("Projector Input command changed to: %s\n",
                                cmd);
  }

  if (oldProjector_value.getPlaybackCommand() != newPrj.getPlaybackCommand())
  {
    // Playback value has changed
    // for the playbac change event in ring buffer
    String cmd =
        String((magic_enum::enum_name(newPrj.getPlaybackCommand())).data());
    rcCmd.cmds.cmd = (ServerDecoder::RC_Cmd_Action)newPrj.getPlaybackCommand();
    enQueueCmd(rcCmd);
    enQueuedCmd = true;
    UtilityFunctions::debugLogf("Projector Playback command changed to: %s\n",
                                cmd);
  }

  oldProjector_value = newPrj; // set the old to current value

  if (!enQueuedCmd)
  {
    UtilityFunctions::debugLog("Command received from AIOT but no change in "
                               "old vs new cloud variable");
  }
}

void AcloudIOT_Decoder::doCmd(ServerDecoder::Remote_Cmd *cmd)
{
  ServerDecoder::Remote_Cmd rcCmd;

  if (cmd != NULL)
  {
    std::string s_cmd = std::string((magic_enum::enum_name((ServerDecoder::RC_Cmd_Action)cmd->cmds.cmd)));
    // UtilityFunctions::debugLogf("Remote cmmand DEQUEUEED Str:%s INt:%i \n", s_cmd.c_str(),cmd->cmds.cmd );

    bool oldProjState = oldProjector_value.getSwitch();
    if (cmd->cmds.cmd == ServerDecoder::RC_Cmd_Action::Off_On_Btn)
    {
      if (oldProjector_value.getSwitch())
      {
        // true is on
        rcCmd.cmds.cmd = ServerDecoder::RC_Cmd_Action::Off_Btn;
        // update cloud also
        projector.setSwitch(false);
        oldProjector_value.setSwitch(false);
      }
      else
      {
        // false is off
        rcCmd.cmds.cmd = ServerDecoder::RC_Cmd_Action::On_Btn;
        // update cloud also
        projector.setSwitch(true);
        oldProjector_value.setSwitch(true);
      }
      enQueueCmd(rcCmd);
      std::string s_cmd_add = std::string((magic_enum::enum_name((ServerDecoder::RC_Cmd_Action)rcCmd.cmds.cmd)));
      UtilityFunctions::debugLogf("By AIOT decoder received cmd Str:%s INt:%i cloud variable state:%i added command %s in queue new sate of cloud var:%i and cache copy var:%i\n", s_cmd.c_str(), cmd->cmds.cmd, oldProjState, s_cmd_add.c_str(), projector.getSwitch(), oldProjector_value.getSwitch());
    }
    else
    {
      UtilityFunctions::debugLogf("By AIOT decoder Remote cmmand NOT HANDLED Str:%s INt:%i \n", s_cmd.c_str(), cmd->cmds.cmd);
    }
  }
}