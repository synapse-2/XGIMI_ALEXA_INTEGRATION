
#include <WiFiManager.h>      // https://github.com/tzapu/WiFiManager
#include <UtilityFunctions.h> // Custom utility functions
#include "Master.h"           // Master class for I2C master functionality

WiFiManager Master::wm;

Master::Master() {}

void Master::checkResetPressed()
{
    if (UtilityFunctions::isResetPressed())
    {
        wm.resetSettings(); // Reset WiFi settings
        Serial.println("Resetting WiFi settings...");
        for (int i = 0; i < 5; i++)
        {
            UtilityFunctions::ledYellow();
            delay(30);
            UtilityFunctions::ledStop();
            delay(30);
        }
        delay(1000); // Wait for a second before restarting
        Serial.println("Restarting ESP...");
        ESP.restart();
    }
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


    wm.setConfigPortalTimeout(AP_CONNECT_TIMEOUT); // Set the timeout for the configuration portal
    wm.setDebugOutput(true);
    wm.setConfigPortalBlocking(true);
    wm.setHostname(HOHSTNAME_Local); // Set the hostname for the device
    res = wm.autoConnect();          // auto generated AP name from chipid
    // res = wm.autoConnect("AutoConnectAP"); // anonymous ap
    // res = wm.autoConnect("AutoConnectAP","password"); // password protected ap

    if (!res)
    {
        Serial.println("Failed to connect");
        for (int i = 0; i < 5; i++)
        {
            UtilityFunctions::ledRed();
            delay(30);
            UtilityFunctions::ledStop();
            delay(30);
        }
        Serial.println("Failed to connect: RESTARTING");
        ESP.restart();
    }
    else
    {
        // if you get here you have connected to the WiFi
        Serial.println("connected...yeey :)");
        for (int i = 0; i < 10; i++)
        {
            UtilityFunctions::ledGreen();
            delay(30);
            UtilityFunctions::ledStop();
            delay(30);
        }
    }
}
