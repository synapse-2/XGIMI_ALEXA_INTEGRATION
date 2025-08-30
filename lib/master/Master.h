#pragma once

#include "defaults.h"
#include "BlueRC.h"
#include <WiFiManager.h>
#include <ArduinoIoTCloud.h>
#include <freertos/ringbuf.h>

class Master
{
public:
    Master();
    Master(RingbufHandle_t buf);
    void start();
    String getSSID();
    String getPSK();
    void LogWifiDebugInfo();
    static void checkResetPressed();

    void onNetworkConnect();

    void onNetworkDisconnect();

    void onNetworkError();

    void onProjectorChange(CloudTelevision newPrj);

    void enQueueCmd(BlueRC::Remote_Cmd cmd);

protected:
    static WiFiManager wm;
    static RingbufHandle_t ringBufHandle;
    bool firstCloudSyncHasHappened = false; // used to ignore the first cloud update after connection as it syncs the variable
    CloudTelevision oldProjector_value;     // used to compare the value to derermine what to do
};