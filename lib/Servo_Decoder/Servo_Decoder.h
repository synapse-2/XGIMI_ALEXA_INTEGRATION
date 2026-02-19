/**
 * @file Servo_Decoder.h
 * @brief Brief summary of this file.
 *
 * Detailed description of the file and its purpose.
 */


#include "defaults.h"
#include <freertos/ringbuf.h>
#include "ServerDecoder.h"

class Servo_Decoder : public ServerDecoderHandler
{
public:
    Servo_Decoder();
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
};
