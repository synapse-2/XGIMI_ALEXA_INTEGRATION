
#include <WiFiManager.h>      // https://github.com/tzapu/WiFiManager
#include <UtilityFunctions.h> // Custom utility functions
#include "BLE_Remote_Decoder.h"
#include "BlueXGIMI_RC.h" // Slave class for I2C Slave functionality
#include "UtilityFunctions.h" // Utility functions for LED control and other utilities
#include "CmdRingBuffer.h"



BLE_Remote_Decoder::BLE_Remote_Decoder(){
}

void BLE_Remote_Decoder::start()
{
    // need to make the Bluetooth client

    
    NimBLEDevice::init(HID_DEVICE_NAME);
    NimBLEServer *pServer = NimBLEDevice::createServer();
    if (pServer == NULL){
        UtilityFunctions::debugLog("Null BLE devide server created stopping BLE ");
        UtilityFunctions::ledBlinkRedLong();
        UtilityFunctions::ledBlinkRed();
        return;
    }
     UtilityFunctions::debugLog("BLE server created");
    rcHID = new BlueXGIMI_RC(pServer); 

     // if you get here you have connected to the WiFi
    UtilityFunctions::debugLog("Bluetooth started...yeey :)");
}

void BLE_Remote_Decoder::dequeueCmd()
{
    BlueRC::Remote_Cmd* cmd;
    size_t received_len;
    cmd = CmdRingBuffer::peekCmd();
    if (cmd != NULL)
    {
        std::string s_cmd =  std::string((magic_enum::enum_name((BlueRC::RC_Cmd_Action) cmd->cmds.cmd))); 
        UtilityFunctions::debugLogf("Remote cmmand DEQUEUEED Str:%s INt:%i \n", s_cmd.c_str(),cmd->cmds.cmd );

        bool cmdhandled = false;
        if ((rcHID != NULL) && (rcHID->canHandleButtonPress(*cmd))){
                rcHID->sendButtonPress(*cmd);
                cmdhandled = true;
        }

        // debug log if the cmd was not handled as not all remotes handle all commands
        if (!cmdhandled){
            UtilityFunctions::debugLogf("Remote cmmand NOT HANDLED Str:%s INt:%i \n", s_cmd.c_str(),cmd->cmds.cmd );   
        }
        // free up the buffer
        CmdRingBuffer::dequeueCmd(cmd);
    }
}
