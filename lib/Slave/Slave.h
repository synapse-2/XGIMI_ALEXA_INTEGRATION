#pragma once
#include <WiFiManager.h>
#include "BluetoothHID_RC.h"

class Slave {
public:
    Slave();
    void start();


private:
     BluetoothHID_RC* rcHID;
};