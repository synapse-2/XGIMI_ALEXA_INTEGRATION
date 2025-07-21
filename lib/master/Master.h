#pragma once
#include <WiFiManager.h>

class Master {
public:
    Master();
    void start();
    String getSSID();
    String getPSK();
    static void checkResetPressed();


private:
    static WiFiManager wm;
};