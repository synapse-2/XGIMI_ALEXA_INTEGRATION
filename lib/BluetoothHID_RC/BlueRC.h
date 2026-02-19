/**
 * @file BlueRC.h
 * @brief Brief summary of this file.
 *
 * Detailed description of the file and its purpose.
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
/**
 * @brief Brief description of setDeviceAppreance.
 *
 * @param appearance Describe this parameter.
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
        void setDeviceAppreance(uint16_t appearance);
/**
 * @brief Brief description of sendButtonPress.
 *
 * @param command Describe this parameter.
 * @return virtual void Describe the return value.
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
        virtual void sendButtonPress(ServerDecoder::Remote_Cmd command);
/**
 * @brief Brief description of canHandleButtonPress.
 *
 * @param command Describe this parameter.
 * @return virtual bool Describe the return value.
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
        virtual bool canHandleButtonPress(ServerDecoder::Remote_Cmd command);
/**
 * @brief Brief description of startStandardAdv.
 *
 * @return virtual void Describe the return value.
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
        virtual void startStandardAdv();
/**
 * @brief Brief description of initStandardAdvData.
 *
 * @return virtual void Describe the return value.
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
        virtual void initStandardAdvData();
/**
 * @brief Brief description of startServices.
 *
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
        void startServices();
/**
 * @brief Brief description of BluetoothHID_RC.
 *
 * @return virtual ~ Describe the return value.
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
        virtual ~BluetoothHID_RC();
/**
 * @brief Brief description of setReportMap.
 *
 * @param param Describe this parameter.
 * @param uint16_t Describe this parameter.
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
        void setReportMap(uint8_t *map, uint16_t);
/**
 * @brief Brief description of setManufacturer.
 *
 * @param param Describe this parameter.
 * @return bool Describe the return value.
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
        bool setManufacturer(const std::string &name);
/**
 * @brief Brief description of setPnp.
 *
 * @param sig Describe this parameter.
 * @param vid Describe this parameter.
 * @param pid Describe this parameter.
 * @param version Describe this parameter.
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
        void setPnp(uint8_t sig, uint16_t vid, uint16_t pid, uint16_t version);
/**
 * @brief Brief description of setHidInfo.
 *
 * @param country Describe this parameter.
 * @param flags Describe this parameter.
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
        void setHidInfo(uint8_t country, uint8_t flags);
/**
 * @brief Brief description of setBatteryLevel.
 *
 * @param level Describe this parameter.
 * @param false Describe this parameter.
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
        void setBatteryLevel(uint8_t level, bool notify = false);
/**
 * @brief Brief description of getBatteryLevel.
 *
 * @return NimBLECharacteristic * Describe the return value.
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
        NimBLECharacteristic *getBatteryLevel();
/**
 * @brief Brief description of getReportMap.
 *
 * @return NimBLECharacteristic * Describe the return value.
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
        NimBLECharacteristic *getReportMap();
/**
 * @brief Brief description of getHidControl.
 *
 * @return NimBLECharacteristic * Describe the return value.
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
        NimBLECharacteristic *getHidControl();
/**
 * @brief Brief description of getInputReport.
 *
 * @param reportId Describe this parameter.
 * @return NimBLECharacteristic * Describe the return value.
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
        NimBLECharacteristic *getInputReport(uint8_t reportId);
/**
 * @brief Brief description of getOutputReport.
 *
 * @param reportId Describe this parameter.
 * @return NimBLECharacteristic * Describe the return value.
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
        NimBLECharacteristic *getOutputReport(uint8_t reportId);
/**
 * @brief Brief description of getFeatureReport.
 *
 * @param reportId Describe this parameter.
 * @return NimBLECharacteristic * Describe the return value.
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
        NimBLECharacteristic *getFeatureReport(uint8_t reportId);
/**
 * @brief Brief description of getProtocolMode.
 *
 * @return NimBLECharacteristic * Describe the return value.
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
        NimBLECharacteristic *getProtocolMode();
/**
 * @brief Brief description of getBootInput.
 *
 * @return NimBLECharacteristic * Describe the return value.
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
        NimBLECharacteristic *getBootInput();
/**
 * @brief Brief description of getBootOutput.
 *
 * @return NimBLECharacteristic * Describe the return value.
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
        NimBLECharacteristic *getBootOutput();
/**
 * @brief Brief description of getPnp.
 *
 * @return NimBLECharacteristic * Describe the return value.
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
        NimBLECharacteristic *getPnp();
/**
 * @brief Brief description of getHidInfo.
 *
 * @return NimBLECharacteristic * Describe the return value.
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
        NimBLECharacteristic *getHidInfo();
/**
 * @brief Brief description of getDeviceInfoService.
 *
 * @return NimBLEService * Describe the return value.
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
        NimBLEService *getDeviceInfoService();
/**
 * @brief Brief description of getHidService.
 *
 * @return NimBLEService * Describe the return value.
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
        NimBLEService *getHidService();
/**
 * @brief Brief description of getBatteryService.
 *
 * @return NimBLEService * Describe the return value.
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
        NimBLEService *getBatteryService();
/**
 * @brief Brief description of getConnectedCount.
 *
 * @return uint8_t Describe the return value.
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
        uint8_t getConnectedCount();
/**
 * @brief Brief description of getPeerDevices.
 *
 * @return std::vector<uint16_t> Describe the return value.
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
        std::vector<uint16_t> getPeerDevices() const;
/**
 * @brief Brief description of getPeerInfo.
 *
 * @param handle Describe this parameter.
 * @return NimBLEConnInfo Describe the return value.
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
        NimBLEConnInfo getPeerInfo(uint8_t handle);
/**
 * @brief Brief description of disconnect.
 *
 * @param connHandle Describe this parameter.
 * @param reason Describe this parameter.
 * @return bool Describe the return value.
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

/**
 * @brief Brief description of locateReportCharacteristicByIdAndType.
 *
 * @param reportId Describe this parameter.
 * @param reportType Describe this parameter.
 * @return NimBLECharacteristic * Describe the return value.
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

