
#include <WiFiManager.h>      // https://github.com/tzapu/WiFiManager
#include <UtilityFunctions.h> // Custom utility functions
#include "BLE_Remote_Decoder.h"
#include "BlueXGIMI_RC.h" // Slave class for I2C Slave functionality
#include "UtilityFunctions.h" // Utility functions for LED control and other utilities
#include <freertos/ringbuf.h>


RingbufHandle_t BLE_Remote_Decoder::ringBufHandle;

BLE_Remote_Decoder::BLE_Remote_Decoder(RingbufHandle_t buf) {

    ringBufHandle = buf;
}

BLE_Remote_Decoder::BLE_Remote_Decoder() : BLE_Remote_Decoder(NULL){
}

void BLE_Remote_Decoder::start()
{
    // need to make the Bluetooth client

    UtilityFunctions::ledBlue();
    NimBLEDevice::init(HID_DEVICE_NAME);
    NimBLEServer *pServer = NimBLEDevice::createServer();
    if (pServer == NULL){
        UtilityFunctions::debugLog("Null BLE devide server created stopping BLE ");
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
    cmd = (BlueRC::Remote_Cmd*)xRingbufferReceive(ringBufHandle, &received_len, 50);
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
        vRingbufferReturnItem(ringBufHandle, (void *)cmd);
    }
}
