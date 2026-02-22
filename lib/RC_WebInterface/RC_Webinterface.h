/*
 * Overview:
 * Web interface wrapper used to expose a small HTTP admin UI and
 * accept remote-control commands from the browser. Routes are
 * registered by `setupRoutes()` and commands are queued for the
 * central command processor using `enQueueCmd()`.
 */

#ifndef RC_WebInterface_H
#define RC_WebInterface_H

#include <WebServer.h>
#include "defaults.h"
#include <Preferences.h>
#include "ServerDecoder.h"
#include "CmdRingBuffer.h"

class RC_WebInterface {
public:
    /**
     * @brief Construct a new RC_WebInterface object.
     */
    RC_WebInterface();

    /**
     * @brief Initialize the webserver and internal resources.
     *
     * Registers HTTP routes and starts any internal tasks needed by
     * the web interface.
     */
    bool begin();

    /**
     * @brief Poll the HTTP server and process incoming requests.
     *
     * Intended to be called frequently from the main loop.
     */
    void handleClient();

    /**
     * @brief Background task that dequeues and processes commands.
     * @param pvParameters FreeRTOS task parameter (unused).
     */
    static void commandProcessorTask(void* pvParameters);

protected:
    WebServer _server;
    Preferences _preferences;
    String _adminPassword;
    String globalJS;

    /**
     * @brief Load the administrator password from persistent storage.
     */
    void loadAdminPassword();

    /**
     * @brief Save a new administrator password to persistent storage.
     * @param newPassword The password to store.
     */
    void saveAdminPassword(String newPassword);
    bool checkAdminAuth(); // Modified to work with synchronous server
    /**
     * @brief Register HTTP routes and handlers on the internal server.
     */
    void setupRoutes();

    /**
     * @brief Called when a remote control press arrives via the web UI.
     *
     * Converts web input into a `Remote_Cmd` and queues it for
     * processing.
     */
    void handleRemotePress();

    /**
     * @brief HTTP handler to update the administrator password.
     */
    void handleUpdatePassword();

    /**
     * @brief Refresh any generated global JavaScript served by the UI.
     */
    void refreshGlobalJS();

    /**
     * @brief Convert RSSI value into a human-friendly signal string.
     * @param rssi RSSI in dBm.
     * @return Human-friendly signal description (e.g., "Excellent").
     */
    String wifiSignalStrengthDecoder(int8_t rssi);

    /**
     * @brief Enqueue a decoded remote command for processing.
     * @param cmd Decoded remote command to enqueue (copied).
     */
    void enQueueCmd(ServerDecoder::Remote_Cmd cmd);
};

#endif
