
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

    iot_servo_write_angle(ledc_mode_t::LEDC_LOW_SPEED_MODE, 0, UtilityFunctions::loadServoRestAngle());
}

void Servo_Decoder::doCmd(ServerDecoder::Remote_Cmd *cmd)
{

    if (cmd != NULL)
    {
        std::string s_cmd = std::string((magic_enum::enum_name((ServerDecoder::RC_Cmd_Action)cmd->cmds.cmd)));
        // UtilityFunctions::debugLogf("Remote cmmand DEQUEUEED Str:%s INt:%i \n", s_cmd.c_str(),cmd->cmds.cmd );

        bool cmdhandled = false;
        if (cmd->cmds.cmd == ServerDecoder::RC_Cmd_Action::On_OFF_Btn)
        {
            iot_servo_write_angle(ledc_mode_t::LEDC_LOW_SPEED_MODE, 0, UtilityFunctions::loadServoRestAngle());
            iot_servo_write_angle(ledc_mode_t::LEDC_LOW_SPEED_MODE, 0, UtilityFunctions::loadServoActionAngle());
            UtilityFunctions::debugLogf("Servo at action angle\n");
            UtilityFunctions::delay(UtilityFunctions::loadServoActionHold());
            iot_servo_write_angle(LEDC_LOW_SPEED_MODE, 0, UtilityFunctions::loadServoRestAngle());
            UtilityFunctions::debugLogf("Servo at rest angle\n");
        }
        else
        {

            UtilityFunctions::debugLogf("By Servo decoder Remote cmmand NOT HANDLED Str:%s INt:%i \n", s_cmd.c_str(), cmd->cmds.cmd);
        }
    }
}
