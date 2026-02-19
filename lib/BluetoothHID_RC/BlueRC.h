/*
 * Overview:
 * This file is part of the BluetoothESP32 device project.
 * It contains code used to handle hardware, BLE, WiFi, or web UI
 * interactions for the device. Comments marked with [AUTO-DOC] were
 * added automatically; please refine them to be more specific.
 */

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

    // default BASE class for a bluetooth remote
    class BluetoothHID_RC : public NimBLEServerCallbacks, public NimBLECharacteristicCallbacks, public ServerDecoderBase
    {
    public:
        BluetoothHID_RC(BLEServer *server);

        // we donot have these in NibmeBLE NimBLEDevice so we need to ADD !!!
/* [AUTO-DOC] */
/**
 * @brief Handles set Device Appreance.
 *
 * @param appearance Describe this parameter.
 *
 * Algorithm:
 * - High-level steps the function performs.
 * - Mention important side-effects (WiFi, BLE, hardware).
 *
 * Loops:
 * - Describe each loop purpose and termination condition here.
 *
 * Complexity:
 * - Time: O(...)
 * - Space: O(...)
 */
        void setDeviceAppreance(uint16_t appearance);
/* [AUTO-DOC] */
/**
 * @brief Performs send Button Press operations.
 *
 * @param command Describe this parameter.
 * @return virtual void Describe the return value.
 *
 * Algorithm:
 * - High-level steps the function performs.
 * - Mention important side-effects (WiFi, BLE, hardware).
 *
 * Loops:
 * - Describe each loop purpose and termination condition here.
 *
 * Complexity:
 * - Time: O(...)
 * - Space: O(...)
 */
        virtual void sendButtonPress(ServerDecoder::Remote_Cmd command);
/* [AUTO-DOC] */
/**
 * @brief Performs can Handle Button Press operations.
 *
 * @param command Describe this parameter.
 * @return virtual bool Describe the return value.
 *
 * Algorithm:
 * - High-level steps the function performs.
 * - Mention important side-effects (WiFi, BLE, hardware).
 *
 * Loops:
 * - Describe each loop purpose and termination condition here.
 *
 * Complexity:
 * - Time: O(...)
 * - Space: O(...)
 */
        virtual bool canHandleButtonPress(ServerDecoder::Remote_Cmd command);
/* [AUTO-DOC] */
/**
 * @brief Initializes or starts start Standard Adv.
 *
 * @return virtual void Describe the return value.
 *
 * Algorithm:
 * - High-level steps the function performs.
 * - Mention important side-effects (WiFi, BLE, hardware).
 *
 * Loops:
 * - Describe each loop purpose and termination condition here.
 *
 * Complexity:
 * - Time: O(...)
 * - Space: O(...)
 */
        virtual void startStandardAdv();
/* [AUTO-DOC] */
/**
 * @brief Initializes or starts init Standard Adv Data.
 *
 * @return virtual void Describe the return value.
 *
 * Algorithm:
 * - High-level steps the function performs.
 * - Mention important side-effects (WiFi, BLE, hardware).
 *
 * Loops:
 * - Describe each loop purpose and termination condition here.
 *
 * Complexity:
 * - Time: O(...)
 * - Space: O(...)
 */
        virtual void initStandardAdvData();
/* [AUTO-DOC] */
/**
 * @brief Initializes or starts start Services.
 *
 *
 * Algorithm:
 * - High-level steps the function performs.
 * - Mention important side-effects (WiFi, BLE, hardware).
 *
 * Loops:
 * - Describe each loop purpose and termination condition here.
 *
 * Complexity:
 * - Time: O(...)
 * - Space: O(...)
 */
        void startServices();
/* [AUTO-DOC] */
/**
 * @brief Performs Bluetooth HID RC operations.
 *
 * @return virtual ~ Describe the return value.
 *
 * Algorithm:
 * - High-level steps the function performs.
 * - Mention important side-effects (WiFi, BLE, hardware).
 *
 * Loops:
 * - Describe each loop purpose and termination condition here.
 *
 * Complexity:
 * - Time: O(...)
 * - Space: O(...)
 */
        virtual ~BluetoothHID_RC();
/* [AUTO-DOC] */
/**
 * @brief Handles set Report Map.
 *
 * @param *map Describe this parameter.
 * @param uint16_t Describe this parameter.
 *
 * Algorithm:
 * - High-level steps the function performs.
 * - Mention important side-effects (WiFi, BLE, hardware).
 *
 * Loops:
 * - Describe each loop purpose and termination condition here.
 *
 * Complexity:
 * - Time: O(...)
 * - Space: O(...)
 */
        void setReportMap(uint8_t *map, uint16_t);
/* [AUTO-DOC] */
/**
 * @brief Handles set Manufacturer.
 *
 * @param &name Describe this parameter.
 * @return bool Describe the return value.
 *
 * Algorithm:
 * - High-level steps the function performs.
 * - Mention important side-effects (WiFi, BLE, hardware).
 *
 * Loops:
 * - Describe each loop purpose and termination condition here.
 *
 * Complexity:
 * - Time: O(...)
 * - Space: O(...)
 */
        bool setManufacturer(const std::string &name);
/* [AUTO-DOC] */
/**
 * @brief Handles set Pnp.
 *
 * @param sig Describe this parameter.
 * @param vid Describe this parameter.
 * @param pid Describe this parameter.
 * @param version Describe this parameter.
 *
 * Algorithm:
 * - High-level steps the function performs.
 * - Mention important side-effects (WiFi, BLE, hardware).
 *
 * Loops:
 * - Describe each loop purpose and termination condition here.
 *
 * Complexity:
 * - Time: O(...)
 * - Space: O(...)
 */
        void setPnp(uint8_t sig, uint16_t vid, uint16_t pid, uint16_t version);
/* [AUTO-DOC] */
/**
 * @brief Handles set Hid Info.
 *
 * @param country Describe this parameter.
 * @param flags Describe this parameter.
 *
 * Algorithm:
 * - High-level steps the function performs.
 * - Mention important side-effects (WiFi, BLE, hardware).
 *
 * Loops:
 * - Describe each loop purpose and termination condition here.
 *
 * Complexity:
 * - Time: O(...)
 * - Space: O(...)
 */
        void setHidInfo(uint8_t country, uint8_t flags);
/* [AUTO-DOC] */
/**
 * @brief Handles set Battery Level.
 *
 * @param level Describe this parameter.
 * @param false Describe this parameter.
 *
 * Algorithm:
 * - High-level steps the function performs.
 * - Mention important side-effects (WiFi, BLE, hardware).
 *
 * Loops:
 * - Describe each loop purpose and termination condition here.
 *
 * Complexity:
 * - Time: O(...)
 * - Space: O(...)
 */
        void setBatteryLevel(uint8_t level, bool notify = false);
/* [AUTO-DOC] */
/**
 * @brief Handles get Battery Level.
 *
 * @return NimBLECharacteristic * Describe the return value.
 *
 * Algorithm:
 * - High-level steps the function performs.
 * - Mention important side-effects (WiFi, BLE, hardware).
 *
 * Loops:
 * - Describe each loop purpose and termination condition here.
 *
 * Complexity:
 * - Time: O(...)
 * - Space: O(...)
 */
        NimBLECharacteristic *getBatteryLevel();
/* [AUTO-DOC] */
/**
 * @brief Handles get Report Map.
 *
 * @return NimBLECharacteristic * Describe the return value.
 *
 * Algorithm:
 * - High-level steps the function performs.
 * - Mention important side-effects (WiFi, BLE, hardware).
 *
 * Loops:
 * - Describe each loop purpose and termination condition here.
 *
 * Complexity:
 * - Time: O(...)
 * - Space: O(...)
 */
        NimBLECharacteristic *getReportMap();
/* [AUTO-DOC] */
/**
 * @brief Handles get Hid Control.
 *
 * @return NimBLECharacteristic * Describe the return value.
 *
 * Algorithm:
 * - High-level steps the function performs.
 * - Mention important side-effects (WiFi, BLE, hardware).
 *
 * Loops:
 * - Describe each loop purpose and termination condition here.
 *
 * Complexity:
 * - Time: O(...)
 * - Space: O(...)
 */
        NimBLECharacteristic *getHidControl();
/* [AUTO-DOC] */
/**
 * @brief Handles get Input Report.
 *
 * @param reportId Describe this parameter.
 * @return NimBLECharacteristic * Describe the return value.
 *
 * Algorithm:
 * - High-level steps the function performs.
 * - Mention important side-effects (WiFi, BLE, hardware).
 *
 * Loops:
 * - Describe each loop purpose and termination condition here.
 *
 * Complexity:
 * - Time: O(...)
 * - Space: O(...)
 */
        NimBLECharacteristic *getInputReport(uint8_t reportId);
/* [AUTO-DOC] */
/**
 * @brief Handles get Output Report.
 *
 * @param reportId Describe this parameter.
 * @return NimBLECharacteristic * Describe the return value.
 *
 * Algorithm:
 * - High-level steps the function performs.
 * - Mention important side-effects (WiFi, BLE, hardware).
 *
 * Loops:
 * - Describe each loop purpose and termination condition here.
 *
 * Complexity:
 * - Time: O(...)
 * - Space: O(...)
 */
        NimBLECharacteristic *getOutputReport(uint8_t reportId);
/* [AUTO-DOC] */
/**
 * @brief Handles get Feature Report.
 *
 * @param reportId Describe this parameter.
 * @return NimBLECharacteristic * Describe the return value.
 *
 * Algorithm:
 * - High-level steps the function performs.
 * - Mention important side-effects (WiFi, BLE, hardware).
 *
 * Loops:
 * - Describe each loop purpose and termination condition here.
 *
 * Complexity:
 * - Time: O(...)
 * - Space: O(...)
 */
        NimBLECharacteristic *getFeatureReport(uint8_t reportId);
/* [AUTO-DOC] */
/**
 * @brief Handles get Protocol Mode.
 *
 * @return NimBLECharacteristic * Describe the return value.
 *
 * Algorithm:
 * - High-level steps the function performs.
 * - Mention important side-effects (WiFi, BLE, hardware).
 *
 * Loops:
 * - Describe each loop purpose and termination condition here.
 *
 * Complexity:
 * - Time: O(...)
 * - Space: O(...)
 */
        NimBLECharacteristic *getProtocolMode();
/* [AUTO-DOC] */
/**
 * @brief Handles get Boot Input.
 *
 * @return NimBLECharacteristic * Describe the return value.
 *
 * Algorithm:
 * - High-level steps the function performs.
 * - Mention important side-effects (WiFi, BLE, hardware).
 *
 * Loops:
 * - Describe each loop purpose and termination condition here.
 *
 * Complexity:
 * - Time: O(...)
 * - Space: O(...)
 */
        NimBLECharacteristic *getBootInput();
/* [AUTO-DOC] */
/**
 * @brief Handles get Boot Output.
 *
 * @return NimBLECharacteristic * Describe the return value.
 *
 * Algorithm:
 * - High-level steps the function performs.
 * - Mention important side-effects (WiFi, BLE, hardware).
 *
 * Loops:
 * - Describe each loop purpose and termination condition here.
 *
 * Complexity:
 * - Time: O(...)
 * - Space: O(...)
 */
        NimBLECharacteristic *getBootOutput();
/* [AUTO-DOC] */
/**
 * @brief Handles get Pnp.
 *
 * @return NimBLECharacteristic * Describe the return value.
 *
 * Algorithm:
 * - High-level steps the function performs.
 * - Mention important side-effects (WiFi, BLE, hardware).
 *
 * Loops:
 * - Describe each loop purpose and termination condition here.
 *
 * Complexity:
 * - Time: O(...)
 * - Space: O(...)
 */
        NimBLECharacteristic *getPnp();
/* [AUTO-DOC] */
/**
 * @brief Handles get Hid Info.
 *
 * @return NimBLECharacteristic * Describe the return value.
 *
 * Algorithm:
 * - High-level steps the function performs.
 * - Mention important side-effects (WiFi, BLE, hardware).
 *
 * Loops:
 * - Describe each loop purpose and termination condition here.
 *
 * Complexity:
 * - Time: O(...)
 * - Space: O(...)
 */
        NimBLECharacteristic *getHidInfo();
/* [AUTO-DOC] */
/**
 * @brief Handles get Device Info Service.
 *
 * @return NimBLEService * Describe the return value.
 *
 * Algorithm:
 * - High-level steps the function performs.
 * - Mention important side-effects (WiFi, BLE, hardware).
 *
 * Loops:
 * - Describe each loop purpose and termination condition here.
 *
 * Complexity:
 * - Time: O(...)
 * - Space: O(...)
 */
        NimBLEService *getDeviceInfoService();
/* [AUTO-DOC] */
/**
 * @brief Handles get Hid Service.
 *
 * @return NimBLEService * Describe the return value.
 *
 * Algorithm:
 * - High-level steps the function performs.
 * - Mention important side-effects (WiFi, BLE, hardware).
 *
 * Loops:
 * - Describe each loop purpose and termination condition here.
 *
 * Complexity:
 * - Time: O(...)
 * - Space: O(...)
 */
        NimBLEService *getHidService();
/* [AUTO-DOC] */
/**
 * @brief Handles get Battery Service.
 *
 * @return NimBLEService * Describe the return value.
 *
 * Algorithm:
 * - High-level steps the function performs.
 * - Mention important side-effects (WiFi, BLE, hardware).
 *
 * Loops:
 * - Describe each loop purpose and termination condition here.
 *
 * Complexity:
 * - Time: O(...)
 * - Space: O(...)
 */
        NimBLEService *getBatteryService();
/* [AUTO-DOC] */
/**
 * @brief Handles get Connected Count.
 *
 * @return uint8_t Describe the return value.
 *
 * Algorithm:
 * - High-level steps the function performs.
 * - Mention important side-effects (WiFi, BLE, hardware).
 *
 * Loops:
 * - Describe each loop purpose and termination condition here.
 *
 * Complexity:
 * - Time: O(...)
 * - Space: O(...)
 */
        uint8_t getConnectedCount();
/* [AUTO-DOC] */
/**
 * @brief Handles get Peer Devices.
 *
 * @return std::vector<uint16_t> Describe the return value.
 *
 * Algorithm:
 * - High-level steps the function performs.
 * - Mention important side-effects (WiFi, BLE, hardware).
 *
 * Loops:
 * - Describe each loop purpose and termination condition here.
 *
 * Complexity:
 * - Time: O(...)
 * - Space: O(...)
 */
        std::vector<uint16_t> getPeerDevices() const;
/* [AUTO-DOC] */
/**
 * @brief Handles get Peer Info.
 *
 * @param handle Describe this parameter.
 * @return NimBLEConnInfo Describe the return value.
 *
 * Algorithm:
 * - High-level steps the function performs.
 * - Mention important side-effects (WiFi, BLE, hardware).
 *
 * Loops:
 * - Describe each loop purpose and termination condition here.
 *
 * Complexity:
 * - Time: O(...)
 * - Space: O(...)
 */
        NimBLEConnInfo getPeerInfo(uint8_t handle);
/* [AUTO-DOC] */
/**
 * @brief Performs disconnect operations.
 *
 * @param connHandle Describe this parameter.
 * @param reason Describe this parameter.
 * @return bool Describe the return value.
 *
 * Algorithm:
 * - High-level steps the function performs.
 * - Mention important side-effects (WiFi, BLE, hardware).
 *
 * Loops:
 * - Describe each loop purpose and termination condition here.
 *
 * Complexity:
 * - Time: O(...)
 * - Space: O(...)
 */
        bool disconnect(uint16_t connHandle, uint8_t reason) const;

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

/* [AUTO-DOC] */
/**
 * @brief Performs locate Report Characteristic By Id And Type operations.
 *
 * @param reportId Describe this parameter.
 * @param reportType Describe this parameter.
 * @return NimBLECharacteristic * Describe the return value.
 *
 * Algorithm:
 * - High-level steps the function performs.
 * - Mention important side-effects (WiFi, BLE, hardware).
 *
 * Loops:
 * - Describe each loop purpose and termination condition here.
 *
 * Complexity:
 * - Time: O(...)
 * - Space: O(...)
 */
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

