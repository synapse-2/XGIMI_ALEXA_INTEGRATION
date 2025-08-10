#include "UtilityFunctions.h"
#include "BlueXGIMI_RC.h"

BluetoothHID_RC::BluetoothHID_RC(BLEServer *server) : BLEHIDDevice(server)
{

  UtilityFunctions::debugLog("In Virtual HID startup!");

  BLE_server = server;
  BLE_server->setCallbacks(this);
}

BluetoothHID_RC::~BluetoothHID_RC()
{
#ifndef CONFIG_BT_NIMBLE_EXT_ADV
  // Cleanup resources if needed
  if (advertising)
  {
    advertising->stop();
    delete advertising;
  }
#endif
  if (BLE_server->getConnectedCount() > 0)
  {

#ifndef USE_H2ZERO_NIMBLE_LIB
    BLE_server->disconnect(BLE_server->getConnId());
#else

    std::vector<uint16_t> conIds = BLE_server->getPeerDevices();
    for (const auto &peer : conIds)
    {
      BLE_server->disconnect(peer);
    }

#endif
  }
  else
  {
    UtilityFunctions::debugLog("No active connections to disconnect");
  }
  //delete BLE_server;
  UtilityFunctions::debugLog("BluetoothHID_RC destroyed");
}

#ifdef USE_H2ZERO_NIMBLE_LIB
void BluetoothHID_RC::onConnect(BLEServer *pServer, BLEConnInfo &connInfo)
#else
void BluetoothHID_RC::onConnect(BLEServer *pServer)
#endif
{
  connected = true;

  UtilityFunctions::debugLog("Bluetooth Client connected");
}

#ifdef USE_H2ZERO_NIMBLE_LIB
void BluetoothHID_RC::onDisconnect(BLEServer *pServer, BLEConnInfo &connInfo, int reason)
#else
void BluetoothHID_RC::onDisconnect(BLEServer *pServer)
#endif
{
  connected = false;

  UtilityFunctions::debugLog("Bluetooth Client disconnected");
}

#ifdef USE_H2ZERO_NIMBLE_LIB
void BluetoothHID_RC::onRead(NimBLECharacteristic *pCharacteristic, NimBLEConnInfo &connInfo) {}
void BluetoothHID_RC::onWrite(NimBLECharacteristic *pCharacteristic, NimBLEConnInfo &connInfo) {};
void BluetoothHID_RC::onStatus(NimBLECharacteristic *pCharacteristic, int code) {};
void BluetoothHID_RC::onSubscribe(NimBLECharacteristic *pCharacteristic, NimBLEConnInfo &connInfo, uint16_t subValue) {};
#else
void BluetoothHID_RC::onRead(BLECharacteristic *pCharacteristic) {};
void BluetoothHID_RC::onWrite(BLECharacteristic *pCharacteristic) {};
void BluetoothHID_RC::onNotify(BLECharacteristic *pCharacteristic) {};
void BluetoothHID_RC::onStatus(BLECharacteristic *pCharacteristic, Status s, uint32_t code) {};
#endif

#ifdef USE_H2ZERO_NIMBLE_LIB
#else
BLEService *BluetoothHID_RC::getDeviceInfoService()
{

  return deviceInfo();
}

BLECharacteristic *BluetoothHID_RC::getInputReport(uint8_t reportID)
{
  return (inputReport(reportID));
}

BLECharacteristic *BluetoothHID_RC::getOutputReport(uint8_t reportID)
{

  return (outputReport(reportID));
}

void BluetoothHID_RC::setManufacturer(std::string name)
{
  manufacturer();
  String sname = String(name.c_str());
  UtilityFunctions::debugLog("set manu name" + sname);
  manufacturer(sname);
  UtilityFunctions::debugLog("set manu name COMPLETE");
}

void BluetoothHID_RC::setPnp(uint8_t sig, uint16_t vid, uint16_t pid, uint16_t version)
{
  pnp(sig, vid, pid, version);
}

void BluetoothHID_RC::setHidInfo(uint8_t country, uint8_t flags)
{
  hidInfo(country, flags);
}

void BluetoothHID_RC::setReportMap(uint8_t *map, uint16_t size)
{
  reportMap(map, size);
}

BLEService *BluetoothHID_RC::getHidService() { return hidService(); }
BLEService *BluetoothHID_RC::getBatteryService() { return batteryService(); }

#endif
