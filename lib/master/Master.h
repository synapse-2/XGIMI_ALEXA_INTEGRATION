#pragma once

#include "defaults.h"
#include <WiFiManager.h>

class Master {
public:
    Master();
    void start();
    String getSSID();
    String getPSK();
    void LogWifiDebugInfo();
    static void checkResetPressed();


private:
    static WiFiManager wm;
};