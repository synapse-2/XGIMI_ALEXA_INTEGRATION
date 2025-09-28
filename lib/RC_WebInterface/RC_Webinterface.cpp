#include "RC_WebInterface.h"
#include "FS.h"
#include "FFat.h"
#include "UtilityFunctions.h"
#include "defaults.h"
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <Preferences.h>

// Constructor
RC_WebInterface::RC_WebInterface()
    : _server(80) {

}

// Public begin method to start the web interface
void RC_WebInterface::begin() {

    if (!FFat.begin(true)) {
        UtilityFunctions::debugLog("An Error has occurred while mounting FFat");
        return;
    }

    loadAdminPassword();

    setupRoutes();
    _server.begin();
}

// Public handleClient method to be called in the main loop
void RC_WebInterface::handleClient() {
    _server.handleClient();
}


// Load password from NVRAM
void RC_WebInterface::loadAdminPassword() {
    _preferences.begin(NVRAM_PERFS, false);
    _adminPassword = _preferences.getString(NVRAM_PERFS_ADMIN_PASS_PROP, _defaultPassword);
    _preferences.end();
    UtilityFunctions::debugLog("Admin password loaded from NVRAM.");
}

// Save password to NVRAM
void RC_WebInterface::saveAdminPassword(String newPassword) {
    _preferences.begin(NVRAM_PERFS, false);
    _preferences.putString(NVRAM_PERFS_ADMIN_PASS_PROP, newPassword);
    _preferences.end();
    _adminPassword = newPassword;
    UtilityFunctions::debugLog("Admin password updated and saved to NVRAM.");
}

// Check for HTTP basic authentication
bool RC_WebInterface::checkAdminAuth() {
    if (!_server.authenticate(_adminUsername, _adminPassword.c_str())) {
        _server.requestAuthentication();
        return false;
    }
    return true;
}

// Handle remote control button presses by sending to ring buffer
void RC_WebInterface::handleRemotePress() {
    if (_server.hasArg("action")) {
        String action = _server.arg("action");
        Serial.println("Received guest command: " + action);
        WebCommand cmd = {action};
        
        // if (xRingbufferSend(_commandRingBuffer, (void*)&cmd, sizeof(WebCommand), pdMS_TO_TICKS(10)) != pdTRUE) {
        //     _server.send(500, "text/plain", "Failed to send command to buffer.");
        //     return;
        // }
    }
    _server.send(200, "text/plain", "OK");
}

// Handle admin password change requests
void RC_WebInterface::handleUpdatePassword() {
    if (!checkAdminAuth()) return;
    if (_server.hasArg("new_password")) {
        String newPassword = _server.arg("new_password");
        if (newPassword.length() > 0) {
            saveAdminPassword(newPassword);
            _server.send(200, "text/plain", "Password updated successfully!");
        } else {
            _server.send(400, "text/plain", "Password cannot be empty.");
        }
    } else {
        _server.send(400, "text/plain", "Invalid request.");
    }
}

// Set up web server routes
void RC_WebInterface::setupRoutes() {
    // Guest remote page
    _server.on("/", HTTP_GET, [this]() {
        File file = FFat.open("/index.htm", "r");
        if (file) {
            _server.streamFile(file, "text/html");
            file.close();
        } else {
            _server.send(404, "text/plain", "File not found");
        }
    });

    // Handle button presses
    _server.on("/press", HTTP_POST, std::bind(&RC_WebInterface::handleRemotePress, this));

    // Admin settings page (requires auth)
    _server.on("/settings", HTTP_GET, [this]() {
        if (!checkAdminAuth()) return;
        File file = FFat.open("/settings.htm", "r");
        if (file) {
            _server.streamFile(file, "text/html");
            file.close();
        } else {
            _server.send(404, "text/plain", "File not found");
        }
    });

    // Handle password change (requires auth)
    _server.on("/update_password", HTTP_POST, std::bind(&RC_WebInterface::handleUpdatePassword, this));
}
