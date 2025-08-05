#pragma once
#include "defaults.h"

#ifdef USE_H2ZERO_NIMBLE_LIB
#include <nimconfig.h>
#include <NimBLEDevice.h>
#include <NimBLEHIDDevice.h>
#include <HIDTypes.h>
// for compatabality it does not exiist in Nimble code
#define BLEHIDDevice NimBLEHIDDevice
#else
#include <BLEDevice.h>
#include <BLEHIDDevice.h>
#include <HIDTypes.h>
#include <BLE2902.h>
#include <BLE2904.h>
#include <BLECharacteristic.h>
#define  NIMBLE_PROPERTY BLECharacteristic
#endif

#define HID_REMOTE 0x0180

class BluetoothHID_RC : public BLEHIDDevice, public BLEServerCallbacks, public BLECharacteristicCallbacks
{
public:
    BluetoothHID_RC(BLEServer *server);
    void sendButtonPress(uint8_t command);
    void startServices();
    virtual ~BluetoothHID_RC();

protected:
    BLEServer *BLE_server;
    BLEAdvertising *advertising;
    BLEAdvertisementData advertisingData;
    bool connected = false;

#ifdef USE_H2ZERO_NIMBLE_LIB
    // BLEServerCallbacks
    virtual void onConnect(BLEServer *pServer, BLEConnInfo &connInfo) override;
    virtual void onDisconnect(BLEServer *pServer, BLEConnInfo &connInfo, int reason) override;
    // BLECharacteristicCallbacks
    virtual void onRead(NimBLECharacteristic *pCharacteristic, NimBLEConnInfo &connInfo) override;
    virtual void onWrite(NimBLECharacteristic *pCharacteristic, NimBLEConnInfo &connInfo) override;
    virtual void onStatus(NimBLECharacteristic *pCharacteristic, int code) override;
    virtual void onSubscribe(NimBLECharacteristic *pCharacteristic, NimBLEConnInfo &connInfo, uint16_t subValue) override;
#else
    virtual void onConnect(BLEServer *pServer) override;
    virtual void onDisconnect(BLEServer *pServer) override;

    virtual void onRead(BLECharacteristic *pCharacteristic) override;
    virtual void onWrite(BLECharacteristic *pCharacteristic) override;
    virtual void onNotify(BLECharacteristic *pCharacteristic) override;
    virtual void onStatus(BLECharacteristic *pCharacteristic, Status s, uint32_t code) override;

    BLEService *getDeviceInfoService();
    BLECharacteristic* getInputReport(uint8_t reportID);
    BLECharacteristic* getOutputReport(uint8_t reportID);
    void setManufacturer(std::string name);
    void setPnp(uint8_t sig, uint16_t vid, uint16_t pid, uint16_t version);
    void setHidInfo(uint8_t country, uint8_t flags);
    void setReportMap(uint8_t *map, uint16_t size);
    BLEService *getHidService();
    BLEService *getBatteryService();

#endif
};