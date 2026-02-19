/**
 * @file Servo_Decoder.cpp
 * @brief Brief summary of this file.
 *
 * Detailed description of the file and its purpose.
 */


#include <UtilityFunctions.h> // Custom utility functions
#include "Servo_Decoder.h"
#include "UtilityFunctions.h" // Utility functions for LED control and other utilities
#include "CmdRingBuffer.h"
#include <math.h>
#include "esp_system.h"
#include "sdkconfig.h"
#include "iot_servo.h"

Servo_Decoder::Servo_Decoder()
{
}

void Servo_Decoder::start()
{
    // need init the servo

    if (UtilityFunctions::loadServoEnableFlag())
    {
        // Configure the servo
        servo_config_t servo_cfg = {
            .max_angle = UtilityFunctions::loadServoMaxAngle(),
            .min_width_us = UtilityFunctions::loadServoPWMMinWidth(),
            .max_width_us = UtilityFunctions::loadServoPWMMaxWidth(),
            .freq = UtilityFunctions::loadServoPWMFreq(),
            .timer_number = LEDC_TIMER_0,
            .channels = {
                .servo_pin = {
                    (gpio_num_t)UtilityFunctions::loadServoIOPin(),
                },
                .ch = {
                    LEDC_CHANNEL_0,
                },
            },
            .channel_number = 1,
        };

        // Initialize the servo
        iot_servo_init(ledc_mode_t::LEDC_LOW_SPEED_MODE, &servo_cfg);

        // set servo to rest angle

/**
 * @brief Brief description of UtilityFunctions::loadServoRestAngle.
 *
 * @param ) Describe this parameter.
 * @return iot_servo_write_angle(ledc_mode_t::LEDC_LOW_SPEED_MODE, 0, Describe the return value.
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
        iot_servo_write_angle(ledc_mode_t::LEDC_LOW_SPEED_MODE, 0, UtilityFunctions::loadServoRestAngle());
        // iot_servo_write_angle(ledc_mode_t::LEDC_LOW_SPEED_MODE, 0, 10);
    }
}

void Servo_Decoder::doCmd(ServerDecoder::Remote_Cmd *cmd)
{
    if (UtilityFunctions::loadServoEnableFlag())
    {
        if (cmd != NULL)
        {
            std::string s_cmd = std::string((magic_enum::enum_name((ServerDecoder::RC_Cmd_Action)cmd->cmds.cmd)));
            // UtilityFunctions::debugLogf("Remote cmmand DEQUEUEED Str:%s INt:%i \n", s_cmd.c_str(),cmd->cmds.cmd );

            bool cmdhandled = false;
            if (cmd->cmds.cmd == ServerDecoder::RC_Cmd_Action::On_Btn)
            {
                int actionAngle = UtilityFunctions::loadServoActionAngle();
                int restAngle = UtilityFunctions::loadServoRestAngle();

                iot_servo_write_angle(ledc_mode_t::LEDC_LOW_SPEED_MODE, 0, restAngle);

                if (actionAngle == restAngle)
                {
/**
 * @brief Brief description of debugLogf.
 *
 * @param same\n" Describe this parameter.
 * @param actionAngle Describe this parameter.
 * @param restAngle Describe this parameter.
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
                    UtilityFunctions::debugLogf("Rest:%i and Action:%i Servo angles are same\n", actionAngle, restAngle);
                    return;
                }

                int degreeMove = abs(restAngle - actionAngle);
                int dirMove = (restAngle >= actionAngle) ? -1 : 1;
                int delay = 500 / degreeMove;

                for (int i = 0; i < degreeMove; i++)
                {
                    iot_servo_write_angle(ledc_mode_t::LEDC_LOW_SPEED_MODE, 0, restAngle + (dirMove * i));
/**
 * @brief Brief description of delay.
 *
 * @param delay Describe this parameter.
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
                    UtilityFunctions::delay(delay);
                }
                iot_servo_write_angle(ledc_mode_t::LEDC_LOW_SPEED_MODE, 0, actionAngle);
/**
 * @brief Brief description of debugLogf.
 *
 * @param angle\n" Describe this parameter.
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
                UtilityFunctions::debugLogf("Servo at action angle\n");
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
                UtilityFunctions::delay(UtilityFunctions::loadServoActionHold());

                for (int i = 0; i < degreeMove; i++)
                {
                    iot_servo_write_angle(ledc_mode_t::LEDC_LOW_SPEED_MODE, 0, actionAngle - (dirMove * i));
/**
 * @brief Brief description of delay.
 *
 * @param delay Describe this parameter.
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
                    UtilityFunctions::delay(delay);
                }
                iot_servo_write_angle(ledc_mode_t::LEDC_LOW_SPEED_MODE, 0, restAngle);
/**
 * @brief Brief description of debugLogf.
 *
 * @param angle\n" Describe this parameter.
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
                UtilityFunctions::debugLogf("Servo at rest angle\n");
            }
            else if (cmd->cmds.cmd == ServerDecoder::RC_Cmd_Action::Off_Btn)
            {
                int restAngle = UtilityFunctions::loadServoRestAngle();

                iot_servo_write_angle(ledc_mode_t::LEDC_LOW_SPEED_MODE, 0, restAngle);

/**
 * @brief Brief description of debugLogf.
 *
 * @param angle\n" Describe this parameter.
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
                UtilityFunctions::debugLogf("Servo at rest angle\n");
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
                UtilityFunctions::debugLogf("By Servo decoder Remote cmmand NOT HANDLED Str:%s INt:%i \n", s_cmd.c_str(), cmd->cmds.cmd);
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
        UtilityFunctions::debugLogf("Servo Server NOT enabled\n");
    }
}
