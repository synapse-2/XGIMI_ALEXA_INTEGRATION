/*
 * Overview:
 * This file is part of the BluetoothESP32 device project.
 * It contains code used to handle hardware, BLE, WiFi, or web UI
 * interactions for the device. Comments marked with [AUTO-DOC] were
 * added automatically; please refine them to be more specific.
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
    RC_WebInterface();
/* [AUTO-DOC] */
/**
 * @brief Initializes or starts begin.
 *
 *
 * Algorithm:
 * - High-level steps the function performs.
 * - Mention important side-effects (WiFi, BLE, hardware).
 *
 * Loops:
 * - Describe each loop purpose and termination condition here.
 *
 * Complexity:
 * - Time: O(...)
 * - Space: O(...)
 */
    void begin();
/* [AUTO-DOC] */
/**
 * @brief Performs handle Client operations.
 *
 *
 * Algorithm:
 * - High-level steps the function performs.
 * - Mention important side-effects (WiFi, BLE, hardware).
 *
 * Loops:
 * - Describe each loop purpose and termination condition here.
 *
 * Complexity:
 * - Time: O(...)
 * - Space: O(...)
 */
    void handleClient();
/* [AUTO-DOC] */
/**
 * @brief Performs command Processor Task operations.
 *
 * @param pvParameters Describe this parameter.
 * @return static void Describe the return value.
 *
 * Algorithm:
 * - High-level steps the function performs.
 * - Mention important side-effects (WiFi, BLE, hardware).
 *
 * Loops:
 * - Describe each loop purpose and termination condition here.
 *
 * Complexity:
 * - Time: O(...)
 * - Space: O(...)
 */
    static void commandProcessorTask(void* pvParameters);

protected:
    WebServer _server;
    Preferences _preferences;
    String _adminPassword;
    String globalJS;

/* [AUTO-DOC] */
/**
 * @brief Performs load Admin Password operations.
 *
 *
 * Algorithm:
 * - High-level steps the function performs.
 * - Mention important side-effects (WiFi, BLE, hardware).
 *
 * Loops:
 * - Describe each loop purpose and termination condition here.
 *
 * Complexity:
 * - Time: O(...)
 * - Space: O(...)
 */
    void loadAdminPassword();
/* [AUTO-DOC] */
/**
 * @brief Performs save Admin Password operations.
 *
 * @param newPassword Describe this parameter.
 *
 * Algorithm:
 * - High-level steps the function performs.
 * - Mention important side-effects (WiFi, BLE, hardware).
 *
 * Loops:
 * - Describe each loop purpose and termination condition here.
 *
 * Complexity:
 * - Time: O(...)
 * - Space: O(...)
 */
    void saveAdminPassword(String newPassword);
    bool checkAdminAuth(); // Modified to work with synchronous server
/* [AUTO-DOC] */
/**
 * @brief Initializes or starts setup Routes.
 *
 *
 * Algorithm:
 * - High-level steps the function performs.
 * - Mention important side-effects (WiFi, BLE, hardware).
 *
 * Loops:
 * - Describe each loop purpose and termination condition here.
 *
 * Complexity:
 * - Time: O(...)
 * - Space: O(...)
 */
    void setupRoutes();
/* [AUTO-DOC] */
/**
 * @brief Performs handle Remote Press operations.
 *
 *
 * Algorithm:
 * - High-level steps the function performs.
 * - Mention important side-effects (WiFi, BLE, hardware).
 *
 * Loops:
 * - Describe each loop purpose and termination condition here.
 *
 * Complexity:
 * - Time: O(...)
 * - Space: O(...)
 */
    void handleRemotePress();
/* [AUTO-DOC] */
/**
 * @brief Performs handle Update Password operations.
 *
 *
 * Algorithm:
 * - High-level steps the function performs.
 * - Mention important side-effects (WiFi, BLE, hardware).
 *
 * Loops:
 * - Describe each loop purpose and termination condition here.
 *
 * Complexity:
 * - Time: O(...)
 * - Space: O(...)
 */
    void handleUpdatePassword();

/* [AUTO-DOC] */
/**
 * @brief Performs refresh Global JS operations.
 *
 *
 * Algorithm:
 * - High-level steps the function performs.
 * - Mention important side-effects (WiFi, BLE, hardware).
 *
 * Loops:
 * - Describe each loop purpose and termination condition here.
 *
 * Complexity:
 * - Time: O(...)
 * - Space: O(...)
 */
    void refreshGlobalJS();
/* [AUTO-DOC] */
/**
 * @brief Performs wifi Signal Strength Decoder operations.
 *
 * @param rssi Describe this parameter.
 * @return String Describe the return value.
 *
 * Algorithm:
 * - High-level steps the function performs.
 * - Mention important side-effects (WiFi, BLE, hardware).
 *
 * Loops:
 * - Describe each loop purpose and termination condition here.
 *
 * Complexity:
 * - Time: O(...)
 * - Space: O(...)
 */
    String wifiSignalStrengthDecoder(int8_t rssi);
/* [AUTO-DOC] */
/**
 * @brief Performs en Queue Cmd operations.
 *
 * @param cmd Describe this parameter.
 *
 * Algorithm:
 * - High-level steps the function performs.
 * - Mention important side-effects (WiFi, BLE, hardware).
 *
 * Loops:
 * - Describe each loop purpose and termination condition here.
 *
 * Complexity:
 * - Time: O(...)
 * - Space: O(...)
 */
    void enQueueCmd(ServerDecoder::Remote_Cmd cmd);
};

#endif
