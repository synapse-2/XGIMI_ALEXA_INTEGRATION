
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
            if (cmd->cmds.cmd == ServerDecoder::RC_Cmd_Action::On_OFF_Btn)
            {
                int ioPIN = UtilityFunctions::loadRelayIOPin();

                gpio_set_level((gpio_num_t)ioPIN, 1);
                UtilityFunctions::debugLogf("Relay Pin HIGH\n");
                UtilityFunctions::delay(UtilityFunctions::loadRelayActionHold());

                gpio_set_level((gpio_num_t)ioPIN, 0);
                UtilityFunctions::debugLogf("Relay Pin LOW\n");
            }
            else
            {

                UtilityFunctions::debugLogf("By Relay decoder Remote cmmand NOT HANDLED Str:%s INt:%i \n", s_cmd.c_str(), cmd->cmds.cmd);
            }
        }
    }else{
        UtilityFunctions::debugLogf("Relay NOT enabled\n");
    }
}
