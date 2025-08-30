
#include <Arduino.h>
#include "UtilityFunctions.h"
#include "BlueXGIMI_RC.h"

void BlueXGIMI_RC::initOnButtonAdvData()
{

  // we are doing BLE  non extended 31 byte restricted advertisment

  // set up  the advertisment packet for on button
  advertisingOnButtonDataType1.setName(ONBTN_HID_DEVICE_SHORT_NAME);
  advertisingOnButtonDataType1.addServiceUUID(NimBLEUUID((uint16_t)0x1812));

  NimClassOfDeviceType::bluetooth_cod_t cod = NimClassOfDeviceType::encodeClassOfDevice(
      NimClassOfDeviceType::service_class_t::COD_SERVICE_NA,
      NimClassOfDeviceType::major_device_class_t::COD_MAJOR_PERIPHERAL,
      NimClassOfDeviceType::peripheral_pointing_device_t::COD_MINOR_PERIPHERAL_KBD_UNCATEGORIZED);

  advertisingOnButtonDataType1.setCODData(cod);
  advertisingOnButtonDataType1.setFlags(BLE_HS_ADV_F_BREDR_UNSUP | BLE_HS_ADV_F_DISC_LTD);
}

void BlueXGIMI_RC::initStandardAdvData()
{

  // we are doing BLE  non extended 31 byte restricted advertisment
  // advertisingData.setName("");
  // BLE_HS_ADV_F_DISC_LTD (0x01) - limited discoverability
  //  BLE_HS_ADV_F_DISC_GEN (0x02) - general discoverability
  // BLE_HS_ADV_F_BREDR_UNSUP (0x4)- BR/EDR not supported

  advertisingData.setFlags(BLE_HS_ADV_F_BREDR_UNSUP | BLE_HS_ADV_F_DISC_LTD);
  advertisingData.addServiceUUID(NimBLEUUID((uint16_t)0x1812));
  advertisingData.setAppearance(HID_REMOTE);
  advertisingData.setManufacturerData(HID_AD_MANUF_DATA);
  // advertisingData.addTxPower(); // this is automatically added as the last byte

  // we create the sacan data that has name etc in there wth extnded manuf data
  advertisingScanData = NimBLEAdvertisementData();
  advertisingScanData.setName(HID_DEVICE_NAME);
  advertisingScanData.setManufacturerData(HID_AD_SACAN_MANUF_DATA);
  // scanAdvData.addTxPower(); // this is automatically added as the last byte
}

BlueXGIMI_RC::BlueXGIMI_RC(BLEServer *server) : BluetoothHID_RC(server)
{

  UtilityFunctions::debugLog("In XIGIMI RC HID startup!");

  // link the report ID's in the HID infopacket
  keyboardInput_01 = getInputReport(KEYBOARD_REPORT_ID);
  keyboardOutput_01 = getOutputReport(KEYBOARD_REPORT_ID);
  secondDeviceInput_30 = getInputReport(SECOND_REPORT_ID);
  secondDeviceOutput_30 = getOutputReport(SECOND_REPORT_ID);
  thirdDeviceInput_03 = getInputReport(THIRD_REPORT_ID);

  keyboardInput_01->setCallbacks(new Callback_handler_Rep_Inp_01(this));
  keyboardOutput_01->setCallbacks(new Callback_handler_Rep_Out_01(this));
  secondDeviceInput_30->setCallbacks(new Callback_handler_Rep_Inp_30(this));
  secondDeviceOutput_30->setCallbacks(new Callback_handler_Rep_Out_30(this));
  thirdDeviceInput_03->setCallbacks(new Callback_handler_Rep_Inp_03(this));

  UtilityFunctions::debugLog("In XIGIMI RC HID startup callback registered level 1");

  // format is not shown in xigimi rc remote so overridethe descriptor set by the BLE HID Class
  // BLEDescriptor *batteryLevelDescriptor = new BLEDescriptor(BLEUUID((uint16_t)0x2904));
  // batteryService()->getCharacteristic((uint16_t)0x2a19)->addDescriptor(batteryLevelDescriptor); // set battlevel cchar desc to null format

  /*
  B) Device information  service UUID 0x180A - Primary has
  1) uuid 0x2a29 - manufacturer string , read only, value "Realtek BT" (simple one byte text ) (ALREADY Done by BLEHIDevice class)
  2) uuid 0x2a24 - Model number string, read only, value "Model nbr 0.9" (simple one byte text )
  3) uuid 0x2a25 - Serial number string, read only, value "RTKBeeSerialNum" (simple one byte text )
  4) uuid 0x2a27 - hardware revision string, read only, value  "V1.0.0" (simple one byte text )
  5) uuid 0x2a26 - firmware revision string, read only, value "B981C_HG2_CY_V1.0.9" (simple one byte text )
  6) uuid 0x2a28 - software revision string, read only, value "B981C_HG2_CY_V1.0.9" (simple one byte text )
  7) uuid 0x2a23 - system ID, readonly, value "0x0001020000030405"
  8) uuid 0x2a2a - regulatory cretificate, read only, value "RTKBeeIEEDatalist"
  9) uuid 0x2a50 - pnp id, red only,  value "0x010D0038380300" (note big endian representation)
  */

  m_modelNumberCharacteristic = getDeviceInfoService()->createCharacteristic((uint16_t)0x2a24, NIMBLE_PROPERTY::BLE_READ);         // uuid 0x2a24
  m_SerialNumberCharacteristic = getDeviceInfoService()->createCharacteristic((uint16_t)0x2a25, NIMBLE_PROPERTY::BLE_READ);        // uuid 0x2a25
  m_hardwarRevisionStrCharacteristic = getDeviceInfoService()->createCharacteristic((uint16_t)0x2a27, NIMBLE_PROPERTY::BLE_READ);  // uuid 0x2a27
  m_firmwareRevisionStrCharacteristic = getDeviceInfoService()->createCharacteristic((uint16_t)0x2a26, NIMBLE_PROPERTY::BLE_READ); // uuid 0x2a26
  m_softwareRevisionCharacteristic = getDeviceInfoService()->createCharacteristic((uint16_t)0x2a28, NIMBLE_PROPERTY::BLE_READ);    // uuid 0x2a28
  m_systemIDCharacteristic = getDeviceInfoService()->createCharacteristic((uint16_t)0x2a23, NIMBLE_PROPERTY::BLE_READ);            // uuid 0x2a23
  m_IEE11073CertCharacteristic = getDeviceInfoService()->createCharacteristic((uint16_t)0x2a2a, NIMBLE_PROPERTY::BLE_READ);        // uuid 0x2a2a

  UtilityFunctions::debugLog("In XIGIMI RC HID startup callback registered level 1 chars creaetd");

  m_modelNumberCharacteristic->setValue(HID_MODEL_NUMBER);
  m_SerialNumberCharacteristic->setValue(HID_SERIAL_NUMBER);
  m_hardwarRevisionStrCharacteristic->setValue(HID_HARDWARE_REVISION);
  m_firmwareRevisionStrCharacteristic->setValue(HID_FIRMWARE_REVISION);
  m_softwareRevisionCharacteristic->setValue(HID_SOFTWARE_REVISION);
  m_systemIDCharacteristic->setValue(HID_SYSTEM_ID, sizeof(HID_SYSTEM_ID));
  m_IEE11073CertCharacteristic->setValue(HID_IEE11073_CERT);

  // set the manufacturer name

  setManufacturer(std::string(HID_MANUFACTURER_NAME).substr(0, 15));

  // set the devide PNP information
  setPnp(VENDORID_SIG, VENDORID_MANUF, PRODUCTID, VERSION);

  // set the HID countrt and flags information
  setHidInfo(HID_COUNTRY, HID_FLAGS);
  // set the battery level characteristic

  UtilityFunctions::debugLog("In XIGIMI RC HID startup info set level 2");

  /* C) Customm service uuid 0000D1FF-3C17-D293-8E48-14FE2E4DA212 primary has
     1) uuid 0xa001, custom characteristic, write, no response, notify,
      descriptor (typ) for above, uuid 0x2902 - client characteristics config, readonly
     2) uuid 0xa002, custom charasteristic, write no response
     */
  m_CustomService1 = server->createService(BLEUUID("0000D1FF-3C17-D293-8E48-14FE2E4DA212")); // uuid 0000D1FF-3C17-D293-8E48-14FE2E4DA212

  if (m_CustomService1 == NULL)
  {

    UtilityFunctions::debugLog("In XIGIMI RC HID startup info set level 2 after unable to create service 1");
  }

  m_0xA001Characteristic = m_CustomService1->createCharacteristic((uint16_t)0xA001, NIMBLE_PROPERTY::WRITE_NR | NIMBLE_PROPERTY::NOTIFY); // uuid 0xa001,
  m_0xA002Characteristic = m_CustomService1->createCharacteristic((uint16_t)0xA002, NIMBLE_PROPERTY::WRITE_NR);                           // uuid 0xa002,

  // BLE2902 *m_0xA001CharacteristicDescriptor = new BLE2902();

  // m_0xA001Characteristic->addDescriptor(m_0xA001CharacteristicDescriptor);
  m_0xA001Characteristic->setCallbacks(this);
  m_0xA002Characteristic->setCallbacks(this);

  UtilityFunctions::debugLog("In XIGIMI RC HID startup callback setup level 3");

  /* D) Customm service uuid 0000D0FF-3C17-D293-8E48-14FE2E4DA212 primary has
     1) uuid 0xffd1, custom characteristic, write, no response,
     2) uuid 0xffd2, custom characteristic, read (value 0x1cf302a896dc)
     3) uuid 0xffd3, custom characteristic, read (value 0x1a01028)
     4) uuid 0xffd4, custom characteristic, read (value 0x11000000)
     5) uuid 0xffd5, custom characteristic, read
     6) uuid 0xffd8, custom characteristic, write no response
     7) uuid 0xffF1, custom characteristic, read (value 0501 0007 0008 0000 f00 f00 000)
     8) uuid 0xffF2, custom characteristic, write
     9) uuid 0xffE0, custom characteristic, read (value 0x000000 0101 0000000 01a00102811000000)
     */
  m_CustomService2 = server->createService(BLEUUID("0000D0FF-3C17-D293-8E48-14FE2E4DA212")); // uuid 0000D0FF-3C17-D293-8E48-14FE2E4DA21

  m_0xFFD1Characteristic = m_CustomService2->createCharacteristic((uint16_t)0xFFD1, NIMBLE_PROPERTY::WRITE_NR);  // uuid 0xFFD1
  m_0xFFD2Characteristic = m_CustomService2->createCharacteristic((uint16_t)0xFFD2, NIMBLE_PROPERTY::BLE_READ);  // uuid 0xFFD2
  m_0xFFD3Characteristic = m_CustomService2->createCharacteristic((uint16_t)0xFFD3, NIMBLE_PROPERTY::BLE_READ);  // uuid 0xFFD
  m_0xFFD4Characteristic = m_CustomService2->createCharacteristic((uint16_t)0xFFD4, NIMBLE_PROPERTY::BLE_READ);  // uuid 0xFFD4
  m_0xFFD5Characteristic = m_CustomService2->createCharacteristic((uint16_t)0xFFD5, NIMBLE_PROPERTY::BLE_READ);  // uuid 0xFFD5
  m_0xFFD8Characteristic = m_CustomService2->createCharacteristic((uint16_t)0xFFD8, NIMBLE_PROPERTY::WRITE_NR);  // uuid 0xFFD8
  m_0xFFF1Characteristic = m_CustomService2->createCharacteristic((uint16_t)0xFFF1, NIMBLE_PROPERTY::BLE_READ);  // uuid 0xFFF1
  m_0xFFF2Characteristic = m_CustomService2->createCharacteristic((uint16_t)0xFFF2, NIMBLE_PROPERTY::BLE_WRITE); // uuid 0xFFF2
  m_0xFFE0Characteristic = m_CustomService2->createCharacteristic((uint16_t)0xFFE0, NIMBLE_PROPERTY::BLE_READ);  // uuid 0xFFE0

  m_0xFFD1Characteristic->setCallbacks(this);
  m_0xFFD2Characteristic->setValue(HID_uuid_0xffd2, sizeof(HID_uuid_0xffd2));
  m_0xFFD3Characteristic->setValue(HID_uuid_0xffd3, sizeof(HID_uuid_0xffd3));
  m_0xFFD4Characteristic->setValue(HID_uuid_0xffd4, sizeof(HID_uuid_0xffd4));
  m_0xFFD8Characteristic->setCallbacks(this);
  m_0xFFF1Characteristic->setValue(HID_uuid_0xfff1, sizeof(HID_uuid_0xfff1));
  m_0xFFF2Characteristic->setCallbacks(this);
  m_0xFFE0Characteristic->setValue(HID_uuid_0xffe0, sizeof(HID_uuid_0xffe0));

  /* E) Customm service uuid 00006287-3C17-D293-8E48-14FE2E4DA212 primary has
    1) uuid 00006387-3C17-D293-8E48-14FE2E4DA212, write no response
    2) uuid 00006487-3C17-D293-8E48-14FE2E4DA212, write, nitify
        descriptor (typ) for above, uuid 0x2902 - client characteristics config, readonly
    */
  m_CustomService3 = server->createService(BLEUUID("00006287-3C17-D293-8E48-14FE2E4DA212")); // uuid 00006287-3C17-D293-8E48-14FE2E4DA212

  m_0x6287Characteristic = m_CustomService3->createCharacteristic("00006387-3C17-D293-8E48-14FE2E4DA212", NIMBLE_PROPERTY::BLE_WRITE | NIMBLE_PROPERTY::NOTIFY); // uuid 00006387-3C17-D293-8E48-14FE2E4DA212

  UtilityFunctions::debugLog("In XIGIMI RC HID startup callback setup level 4");
  BLE2904 *m_0x6287CharacteristicDescriptor = new BLE2904();
  m_0xA001Characteristic->addDescriptor(m_0x6287CharacteristicDescriptor);
  m_0x6287Characteristic->setCallbacks(this);

  // set the report map
  setReportMap((uint8_t *)HID_REPORT_DESCRIPTOR, sizeof(HID_REPORT_DESCRIPTOR));
  UtilityFunctions::debugLogf("Set HID report map %s\n", HEXBuilder::bytes2hex((uint8_t *)HID_REPORT_DESCRIPTOR, sizeof(HID_REPORT_DESCRIPTOR)).c_str());

  UtilityFunctions::debugLog("IN XGIMI HIG Security startup!");

  // BLE_SM_PAIR_AUTHREQ_BOND  - bond
  // BLE_SM_PAIR_AUTHREQ_MITM  - man in middle
  // BLE_SM_PAIR_AUTHREQ_SC - secure connection
  NimBLEDevice::setSecurityAuth(BLE_SM_PAIR_AUTHREQ_BOND);
  NimBLEDevice::setSecurityIOCap(BLE_HS_IO_NO_INPUT_OUTPUT);
  // BLE_SM_PAIR_KEY_DIST_ENC LTK key
  // BLE_SM_PAIR_KEY_DIST_ID  IRK key
  // BLE_SM_PAIR_KEY_DIST_SIGN CRSK key
  NimBLEDevice::setSecurityInitKey(BLE_SM_PAIR_KEY_DIST_ENC | BLE_SM_PAIR_KEY_DIST_ID | BLE_SM_PAIR_KEY_DIST_SIGN);
  NimBLEDevice::setSecurityRespKey(BLE_SM_PAIR_KEY_DIST_ID | BLE_SM_PAIR_KEY_DIST_SIGN);

  UtilityFunctions::debugLog("Starting HID services");
  startServices(); // HID start service - must be called after all charstics have been built

  UtilityFunctions::debugLog("Setting up Advertising");

#ifndef CONFIG_BT_NIMBLE_EXT_ADV

  initStandardAdvData();
  initOnButtonAdvData();

  advertising->setAdvertisementData(advertisingData);
  advertising->setScanResponseData(advertisingScanData);

  /**
   * * BLE_GAP_CONN_MODE_NON    (0) - not connectable advertising
   * * BLE_GAP_CONN_MODE_DIR    (1) - directed connectable advertising
   * * BLE_GAP_CONN_MODE_UND    (2) - undirected connectable advertising
   */
  advertising->setConnectableMode(BLE_GAP_CONN_MODE_UND);
  advertising->enableScanResponse(true);
  advertising->start();
#else
  // we are doing BLE 5.0 non extended  byte  advertisment
  NimBLEExtAdvertising *advertising = server->getAdvertising();
  NimBLEExtAdvertisement advertisingData = NimBLEExtAdvertisement();

  advertisingData.setName(HID_DEVICE_NAME);
  advertisingData.setAppearance(HID_REMOTE);
  advertisingData.setManufacturerData(HID_AD_MANUF_DATA);
  advertisingData.setPartialServices(getHidService()->getUUID());

  /** Set the advertisement as connectable */
  advertisingData.setConnectable(true);

  /** As per Bluetooth specification, extended advertising cannot be both scannable and connectable */
  advertisingData.setScannable(false); // The default is false, set here for demonstration.
  // advertisingData.setShortName(HID_DEVICE_NAME);
  // advertisingData.addTxPower();

  advertising->setInstanceData(0, advertisingData);

  advertising->start(0);
#endif

  uint8_t batteryLevel = 100;    // battery lvel can only be set AFTER advitsment and server start
  setBatteryLevel(batteryLevel); // set initial battery level to 100%

  UtilityFunctions::debugLog("Standard Advertising started!");
}

void BlueXGIMI_RC::startServices()
{
  BLEHIDDevice::startServices();
  m_CustomService1->start();
  m_CustomService2->start();
  m_CustomService3->start();
}

void BlueXGIMI_RC::sendButtonPress(uint8_t command)
{
}

// generic handlers
void BlueXGIMI_RC::onWrite(NimBLECharacteristic *pCharacteristic, NimBLEConnInfo &connInfo)
{

  UtilityFunctions::debugLogf("UUID ");
  UtilityFunctions::debugLog(pCharacteristic->getUUID().toString().c_str());
  UtilityFunctions::debugLogf("Data length %i\n", pCharacteristic->getLength());

  NimBLEAttValue val = pCharacteristic->getValue();
  String value = HEXBuilder::bytes2hex(val.data(), val.length());
  UtilityFunctions::debugLogf("Data received HEX %s\n", value);
  UtilityFunctions::debugLogf("Data received RAW %s\n", pCharacteristic->getValue());
}

void BlueXGIMI_RC::onRead(NimBLECharacteristic *pCharacteristic, NimBLEConnInfo &connInfo)
{
  UtilityFunctions::debugLogf("UUID ");
  UtilityFunctions::debugLog(pCharacteristic->getUUID().toString().c_str());
  UtilityFunctions::debugLogf("Data length %i\n", pCharacteristic->getLength());

  NimBLEAttValue val = pCharacteristic->getValue();
  String value = HEXBuilder::bytes2hex(val.data(), val.length());
  UtilityFunctions::debugLogf("Data SENT HEX %s\n", value);
  UtilityFunctions::debugLogf("Data SENT RAW %s\n", pCharacteristic->getValue());
}

void BlueXGIMI_RC::onStatus(NimBLECharacteristic *pCharacteristic, int code)
{
  UtilityFunctions::debugLogf("UUID ");
  UtilityFunctions::debugLog(pCharacteristic->getUUID().toString().c_str());
  UtilityFunctions::debugLogf("Data length %i\n", pCharacteristic->getLength());

  UtilityFunctions::debugLogf("CODE RAW %i\n", code);
  UtilityFunctions::debugLogf("CODE HEX %02x\n", code);
}
void BlueXGIMI_RC::onSubscribe(NimBLECharacteristic *pCharacteristic, NimBLEConnInfo &connInfo, uint16_t subValue)
{

  UtilityFunctions::debugLogf("UUID ");
  UtilityFunctions::debugLog(pCharacteristic->getUUID().toString().c_str());
  UtilityFunctions::debugLogf("Data length %i\n", pCharacteristic->getLength());

  UtilityFunctions::debugLogf("SUBValue RAW %i\n", subValue);
  UtilityFunctions::debugLogf("SUBValue HEX %02x\n", subValue);
}

void BlueXGIMI_RC::onConnect(NimBLEServer *pServer, NimBLEConnInfo &connInfo)
{
  BluetoothHID_RC::onConnect(pServer, connInfo);
}

void BlueXGIMI_RC::onDisconnect(NimBLEServer *pServer, NimBLEConnInfo &connInfo, int reason)
{
  BluetoothHID_RC::onDisconnect(pServer, connInfo, reason);
}

BlueXGIMI_RC::~BlueXGIMI_RC() {}

// report id call backs for Report#01 Input i.e bytes sent from esp32 to the projector/computer
//  BLECharacteristicCallbacks
void BlueXGIMI_RC::ReportInput_01_onRead(NimBLECharacteristic *pCharacteristic, NimBLEConnInfo &connInfo)
{
  UtilityFunctions::debugLog(" IN ReportInput_01_onRead - passing to default handler");
  onRead(pCharacteristic, connInfo);
}
void BlueXGIMI_RC::ReportInput_01_onWrite(NimBLECharacteristic *pCharacteristic, NimBLEConnInfo &connInfo)
{
  UtilityFunctions::debugLog(" IN ReportInput_01_onWrite - passing to default handler");
  onWrite(pCharacteristic, connInfo);
}
void BlueXGIMI_RC::ReportInput_01_onStatus(NimBLECharacteristic *pCharacteristic, int code)
{
  UtilityFunctions::debugLog(" IN ReportInput_01_onStatus - passing to default handler");
  onStatus(pCharacteristic, code);
}

void BlueXGIMI_RC::ReportInput_01_onSubscribe(NimBLECharacteristic *pCharacteristic, NimBLEConnInfo &connInfo, uint16_t subValue)
{
  UtilityFunctions::debugLog(" IN ReportInput_01_onSubscribe - passing to default handler");
  onSubscribe(pCharacteristic, connInfo, subValue);
}

// report id call backs for Report#01 Output i.e bytes sent from projector/computer to theesp32
//  BLECharacteristicCallbacks
void BlueXGIMI_RC::ReportOutput_01_onRead(NimBLECharacteristic *pCharacteristic, NimBLEConnInfo &connInfo)
{
  UtilityFunctions::debugLog(" IN ReportOutput_01_onRead - passing to default handler");
  onRead(pCharacteristic, connInfo);
}
void BlueXGIMI_RC::ReportOutput_01_onWrite(NimBLECharacteristic *pCharacteristic, NimBLEConnInfo &connInfo)
{
  UtilityFunctions::debugLog(" IN ReportOutput_01_onWrite - passing to default handler");
  onWrite(pCharacteristic, connInfo);
}
void BlueXGIMI_RC::ReportOutput_01_onStatus(NimBLECharacteristic *pCharacteristic, int code)
{
  UtilityFunctions::debugLog(" IN ReportOutput_01_onStatus - passing to default handler");
  onStatus(pCharacteristic, code);
}
void BlueXGIMI_RC::ReportOutput_01_onSubscribe(NimBLECharacteristic *pCharacteristic, NimBLEConnInfo &connInfo, uint16_t subValue)
{
  UtilityFunctions::debugLog(" IN ReportOutput_01_onSubscribe - passing to default handler");
  onSubscribe(pCharacteristic, connInfo, subValue);
}

// report id call backs for Report#30 Input i.e bytes sent from esp32 to the projector/computer
// BLECharacteristicCallbacks
void BlueXGIMI_RC::ReportInput_30_onRead(NimBLECharacteristic *pCharacteristic, NimBLEConnInfo &connInfo)
{
  UtilityFunctions::debugLog(" IN ReportInput_30_onRead - passing to default handler");
  onRead(pCharacteristic, connInfo);
}
void BlueXGIMI_RC::ReportInput_30_onWrite(NimBLECharacteristic *pCharacteristic, NimBLEConnInfo &connInfo)
{
  UtilityFunctions::debugLog(" IN ReportInput_30_onWrite - passing to default handler");
  onWrite(pCharacteristic, connInfo);
}
void BlueXGIMI_RC::ReportInput_30_onStatus(NimBLECharacteristic *pCharacteristic, int code)
{
  UtilityFunctions::debugLog(" IN ReportInput_30_onStatus - passing to default handler");
  onStatus(pCharacteristic, code);
}
void BlueXGIMI_RC::ReportInput_30_onSubscribe(NimBLECharacteristic *pCharacteristic, NimBLEConnInfo &connInfo, uint16_t subValue)
{
  UtilityFunctions::debugLog(" IN ReportInput_30_onSubscribe - passing to default handler");
  onSubscribe(pCharacteristic, connInfo, subValue);
}

// report id call backs for Report#30 Output i.e bytes sent from projector/computer to theesp32
//  BLECharacteristicCallbacks
void BlueXGIMI_RC::ReportOutput_30_onRead(NimBLECharacteristic *pCharacteristic, NimBLEConnInfo &connInfo)
{
  UtilityFunctions::debugLog(" IN ReportOutput_30_onRead - passing to default handler");
  onRead(pCharacteristic, connInfo);
}
void BlueXGIMI_RC::ReportOutput_30_onWrite(NimBLECharacteristic *pCharacteristic, NimBLEConnInfo &connInfo)
{
  UtilityFunctions::debugLog(" IN ReportOutput_30_onWrite - passing to default handler");
  onWrite(pCharacteristic, connInfo);
}
void BlueXGIMI_RC::ReportOutput_30_onStatus(NimBLECharacteristic *pCharacteristic, int code)
{
  UtilityFunctions::debugLog(" IN ReportOutput_30_onStatus - passing to default handler");
  onStatus(pCharacteristic, code);
}
void BlueXGIMI_RC::ReportOutput_30_onSubscribe(NimBLECharacteristic *pCharacteristic, NimBLEConnInfo &connInfo, uint16_t subValue)
{
  UtilityFunctions::debugLog(" IN ReportOutput_30_onSubscribe - passing to default handler");
  onSubscribe(pCharacteristic, connInfo, subValue);
}

// report id call backs for Report#03 Input i.e bytes sent from esp32 to the projector/computer
// BLECharacteristicCallbacks
void BlueXGIMI_RC::ReportInput_03_onRead(NimBLECharacteristic *pCharacteristic, NimBLEConnInfo &connInfo)
{
  UtilityFunctions::debugLog(" IN ReportInput_03_onRead - passing to default handler");
  onRead(pCharacteristic, connInfo);
}
void BlueXGIMI_RC::ReportInput_03_onWrite(NimBLECharacteristic *pCharacteristic, NimBLEConnInfo &connInfo)
{
  UtilityFunctions::debugLog(" IN ReportInput_03_onWrite - passing to default handler");
  onWrite(pCharacteristic, connInfo);
}
void BlueXGIMI_RC::ReportInput_03_onStatus(NimBLECharacteristic *pCharacteristic, int code)
{
  UtilityFunctions::debugLog(" IN ReportInput_03_onStatus - passing to default handler");
  onStatus(pCharacteristic, code);
}
void BlueXGIMI_RC::ReportInput_03_onSubscribe(NimBLECharacteristic *pCharacteristic, NimBLEConnInfo &connInfo, uint16_t subValue)
{
  UtilityFunctions::debugLog(" IN ReportInput_03_onSubscribe - passing to default handler");
  onSubscribe(pCharacteristic, connInfo, subValue);
}