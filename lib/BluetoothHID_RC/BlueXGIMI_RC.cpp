
#include <Arduino.h>
#include "UtilityFunctions.h"
#include "BlueRC.h"
#include "BlueXGIMI_RC.h"
#include "magicEnum/magic_enum.hpp"
#include "NimClassOfDeviceType.h"
#include "NimBLEUtils.h"
#ifdef XGIMI_USE_EXT_ADV
#include <NimBLEExtAdvertising.h>
#endif

void BlueXGIMI_RC::initOnButtonAdvData()
{

  // we are doing BLE  non extended 31 byte restricted advertisment

  advertisingOnButtonDataType1.setPrimaryPhy(BLE_HCI_LE_PHY_1M);
  advertisingOnButtonDataType1.setSecondaryPhy(0);
  advertisingOnButtonDataType1.setConnectable(true);
  advertisingOnButtonDataType1.setScannable(true);
  advertisingOnButtonDataType1.setLegacyAdvertising(true);

  // in the right oder to match the original xigimi rc packet
  advertisingOnButtonDataType1.setFlags(BLE_HS_ADV_F_BREDR_UNSUP | BLE_HS_ADV_F_DISC_LTD);
  // set up  the advertisment packet for on button

  advertisingOnButtonDataType1.addServiceUUID(NimBLEUUID((uint16_t)0x1812));

  advertisingOnButtonDataType1.setAppearance(HID_REMOTE_APPEARANCE);
  advertisingOnButtonDataScanDataType1.setAppearance(HID_REMOTE_APPEARANCE);

  // set up  the advertisment packet for on button
  advertisingOnButtonDataType2.setPrimaryPhy(BLE_HCI_LE_PHY_1M);
  advertisingOnButtonDataType2.setSecondaryPhy(0);
  advertisingOnButtonDataType2.setConnectable(true);
  advertisingOnButtonDataType2.setScannable(true);
  advertisingOnButtonDataType2.setLegacyAdvertising(true);

  advertisingOnButtonDataType2.setFlags(BLE_HS_ADV_F_BREDR_UNSUP | BLE_HS_ADV_F_DISC_LTD);
  advertisingOnButtonDataType2.addServiceUUID(NimBLEUUID((uint16_t)0x1812));
  advertisingOnButtonDataType2.setAppearance(HID_KEYBORAD_APPEARANCE);
  advertisingOnButtonDataType2.setManufacturerData(HID_AD2_MANUF_DATA);

  advertisingOnButtonDataScanDataType2.setName(std::string(UtilityFunctions::loadBlueToothName().c_str()));
  advertisingOnButtonDataScanDataType2.setManufacturerData(HID_AD_SCAN_MANUF_DATA);

  // set up  the advertisment packet for on button
  advertisingOnButtonDataType3.setPrimaryPhy(BLE_HCI_LE_PHY_1M);
  advertisingOnButtonDataType3.setSecondaryPhy(0);
  advertisingOnButtonDataType3.setConnectable(true);
  advertisingOnButtonDataType3.setScannable(true);
  advertisingOnButtonDataType3.setLegacyAdvertising(true);

  advertisingOnButtonDataType3.setFlags(BLE_HS_ADV_F_BREDR_UNSUP | BLE_HS_ADV_F_DISC_LTD);
  advertisingOnButtonDataType3.setName(ONBTN_HID_DEVICE_SHORT_NAME, false);
  advertisingOnButtonDataType3.addServiceUUID(NimBLEUUID((uint16_t)0x1812));

  NimClassOfDeviceType::bluetooth_cod_t cod = NimClassOfDeviceType::encodeClassOfDevice(
      NimClassOfDeviceType::service_class_t::COD_SERVICE_NA,
      NimClassOfDeviceType::major_device_class_t::COD_MAJOR_PERIPHERAL,
      NimClassOfDeviceType::peripheral_pointing_device_t::COD_MINOR_PERIPHERAL_KBD_UNCATEGORIZED);

  advertisingOnButtonDataType3.setCODData(cod);

  advertisingOnButtonDataScanDataType3.setName(std::string(UtilityFunctions::loadBlueToothName().c_str()));
  advertisingOnButtonDataScanDataType3.setManufacturerData(HID_AD_SCAN_MANUF_DATA);

#ifdef XGIMI_USE_EXT_ADV
  advertisingOnButtonDataType1.setConnectable(true);
  advertisingOnButtonDataType1.setScannable(true);
  advertisingOnButtonDataType1.setLegacyAdvertising(true);
  advertisingOnButtonDataType1.setAnonymous(false);

  advertisingOnButtonDataType2.setConnectable(true);
  advertisingOnButtonDataType2.setScannable(true);
  advertisingOnButtonDataType2.setLegacyAdvertising(true);
  advertisingOnButtonDataType2.setAnonymous(false);

#endif
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
  advertisingData.setAppearance(HID_REMOTE_APPEARANCE);
  advertisingData.setManufacturerData(HID_AD_MANUF_DATA);
  // advertisingData.addTxPower(); // this is automatically added as the last byte

  // we create the sacan data that has name etc in there wth extnded manuf data
  // advertisingScanData = NimBLEAdvertisementData();
  advertisingScanData.setName(std::string(UtilityFunctions::loadBlueToothName().c_str()));
  advertisingScanData.setManufacturerData(HID_AD_SCAN_MANUF_DATA);
  // scanAdvData.addTxPower(); // this is automatically added as the last byte
}

void BlueXGIMI_RC::startStandardAdv()
{

#ifndef XGIMI_USE_EXT_ADV

  advertising->setAdvertisementData(advertisingData);
  advertising->setScanResponseData(advertisingScanData);

  /**
   * * BLE_GAP_CONN_MODE_NON    (0) - not connectable advertising
   * * BLE_GAP_CONN_MODE_DIR    (1) - directed connectable advertising
   * * BLE_GAP_CONN_MODE_UND    (2) - undirected connectable advertising
   */
  advertising->setConnectableMode(BLE_GAP_CONN_MODE_UND);
  advertising->enableScanResponse(true);
  advertising->setMinInterval(0);
  advertising->setMaxInterval(0);
  advertising->start();

#else
  // we are doing BLE 5.0 non extended  byte  advertisment

  advertisingData.setPrimaryPhy(BLE_HCI_LE_PHY_1M);
  advertisingData.setSecondaryPhy(BLE_HCI_LE_PHY_1M);
  /** Set the advertisement as connectable */
  advertisingData.setConnectable(true);
  advertisingData.setScannable(true);
  advertisingData.setLegacyAdvertising(true);
  // advertisingData.setAnonymous(true);

  // advertisingData.setShortName(HID_DEVICE_NAME);
  // advertisingData.addTxPower();
  advertisingScanData.setPrimaryPhy(BLE_HCI_LE_PHY_1M);
  advertisingScanData.setSecondaryPhy(BLE_HCI_LE_PHY_1M);
  advertisingScanData.setConnectable(true);
  advertisingScanData.setScannable(false);
  // advertisingScanData.setLegacyAdvertising(true);

  // UtilityFunctions::debugLogf("STD ADV packet data %s\n", advertisingData.toString().c_str());

  UtilityFunctions::debugLog("STD ADV before instnce set");
  advertising->setInstanceData(HID_ADV_STD_ID, advertisingData);

  UtilityFunctions::debugLog("STD ADV After instnce set");
  advertising->setScanResponseData(HID_ADV_STD_ID, advertisingScanData);

  UtilityFunctions::debugLog("STD ADV after scan data set");
  if (advertising->start(HID_ADV_STD_ID))
  {
    UtilityFunctions::debugLog("ADV MAIN STARTED");
  }
  else
  {

    UtilityFunctions::debugLog("ADV MAIN DID NOT START");
  }

#endif
}

BlueXGIMI_RC::BlueXGIMI_RC(BLEServer *server) : BluetoothHID_RC(server)
{

  UtilityFunctions::debugLog("In XIGIMI RC HID startup!");

  NimBLEDevice::setDeviceName(std::string(UtilityFunctions::loadBlueToothName().c_str()));

  // seems this is by default what xgimi listens on
  NimBLEDevice::setDefaultPhy(BLE_GAP_LE_PHY_1M_MASK, BLE_GAP_LE_PHY_1M_MASK);

  m_CustomService1 = server->createService(BLEUUID("0000D1FF-3C17-D293-8E48-14FE2E4DA212")); // uuid 0000D1FF-3C17-D293-8E48-14FE2E4DA212

  m_CustomService2 = server->createService(BLEUUID("0000D0FF-3C17-D293-8E48-14FE2E4DA212")); // uuid 0000D0FF-3C17-D293-8E48-14FE2E4DA21

  m_CustomService3 = server->createService(BLEUUID("00006287-3C17-D293-8E48-14FE2E4DA212")); // uuid 00006287-3C17-D293-8E48-14FE2E4DA212

  // link the report ID's in the HID infopacket
  keyboardInput_01 = getInputReport(KEYBOARD_REPORT_ID);
  keyboardOutput_01 = getOutputReport(KEYBOARD_REPORT_ID);

  static uint8_t Null_CMD[] = {0x00};
  keyboardOutput_01->setValue(Null_CMD, 1);

  secondDeviceInput_30 = getInputReport(SECOND_REPORT_ID);
  secondDeviceOutput_30 = getOutputReport(SECOND_REPORT_ID);
  thirdDeviceInput_03 = getInputReport(THIRD_REPORT_ID);

  keyboardInput_01->setCallbacks(new Callback_handler_Rep_Inp_01(this));
  keyboardOutput_01->setCallbacks(new Callback_handler_Rep_Out_01(this));
  secondDeviceInput_30->setCallbacks(new Callback_handler_Rep_Inp_30(this));
  secondDeviceOutput_30->setCallbacks(new Callback_handler_Rep_Out_30(this));
  thirdDeviceInput_03->setCallbacks(new Callback_handler_Rep_Inp_03(this));

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

  // set the manufacturer name

  setManufacturer(std::string(HID_MANUFACTURER_NAME).substr(0, 15));
  m_modelNumberCharacteristic = getDeviceInfoService()->createCharacteristic((uint16_t)0x2a24, NIMBLE_PROPERTY::BLE_READ);         // uuid 0x2a24
  m_SerialNumberCharacteristic = getDeviceInfoService()->createCharacteristic((uint16_t)0x2a25, NIMBLE_PROPERTY::BLE_READ);        // uuid 0x2a25
  m_hardwarRevisionStrCharacteristic = getDeviceInfoService()->createCharacteristic((uint16_t)0x2a27, NIMBLE_PROPERTY::BLE_READ);  // uuid 0x2a27
  m_firmwareRevisionStrCharacteristic = getDeviceInfoService()->createCharacteristic((uint16_t)0x2a26, NIMBLE_PROPERTY::BLE_READ); // uuid 0x2a26
  m_softwareRevisionCharacteristic = getDeviceInfoService()->createCharacteristic((uint16_t)0x2a28, NIMBLE_PROPERTY::BLE_READ);    // uuid 0x2a28
  m_systemIDCharacteristic = getDeviceInfoService()->createCharacteristic((uint16_t)0x2a23, NIMBLE_PROPERTY::BLE_READ);            // uuid 0x2a23
  m_IEE11073CertCharacteristic = getDeviceInfoService()->createCharacteristic((uint16_t)0x2a2a, NIMBLE_PROPERTY::BLE_READ);        // uuid 0x2a2a
                                                                                                                                   // set the devide PNP information
  setPnp(VENDORID_SIG, VENDORID_MANUF, PRODUCTID, VERSION);

  m_modelNumberCharacteristic->setValue(HID_MODEL_NUMBER);
  m_SerialNumberCharacteristic->setValue(HID_SERIAL_NUMBER);
  m_hardwarRevisionStrCharacteristic->setValue(HID_HARDWARE_REVISION);
  m_firmwareRevisionStrCharacteristic->setValue(HID_FIRMWARE_REVISION);
  m_softwareRevisionCharacteristic->setValue(HID_SOFTWARE_REVISION);
  m_systemIDCharacteristic->setValue(HID_SYSTEM_ID, sizeof(HID_SYSTEM_ID));
  m_IEE11073CertCharacteristic->setValue(HID_IEE11073_CERT);

  // set the HID countrt and flags information
  setHidInfo(HID_COUNTRY, HID_FLAGS);
  // set the battery level characteristic

  /* C) Customm service uuid 0000D1FF-3C17-D293-8E48-14FE2E4DA212 primary has
   1) uuid 0xa001, custom characteristic, write, no response, notify,
    descriptor (typ) for above, uuid 0x2902 - client characteristics config, readonly
   2) uuid 0xa002, custom charasteristic, write no response
   */
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
  m_0x6287Characteristic = m_CustomService3->createCharacteristic("00006387-3C17-D293-8E48-14FE2E4DA212", NIMBLE_PROPERTY::BLE_WRITE | NIMBLE_PROPERTY::NOTIFY); // uuid 00006387-3C17-D293-8E48-14FE2E4DA212

  BLE2904 *m_0x6287CharacteristicDescriptor = new BLE2904();
  m_0xA001Characteristic->addDescriptor(m_0x6287CharacteristicDescriptor);
  m_0x6287Characteristic->setCallbacks(this);

  // set the report map
  setReportMap((uint8_t *)HID_REPORT_DESCRIPTOR, sizeof(HID_REPORT_DESCRIPTOR));
  // UtilityFunctions::debugLogf("Set HID report map %s\n", NimBLEUtils::dataToHexString((uint8_t *)HID_REPORT_DESCRIPTOR, sizeof(HID_REPORT_DESCRIPTOR)).c_str());

  // UtilityFunctions::debugLog("IN XGIMI HID Security startup!");

  // BLE_SM_PAIR_AUTHREQ_BOND  - bond yes
  // BLE_SM_PAIR_AUTHREQ_MITM  - man in middle NO
  // BLE_SM_PAIR_AUTHREQ_SC - secure connection yes
  NimBLEDevice::setSecurityAuth(true, false, false);
  NimBLEDevice::setSecurityIOCap(BLE_HS_IO_NO_INPUT_OUTPUT);
  // BLE_SM_PAIR_KEY_DIST_ENC LTK key
  // BLE_SM_PAIR_KEY_DIST_ID  IRK key
  // BLE_SM_PAIR_KEY_DIST_SIGN CRSK key
  NimBLEDevice::setSecurityInitKey(BLE_SM_PAIR_KEY_DIST_ENC | BLE_SM_PAIR_KEY_DIST_ID | BLE_SM_PAIR_KEY_DIST_SIGN);
  NimBLEDevice::setSecurityRespKey(BLE_SM_PAIR_KEY_DIST_ENC | BLE_SM_PAIR_KEY_DIST_ID | BLE_SM_PAIR_KEY_DIST_SIGN);

  UtilityFunctions::debugLog("Starting HID services");
  startServices(); // HID start service - must be called after all charstics have been built

  UtilityFunctions::debugLog("Setting up Advertising");

  initStandardAdvData();
  initOnButtonAdvData();

  // UtilityFunctions::debugLog("Before std ADV start");
  startStandardAdv();
  // UtilityFunctions::debugLog("After std ADV start");

  uint8_t batteryLevel = 100;    // battery lvel can only be set AFTER advitsment and server start
  setBatteryLevel(batteryLevel); // set initial battery level to 100%

  UtilityFunctions::debugLog("Advertising started....");
}

void BlueXGIMI_RC::startServices()
{
  // UtilityFunctions::debugLogf("BLE server dump %s\n", BLE_server->toString().c_str());
  BlueRC::BluetoothHID_RC::startServices();
  m_CustomService1->start();
  m_CustomService2->start();
  m_CustomService3->start();

  // UtilityFunctions::debugLog("Services satrted GATT serice dump start ->");
  //  UtilityFunctions::debugLogf("Services count in GATT%i/n",BLE_server->m_svcVec.size());
  // UtilityFunctions::debugLog("Services satrted GATT serice dump end:");
}

void BlueXGIMI_RC::doButtons(ServerDecoder::Remote_Cmd command)
{
  int btnPressedType = 0;
  if (BLE_server->getConnectedCount() > 0)
  {
    switch (command.cmds.cmd)
    {

    case ServerDecoder::RC_Cmd_Action::Raw_2Byte:

      UtilityFunctions::debugLogf("Sending RAW 2 byte button %0X,%0X\n", command.cmds.codeData[0], command.cmds.codeData[1]);
      thirdDeviceInput_03->setValue(command.cmds.codeData, 2);
      btnPressedType = 3;
      // tell value has changed
      if (!thirdDeviceInput_03->notify())
      {
        UtilityFunctions::debugLog("RAW 2 byte button Notify failed");
      }
      break;

    case ServerDecoder::RC_Cmd_Action::Raw_8Byte:

      UtilityFunctions::debugLogf("Sending RAW 8 byte button %0X,%0X,%0X,%0X,%0X,%0X,%0X,%0X\n", command.cmds.codeData[0], command.cmds.codeData[1], command.cmds.codeData[2], command.cmds.codeData[3], command.cmds.codeData[4], command.cmds.codeData[5], command.cmds.codeData[6], command.cmds.codeData[7]);
      keyboardInput_01->setValue(command.cmds.codeData, 8);
      btnPressedType = 1;
      // tell value has changed
      if (!keyboardInput_01->notify())
      {
        UtilityFunctions::debugLog("RAW 8 byte button Notify failed");
      }
      break;

    case ServerDecoder::RC_Cmd_Action::Previous:

      // //Back button pressed
      // Bluetooth Attribute Protocol
      //     Opcode: Handle Value Notification (0x1b)
      //         0... .... = Authentication Signature: False
      //         .0.. .... = Command: False
      //         ..01 1011 = Method: Handle Value Notification (0x1b)
      //     Handle: 0x003d (Human Interface Device: Report)
      //         [Service UUID: Human Interface Device (0x1812)]
      //         [UUID: Report (0x2a4d)]
      //     Value: 2402
      //         [Expert Info (Note/Undecoded): Undecoded]

      // //back button released
      // Bluetooth Attribute Protocol
      //     Opcode: Handle Value Notification (0x1b)
      //         0... .... = Authentication Signature: False
      //         .0.. .... = Command: False
      //         ..01 1011 = Method: Handle Value Notification (0x1b)
      //     Handle: 0x003d (Human Interface Device: Report)
      //         [Service UUID: Human Interface Device (0x1812)]
      //         [UUID: Report (0x2a4d)]
      //     Value: 0000
      //         [Expert Info (Note/Undecoded): Undecoded]

      UtilityFunctions::debugLog("Sending back button");
      thirdDeviceInput_03->setValue(HID_REP03_BACK_CMD, sizeof(HID_REP03_BACK_CMD));
      btnPressedType = 3;
      // tell value has changed
      if (!thirdDeviceInput_03->notify())
      {
        UtilityFunctions::debugLog("back button Notify failed");
      }
      break;

    // OK_btn = 237
    case ServerDecoder::RC_Cmd_Action::Ok_Btn:

      UtilityFunctions::debugLog("Sending OK button");
      thirdDeviceInput_03->setValue(HID_REP03_OK_CMD, sizeof(HID_REP03_OK_CMD));
      btnPressedType = 3;
      // tell value has changed
      if (!thirdDeviceInput_03->notify())
      {
        UtilityFunctions::debugLog("Ok button Notify failed");
      }
      break;

    // Up_Btn = 238
    case ServerDecoder::RC_Cmd_Action::Up_Btn:

      UtilityFunctions::debugLog("Sending up button");
      thirdDeviceInput_03->setValue(HID_REP03_UP_CMD, sizeof(HID_REP03_UP_CMD));
      btnPressedType = 3;
      // tell value has changed
      if (!thirdDeviceInput_03->notify())
      {
        UtilityFunctions::debugLog("UP button Notify failed");
      }
      break;

    // Right_Btn = 239,
    case ServerDecoder::RC_Cmd_Action::Right_Btn:

      UtilityFunctions::debugLog("Sending right button");
      thirdDeviceInput_03->setValue(HID_REP03_RIGHT_CMD, sizeof(HID_REP03_RIGHT_CMD));
      btnPressedType = 3;
      // tell value has changed
      if (!thirdDeviceInput_03->notify())
      {
        UtilityFunctions::debugLog("Right button Notify failed");
      }
      break;

    // Left_Btn = 240,
    case ServerDecoder::RC_Cmd_Action::Left_Btn:

      UtilityFunctions::debugLog("Sending left button");
      thirdDeviceInput_03->setValue(HID_REP03_LEFT_CMD, sizeof(HID_REP03_LEFT_CMD));
      btnPressedType = 3;
      // tell value has changed
      if (!thirdDeviceInput_03->notify())
      {
        UtilityFunctions::debugLog("Left button Notify failed");
      }
      break;

    // Down_Btn = 241,
    case ServerDecoder::RC_Cmd_Action::Down_Btn:

      UtilityFunctions::debugLog("Sending down button");
      thirdDeviceInput_03->setValue(HID_REP03_DOWN_CMD, sizeof(HID_REP03_DOWN_CMD));
      btnPressedType = 3;
      // tell value has changed
      if (!thirdDeviceInput_03->notify())
      {
        UtilityFunctions::debugLog("Down button Notify failed");
      }

      break;

    // Vol_Up_Btn = 242,
    case ServerDecoder::RC_Cmd_Action::Vol_Up_Btn:

      UtilityFunctions::debugLog("Sending vol up  button");
      thirdDeviceInput_03->setValue(HID_REP03_VOL_UP_CMD, sizeof(HID_REP03_VOL_UP_CMD));
      btnPressedType = 3;
      // tell value has changed
      if (!thirdDeviceInput_03->notify())
      {
        UtilityFunctions::debugLog("Down vol up Notify failed");
      }
      break;

      // Vol_Dn_Btn = 243,
    case ServerDecoder::RC_Cmd_Action::Vol_Dn_Btn:

      UtilityFunctions::debugLog("Sending vol down button");
      thirdDeviceInput_03->setValue(HID_REP03_VOL_DOWN_CMD, sizeof(HID_REP03_VOL_DOWN_CMD));
      btnPressedType = 3;
      // tell value has changed
      if (!thirdDeviceInput_03->notify())
      {
        UtilityFunctions::debugLog("Down vol down Notify failed");
      }
      break;

    // Home_Btn = 246,
    case ServerDecoder::RC_Cmd_Action::Home_Btn:

      UtilityFunctions::debugLog("Sending home button");
      thirdDeviceInput_03->setValue(HID_REP03_HOME_CMD, sizeof(HID_REP03_HOME_CMD));
      btnPressedType = 3;
      // tell value has changed
      if (!thirdDeviceInput_03->notify())
      {
        UtilityFunctions::debugLog("Home Notify failed");
      }
      break;

    // Options_Btn = 247,
    case ServerDecoder::RC_Cmd_Action::Options_Btn:

      UtilityFunctions::debugLog("Sending options button");
      thirdDeviceInput_03->setValue(HID_REP03_OPTIONS_CMD, sizeof(HID_REP03_OPTIONS_CMD));
      btnPressedType = 3;
      // tell value has changed
      if (!thirdDeviceInput_03->notify())
      {
        UtilityFunctions::debugLog("Options Notify failed");
      }
      break;

      // Channel_Up_Btn
    case ServerDecoder::RC_Cmd_Action::Channel_Up_Btn:
      UtilityFunctions::debugLog("Sending ch up as vol up button");
      thirdDeviceInput_03->setValue(HID_REP03_OPTIONS_CMD, sizeof(HID_REP03_VOL_UP_CMD));
      btnPressedType = 3;
      // tell value has changed
      if (!thirdDeviceInput_03->notify())
      {
        UtilityFunctions::debugLog("Options Notify failed");
      }
      break;

      // Channel_dn_Btn
    case ServerDecoder::RC_Cmd_Action::Channel_Dn_Btn:
      UtilityFunctions::debugLog("Sending ch dn as vol dn button");
      thirdDeviceInput_03->setValue(HID_REP03_OPTIONS_CMD, sizeof(HID_REP03_VOL_DOWN_CMD));
      btnPressedType = 3;
      // tell value has changed
      if (!thirdDeviceInput_03->notify())
      {
        UtilityFunctions::debugLog("Options Notify failed");
      }
      break;

      // projector settings button
    case ServerDecoder::RC_Cmd_Action::Projector_Setting_Btn:
      UtilityFunctions::debugLog("Sending prrojector settings button");
      keyboardInput_01->setValue(HID_REP01_PROJ_SET_CMD, sizeof(HID_REP01_PROJ_SET_CMD));
      btnPressedType = 1;
      // tell value has changed
      if (!keyboardInput_01->notify())
      {
        UtilityFunctions::debugLog("Options Notify failed");
      }
      break;

      // settings button
    case ServerDecoder::RC_Cmd_Action::Settings_Btn:
      UtilityFunctions::debugLog("Sending settings button");
      keyboardInput_01->setValue(HID_REP01_SETTING_CMD, sizeof(HID_REP01_SETTING_CMD));
      btnPressedType = 1;
      // tell value has changed
      if (!keyboardInput_01->notify())
      {
        UtilityFunctions::debugLog("Options Notify failed");
      }
      break;

    } // end of case

    if (btnPressedType == 3)
    {
      UtilityFunctions::delay(HID_KEY_DELAY);
      UtilityFunctions::debugLog("Sending button null");
      thirdDeviceInput_03->setValue(HID_REP03_NULL_CMD, sizeof(HID_REP03_NULL_CMD));
      // tell value has changed
      if (!thirdDeviceInput_03->notify())
      {
        UtilityFunctions::debugLog("On off button Notify failed");
      }
    }

    if (btnPressedType == 1)
    {
      UtilityFunctions::delay(HID_KEY_DELAY);
      UtilityFunctions::debugLog("Sending button null");
      keyboardInput_01->setValue(HID_REP01_NULL_CMD, sizeof(HID_REP01_NULL_CMD));
      // tell value has changed
      if (!keyboardInput_01->notify())
      {
        UtilityFunctions::debugLog("On off button Notify failed");
      }
    }
  }
  else
  {
    UtilityFunctions::debugLog("BLE NOT conneccted ignoring button push");
  }
}
void BlueXGIMI_RC::doCMD_ON_OFF()
{

  if (BLE_server->getConnectedCount() > 0)
  {
    // ok we are connedted to the projector so the projector must me ON we send the off command
    // off button pressed
    // Bluetooth Attribute Protocol
    // Opcode: Handle Value Notification (0x1b)
    //     0... .... = Authentication Signature: False
    //     .0.. .... = Command: False
    //     ..01 1011 = Method: Handle Value Notification (0x1b)
    // Handle: 0x002f (Human Interface Device: Report)
    //     [Service UUID: Human Interface Device (0x1812)]
    //     [UUID: Report (0x2a4d)] ie repor1
    // Value: 0000660000000000
    //     [Expert Info (Note/Undecoded): Undecoded]

    keyboardInput_01->setValue(HID_REP01_OFF_CMD, sizeof(HID_REP01_OFF_CMD));

    // tell value has changed
    if (!keyboardInput_01->notify())
    {
      UtilityFunctions::debugLog("On off button Notify failed");
    }
    UtilityFunctions::delay(HID_KEY_DELAY);
    keyboardInput_01->setValue(HID_REP01_NULL_CMD, sizeof(HID_REP01_NULL_CMD));
    // tell value has changed
    if (!keyboardInput_01->notify())
    {
      UtilityFunctions::debugLog("On off button Notify failed");
    }

    // now send ok button
    ServerDecoder::Remote_Cmd command;
    command.cmds.cmd = ServerDecoder::RC_Cmd_Action::Ok_Btn;
    UtilityFunctions::delay(HID_KEY_DELAY);
    doButtons(command);
  }
  else
  {
    // we are not connected to the projector so we need to send the on command
    // we advertise the ty1 adv 1 for 1 secs
    // then we advertise the sdv 2 - aka media tek for 1 secs
    // and stop

    bool wasAdvertisingSTDpkt = advertising->isActive(HID_ADV_STD_ID);

    while (advertising->isActive(HID_ADV_STD_ID))
    {
      advertising->stop(HID_ADV_STD_ID);
      UtilityFunctions::delay(1000);
    }

#ifdef XGIMI_USE_EXT_ADV

    advertising->setInstanceData(HID_ADV_ONDATA1_ID, advertisingOnButtonDataType1);
    advertising->setScanResponseData(HID_ADV_ONDATA1_ID, advertisingOnButtonDataScanDataType1);
    /** Set the advertisement as connectable */

    advertising->start(HID_ADV_ONDATA1_ID, 1000, 0);
#else
    advertising->setAdvertisementData(advertisingOnButtonDataType1);
    advertising->setScanResponseData(advertisingOnButtonDataScanDataType1);

    /**
     * * BLE_GAP_CONN_MODE_NON    (0) - not connectable advertising
     * * BLE_GAP_CONN_MODE_DIR    (1) - directed connectable advertising
     * * BLE_GAP_CONN_MODE_UND    (2) - undirected connectable advertising
     */
    advertising->setConnectableMode(BLE_GAP_CONN_MODE_UND);
    advertising->enableScanResponse(true);
    advertising->setMinInterval(50);
    advertising->setMaxInterval(60);
    advertising->start(500);
#endif
    UtilityFunctions::debugLog("On button type 1 advertisment started");
    UtilityFunctions::delay(1000);

    while (advertising->isActive(HID_ADV_ONDATA1_ID))
    {
      advertising->stop(HID_ADV_ONDATA1_ID);
      UtilityFunctions::delay(1000);
    }

    // repeat this 3 timmes
    for (int f = 1; f <= 3; f++)
    {
#ifdef XGIMI_USE_EXT_ADV

      advertising->setInstanceData(HID_ADV_ONDATA2_ID, advertisingOnButtonDataType2);
      advertising->setScanResponseData(HID_ADV_ONDATA2_ID, advertisingOnButtonDataScanDataType2);
      /** Set the advertisement as connectable */

      advertising->start(HID_ADV_ONDATA2_ID, 500, 0);
#else
      advertising->setAdvertisementData(advertisingOnButtonDataType2);
      advertising->setScanResponseData(advertisingOnButtonDataScanDataType2);

      /**
       * * BLE_GAP_CONN_MODE_NON    (0) - not connectable advertising
       * * BLE_GAP_CONN_MODE_DIR    (1) - directed connectable advertising
       * * BLE_GAP_CONN_MODE_UND    (2) - undirected connectable advertising
       */
      advertising->setConnectableMode(BLE_GAP_CONN_MODE_UND);
      advertising->enableScanResponse(true);
      advertising->setMinInterval(50);
      advertising->setMaxInterval(60);
      advertising->start(500);
#endif
      UtilityFunctions::debugLogf("Cycle num:%i - On button type 2 advertisment started\n", f);
      UtilityFunctions::delay(500);

      while (advertising->isActive(HID_ADV_ONDATA2_ID))
      {
        advertising->stop(HID_ADV_ONDATA2_ID);
        UtilityFunctions::delay(1000);
      }

#ifdef XGIMI_USE_EXT_ADV

      //UtilityFunctions::debugLogf("On button type 3 adv data %s\n", advertisingOnButtonDataType3.toString().c_str());
      advertising->setInstanceData(HID_ADV_ONDATA3_ID, advertisingOnButtonDataType3);
      advertising->setScanResponseData(HID_ADV_ONDATA3_ID, advertisingOnButtonDataScanDataType3);
      /** Set the advertisement as connectable */

      advertising->start(HID_ADV_ONDATA3_ID, 500, 0);
#else
      advertising->setAdvertisementData(advertisingOnButtonDataType3);
      advertising->setScanResponseData(advertisingOnButtonDataScanDataType3);

      /**
       * * BLE_GAP_CONN_MODE_NON    (0) - not connectable advertising
       * * BLE_GAP_CONN_MODE_DIR    (1) - directed connectable advertising
       * * BLE_GAP_CONN_MODE_UND    (2) - undirected connectable advertising
       */
      advertising->setConnectableMode(BLE_GAP_CONN_MODE_UND);
      advertising->enableScanResponse(true);
      advertising->setMinInterval(50);
      advertising->setMaxInterval(60);
      advertising->start(500);
#endif
      UtilityFunctions::debugLogf("Cycle num:%i - On button type 3 advertisment started", f);
      UtilityFunctions::delay(500);

      while (advertising->isActive(HID_ADV_ONDATA3_ID))
      {
        advertising->stop(HID_ADV_ONDATA3_ID);
        UtilityFunctions::delay(1000);
      }
    } // end for

    UtilityFunctions::debugLog("On button advertisment COMPLETE");

    // we wait for 5 secs for projector to connect back
    UtilityFunctions::delay(5000);

    if (wasAdvertisingSTDpkt)
    {
      // restore the std packet for advistsment
      startStandardAdv();
    }
  }
}

bool BlueXGIMI_RC::canHandleButtonPress(ServerDecoder::Remote_Cmd command)
{

  switch (command.cmds.cmd)
  {
  case ServerDecoder::RC_Cmd_Action::On_OFF_Btn:
  case ServerDecoder::RC_Cmd_Action::Channel_Dn_Btn:
  case ServerDecoder::RC_Cmd_Action::Channel_Up_Btn:
  case ServerDecoder::RC_Cmd_Action::Down_Btn:
  case ServerDecoder::RC_Cmd_Action::Home_Btn:
  case ServerDecoder::RC_Cmd_Action::Left_Btn:
  case ServerDecoder::RC_Cmd_Action::Ok_Btn:
  case ServerDecoder::RC_Cmd_Action::Options_Btn:
  case ServerDecoder::RC_Cmd_Action::Pairing_On:
  case ServerDecoder::RC_Cmd_Action::Pairing_Off:
  case ServerDecoder::RC_Cmd_Action::Projector_Setting_Btn:
  case ServerDecoder::RC_Cmd_Action::Previous:
  case ServerDecoder::RC_Cmd_Action::Right_Btn:
  case ServerDecoder::RC_Cmd_Action::Settings_Btn:
  case ServerDecoder::RC_Cmd_Action::Up_Btn:
  case ServerDecoder::RC_Cmd_Action::Vol_Dn_Btn:
  case ServerDecoder::RC_Cmd_Action::Vol_Up_Btn:
  case ServerDecoder::RC_Cmd_Action::Volume:
  case ServerDecoder::RC_Cmd_Action::Channel:
  case ServerDecoder::RC_Cmd_Action::Raw_2Byte:
  case ServerDecoder::RC_Cmd_Action::Raw_8Byte:

    return true;
  }

  return false;
}

void BlueXGIMI_RC::sendButtonPress(ServerDecoder::Remote_Cmd command)
{
  std::string s_cmd = std::string(magic_enum::enum_name((ServerDecoder::RC_Cmd_Action)command.cmds.cmd));

  UtilityFunctions::debugLogf("XIGIMI Remote received command %s amd in int %i\n", s_cmd.c_str(), command.cmds.cmd);
  bool cmdExecuted = false;
  switch (command.cmds.cmd)
  {
  case ServerDecoder::RC_Cmd_Action::On_OFF_Btn:
    doCMD_ON_OFF();
    cmdExecuted = true;
    break;

  case ServerDecoder::RC_Cmd_Action::Channel_Dn_Btn:
  case ServerDecoder::RC_Cmd_Action::Channel_Up_Btn:
  case ServerDecoder::RC_Cmd_Action::Down_Btn:
  case ServerDecoder::RC_Cmd_Action::Home_Btn:
  case ServerDecoder::RC_Cmd_Action::Left_Btn:
  case ServerDecoder::RC_Cmd_Action::Ok_Btn:
  case ServerDecoder::RC_Cmd_Action::Options_Btn:
  case ServerDecoder::RC_Cmd_Action::Projector_Setting_Btn:
  case ServerDecoder::RC_Cmd_Action::Previous:
  case ServerDecoder::RC_Cmd_Action::Right_Btn:
  case ServerDecoder::RC_Cmd_Action::Settings_Btn:
  case ServerDecoder::RC_Cmd_Action::Up_Btn:
  case ServerDecoder::RC_Cmd_Action::Vol_Dn_Btn:
  case ServerDecoder::RC_Cmd_Action::Vol_Up_Btn:
  case ServerDecoder::RC_Cmd_Action::Raw_2Byte:
  case ServerDecoder::RC_Cmd_Action::Raw_8Byte:

    doButtons(command);
    cmdExecuted = true;
    break;

  case ServerDecoder::RC_Cmd_Action::Volume:
  case ServerDecoder::RC_Cmd_Action::Channel:

    int steps = command.cmds.toVal - command.cmds.fromVal;

    if (steps == 0)
      break;
    steps = (steps < 0) ? -steps : steps;
    UtilityFunctions::debugLogf("XIGIMI Remote step function of steps %i \n", steps);
    while (steps > 0)
    {
      ServerDecoder::Remote_Cmd newCMD;
      if ((command.cmds.toVal - command.cmds.fromVal) < 0)
      {
        newCMD.cmds.cmd = (command.cmds.cmd == ServerDecoder::RC_Cmd_Action::Volume) ? ServerDecoder::RC_Cmd_Action::Vol_Dn_Btn : ServerDecoder::RC_Cmd_Action::Channel_Dn_Btn;
      }
      else
      {

        newCMD.cmds.cmd = (command.cmds.cmd == ServerDecoder::RC_Cmd_Action::Volume) ? ServerDecoder::RC_Cmd_Action::Vol_Up_Btn : ServerDecoder::RC_Cmd_Action::Channel_Up_Btn;
      }
      UtilityFunctions::debugLogf("XIGIMI Remote while function of steps %i \n", steps);
      doButtons(newCMD);
      steps--;
    }
    cmdExecuted = true;
    break;
  }

  UtilityFunctions::debugLogf("XIGIMI Remote receied command %s in int %i and execution status is %i\n", s_cmd.c_str(), command.cmds.cmd, cmdExecuted);
}

// generic handlers
void BlueXGIMI_RC::onWrite(NimBLECharacteristic *pCharacteristic, NimBLEConnInfo &connInfo)
{

  UtilityFunctions::debugLogf("UUID ");
  UtilityFunctions::debugLog(pCharacteristic->getUUID().toString().c_str());
  UtilityFunctions::debugLogf("Data length %i\n", pCharacteristic->getLength());

  NimBLEAttValue val = pCharacteristic->getValue();
  std::string value = NimBLEUtils::dataToHexString(val.data(), val.length());
  UtilityFunctions::debugLogf("Data received HEX %s\n", value.c_str());
  if (val.length() > 0)
  {
    UtilityFunctions::debugLogf("Data received RAW %s\n", pCharacteristic->getValue());
  }
}

void BlueXGIMI_RC::onRead(NimBLECharacteristic *pCharacteristic, NimBLEConnInfo &connInfo)
{
  UtilityFunctions::debugLogf("UUID ");
  UtilityFunctions::debugLog(pCharacteristic->getUUID().toString().c_str());
  UtilityFunctions::debugLogf("Data length %i\n", pCharacteristic->getLength());

  NimBLEAttValue val = pCharacteristic->getValue();
  std::string value = NimBLEUtils::dataToHexString(val.data(), val.length());
  UtilityFunctions::debugLogf("Data SENT HEX %s\n", value.c_str());
  if (val.length() > 0)
  {
    UtilityFunctions::debugLogf("Data received RAW %s\n", pCharacteristic->getValue());
  }
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