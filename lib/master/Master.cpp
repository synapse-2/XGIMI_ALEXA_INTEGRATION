#include <WiFiManager.h>      // https://github.com/tzapu/WiFiManager
#include "UtilityFunctions.h" // Custom utility functions
#include "Master.h"           // Master class for I2C master functionality

WiFiManager Master::wm;

Master::Master() {}

void Master::checkResetPressed()
{
    if (UtilityFunctions::isMaster())
    {
        // only check the boot button is pressed if we are the master
        
        if (UtilityFunctions::isResetPressed())
        {
            UtilityFunctions::debugLogf("Reset pressed num time: %i need 3 to reset system", UtilityFunctions::numTimesResetPressed());

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
        for (int i = 0; i < 5; i++)
        {
            UtilityFunctions::ledRed();
            UtilityFunctions::delay(30);
            UtilityFunctions::ledStop();
            UtilityFunctions::delay(30);
        }
        UtilityFunctions::debugLog("Failed to connect: RESTARTING");
        ESP.restart();
    }
    else
    {
        // if you get here you have connected to the WiFi
        UtilityFunctions::debugLog("connected...yeey :)");
        Master::LogWifiDebugInfo();
        for (int i = 0; i < 10; i++)
        {
            UtilityFunctions::ledGreen();
            UtilityFunctions::delay(30);
            UtilityFunctions::ledStop();
            UtilityFunctions::delay(30);
        }
    }
}
