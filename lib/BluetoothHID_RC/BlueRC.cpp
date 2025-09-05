#include "UtilityFunctions.h"
#include "BlueRC.h"
#include "services/gap/ble_svc_gap.h"
#include "magicEnum/magic_enum.hpp"

BlueRC::BluetoothHID_RC::BluetoothHID_RC(NimBLEServer *server) : NimBLEHIDDevice(server)
{

  UtilityFunctions::debugLog("In Virtual HID startup!");

  BLE_server = server;
  BLE_server->setCallbacks(this);
  setDeviceAppreance(HID_REMOTE);
  advertising = server->getAdvertising();
  advertising->stop();
}

void BlueRC::BluetoothHID_RC::setDeviceAppreance(uint16_t appearance)
{

  ble_svc_gap_device_appearance_set(appearance);
}

BlueRC::BluetoothHID_RC::~BluetoothHID_RC()
{

  // Cleanup resources if needed
  if (advertising != NULL)
  {
    advertising->stop();
    delete advertising;
  }
  // delete BLE_server;
  UtilityFunctions::debugLog("BluetoothHID_RC destroyed");
}

void BlueRC::BluetoothHID_RC::onConnect(NimBLEServer *pServer, NimBLEConnInfo &connInfo)
{
  connected = true;
  UtilityFunctions::debugLog("Bluetooth Client connected");
  if (advertising != NULL)
  {
    UtilityFunctions::debugLogf("State of advertising is %i\n", advertising->isAdvertising());
  }
  if ((advertising != NULL) && (advertising->isAdvertising()))
  {
    UtilityFunctions::debugLog("Bluetooth Client connected and we were advertising so we stop advertising now");
  }
}

void BlueRC::BluetoothHID_RC::onDisconnect(NimBLEServer *pServer, NimBLEConnInfo &connInfo, int reason)
{
  connected = false;

  UtilityFunctions::debugLog("Bluetooth Client disconnected");
  if ((advertising != NULL) && (!advertising->isAdvertising()))
  {
    UtilityFunctions::debugLog("Bluetooth Client disconnected and we were  NOT advertising so we start advertising now");
    advertising->start();
  }
}

// the infamous entry in vtables :)
void BlueRC::BluetoothHID_RC::sendButtonPress(BlueRC::Remote_Cmd command) {}
bool canHandleButtonPress(BlueRC::Remote_Cmd command) {}
void BlueRC::BluetoothHID_RC::onRead(NimBLECharacteristic *pCharacteristic, NimBLEConnInfo &connInfo) {}
void BlueRC::BluetoothHID_RC::onWrite(NimBLECharacteristic *pCharacteristic, NimBLEConnInfo &connInfo) {}
void BlueRC::BluetoothHID_RC::onStatus(NimBLECharacteristic *pCharacteristic, int code) {}
void BlueRC::BluetoothHID_RC::onSubscribe(NimBLECharacteristic *pCharacteristic, NimBLEConnInfo &connInfo, uint16_t subValue) {}
