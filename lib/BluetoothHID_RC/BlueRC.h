#pragma once
#include "defaults.h"
#include "magicEnum/magic_enum.hpp"
#include <NimBLEDevice.h>
#include <NimBLEHIDDevice.h>
#include <HIDTypes.h>
#ifdef XGIMI_USE_EXT_ADV
#include <NimBLEExtAdvertising.h>
#endif
#include "ServerDecoder.h"


// appearance of the device and the advertisment
#define HID_REMOTE_APPEARANCE 0x0180
#define HID_KEYBORAD_APPEARANCE 0x03c1

static constexpr uint16_t deviceInfoSvcUuid = 0x180a;
static constexpr uint16_t hidSvcUuid        = 0x1812;
static constexpr uint16_t batterySvcUuid    = 0x180f;

static constexpr uint16_t pnpCharUuid           = 0x2a50;
static constexpr uint16_t hidInfoCharUuid       = 0x2a4a;
static constexpr uint16_t reportMapCharUuid     = 0x2a4b;
static constexpr uint16_t hidControlCharUuid    = 0x2a4c;
static constexpr uint16_t inputReportChrUuid    = 0x2a4d;
static constexpr uint16_t protocolModeCharUuid  = 0x2a4e;
static constexpr uint16_t batteryLevelCharUuid  = 0x2a19;
static constexpr uint16_t batteryLevelDscUuid   = 0x2904;
static constexpr uint16_t featureReportDscUuid  = 0x2908;
static constexpr uint16_t m_manufacturerChrUuid = 0x2a29;
static constexpr uint16_t bootInputChrUuid      = 0x2a22;
static constexpr uint16_t bootOutputChrUuid     = 0x2a32;

// define the command set
namespace BlueRC
{

    class BluetoothHID_RC : public NimBLEServerCallbacks, public NimBLECharacteristicCallbacks, public ServerDecoderBase
    {
    public:
        BluetoothHID_RC(BLEServer *server);

        // we donot have these in NibmeBLE NimBLEDevice so we need to ADD !!!
        void setDeviceAppreance(uint16_t appearance);
        virtual void sendButtonPress(ServerDecoder::Remote_Cmd command);
        virtual bool canHandleButtonPress(ServerDecoder::Remote_Cmd command);
        virtual void startStandardAdv();
        virtual void initStandardAdvData();
        void startServices();
        virtual ~BluetoothHID_RC();
        void setReportMap(uint8_t *map, uint16_t);
        bool setManufacturer(const std::string &name);
        void setPnp(uint8_t sig, uint16_t vid, uint16_t pid, uint16_t version);
        void setHidInfo(uint8_t country, uint8_t flags);
        void setBatteryLevel(uint8_t level, bool notify = false);
        NimBLECharacteristic *getBatteryLevel();
        NimBLECharacteristic *getReportMap();
        NimBLECharacteristic *getHidControl();
        NimBLECharacteristic *getInputReport(uint8_t reportId);
        NimBLECharacteristic *getOutputReport(uint8_t reportId);
        NimBLECharacteristic *getFeatureReport(uint8_t reportId);
        NimBLECharacteristic *getProtocolMode();
        NimBLECharacteristic *getBootInput();
        NimBLECharacteristic *getBootOutput();
        NimBLECharacteristic *getPnp();
        NimBLECharacteristic *getHidInfo();
        NimBLEService *getDeviceInfoService();
        NimBLEService *getHidService();
        NimBLEService *getBatteryService();
        uint8_t getConnectedCount();

    protected:
        NimBLEServer *BLE_server;
        NimBLEService *m_deviceInfoSvc{nullptr}; // 0x180a
        NimBLEService *m_hidSvc{nullptr};        // 0x1812
        NimBLEService *m_batterySvc{nullptr};    // 0x180f

        NimBLECharacteristic *m_manufacturerChr{nullptr}; // 0x2a29
        NimBLECharacteristic *m_pnpChr{nullptr};          // 0x2a50
        NimBLECharacteristic *m_hidInfoChr{nullptr};      // 0x2a4a
        NimBLECharacteristic *m_reportMapChr{nullptr};    // 0x2a4b
        NimBLECharacteristic *m_hidControlChr{nullptr};   // 0x2a4c
        NimBLECharacteristic *m_protocolModeChr{nullptr}; // 0x2a4e
        NimBLECharacteristic *m_batteryLevelChr{nullptr}; // 0x2a19

        NimBLECharacteristic *locateReportCharacteristicByIdAndType(uint8_t reportId, uint8_t reportType);

#ifdef XGIMI_USE_EXT_ADV
        NimBLEExtAdvertising *advertising;
        NimBLEExtAdvertisement advertisingData;
        NimBLEExtAdvertisement advertisingScanData;
#else

        NimBLEAdvertising *advertising;
        NimBLEAdvertisementData advertisingData;
        NimBLEAdvertisementData advertisingScanData;
#endif

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

}

