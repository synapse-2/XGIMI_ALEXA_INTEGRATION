#pragma once

#include "defaults.h"
#include <WiFiManager.h>
#include "BlueXGIMI_RC.h"


class Slave {
public:
    Slave();
    void start();


private:
     BlueXGIMI_RC* rcHID;
};