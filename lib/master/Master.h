#pragma once
#include <WiFiManager.h>

class Master {
public:
    Master();
    void start();
    static void checkResetPressed();

private:
    static WiFiManager wm;
};