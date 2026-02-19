/**
 * @file Relay_Decoder.cpp
 * @brief Brief summary of this file.
 *
 * Detailed description of the file and its purpose.
 */


#include <UtilityFunctions.h> // Custom utility functions
#include "Relay_Decoder.h"
#include "UtilityFunctions.h" // Utility functions for LED control and other utilities
#include "CmdRingBuffer.h"
#include "driver/gpio.h"

Relay_Decoder::Relay_Decoder()
{
}

void Relay_Decoder::start()
{
    // set the io pin to low

    if (UtilityFunctions::loadRelayEnableFlag())
    {
        int ioPIN = UtilityFunctions::loadRelayIOPin();
        gpio_set_direction((gpio_num_t)ioPIN, GPIO_MODE_OUTPUT);
        gpio_set_level((gpio_num_t)ioPIN, 0);
    }
}

void Relay_Decoder::doCmd(ServerDecoder::Remote_Cmd *cmd)
{
    if (UtilityFunctions::loadRelayEnableFlag())
    {

        if (cmd != NULL)
        {
            std::string s_cmd = std::string((magic_enum::enum_name((ServerDecoder::RC_Cmd_Action)cmd->cmds.cmd)));
            // UtilityFunctions::debugLogf("Remote cmmand DEQUEUEED Str:%s INt:%i \n", s_cmd.c_str(),cmd->cmds.cmd );

            bool cmdhandled = false;
            if ((cmd->cmds.cmd == ServerDecoder::RC_Cmd_Action::On_Btn) || (cmd->cmds.cmd == ServerDecoder::RC_Cmd_Action::Off_Btn))
            {
                int ioPIN = UtilityFunctions::loadRelayIOPin();

                gpio_set_level((gpio_num_t)ioPIN, 1);
/**
 * @brief Brief description of debugLogf.
 *
 * @param HIGH\n" Describe this parameter.
 * @return UtilityFunctions:: Describe the return value.
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
                UtilityFunctions::debugLogf("Relay Pin HIGH\n");
/**
 * @brief Brief description of delay.
 *
 * @param param Describe this parameter.
 * @return UtilityFunctions:: Describe the return value.
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
                UtilityFunctions::delay(UtilityFunctions::loadRelayActionHold());

                gpio_set_level((gpio_num_t)ioPIN, 0);
/**
 * @brief Brief description of debugLogf.
 *
 * @param LOW\n" Describe this parameter.
 * @return UtilityFunctions:: Describe the return value.
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
                UtilityFunctions::debugLogf("Relay Pin LOW\n");
            }
            else
            {

/**
 * @brief Brief description of debugLogf.
 *
 * @param \n" Describe this parameter.
 * @param param Describe this parameter.
 * @param cmd->cmds.cmd Describe this parameter.
 * @return UtilityFunctions:: Describe the return value.
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
                UtilityFunctions::debugLogf("By Relay decoder Remote cmmand NOT HANDLED Str:%s INt:%i \n", s_cmd.c_str(), cmd->cmds.cmd);
            }
        }
    }
    else
    {
/**
 * @brief Brief description of debugLogf.
 *
 * @param enabled\n" Describe this parameter.
 * @return UtilityFunctions:: Describe the return value.
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
        UtilityFunctions::debugLogf("Relay NOT enabled\n");
    }
}
