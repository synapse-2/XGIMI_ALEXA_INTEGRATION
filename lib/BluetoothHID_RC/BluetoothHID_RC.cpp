#include "UtilityFunctions.h"
#include "BlueXGIMI_RC.h"
// #include "ble_svc_gap.h"
#include "services/gap/ble_svc_gap.h"

BluetoothHID_RC::BluetoothHID_RC(BLEServer *server) : BLEHIDDevice(server)
{

  UtilityFunctions::debugLog("In Virtual HID startup!");

  BLE_server = server;
  BLE_server->setCallbacks(this);
  setDeviceAppreance(HID_REMOTE);
}

void BluetoothHID_RC::setDeviceAppreance(uint16_t appearance)
{

  ble_svc_gap_device_appearance_set(appearance);
}

BluetoothHID_RC::~BluetoothHID_RC()
{

  // Cleanup resources if needed
  if (advertising)
  {
    advertising->stop();
    delete advertising;
  }
  // delete BLE_server;
  UtilityFunctions::debugLog("BluetoothHID_RC destroyed");
}

void BluetoothHID_RC::onConnect(BLEServer *pServer, BLEConnInfo &connInfo)
{
  connected = true;

  UtilityFunctions::debugLog("Bluetooth Client connected");
}

void BluetoothHID_RC::onDisconnect(BLEServer *pServer, BLEConnInfo &connInfo, int reason)
{
  connected = false;

  UtilityFunctions::debugLog("Bluetooth Client disconnected");
}

void BluetoothHID_RC::onRead(NimBLECharacteristic *pCharacteristic, NimBLEConnInfo &connInfo) {}
void BluetoothHID_RC::onWrite(NimBLECharacteristic *pCharacteristic, NimBLEConnInfo &connInfo) {};
void BluetoothHID_RC::onStatus(NimBLECharacteristic *pCharacteristic, int code) {};
void BluetoothHID_RC::onSubscribe(NimBLECharacteristic *pCharacteristic, NimBLEConnInfo &connInfo, uint16_t subValue) {};
