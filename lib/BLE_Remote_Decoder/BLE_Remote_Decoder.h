/**
 * @file BLE_Remote_Decoder.h
 * @brief Brief summary of this file.
 *
 * Detailed description of the file and its purpose.
 */


#include "defaults.h"
#include <freertos/ringbuf.h>
#include "BlueXGIMI_RC.h"
#include "ServerDecoder.h"

class BLE_Remote_Decoder: public ServerDecoderHandler
{
public:
    BLE_Remote_Decoder();
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
 * @brief Brief description of getConnectedCount.
 *
 * @return uint8_t Describe the return value.
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
    uint8_t getConnectedCount();
/**
 * @brief Brief description of getConnectedDevicesTable.
 *
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
    String getConnectedDevicesTable();
/**
 * @brief Brief description of disconnectAllClients.
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
    bool disconnectAllClients();

protected:
    BlueXGIMI_RC *rcHID;
};

