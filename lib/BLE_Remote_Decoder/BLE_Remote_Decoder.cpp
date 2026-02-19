/**
 * @file BLE_Remote_Decoder.cpp
 * @brief Brief summary of this file.
 *
 * Detailed description of the file and its purpose.
 */


#include <WiFiManager.h>      // https://github.com/tzapu/WiFiManager
#include <UtilityFunctions.h> // Custom utility functions
#include "BLE_Remote_Decoder.h"
#include "BlueXGIMI_RC.h"     // Slave class for I2C Slave functionality
#include "UtilityFunctions.h" // Utility functions for LED control and other utilities
#include "CmdRingBuffer.h"

BLE_Remote_Decoder::BLE_Remote_Decoder()
{
}

uint8_t BLE_Remote_Decoder::getConnectedCount()
{
    if (rcHID != NULL)
    {
        return rcHID->getConnectedCount();
    }
    return 0;
}

bool BLE_Remote_Decoder::disconnectAllClients()
{
    bool sucess = true;
    if (rcHID != NULL)
    {
        // Returns a vector of connection handles (uint16_t)
        std::vector<uint16_t> handles = rcHID->getPeerDevices();

/**
 * @brief Brief description of debugLogf.
 *
 * @param disconnect:%i\n" Describe this parameter.
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
        UtilityFunctions::debugLogf("Ble peers for disconnect:%i\n", handles.size());

        for (uint16_t handle : handles)
        {
            // Fetch detailed info using the handle
            if (!rcHID->disconnect(handle, BLE_ERR_REM_USER_CONN_TERM))
            {
                sucess = false;
/**
 * @brief Brief description of debugLogf.
 *
 * @param %i\n" Describe this parameter.
 * @param handle Describe this parameter.
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
                UtilityFunctions::debugLogf("Unable to disconnect handle %i\n", handle);
            }
        }
    }

    if (!BLEDevice::deleteAllBonds()){
        sucess = false;
/**
 * @brief Brief description of debugLog.
 *
 * @param bonds" Describe this parameter.
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
        UtilityFunctions::debugLog("Unable to delete all bonds");
    }
    return sucess;
}

String BLE_Remote_Decoder::getConnectedDevicesTable()
{
    // Retrieve the list of currently connected peer devices
    // true = return client devices (when ESP32 is a server)
    std::string str = "";
    if (rcHID != NULL)
    {
        // Returns a vector of connection handles (uint16_t)
        std::vector<uint16_t> handles = rcHID->getPeerDevices();

/**
 * @brief Brief description of debugLogf.
 *
 * @param peers:%i\n" Describe this parameter.
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
        UtilityFunctions::debugLogf("Ble peers:%i\n", handles.size());

        str = str + std::format("{: <3}{: <4}{: <18}{: <7}{: <4}{: <5}{: <4}\n",
                                "#", "M/S", "ADDR", "Bonded", "Enc", "Auth", "Key");

        if (handles.empty())
        {
            str = str + std::format("{: <3}{: <4}{: <18}{: <7}{: <4}{: <5}{: <4}\n",
                                    "-", "-", "-", "none", "-", "-", "-");
        }
        else
        {
            for (uint16_t handle : handles)
            {
                // Fetch detailed info using the handle
/**
 * @brief Brief description of debugLogf.
 *
 * @param handle:%i\n" Describe this parameter.
 * @param handle Describe this parameter.
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
                UtilityFunctions::debugLogf("BLE info get handle:%i\n", handle);
                NimBLEConnInfo info = rcHID->getPeerInfo(handle);

                if (info.getConnHandle() != BLE_HS_CONN_HANDLE_NONE)
                {
                    str = str + std::format("{: <3}{: <4}{: <18}{: <7}{: <4}{: <5}{: <4}\n",
                                            info.getConnHandle(),
                                            info.isMaster() ? "M" : "S",
                                            info.getIdAddress().toString(),
                                            info.isBonded() ? "Y" : "N",
                                            info.isEncrypted() ? "Y" : "N",
                                            info.isAuthenticated() ? "Y" : "N",
                                            info.getSecKeySize());
                }
            }
        }
    }

/**
 * @brief Brief description of String.
 *
 * @param param Describe this parameter.
 * @return return Describe the return value.
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
    return String(str.c_str());
}

void BLE_Remote_Decoder::start()
{
    // need to make the Bluetooth client

/**
 * @brief Brief description of init.
 *
 * @param param Describe this parameter.
 * @return NimBLEDevice:: Describe the return value.
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
    NimBLEDevice::init(std::string(UtilityFunctions::loadBlueToothName().c_str()));
    NimBLEServer *pServer = NimBLEDevice::createServer();
    if (pServer == NULL)
    {
/**
 * @brief Brief description of debugLog.
 *
 * @param " Describe this parameter.
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
        UtilityFunctions::debugLog("Null BLE devide server created stopping BLE ");
/**
 * @brief Brief description of ledBlinkRedLong.
 *
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
        UtilityFunctions::ledBlinkRedLong();
/**
 * @brief Brief description of ledBlinkRed.
 *
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
        UtilityFunctions::ledBlinkRed();
        return;
    }
/**
 * @brief Brief description of debugLog.
 *
 * @param created" Describe this parameter.
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
    UtilityFunctions::debugLog("BLE server created");
    rcHID = new BlueXGIMI_RC(pServer);

    // if you get here you have connected to the WiFi
/**
 * @brief Brief description of debugLog.
 *
 * @param :)" Describe this parameter.
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
    UtilityFunctions::debugLog("Bluetooth started...yeey :)");
}

void BLE_Remote_Decoder::doCmd(ServerDecoder::Remote_Cmd *cmd)
{

    if (cmd != NULL)
    {
        std::string s_cmd = std::string((magic_enum::enum_name((ServerDecoder::RC_Cmd_Action)cmd->cmds.cmd)));
        // UtilityFunctions::debugLogf("Remote cmmand DEQUEUEED Str:%s INt:%i \n", s_cmd.c_str(),cmd->cmds.cmd );

        bool cmdhandled = false;
        if ((rcHID != NULL) && (rcHID->canHandleButtonPress(*cmd)))
        {
            rcHID->sendButtonPress(*cmd);
            cmdhandled = true;
        }

        // debug log if the cmd was not handled as not all remotes handle all commands
        if (!cmdhandled)
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
            UtilityFunctions::debugLogf("By BLE remote decoder - Remote cmmand NOT HANDLED Str:%s INt:%i \n", s_cmd.c_str(), cmd->cmds.cmd);
        }
    }
}
