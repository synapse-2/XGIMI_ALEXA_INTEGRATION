#include <WiFiManager.h>      // https://github.com/tzapu/WiFiManager
#include "UtilityFunctions.h" // Custom utility functions
#include "Master.h"           // Master class for I2C master functionality
#include "BlueRC.h"
#include "magicEnum/magic_enum.hpp"
#include "magicEnum/magic_enum_iostream.hpp"

WiFiManager Master::wm;
RingbufHandle_t Master::ringBufHandle;

Master::Master(RingbufHandle_t buf)
{

    ringBufHandle = buf;
}

void Master::enQueueCmd(BlueRC::Remote_Cmd cmd)
{
    size_t sizefree = xRingbufferGetCurFreeSize(ringBufHandle);
     UBaseType_t res = xRingbufferSend(ringBufHandle, (void *)&cmd, sizeof(cmd), 100);
    if (res != pdTRUE)
    {
        UtilityFunctions::debugLogf("Failed to enqueue RC cmd SKIPPING free size in ring buffer %i \n",sizefree);
    }
}

Master::Master() : Master(NULL)
{
}

void Master::checkResetPressed()
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

String Master::getSSID() { return wm.getWiFiSSID(); }
String Master::getPSK() { return wm.getWiFiPass(); }

void Master::LogWifiDebugInfo()
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

void Master::start()
{
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

    // Master::LogWifiDebugInfo();
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
        UtilityFunctions::debugLog("connected...yeey :)");
        Master::LogWifiDebugInfo();
        UtilityFunctions::ledBlinkGreenLong();
    }
}

void Master::onNetworkConnect()
{
    UtilityFunctions::debugLog(">>>> CONNECTED to ArduinoIoT network");
}

void Master::onNetworkDisconnect()
{

    firstCloudSyncHasHappened = false; // reset the tag
    UtilityFunctions::debugLog(">>>> DISCONNECTED from ArduinoIoT network. seiing first sync to false");
}

void Master::onNetworkError()
{
    Serial.println(">>>> ERROR");
}

void Master::onProjectorChange(CloudTelevision newPrj)
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

    if (!enQueuedCmd) {
        UtilityFunctions::debugLog("Command received from AIOT but no change in old vs new cloud variable");
    }
}