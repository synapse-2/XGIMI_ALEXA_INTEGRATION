/**
 * @file AcloudIOT_Decoder.h
 * @brief Brief summary of this file.
 *
 * Detailed description of the file and its purpose.
 */



#include "defaults.h"
#include "BlueRC.h"
#include <WiFiManager.h>
#include <ArduinoIoTCloud.h>
#include "ServerDecoder.h"

class AcloudIOT_Decoder: public ServerDecoderHandler
{
public:
    AcloudIOT_Decoder();
/**
 * @brief Brief description of setup.
 *
 * @param ssid Describe this parameter.
 * @param psk Describe this parameter.
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
    void setup(String ssid, String psk);
/**
 * @brief Brief description of start.
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
    void start();
/**
 * @brief Brief description of checkResetPressed.
 *
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
    static void checkResetPressed();
/**
 * @brief Brief description of hasFirstCloudSyncHasHappened.
 *
 * @return bool Describe the return value.
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
    bool hasFirstCloudSyncHasHappened();
/**
 * @brief Brief description of onNetworkConnect.
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
    void onNetworkConnect();
/**
 * @brief Brief description of onNetworkDisconnect.
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
    void onNetworkDisconnect();
/**
 * @brief Brief description of onNetworkError.
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
    void onNetworkError();
/**
 * @brief Brief description of onProjectorChange.
 *
 * @param newPrj Describe this parameter.
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
    void onProjectorChange(CloudTelevision newPrj);
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
/**
 * @brief Brief description of isConnectOK.
 *
 * @return bool Describe the return value.
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
    bool isConnectOK();
/**
 * @brief Brief description of doCmd.
 *
 * @param param Describe this parameter.
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
    void doCmd(ServerDecoder::Remote_Cmd *cmd);
/**
 * @brief Brief description of setOldProjectorSwitch.
 *
 * @param val Describe this parameter.
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
    void setOldProjectorSwitch(bool val);

protected:
    WiFiConnectionHandler * iot_connector;
    bool firstCloudSyncHasHappened = false; // used to ignore the first cloud update after connection as it syncs the variable
    CloudTelevision oldProjector_value;     // used to compare the value to derermine what to do
    bool isConnectOkState = false;
};
