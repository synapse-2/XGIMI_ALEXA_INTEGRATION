#include <WiFiManager.h>       // https://github.com/tzapu/WiFiManager
#include "UtilityFunctions.h"  // Custom utility functions
#include "AcloudIOT_Decoder.h" // Master class for I2C master functionality
#include "BlueRC.h"
#include "CmdRingBuffer.h"
#include "magicEnum/magic_enum.hpp"
#include "magicEnum/magic_enum_iostream.hpp"

AcloudIOT_Decoder::AcloudIOT_Decoder()
{
}

bool AcloudIOT_Decoder::hasFirstCloudSyncHasHappened() { return firstCloudSyncHasHappened; }

void AcloudIOT_Decoder::enQueueCmd(BlueRC::Remote_Cmd cmd)
{
    CmdRingBuffer::enQueueCmd(cmd);
}

void AcloudIOT_Decoder::setup(String ssid, String psk)
{

    // update the IOT device to conenct to
    ArduinoCloud.setBoardId(DEVICE_LOGIN_NAME);
    ArduinoCloud.setSecretDeviceKey(DEVICE_KEY);

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
}

void AcloudIOT_Decoder::start()
{
    ArduinoCloud.begin(*iot_connector);
}

void AcloudIOT_Decoder::addCallback(NetworkConnectionEvent const event, OnNetworkEventCallback callback)
{
    iot_connector->addCallback(event, callback);
}

void AcloudIOT_Decoder::onNetworkConnect()
{
    UtilityFunctions::debugLog(">>>> CONNECTED to ArduinoIoT network");
    UtilityFunctions::ledStop(); // stop the yellow light now we will have green blinks form us
}

void AcloudIOT_Decoder::onNetworkDisconnect()
{

    firstCloudSyncHasHappened = false; // reset the tag
    UtilityFunctions::debugLog(">>>> DISCONNECTED from ArduinoIoT network. setting first sync to false");
    UtilityFunctions::ledYellow();
}

void AcloudIOT_Decoder::onNetworkError()
{
    Serial.println(">>>> ERROR FROM AIoT cloud");
    UtilityFunctions::ledBlinkRed();
}

void AcloudIOT_Decoder::onProjectorChange(CloudTelevision newPrj)
{
    BlueRC::Remote_Cmd rcCmd;
    bool enQueuedCmd = false;

    if (!firstCloudSyncHasHappened)
    {
        // we have connected and we are getting old value no coomand was sent
        oldProjector_value = newPrj;
        firstCloudSyncHasHappened = true;
        UtilityFunctions::debugLog("update received from cloud but this is the FIRST sync no action taken");
        return;
    }
    UtilityFunctions::debugLogf(" firstCloudSyncHasHappened %i \n", firstCloudSyncHasHappened);
    UtilityFunctions::debugLog("update received from cloud");

    if (oldProjector_value.getSwitch() != newPrj.getSwitch())
    {
        // switch value has changed
        // for the switch change event in ring buffer
        rcCmd.cmds.cmd = BlueRC::RC_Cmd_Action::On_OFF_Btn;
        enQueueCmd(rcCmd);
        enQueuedCmd = true;
        UtilityFunctions::debugLog("Projector switch changed to: " + String(newPrj.getSwitch()));
    }

    if (oldProjector_value.getVolume() != newPrj.getVolume())
    {
        // volume value has changed
        // for the volume change event in ring buffer
        rcCmd.cmds.cmd = BlueRC::RC_Cmd_Action::Volume;
        rcCmd.cmds.fromVal = oldProjector_value.getVolume();
        rcCmd.cmds.toVal = newPrj.getVolume();
        enQueueCmd(rcCmd);
        enQueuedCmd = true;
        UtilityFunctions::debugLogf("Projector volume changed to: %i\n", newPrj.getVolume());
    }

    if (oldProjector_value.getChannel() != newPrj.getChannel())
    {
        // channel value has changed
        // for the channel change event in ring buffer
        rcCmd.cmds.cmd = BlueRC::RC_Cmd_Action::Channel;
        rcCmd.cmds.fromVal = oldProjector_value.getChannel();
        rcCmd.cmds.toVal = newPrj.getChannel();
        enQueueCmd(rcCmd);
        enQueuedCmd = true;
        UtilityFunctions::debugLogf("Projector channel changed to: %i\n", newPrj.getChannel());
    }

    if (oldProjector_value.getMute() != newPrj.getMute())
    {
        // Mute value has changed
        // for the mute change event in ring buffer
        rcCmd.cmds.cmd = BlueRC::RC_Cmd_Action::Mute;
        enQueueCmd(rcCmd);
        enQueuedCmd = true;
        UtilityFunctions::debugLog("Projector Mute changed to: " + String(newPrj.getMute()));
    }

    if (oldProjector_value.getInputValue() != newPrj.getInputValue())
    {
        // Input value has changed
        // for the Input change event in ring buffer
        String cmd = String((magic_enum::enum_name(newPrj.getInputValue())).data());
        rcCmd.cmds.cmd = BlueRC::RC_Cmd_Action::ChangeInput;
        rcCmd.cmds.toVal = (uint8_t)newPrj.getInputValue();
        enQueueCmd(rcCmd);
        enQueuedCmd = true;
        UtilityFunctions::debugLogf("Projector Input command changed to: %s\n", cmd);
    }

    if (oldProjector_value.getPlaybackCommand() != newPrj.getPlaybackCommand())
    {
        // Playback value has changed
        // for the playbac change event in ring buffer
        String cmd = String((magic_enum::enum_name(newPrj.getPlaybackCommand())).data());
        rcCmd.cmds.cmd = (BlueRC::RC_Cmd_Action)newPrj.getPlaybackCommand();
        enQueueCmd(rcCmd);
        enQueuedCmd = true;
        UtilityFunctions::debugLogf("Projector Playback command changed to: %s\n", cmd);
    }

    oldProjector_value = newPrj; // set the old to current value

    if (!enQueuedCmd)
    {
        UtilityFunctions::debugLog("Command received from AIOT but no change in old vs new cloud variable");
    }
}