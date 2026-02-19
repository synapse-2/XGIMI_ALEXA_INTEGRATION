/**
 * @file RC_Webinterface.h
 * @brief Brief summary of this file.
 *
 * Detailed description of the file and its purpose.
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
/**
 * @brief Brief description of begin.
 *
 *
 * Algorithm:
 * - Outline the high-level algorithm or approach used.
 * - Mention important data structures or invariants.
 *
 * Loops:
 * - Describe each loop purpose and termination condition.
 * - Note whether loops are nested and their effect on complexity.
 *
 * Complexity:
 * - Time: O(...)
 * - Space: O(...)
 */
    void begin();
/**
 * @brief Brief description of handleClient.
 *
 *
 * Algorithm:
 * - Outline the high-level algorithm or approach used.
 * - Mention important data structures or invariants.
 *
 * Loops:
 * - Describe each loop purpose and termination condition.
 * - Note whether loops are nested and their effect on complexity.
 *
 * Complexity:
 * - Time: O(...)
 * - Space: O(...)
 */
    void handleClient();
/**
 * @brief Brief description of commandProcessorTask.
 *
 * @param pvParameters Describe this parameter.
 * @return static void Describe the return value.
 *
 * Algorithm:
 * - Outline the high-level algorithm or approach used.
 * - Mention important data structures or invariants.
 *
 * Loops:
 * - Describe each loop purpose and termination condition.
 * - Note whether loops are nested and their effect on complexity.
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

/**
 * @brief Brief description of loadAdminPassword.
 *
 *
 * Algorithm:
 * - Outline the high-level algorithm or approach used.
 * - Mention important data structures or invariants.
 *
 * Loops:
 * - Describe each loop purpose and termination condition.
 * - Note whether loops are nested and their effect on complexity.
 *
 * Complexity:
 * - Time: O(...)
 * - Space: O(...)
 */
    void loadAdminPassword();
/**
 * @brief Brief description of saveAdminPassword.
 *
 * @param newPassword Describe this parameter.
 *
 * Algorithm:
 * - Outline the high-level algorithm or approach used.
 * - Mention important data structures or invariants.
 *
 * Loops:
 * - Describe each loop purpose and termination condition.
 * - Note whether loops are nested and their effect on complexity.
 *
 * Complexity:
 * - Time: O(...)
 * - Space: O(...)
 */
    void saveAdminPassword(String newPassword);
    bool checkAdminAuth(); // Modified to work with synchronous server
/**
 * @brief Brief description of setupRoutes.
 *
 *
 * Algorithm:
 * - Outline the high-level algorithm or approach used.
 * - Mention important data structures or invariants.
 *
 * Loops:
 * - Describe each loop purpose and termination condition.
 * - Note whether loops are nested and their effect on complexity.
 *
 * Complexity:
 * - Time: O(...)
 * - Space: O(...)
 */
    void setupRoutes();
/**
 * @brief Brief description of handleRemotePress.
 *
 *
 * Algorithm:
 * - Outline the high-level algorithm or approach used.
 * - Mention important data structures or invariants.
 *
 * Loops:
 * - Describe each loop purpose and termination condition.
 * - Note whether loops are nested and their effect on complexity.
 *
 * Complexity:
 * - Time: O(...)
 * - Space: O(...)
 */
    void handleRemotePress();
/**
 * @brief Brief description of handleUpdatePassword.
 *
 *
 * Algorithm:
 * - Outline the high-level algorithm or approach used.
 * - Mention important data structures or invariants.
 *
 * Loops:
 * - Describe each loop purpose and termination condition.
 * - Note whether loops are nested and their effect on complexity.
 *
 * Complexity:
 * - Time: O(...)
 * - Space: O(...)
 */
    void handleUpdatePassword();

/**
 * @brief Brief description of refreshGlobalJS.
 *
 *
 * Algorithm:
 * - Outline the high-level algorithm or approach used.
 * - Mention important data structures or invariants.
 *
 * Loops:
 * - Describe each loop purpose and termination condition.
 * - Note whether loops are nested and their effect on complexity.
 *
 * Complexity:
 * - Time: O(...)
 * - Space: O(...)
 */
    void refreshGlobalJS();
/**
 * @brief Brief description of wifiSignalStrengthDecoder.
 *
 * @param rssi Describe this parameter.
 * @return String Describe the return value.
 *
 * Algorithm:
 * - Outline the high-level algorithm or approach used.
 * - Mention important data structures or invariants.
 *
 * Loops:
 * - Describe each loop purpose and termination condition.
 * - Note whether loops are nested and their effect on complexity.
 *
 * Complexity:
 * - Time: O(...)
 * - Space: O(...)
 */
    String wifiSignalStrengthDecoder(int8_t rssi);
/**
 * @brief Brief description of enQueueCmd.
 *
 * @param cmd Describe this parameter.
 *
 * Algorithm:
 * - Outline the high-level algorithm or approach used.
 * - Mention important data structures or invariants.
 *
 * Loops:
 * - Describe each loop purpose and termination condition.
 * - Note whether loops are nested and their effect on complexity.
 *
 * Complexity:
 * - Time: O(...)
 * - Space: O(...)
 */
    void enQueueCmd(ServerDecoder::Remote_Cmd cmd);
};

#endif
