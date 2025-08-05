
#include <WiFiManager.h>      // https://github.com/tzapu/WiFiManager
#include <UtilityFunctions.h> // Custom utility functions
#include "Slave.h"
#include "BluetoothHID_RC.h" // Slave class for I2C Slave functionality
#include "UtilityFunctions.h" // Utility functions for LED control and other utilities

Slave::Slave() {}

void Slave::start()
{
    // need to make the Bluetooth client

    UtilityFunctions::ledBlue();
    BLEDevice::init(HID_DEVICE_NAME);
    BLEServer *pServer = BLEDevice::createServer();
    if (pServer == NULL){
        UtilityFunctions::debugLog("Null BLE devide server created stopping BLE ");
        return;
    }
     UtilityFunctions::debugLog("BLE server created");
    rcHID = new BlueXGIMI_RC(pServer);

     // if you get here you have connected to the WiFi
    UtilityFunctions::debugLog("Bluetooth started...yeey :)");
}
