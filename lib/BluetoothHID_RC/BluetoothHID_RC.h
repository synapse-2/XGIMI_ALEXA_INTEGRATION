#pragma once
#include "defaults.h"
#include <HEXBuilder.h>

#include <nimconfig.h>
#include <NimBLEDevice.h>
#include <NimBLEHIDDevice.h>
#include <HIDTypes.h>



// appearance of the device and the advertisment
#define HID_REMOTE 0x0180

/* 
Define the Prefered prepheripral config params for the device in sdkconfig.defaults
this is all done by defines in NimbleBLE - ESP32 so we have to put them in the platform.ini
uint16_t connIntervalMin;    // !< Minimum connection interval. In 1.25 ms unit
uint16_t connIntervalMax;    // !< Maximum connection interval. In 1.25 ms unit
uint16_t connLatency;        // !< Slave latency.
uint16_t supTimeout;         // !< Supervision timeout. In 10 ms unit
uint16_t minCeLen;           // !< Minimum CE length. Set to 0
uint16_t maxCeLen;           // !< Maximum CE length. Set to 0

#ifndef CONFIG_BT_NIMBLE_SVC_GAP_PPCP_MIN_CONN_INTERVAL
#define CONFIG_BT_NIMBLE_SVC_GAP_PPCP_MIN_CONN_INTERVAL (10) // 11.25 ms
#endif

#ifndef CONFIG_BT_NIMBLE_SVC_GAP_PPCP_MAX_CONN_INTERVAL
#define CONFIG_BT_NIMBLE_SVC_GAP_PPCP_MAX_CONN_INTERVAL (10) // 11.25 ms
#endif 

#ifndef CONFIG_BT_NIMBLE_SVC_GAP_PPCP_SLAVE_LATENCY
#define CONFIG_BT_NIMBLE_SVC_GAP_PPCP_SLAVE_LATENCY (50)     // 50 ms
#endif 

#ifndef CONFIG_BT_NIMBLE_SVC_GAP_PPCP_SUPERVISION_TMO
#define CONFIG_BT_NIMBLE_SVC_GAP_PPCP_SUPERVISION_TMO (500)  // 500
#endif


define the CENTRAL_ADDRESS_RESOLUTION params for the device
this is all done by defines in NimbleBLE - ESP32 so we have to put them in the platform.ini

#ifndef CONFIG_BT_NIMBLE_SVC_GAP_CENTRAL_ADDRESS_RESOLUTION
#define CONFIG_BT_NIMBLE_SVC_GAP_CENTRAL_ADDRESS_RESOLUTION 0
#endif
*/

class BluetoothHID_RC : public NimBLEHIDDevice, public NimBLEServerCallbacks, public NimBLECharacteristicCallbacks
{
public:
    BluetoothHID_RC(BLEServer *server);

    // we donot have these in NibmeBLE NimBLEDevice so we need to ADD !!!
    void setDeviceAppreance(uint16_t appearance);
    void sendButtonPress(uint8_t command);
    void startServices();
    virtual ~BluetoothHID_RC();

protected:
    NimBLEServer *BLE_server;

    NimBLEAdvertising *advertising;
    NimBLEAdvertisementData advertisingData;
    NimBLEAdvertisementData advertisingScanData;

    NimBLECharacteristic *manufData;
    bool connected = false;


    // BLEServerCallbacks
    virtual void onConnect(NimBLEServer *pServer, NimBLEConnInfo &connInfo) override;
    virtual void onDisconnect(NimBLEServer *pServer, NimBLEConnInfo &connInfo, int reason) override;
    // BLECharacteristicCallbacks
    virtual void onRead(NimBLECharacteristic *pCharacteristic, NimBLEConnInfo &connInfo) override;
    virtual void onWrite(NimBLECharacteristic *pCharacteristic, NimBLEConnInfo &connInfo) override;
    virtual void onStatus(NimBLECharacteristic *pCharacteristic, int code) override;
    virtual void onSubscribe(NimBLECharacteristic *pCharacteristic, NimBLEConnInfo &connInfo, uint16_t subValue) override;

};