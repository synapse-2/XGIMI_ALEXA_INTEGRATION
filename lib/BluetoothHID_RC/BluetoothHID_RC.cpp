#include <BluetoothHID_RC.h>
#include <BLEDevice.h>
#include <BLESecurity.h>
#include <Arduino.h>
#include <stdint.h>
#include <BLE2904.h>
#include "UtilityFunctions.h"

BluetoothHID_RC::BluetoothHID_RC(BLEServer *server) : BLEHIDDevice(server)
{

  UtilityFunctions::debugLog("In HID startup!");

  ble_server = server;
  ble_server->setCallbacks(this);

  // link the report ID's in the HID infopacket
  keyboardInput = inputReport(KEYBOARD_REPORT_ID);
  keyboardOutput = outputReport(KEYBOARD_REPORT_ID);
  secondDeviceInput = inputReport(SECOND_REPORT_ID);
  secondDeviceOutput = outputReport(SECOND_REPORT_ID);
  thirdDeviceInput = inputReport(THIRD_REPORT_ID);

  keyboardOutput->setCallbacks(this);
  secondDeviceOutput->setCallbacks(this);

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

  m_modelNumberCharacteristic = deviceInfo()->createCharacteristic((uint16_t)0x2a24, BLECharacteristic::PROPERTY_READ); // uuid 0x2a24
  m_modelNumberCharacteristic->setValue(HID_MODEL_NUMBER);

  m_SerialNumberCharacteristic = deviceInfo()->createCharacteristic((uint16_t)0x2a25, BLECharacteristic::PROPERTY_READ); // uuid 0x2a25
  m_SerialNumberCharacteristic->setValue(HID_SERIAL_NUMBER);

  m_hardwarRevisionStrCharacteristic = deviceInfo()->createCharacteristic((uint16_t)0x2a27, BLECharacteristic::PROPERTY_READ); // uuid 0x2a27
  m_hardwarRevisionStrCharacteristic->setValue(HID_HARDWARE_REVISION);

  m_firmwareRevisionStrCharacteristic = deviceInfo()->createCharacteristic((uint16_t)0x2a26, BLECharacteristic::PROPERTY_READ); // uuid 0x2a26
  m_firmwareRevisionStrCharacteristic->setValue(HID_FIRMWARE_REVISION);

  m_softwareRevisionCharacteristic = deviceInfo()->createCharacteristic((uint16_t)0x2a28, BLECharacteristic::PROPERTY_READ); // uuid 0x2a28
  m_softwareRevisionCharacteristic->setValue(HID_SOFTWARE_REVISION);

  m_systemIDCharacteristic = deviceInfo()->createCharacteristic((uint16_t)0x2a23, BLECharacteristic::PROPERTY_READ); // uuid 0x2a23
  m_systemIDCharacteristic->setValue(HID_SYSTEM_ID);

  m_IEE11073CertCharacteristic = deviceInfo()->createCharacteristic((uint16_t)0x2a2a, BLECharacteristic::PROPERTY_READ);
  ; // uuid 0x2a2a
  m_IEE11073CertCharacteristic->setValue(HID_IEE11073_CERT);

  // set the manufacturer name`
  manufacturer()->setValue(std::string(HID_MANUFACTURER_NAME).substr(0, 15));
  // set the devide PNP information
  pnp(VENDORID_SIG, VENDORID_MANUF, PRODUCTID, VERSION);

  // set the HID countrt and flags information
  hidInfo(HID_COUNTRY, HID_FLAGS);
  // set the battery level characteristic

  /* C) Customm service uuid 0000D1FF-3C17-D293-8E48-14FE2E4DA212 primary has
     1) uuid 0xa001, custom characteristic, write, no response, notify,
      descriptor (typ) for above, uuid 0x2902 - client characteristics config, readonly
     2) uuid 0xa002, custom charasteristic, write no response
     */
  m_CustomService1 = server->createService(BLEUUID("0000D1FF-3C17-D293-8E48-14FE2E4DA212"));                                                                    // uuid 0000D1FF-3C17-D293-8E48-14FE2E4DA212
  m_0xA001Characteristic = m_CustomService1->createCharacteristic((uint16_t)0xA001, BLECharacteristic::PROPERTY_WRITE_NR | BLECharacteristic::PROPERTY_NOTIFY); // uuid 0xa001,
  BLE2902 *m_0xA001CharacteristicDescriptor = new BLE2902();
  
  m_0xA001Characteristic->addDescriptor(m_0xA001CharacteristicDescriptor);
  m_0xA001Characteristic->setCallbacks(this);
  m_0xA002Characteristic = m_CustomService1->createCharacteristic((uint16_t)0xA002, BLECharacteristic::PROPERTY_WRITE_NR); // uuid 0xa002,
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
  m_CustomService2 = server->createService(BLEUUID("0000D0FF-3C17-D293-8E48-14FE2E4DA212"));                               // uuid 0000D0FF-3C17-D293-8E48-14FE2E4DA212
  m_0xFFD1Characteristic = m_CustomService2->createCharacteristic((uint16_t)0xFFD1, BLECharacteristic::PROPERTY_WRITE_NR); // uuid 0xFFD1
  m_0xFFD1Characteristic->setCallbacks(this);
  m_0xFFD2Characteristic = m_CustomService2->createCharacteristic((uint16_t)0xFFD2, BLECharacteristic::PROPERTY_READ); // uuid 0xFFD2
  m_0xFFD2Characteristic->setValue(HID_uuid_0xffd2, sizeof(HID_uuid_0xffd2));
  m_0xFFD3Characteristic = m_CustomService2->createCharacteristic((uint16_t)0xFFD3, BLECharacteristic::PROPERTY_READ); // uuid 0xFFD3
  m_0xFFD3Characteristic->setValue(HID_uuid_0xffd3, sizeof(HID_uuid_0xffd3));
  m_0xFFD4Characteristic = m_CustomService2->createCharacteristic((uint16_t)0xFFD4, BLECharacteristic::PROPERTY_READ); // uuid 0xFFD4
  m_0xFFD4Characteristic->setValue(HID_uuid_0xffd4, sizeof(HID_uuid_0xffd4));
  m_0xFFD5Characteristic = m_CustomService2->createCharacteristic((uint16_t)0xFFD5, BLECharacteristic::PROPERTY_READ);     // uuid 0xFFD5
  m_0xFFD8Characteristic = m_CustomService2->createCharacteristic((uint16_t)0xFFD8, BLECharacteristic::PROPERTY_WRITE_NR); // uuid 0xFFD8
  m_0xFFD8Characteristic->setCallbacks(this);
  m_0xFFF1Characteristic = m_CustomService2->createCharacteristic((uint16_t)0xFFF1, BLECharacteristic::PROPERTY_READ); // uuid 0xFFF1

  m_0xFFF1Characteristic->setValue(HID_uuid_0xfff1, sizeof(HID_uuid_0xfff1));
  m_0xFFF2Characteristic = m_CustomService2->createCharacteristic((uint16_t)0xFFF2, BLECharacteristic::PROPERTY_WRITE); // uuid 0xFFF2
  m_0xFFF2Characteristic->setCallbacks(this);
  m_0xFFE0Characteristic = m_CustomService2->createCharacteristic((uint16_t)0xFFE0, BLECharacteristic::PROPERTY_READ); // uuid 0xFFE0
  m_0xFFE0Characteristic->setValue(HID_uuid_0xffe0, sizeof(HID_uuid_0xffe0));

  /* E) Customm service uuid 00006287-3C17-D293-8E48-14FE2E4DA212 primary has
    1) uuid 00006387-3C17-D293-8E48-14FE2E4DA212, write no response
    2) uuid 00006487-3C17-D293-8E48-14FE2E4DA212, write, nitify
        descriptor (typ) for above, uuid 0x2902 - client characteristics config, readonly
    */
  m_CustomService3 = server->createService(BLEUUID("00006287-3C17-D293-8E48-14FE2E4DA212"));                                                                                       // uuid 00006287-3C17-D293-8E48-14FE2E4DA212
  m_0x6287Characteristic = m_CustomService3->createCharacteristic("00006387-3C17-D293-8E48-14FE2E4DA212", BLECharacteristic::PROPERTY_WRITE | BLECharacteristic::PROPERTY_NOTIFY); // uuid 00006387-3C17-D293-8E48-14FE2E4DA212
  BLE2904 *m_0x6287CharacteristicDescriptor = new BLE2904();
  m_0xA001Characteristic->addDescriptor(m_0x6287CharacteristicDescriptor);
  m_0x6287Characteristic->setCallbacks(this);

  BLESecurity *pSecurity = new BLESecurity();

  pSecurity->setAuthenticationMode(ESP_LE_AUTH_REQ_BOND_MITM);
  // pSecurity->setAuthenticationMode(ESP_LE_AUTH_REQ_SC_MITM_BOND);

  // set the report map
  reportMap((uint8_t *)HID_REPORT_DESCRIPTOR, sizeof(HID_REPORT_DESCRIPTOR));

  UtilityFunctions::debugLog("Starting HID services");
  startServices(); // HID start service - must be called after all charstics have been built

  onStarted(ble_server); // notify the erver has started

  UtilityFunctions::debugLog("Setting up Advertising");

  advertisingData.setAppearance(HID_REMOTE);
  advertisingData.setManufacturerData(HID_AD_MANUF_DATA);
  advertisingData.setCompleteServices(hidService()->getUUID());
  advertising = server->getAdvertising();
  advertising->setAdvertisementData(advertisingData);
  advertising->setAppearance(HID_REMOTE);
  advertising->addServiceUUID(hidService()->getUUID());
  advertising->setScanResponse(false);
  advertising->start();

  uint8_t batteryLevel = 100;    // battery lvel can only be set AFTER advitsment and server start
  setBatteryLevel(batteryLevel); // set initial battery level to 100%

  UtilityFunctions::debugLog("Advertising started!");
}

void BluetoothHID_RC::startServices()
{
  BLEHIDDevice::startServices();
  m_CustomService1->start();
  m_CustomService2->start();
  m_CustomService3->start();
}

void BluetoothHID_RC::onConnect(BLEServer *pServer)
{
  connected = true;

  UtilityFunctions::debugLog("Bluetooth Client connected");
}

void BluetoothHID_RC::onDisconnect(BLEServer *pServer)
{
  connected = false;

  UtilityFunctions::debugLog("Bluetooth Client disconnected");
}

void BluetoothHID_RC::sendButtonPress(uint8_t command)
{
}

void BluetoothHID_RC::onWrite(BLECharacteristic *me)
{
  if (me == keyboardInput)
  {
    // Handle keyboard input
    UtilityFunctions::debugLog("Keyboard input received");
  }
  else if (me == secondDeviceInput)
  {
    // Handle second device input
    UtilityFunctions::debugLog("Second device input received");
  }
  else if (me == thirdDeviceInput)
  {
    // Handle third device input
    UtilityFunctions::debugLog("Third device input received");
  }
  else
  {
    UtilityFunctions::debugLog("Unknown characteristic written");
  }
  UtilityFunctions::debugLogf("UUID ");
  UtilityFunctions::debugLog(me->getUUID().toString().c_str());
  UtilityFunctions::debugLogf("Data length %i\n", me->getLength());
  UtilityFunctions::debugLogf("Data received %s\n", me->getValue());
}

BluetoothHID_RC::~BluetoothHID_RC()
{
  // Cleanup resources if needed
  if (advertising)
  {
    advertising->stop();
    delete advertising;
  }

  if (ble_server->getConnectedCount() > 0)
  {

    ble_server->disconnect(ble_server->getConnId());
  }
  else
  {
    UtilityFunctions::debugLog("No active connections to disconnect");
  }
  delete ble_server;
  UtilityFunctions::debugLog("BluetoothHID_RC destroyed");
}
