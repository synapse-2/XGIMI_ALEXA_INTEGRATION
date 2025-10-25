#ifndef RC_WebInterface_H
#define RC_WebInterface_H

#include <WebServer.h>
#include "defaults.h"
#include <Preferences.h>
#include "BlueRC.h"
#include "CmdRingBuffer.h"

class RC_WebInterface {
public:
    RC_WebInterface();
    void begin();
    void handleClient();
    static void commandProcessorTask(void* pvParameters);

protected:
    WebServer _server;
    Preferences _preferences;
    String _adminPassword;
    String globalJS;

    void loadAdminPassword();
    void saveAdminPassword(String newPassword);
    bool checkAdminAuth(); // Modified to work with synchronous server
    void setupRoutes();
    void handleRemotePress();
    void handleUpdatePassword();

    void refreshGlobalJS();
    String wifiSignalStrengthDecoder(int8_t rssi);
    void enQueueCmd(BlueRC::Remote_Cmd cmd);
};

#endif
