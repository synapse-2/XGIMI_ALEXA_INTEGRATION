#ifndef RC_WebInterface_H
#define RC_WebInterface_H

#include <WebServer.h>
#include "defaults.h"
#include <freertos/ringbuf.h>
#include <Preferences.h>

// Struct to hold command data
struct WebCommand {
    String command;
};

class RC_WebInterface {
public:
    RC_WebInterface();
    void begin();
    void handleClient();
    static void commandProcessorTask(void* pvParameters);

protected:
    WebServer _server;
    Preferences _preferences;
    const char* _adminUsername = NVRAM_PERFS_ADMIN_NAME;
    String _adminPassword;
    const char* _defaultPassword = NVRAM_PERFS_ADMIN_PASS_DEFAULT;

    void loadAdminPassword();
    void saveAdminPassword(String newPassword);
    bool checkAdminAuth(); // Modified to work with synchronous server
    void setupRoutes();
    void handleRemotePress();
    void handleUpdatePassword();
};

#endif
