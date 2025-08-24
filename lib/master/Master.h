#pragma once

#include "defaults.h"
#include <WiFiManager.h>
#include <ArduinoIoTCloud.h>

class Master {
public:
    Master();
    void start();
    String getSSID();
    String getPSK();
    void LogWifiDebugInfo();
    static void checkResetPressed();

void onNetworkConnect();

void onNetworkDisconnect();

void onNetworkError() ;

private:
    static WiFiManager wm;
    bool fistCloudSync = false; //used to ignore the first cloud update after connection as it syncs the variable
    CloudTelevision oldProjector_value; // used to compare the value to derermine what to do
};