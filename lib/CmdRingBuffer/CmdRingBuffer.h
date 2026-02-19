/**
 * @file CmdRingBuffer.h
 * @brief Brief summary of this file.
 *
 * Detailed description of the file and its purpose.
 */


#include "defaults.h"
#include "BlueRC.h"
#include <freertos/ringbuf.h>

namespace CmdRingBuffer{

    extern RingbufHandle_t ringBufHandle;
/**
 * @brief Brief description of initCmdRingBuffer.
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
    void initCmdRingBuffer();
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
 * @brief Brief description of peekCmd.
 *
 * @return ServerDecoder::Remote_Cmd* Describe the return value.
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
    ServerDecoder::Remote_Cmd* peekCmd();
/**
 * @brief Brief description of dequeueCmd.
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
    void dequeueCmd(ServerDecoder::Remote_Cmd*  cmd);

}
