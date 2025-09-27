#include "UtilityFunctions.h"
#include "BlueRC.h"
#include "services/gap/ble_svc_gap.h"
#include "magicEnum/magic_enum.hpp"
#include "NimBLEUtils.h"
#ifdef XGIMI_USE_EXT_ADV
#include <NimBLEExtAdvertising.h>
#endif

BlueRC::BluetoothHID_RC::BluetoothHID_RC(NimBLEServer *server)
{

  UtilityFunctions::debugLog("In Virtual HID startup!");

  BLE_server = server;
  BLE_server->setCallbacks(this, false);
  setDeviceAppreance(HID_REMOTE_APPEARANCE);
  advertising = server->getAdvertising();
  advertising->stop();

  // Here we create mandatory services described in bluetooth specification, order is important for the RC remote

  m_batterySvc = BLE_server->createService(batterySvcUuid);
  m_deviceInfoSvc = BLE_server->createService(deviceInfoSvcUuid);
  m_hidSvc = BLE_server->createService(hidSvcUuid);

  // Mandatory battery level characteristic with notification and presence descriptor
  m_batteryLevelChr =
      m_batterySvc->createCharacteristic(batteryLevelCharUuid, NIMBLE_PROPERTY::BLE_READ | NIMBLE_PROPERTY::NOTIFY);
  NimBLE2904 *batteryLevelDescriptor = m_batteryLevelChr->create2904();
  batteryLevelDescriptor->setFormat(NimBLE2904::FORMAT_UINT8);
  batteryLevelDescriptor->setUnit(0x27ad); // percentage

  // Mandatory characteristics for HID service
  m_hidInfoChr = m_hidSvc->createCharacteristic(hidInfoCharUuid, NIMBLE_PROPERTY::BLE_READ);
  m_reportMapChr = m_hidSvc->createCharacteristic(reportMapCharUuid, NIMBLE_PROPERTY::BLE_READ);
  m_hidControlChr = m_hidSvc->createCharacteristic(hidControlCharUuid, NIMBLE_PROPERTY::WRITE_NR);
  m_protocolModeChr =
      m_hidSvc->createCharacteristic(protocolModeCharUuid, NIMBLE_PROPERTY::WRITE_NR | NIMBLE_PROPERTY::BLE_READ);

  // This value is setup here because its default value in most usage cases, it's very rare to use boot mode
  m_protocolModeChr->setValue(static_cast<uint8_t>(0x01));
}

void BlueRC::BluetoothHID_RC::setDeviceAppreance(uint16_t appearance)
{

  int rc = ble_svc_gap_device_appearance_set(appearance);
  if (rc != 0)
  {
    UtilityFunctions::debugLogf("Failed to set device appearance, error code: %d", rc);
    return;
  }
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
    UtilityFunctions::debugLogf("State1 of advertising is %i\n", advertising->isAdvertising());
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
    startStandardAdv();
  }
}

/**
 * @brief Set the report map data formatting information.
 * @param [in] map A pointer to an array with the values to set.
 * @param [in] size The number of values in the array.
 */
void BlueRC::BluetoothHID_RC::setReportMap(uint8_t *map, uint16_t size)
{
  m_reportMapChr->setValue(map, size);
} // setReportMap

/**
 * @brief Start the HID device services.
 * This function called when all the services have been created.
 */
void BlueRC::BluetoothHID_RC::startServices()
{

  m_batterySvc->start();
  m_deviceInfoSvc->start();
  m_hidSvc->start();

} // startServices

/**
 * @brief Get the manufacturer characteristic (this characteristic is optional).
 * @details The characteristic will be created if not already existing.
 * @returns True if the name was set and/or the characteristic created.
 */
bool BlueRC::BluetoothHID_RC::setManufacturer(const std::string &name)
{
  if (m_manufacturerChr == nullptr)
  {
    m_manufacturerChr = m_deviceInfoSvc->createCharacteristic(m_manufacturerChrUuid, NIMBLE_PROPERTY::BLE_READ);
  }

  if (m_manufacturerChr)
  {
    m_manufacturerChr->setValue(name);
    return true;
  }

  return false;
} // setManufacturer

/**
 * @brief Sets the Plug n Play characteristic value.
 * @param [in] sig The vendor ID source number.
 * @param [in] vid The vendor ID number.
 * @param [in] pid The product ID number.
 * @param [in] version The produce version number.
 */
void BlueRC::BluetoothHID_RC::setPnp(uint8_t sig, uint16_t vid, uint16_t pid, uint16_t version)
{

  // Mandatory characteristic for device info service
  if (m_pnpChr == NULL)
  {
    m_pnpChr = m_deviceInfoSvc->createCharacteristic(pnpCharUuid, NIMBLE_PROPERTY::BLE_READ);
  }
  uint8_t pnp[] = {sig,
                   static_cast<uint8_t>(vid & 0xFF),
                   static_cast<uint8_t>((vid >> 8) & 0xFF),
                   static_cast<uint8_t>(pid & 0xFF),
                   static_cast<uint8_t>((pid >> 8) & 0xFF),
                   static_cast<uint8_t>(version & 0xFF),
                   static_cast<uint8_t>((version >> 8) & 0xFF)};

  m_pnpChr->setValue(pnp, sizeof(pnp));
} // setPnp

/**
 * @brief Sets the HID Information characteristic value.
 * @param [in] country The country code for the device.
 * @param [in] flags The HID Class Specification release number to use.
 */
void BlueRC::BluetoothHID_RC::setHidInfo(uint8_t country, uint8_t flags)
{

    // A typical value for the HID Information characteristic is a 4-byte sequence, as seen in firmware examples. The data can be broken down as follows: 
    // Byte  Offset    Size      Field	Description
    // 0-1	  2 bytes   (16 bits) HID Version (BCD)	The version number of the HID Specification, represented in Binary-Coded Decimal (BCD). A value of 0x0111 would mean version 1.11.
    // 2	    1 byte    (8 bits)	Country Code	Identifies the country of origin for the device's hardware. The value 0x00 is often used to indicate that no country code is supported.
    // 3      1 byte    (8 bits)	Flags	A bitfield indicating the device's capabilities. A common value, 0x03, is a bitmask of 0x01 | 0x02, which indicates the device supports "Normal Connect" and "Remote Wake".
  
  // XGIMI RC has code 00 00 00 01
uint8_t info[] = {0x00, 0x00, country, flags};
  m_hidInfoChr->setValue(info, sizeof(info));
} // setHidInfo

/**
 * @brief Set the battery level characteristic value.
 * @param [in] level The battery level value.
 * @param [in] notify If true sends a notification to the peer device, otherwise not. default = false
 */
void BlueRC::BluetoothHID_RC::setBatteryLevel(uint8_t level, bool notify)
{
  m_batteryLevelChr->setValue(&level, 1);
  if (notify)
  {
    m_batteryLevelChr->notify();
  }
} // setBatteryLevel

/**
 * @brief Locate the characteristic for a report ID and a report type.
 *
 * @param [in] reportId Report identifier to locate.
 * @param [in] reportType Type of report (input/output/feature).
 * @return NimBLECharacteristic* The characteristic.
 * @return nullptr If the characteristic does not exist.
 */
NimBLECharacteristic *BlueRC::BluetoothHID_RC::locateReportCharacteristicByIdAndType(uint8_t reportId, uint8_t reportType)
{
  NimBLECharacteristic *candidate = m_hidSvc->getCharacteristic(inputReportChrUuid, 0);
  for (uint16_t i = 1; (candidate != nullptr) && (i != 0); i++)
  {
    NimBLEDescriptor *dsc = candidate->getDescriptorByUUID(featureReportDscUuid);
    NimBLEAttValue desc1_val_att = dsc->getValue();
    const uint8_t *desc1_val = desc1_val_att.data();
    if ((desc1_val[0] == reportId) && (desc1_val[1] == reportType))
      return candidate;
    candidate = m_hidSvc->getCharacteristic(inputReportChrUuid, i);
  }
  return nullptr;
}

/**
 * @brief Get the input report characteristic.
 * @param [in] reportId Input report ID, the same as in report map for input object related to the characteristic.
 * @return NimBLECharacteristic* A pointer to the input report characteristic.
 *                               Store this value to avoid computational overhead.
 * @details This will create the characteristic if not already created.
 */
NimBLECharacteristic *BlueRC::BluetoothHID_RC::getInputReport(uint8_t reportId)
{
  NimBLECharacteristic *inputReportChr = locateReportCharacteristicByIdAndType(reportId, 0x01);
  if (inputReportChr == nullptr)
  {

    // update the input report to be writable also, the RC HID wants this to be true update to orignal nimble fork
    inputReportChr =
        m_hidSvc->createCharacteristic(inputReportChrUuid,
                                       NIMBLE_PROPERTY::BLE_READ | NIMBLE_PROPERTY::NOTIFY | NIMBLE_PROPERTY::READ_ENC | NIMBLE_PROPERTY::BLE_WRITE | NIMBLE_PROPERTY::WRITE_ENC);
    NimBLEDescriptor *inputReportDsc =
        inputReportChr->createDescriptor(featureReportDscUuid, NIMBLE_PROPERTY::BLE_READ | NIMBLE_PROPERTY::READ_ENC | NIMBLE_PROPERTY::BLE_WRITE | NIMBLE_PROPERTY::WRITE_ENC);

    uint8_t desc1_val[] = {reportId, 0x01};
    inputReportDsc->setValue(desc1_val, 2);
  }

  return inputReportChr;
} // getInputReport

/**
 * @brief Get the output report characteristic.
 * @param [in] reportId Output report ID, the same as in report map for output object related to the characteristic.
 * @return NimBLECharacteristic* A pointer to the output report characteristic.
 *                               Store this value to avoid computational overhead.
 * @details This will create the characteristic if not already created.
 */
NimBLECharacteristic *BlueRC::BluetoothHID_RC::getOutputReport(uint8_t reportId)
{
  NimBLECharacteristic *outputReportChr = locateReportCharacteristicByIdAndType(reportId, 0x02);
  if (outputReportChr == nullptr)
  {
    outputReportChr =
        m_hidSvc->createCharacteristic(inputReportChrUuid,
                                       NIMBLE_PROPERTY::BLE_READ | NIMBLE_PROPERTY::BLE_WRITE | NIMBLE_PROPERTY::WRITE_NR |
                                           NIMBLE_PROPERTY::READ_ENC | NIMBLE_PROPERTY::WRITE_ENC);
    NimBLEDescriptor *outputReportDsc = outputReportChr->createDescriptor(
        featureReportDscUuid,
        NIMBLE_PROPERTY::BLE_READ | NIMBLE_PROPERTY::BLE_WRITE | NIMBLE_PROPERTY::READ_ENC | NIMBLE_PROPERTY::WRITE_ENC);
    uint8_t desc1_val[] = {reportId, 0x02};
    outputReportDsc->setValue(desc1_val, 2);
  }

  return outputReportChr;
} // getOutputReport

/**
 * @brief Get the feature report characteristic.
 * @param [in] reportId Feature report ID, the same as in report map for feature object related to the characteristic.
 * @return NimBLECharacteristic* A pointer to feature report characteristic.
 *                               Store this value to avoid computational overhead.
 * @details This will create the characteristic if not already created.
 */
NimBLECharacteristic *BlueRC::BluetoothHID_RC::getFeatureReport(uint8_t reportId)
{
  NimBLECharacteristic *featureReportChr = locateReportCharacteristicByIdAndType(reportId, 0x03);
  if (featureReportChr == nullptr)
  {
    featureReportChr = m_hidSvc->createCharacteristic(
        inputReportChrUuid,
        NIMBLE_PROPERTY::BLE_READ | NIMBLE_PROPERTY::BLE_WRITE | NIMBLE_PROPERTY::READ_ENC | NIMBLE_PROPERTY::WRITE_ENC);
    NimBLEDescriptor *featureReportDsc = featureReportChr->createDescriptor(
        featureReportDscUuid,
        NIMBLE_PROPERTY::BLE_READ | NIMBLE_PROPERTY::BLE_WRITE | NIMBLE_PROPERTY::READ_ENC | NIMBLE_PROPERTY::WRITE_ENC);

    uint8_t desc1_val[] = {reportId, 0x03};
    featureReportDsc->setValue(desc1_val, 2);
  }

  return featureReportChr;
} // getFeatureReport

/**
 * @brief Get a keyboard boot input report characteristic.
 * @returns A pointer to the boot input report characteristic, or nullptr on error.
 * @details This will create the characteristic if not already created.
 */
NimBLECharacteristic *BlueRC::BluetoothHID_RC::getBootInput()
{
  NimBLECharacteristic *bootInputChr = m_hidSvc->getCharacteristic(bootInputChrUuid);
  if (bootInputChr)
  {
    return bootInputChr;
  }

  return m_hidSvc->createCharacteristic(bootInputChrUuid, NIMBLE_PROPERTY::NOTIFY);
} // getBootInput

/**
 * @brief Create a keyboard boot output report characteristic
 * @returns A pointer to the boot output report characteristic, or nullptr on error.
 * @details This will create the characteristic if not already created.
 */
NimBLECharacteristic *BlueRC::BluetoothHID_RC::getBootOutput()
{
  NimBLECharacteristic *bootOutputChr = m_hidSvc->getCharacteristic(bootOutputChrUuid);
  if (bootOutputChr)
  {
    return bootOutputChr;
  }

  return m_hidSvc->createCharacteristic(bootOutputChrUuid,
                                        NIMBLE_PROPERTY::BLE_READ | NIMBLE_PROPERTY::BLE_WRITE | NIMBLE_PROPERTY::WRITE_NR);
} // getBootOutput

/**
 * @brief Get the HID control point characteristic.
 * @returns A pointer to the HID control point characteristic.
 */
NimBLECharacteristic *BlueRC::BluetoothHID_RC::getHidControl()
{
  return m_hidControlChr;
} // getHidControl

/**
 * @brief Get the HID protocol mode characteristic.
 * @returns a pointer to the protocol mode characteristic.
 */
NimBLECharacteristic *BlueRC::BluetoothHID_RC::getProtocolMode()
{
  return m_protocolModeChr;
} // getProtocolMode

/**
 * @brief Get the battery level characteristic
 * @returns A pointer to the battery level characteristic
 */
NimBLECharacteristic *BlueRC::BluetoothHID_RC::getBatteryLevel()
{
  return m_batteryLevelChr;
} // getBatteryLevel

/**
 * @brief Get the report map characteristic.
 * @returns A pointer to the report map characteristic.
 */
NimBLECharacteristic *BlueRC::BluetoothHID_RC::getReportMap()
{
  return m_reportMapChr;
} // getReportMap

/**
 * @brief Get the PnP characteristic.
 * @returns A pointer to the PnP characteristic.
 */
NimBLECharacteristic *BlueRC::BluetoothHID_RC::getPnp()
{
  return m_pnpChr;
} // getPnp

/**
 * @brief Get the HID information characteristic.
 * @returns A pointer to the HID information characteristic.
 */
NimBLECharacteristic *BlueRC::BluetoothHID_RC::getHidInfo()
{
  return m_hidInfoChr;
} // hidInfo

/**
 * @brief Get the manufacturer characteristic.
 * @returns A pointer to the manufacturer characteristic.
 */
NimBLEService *BlueRC::BluetoothHID_RC::getDeviceInfoService()
{
  return m_deviceInfoSvc;
} // getDeviceInfoService

/**
 * @brief Get the HID service.
 * @returns A pointer to the HID service.
 */
NimBLEService *BlueRC::BluetoothHID_RC::getHidService()
{
  return m_hidSvc;
} // getHidService

/**
 * @brief Get the battery service.
 * @returns A pointer to the battery service.
 */
NimBLEService *BlueRC::BluetoothHID_RC::getBatteryService()
{
  return m_batterySvc;
} // getBatteryService

// the infamous entry in vtables :)
void BlueRC::BluetoothHID_RC::sendButtonPress(BlueRC::Remote_Cmd command) {}
bool BlueRC::BluetoothHID_RC::canHandleButtonPress(BlueRC::Remote_Cmd command) {}
void BlueRC::BluetoothHID_RC::startStandardAdv() {}
void BlueRC::BluetoothHID_RC::initStandardAdvData() {}
void BlueRC::BluetoothHID_RC::onRead(NimBLECharacteristic *pCharacteristic, NimBLEConnInfo &connInfo) {}
void BlueRC::BluetoothHID_RC::onWrite(NimBLECharacteristic *pCharacteristic, NimBLEConnInfo &connInfo) {}
void BlueRC::BluetoothHID_RC::onStatus(NimBLECharacteristic *pCharacteristic, int code) {}
void BlueRC::BluetoothHID_RC::onSubscribe(NimBLECharacteristic *pCharacteristic, NimBLEConnInfo &connInfo, uint16_t subValue) {}
