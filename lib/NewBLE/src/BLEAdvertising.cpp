/*
 * BLEAdvertising.cpp
 *
 * This class encapsulates advertising a BLE Server.
 *  Created on: Jun 21, 2017
 *      Author: kolban
 *
 *  Modified on: Feb 18, 2025
 *      Author: lucasssvaz (based on kolban's and h2zero's work)
 *      Description: Added support for NimBLE
 *
 * The ESP-IDF provides a framework for BLE advertising.  It has determined that there are a common set
 * of properties that are advertised and has built a data structure that can be populated by the programmer.
 * This means that the programmer doesn't have to "mess with" the low level construction of a low level
 * BLE advertising frame.  Many of the fields are determined for us while others we can set before starting
 * to advertise.
 *
 * Should we wish to construct our own payload, we can use the BLEAdvertisementData class and call the setters
 * upon it.  Once it is populated, we can then associate it with the advertising and what ever the programmer
 * set in the data will be advertised.
 *
 */

#include "soc/soc_caps.h"
#if SOC_BLE_SUPPORTED

#include "sdkconfig.h"
#if defined(CONFIG_BLUEDROID_ENABLED) || defined(CONFIG_NIMBLE_ENABLED)

/***************************************************************************
 *                           Common includes                               *
 ***************************************************************************/

#include "BLEAdvertising.h"
#include <esp_err.h>
#include "BLEUtils.h"
#include "BLEDevice.h"
#include "GeneralUtils.h"
#include "esp32-hal-log.h"

/***************************************************************************
 *                           Common functions                              *
 ***************************************************************************/

/**
 * @brief Construct a default advertising object.
 */
BLEAdvertising::BLEAdvertising() {
  reset();
}  // BLEAdvertising

/**
 * @brief Add a service uuid to exposed list of services.
 * @param [in] serviceUUID The UUID of the service to expose.
 */
void BLEAdvertising::addServiceUUID(BLEUUID serviceUUID) {
  m_serviceUUIDs.push_back(serviceUUID);
#ifdef CONFIG_NIMBLE_ENABLED
  m_advDataSet = false;
#endif
}  // addServiceUUID

/**
 * @brief Add a service uuid to exposed list of services.
 * @param [in] serviceUUID The string representation of the service to expose.
 */
void BLEAdvertising::addServiceUUID(const char *serviceUUID) {
  addServiceUUID(BLEUUID(serviceUUID));
#ifdef CONFIG_NIMBLE_ENABLED
  m_advDataSet = false;
#endif
}  // addServiceUUID

/**
 * @brief Remove a service uuid to exposed list of services.
 * @param [in] index The index of the service to stop exposing.
 */
bool BLEAdvertising::removeServiceUUID(int index) {

  // If index is larger than the size of the
  // advertised services, return false
  if (index > m_serviceUUIDs.size()) {
    return false;
  }

  m_serviceUUIDs.erase(m_serviceUUIDs.begin() + index);
#ifdef CONFIG_NIMBLE_ENABLED
  m_advDataSet = false;
#endif
  return true;
}

/**
 * @brief Remove a service uuid to exposed list of services.
 * @param [in] serviceUUID The BLEUUID of the service to stop exposing.
 */
bool BLEAdvertising::removeServiceUUID(BLEUUID serviceUUID) {
  for (int i = 0; i < m_serviceUUIDs.size(); i++) {
    if (m_serviceUUIDs.at(i).equals(serviceUUID)) {
      return removeServiceUUID(i);
    }
  }
  return false;
}

/**
 * @brief Remove a service uuid to exposed list of services.
 * @param [in] serviceUUID The string of the service to stop exposing.
 */
bool BLEAdvertising::removeServiceUUID(const char *serviceUUID) {
  return removeServiceUUID(BLEUUID(serviceUUID));
}

/**
 * @brief Set the device appearance in the advertising data.
 * The appearance attribute is of type 0x19.  The codes for distinct appearances can be found here:
 * https://www.bluetooth.com/specifications/gatt/viewer?attributeXmlFile=org.bluetooth.characteristic.gap.appearance.xml.
 * @param [in] appearance The appearance of the device in the advertising data.
 * @return N/A.
 */
void BLEAdvertising::setAppearance(uint16_t appearance) {
  m_advData.appearance = appearance;
#ifdef CONFIG_NIMBLE_ENABLED
  m_advData.appearance_is_present = 1;
  m_advDataSet = false;
#endif
}  // setAppearance

void BLEAdvertising::setAdvertisementType(uint8_t adv_type) {
#ifdef CONFIG_BLUEDROID_ENABLED
  m_advParams.adv_type = (esp_ble_adv_type_t)adv_type;
#endif

#if defined(CONFIG_NIMBLE_ENABLED)
  m_advParams.conn_mode = adv_type;
#endif
}  // setAdvertisementType

void BLEAdvertising::setMinInterval(uint16_t mininterval) {
#ifdef CONFIG_BLUEDROID_ENABLED
  m_advParams.adv_int_min = mininterval;
#endif

#if defined(CONFIG_NIMBLE_ENABLED)
  m_advParams.itvl_min = mininterval;
#endif
}  // setMinInterval

void BLEAdvertising::setMaxInterval(uint16_t maxinterval) {
#ifdef CONFIG_BLUEDROID_ENABLED
  m_advParams.adv_int_max = maxinterval;
#endif

#if defined(CONFIG_NIMBLE_ENABLED)
  m_advParams.itvl_max = maxinterval;
#endif
}  // setMaxInterval

void BLEAdvertising::setMinPreferred(uint16_t mininterval) {
#ifdef CONFIG_BLUEDROID_ENABLED
  m_advData.min_interval = mininterval;
#endif

#if defined(CONFIG_NIMBLE_ENABLED)
  // invalid parameters, set the slave interval to null
  if (mininterval < 0x0006 || mininterval > 0x0C80) {
    m_advData.slave_itvl_range = nullptr;
    return;
  }

  if (m_advData.slave_itvl_range == nullptr) {
    m_advData.slave_itvl_range = m_slaveItvl;
  }

  m_slaveItvl[0] = mininterval;
  m_slaveItvl[1] = mininterval >> 8;

  uint16_t maxinterval = *(uint16_t *)(m_advData.slave_itvl_range + 2);

  // If mininterval is higher than the maxinterval make them the same
  if (mininterval > maxinterval) {
    m_slaveItvl[2] = m_slaveItvl[0];
    m_slaveItvl[3] = m_slaveItvl[1];
  }

  m_advDataSet = false;
#endif
}  //

void BLEAdvertising::setMaxPreferred(uint16_t maxinterval) {
#ifdef CONFIG_BLUEDROID_ENABLED
  m_advData.max_interval = maxinterval;
#endif

#if defined(CONFIG_NIMBLE_ENABLED)
  // invalid parameters, set the slave interval to null
  if (maxinterval < 0x0006 || maxinterval > 0x0C80) {
    m_advData.slave_itvl_range = nullptr;
    return;
  }
  if (m_advData.slave_itvl_range == nullptr) {
    m_advData.slave_itvl_range = m_slaveItvl;
  }
  m_slaveItvl[2] = maxinterval;
  m_slaveItvl[3] = maxinterval >> 8;

  uint16_t mininterval = *(uint16_t *)(m_advData.slave_itvl_range);

  // If mininterval is higher than the maxinterval make them the same
  if (mininterval > maxinterval) {
    m_slaveItvl[0] = m_slaveItvl[2];
    m_slaveItvl[1] = m_slaveItvl[3];
  }

  m_advDataSet = false;
#endif
}  //

void BLEAdvertising::setScanResponse(bool set) {
  m_scanResp = set;
#ifdef CONFIG_NIMBLE_ENABLED
  m_advDataSet = false;
#endif
}

/**
 * @brief Set the filtering for the scan filter.
 * @param [in] scanRequestWhitelistOnly If true, only allow scan requests from those on the white list.
 * @param [in] connectWhitelistOnly If true, only allow connections from those on the white list.
 */
void BLEAdvertising::setScanFilter(bool scanRequestWhitelistOnly, bool connectWhitelistOnly) {
  log_v(">> setScanFilter: scanRequestWhitelistOnly: %d, connectWhitelistOnly: %d", scanRequestWhitelistOnly, connectWhitelistOnly);
  if (!scanRequestWhitelistOnly && !connectWhitelistOnly) {

#ifdef CONFIG_BLUEDROID_ENABLED
    m_advParams.adv_filter_policy = ADV_FILTER_ALLOW_SCAN_ANY_CON_ANY;
#endif

#if defined(CONFIG_NIMBLE_ENABLED)
    m_advParams.filter_policy = BLE_HCI_ADV_FILT_NONE;
#endif

    log_v("<< setScanFilter");
    return;
  }
  if (scanRequestWhitelistOnly && !connectWhitelistOnly) {

#ifdef CONFIG_BLUEDROID_ENABLED
    m_advParams.adv_filter_policy = ADV_FILTER_ALLOW_SCAN_WLST_CON_ANY;
#endif

#if defined(CONFIG_NIMBLE_ENABLED)
    m_advParams.filter_policy = BLE_HCI_ADV_FILT_SCAN;
#endif

    log_v("<< setScanFilter");
    return;
  }
  if (!scanRequestWhitelistOnly && connectWhitelistOnly) {

#ifdef CONFIG_BLUEDROID_ENABLED
    m_advParams.adv_filter_policy = ADV_FILTER_ALLOW_SCAN_ANY_CON_WLST;
#endif

#if defined(CONFIG_NIMBLE_ENABLED)
    m_advParams.filter_policy = BLE_HCI_ADV_FILT_CONN;
#endif

    log_v("<< setScanFilter");
    return;
  }
  if (scanRequestWhitelistOnly && connectWhitelistOnly) {

#ifdef CONFIG_BLUEDROID_ENABLED
    m_advParams.adv_filter_policy = ADV_FILTER_ALLOW_SCAN_WLST_CON_WLST;
#endif

#if defined(CONFIG_NIMBLE_ENABLED)
    m_advParams.filter_policy = BLE_HCI_ADV_FILT_BOTH;
#endif

    log_v("<< setScanFilter");
    return;
  }
}  // setScanFilter

/**
 * @brief Set the advertisement data that is to be published in a regular advertisement.
 * @param [in] advertisementData The data to be advertised.
 */
bool BLEAdvertising::setAdvertisementData(BLEAdvertisementData &advertisementData) {
  log_v(">> setAdvertisementData");

#ifdef CONFIG_BLUEDROID_ENABLED
  esp_err_t errRc = ::esp_ble_gap_config_adv_data_raw((uint8_t *) &advertisementData.getPayload()[0], advertisementData.getPayload().size());
  if (errRc != ESP_OK) {
    log_e("esp_ble_gap_config_adv_data_raw: %d %s", errRc, GeneralUtils::errorToString(errRc));
  }
#endif

#if defined(CONFIG_NIMBLE_ENABLED)
  esp_err_t errRc = ble_gap_adv_set_data((uint8_t *) &advertisementData.getPayload()[0], advertisementData.getPayload().size());
  if (errRc != ESP_OK) {
    log_e("ble_gap_adv_set_data: %d %s", errRc, BLEUtils::returnCodeToString(errRc));
  }
#endif

  m_customAdvData = true;  // Set the flag that indicates we are using custom advertising data.
  log_v("<< setAdvertisementData");
  return ESP_OK == errRc;
}  // setAdvertisementData

/**
 * @brief Set the advertisement data that is to be published in a scan response.
 * @param [in] advertisementData The data to be advertised.
 */
bool BLEAdvertising::setScanResponseData(BLEAdvertisementData &advertisementData) {
  log_v(">> setScanResponseData");

#ifdef CONFIG_BLUEDROID_ENABLED
  esp_err_t errRc = ::esp_ble_gap_config_scan_rsp_data_raw((uint8_t *) &advertisementData.getPayload()[0], advertisementData.getPayload().size());
  if (errRc != ESP_OK) {
    log_e("esp_ble_gap_config_scan_rsp_data_raw: %d %s", errRc, GeneralUtils::errorToString(errRc));
  }
#endif

#if defined(CONFIG_NIMBLE_ENABLED)
  esp_err_t errRc = ble_gap_adv_rsp_set_data((uint8_t *) &advertisementData.getPayload()[0], advertisementData.getPayload().size());
  if (errRc != ESP_OK) {
    log_e("ble_gap_adv_rsp_set_data: %d %s", errRc, BLEUtils::returnCodeToString(errRc));
  }
#endif

  m_customScanResponseData = true;  // Set the flag that indicates we are using custom scan response data.
  log_v("<< setScanResponseData");
  return ESP_OK == errRc;
}  // setScanResponseData


/***************************************************************************
 *                           Bluedroid functions                           *
 ***************************************************************************/

#if defined(CONFIG_BLUEDROID_ENABLED)

void BLEAdvertising::reset() {
  if (BLEDevice::getInitialized()) {
    stop();
  }

  memset(&m_scanRespData, 0, sizeof(esp_ble_adv_data_t));
  memset(&m_advData, 0, sizeof(esp_ble_adv_data_t));
  memset(&m_advParams, 0, sizeof(esp_ble_adv_params_t));

  m_advData.set_scan_rsp = false;
  m_advData.include_name = true;
  m_advData.include_txpower = true;
  m_advData.min_interval = 0x20;
  m_advData.max_interval = 0x40;
  m_advData.appearance = 0x00;
  m_advData.manufacturer_len = 0;
  m_advData.p_manufacturer_data = nullptr;
  m_advData.service_data_len = 0;
  m_advData.p_service_data = nullptr;
  m_advData.service_uuid_len = 0;
  m_advData.p_service_uuid = nullptr;
  m_advData.flag = (ESP_BLE_ADV_FLAG_GEN_DISC | ESP_BLE_ADV_FLAG_BREDR_NOT_SPT);

  m_advParams.adv_int_min = 0x20;
  m_advParams.adv_int_max = 0x40;
  m_advParams.adv_type = ADV_TYPE_IND;
  m_advParams.own_addr_type = BLE_ADDR_TYPE_PUBLIC;
  m_advParams.channel_map = ADV_CHNL_ALL;
  m_advParams.adv_filter_policy = ADV_FILTER_ALLOW_SCAN_ANY_CON_ANY;
  m_advParams.peer_addr_type = BLE_ADDR_TYPE_PUBLIC;

  m_customAdvData = false;           // No custom advertising data
  m_customScanResponseData = false;  // No custom scan response data
}  // BLEAdvertising

void BLEAdvertising::setAdvertisementChannelMap(esp_ble_adv_channel_t channel_map) {
  m_advParams.channel_map = channel_map;
}  // setAdvertisementChannelMap

/**
 * @brief Start advertising.
 * Start advertising.
 * @return N/A.
 */
bool BLEAdvertising::start() {
  log_v(">> start: customAdvData: %d, customScanResponseData: %d", m_customAdvData, m_customScanResponseData);

  // We have a vector of service UUIDs that we wish to advertise.  In order to use the
  // ESP-IDF framework, these must be supplied in a contiguous array of their 128bit (16 byte)
  // representations.  If we have 1 or more services to advertise then we allocate enough
  // storage to host them and then copy them in one at a time into the contiguous storage.
  int numServices = m_serviceUUIDs.size();
  if (numServices > 0) {
    m_advData.service_uuid_len = 16 * numServices;
    m_advData.p_service_uuid = (uint8_t *)malloc(m_advData.service_uuid_len);
    if (!m_advData.p_service_uuid) {
      log_e(">> start failed: out of memory");
      return false;
    }

    uint8_t *p = m_advData.p_service_uuid;
    for (int i = 0; i < numServices; i++) {
      log_d("- advertising service: %s", m_serviceUUIDs[i].toString().c_str());
      BLEUUID serviceUUID128 = m_serviceUUIDs[i].to128();
      memcpy(p, serviceUUID128.getNative()->uuid.uuid128, 16);
      p += 16;
    }
  } else {
    m_advData.service_uuid_len = 0;
    log_d("- no services advertised");
  }

  esp_err_t errRc;

  if (!m_customAdvData) {
    // Set the configuration for advertising.
    m_advData.set_scan_rsp = false;
    m_advData.include_name = !m_scanResp;
    m_advData.include_txpower = !m_scanResp;
    errRc = ::esp_ble_gap_config_adv_data(&m_advData);
    if (errRc != ESP_OK) {
      log_e("<< esp_ble_gap_config_adv_data: rc=%d %s", errRc, GeneralUtils::errorToString(errRc));
      return false;
    }
  }

  if (!m_customScanResponseData && m_scanResp) {
    // Set the configuration for scan response.
    memcpy(&m_scanRespData, &m_advData, sizeof(esp_ble_adv_data_t));  // Copy the content of m_advData.
    m_scanRespData.set_scan_rsp = true;                               // Define this struct as scan response data
    m_scanRespData.include_name = true;                               // Caution: This may lead to a crash if the device name has more than 29 characters
    m_scanRespData.include_txpower = true;
    m_scanRespData.appearance = 0;  // If defined the 'Appearance' attribute is already included in the advertising data
    m_scanRespData.flag = 0;        // 'Flags' attribute should no be included in the scan response

    errRc = ::esp_ble_gap_config_adv_data(&m_scanRespData);
    if (errRc != ESP_OK) {
      log_e("<< esp_ble_gap_config_adv_data (Scan response): rc=%d %s", errRc, GeneralUtils::errorToString(errRc));
      return false;
    }
  }

  // If we had services to advertise then we previously allocated some storage for them.
  // Here we release that storage.
  free(m_advData.p_service_uuid);  //TODO change this variable to local scope?
  m_advData.p_service_uuid = nullptr;

  // Start advertising.
  errRc = ::esp_ble_gap_start_advertising(&m_advParams);
  if (errRc != ESP_OK) {
    log_e("<< esp_ble_gap_start_advertising: rc=%d %s", errRc, GeneralUtils::errorToString(errRc));
  } else {
    log_v("<< start");
  }
  return ESP_OK == errRc;
}  // start

/**
 * @brief Stop advertising.
 * Stop advertising.
 * @return N/A.
 */
bool BLEAdvertising::stop() {
  log_v(">> stop");
  esp_err_t errRc = ::esp_ble_gap_stop_advertising();
  if (errRc != ESP_OK) {
    log_e("esp_ble_gap_stop_advertising: rc=%d %s", errRc, GeneralUtils::errorToString(errRc));
  } else {
    log_v("<< stop");
  }
  return ESP_OK == errRc;
}  // stop

void BLEAdvertising::handleGAPEvent(esp_gap_ble_cb_event_t event, esp_ble_gap_cb_param_t *param) {
  log_d("handleGAPEvent [event no: %d]", (int)event);

  switch (event) {
    case ESP_GAP_BLE_ADV_DATA_SET_COMPLETE_EVT:
    {
      // m_semaphoreSetAdv.give();
      break;
    }
    case ESP_GAP_BLE_SCAN_RSP_DATA_SET_COMPLETE_EVT:
    {
      // m_semaphoreSetAdv.give();
      break;
    }
    case ESP_GAP_BLE_ADV_START_COMPLETE_EVT:
    {
      // m_semaphoreSetAdv.give();
      break;
    }
    case ESP_GAP_BLE_ADV_STOP_COMPLETE_EVT:
    {
      log_i("STOP advertising");
      //start();
      break;
    }
    default: break;
  }
}

/**
 * @brief Set BLE address.
 * @param [in] Bluetooth address.
 * @param [in] Bluetooth address type.
 * Set BLE address.
 */
bool BLEAdvertising::setDeviceAddress(esp_bd_addr_t addr, esp_ble_addr_type_t type) {
  log_v(">> setPrivateAddress");

  m_advParams.own_addr_type = type;
  esp_err_t errRc = esp_ble_gap_set_rand_addr((uint8_t *)addr);
  if (errRc != ESP_OK) {
    log_e("esp_ble_gap_set_rand_addr: rc=%d %s", errRc, GeneralUtils::errorToString(errRc));
  } else {
    log_v("<< setPrivateAddress");
  }
  return ESP_OK == errRc;
}  // setPrivateAddress

#if defined(SOC_BLE_50_SUPPORTED)

/**
* @brief           Creator
*
* @param[in]       instance : number of multi advertising instances
*
*
*/
BLEMultiAdvertising::BLEMultiAdvertising(uint8_t num) {
  params_arrays = (esp_ble_gap_ext_adv_params_t *)calloc(num, sizeof(esp_ble_gap_ext_adv_params_t));
  ext_adv = (esp_ble_gap_ext_adv_t *)calloc(num, sizeof(esp_ble_gap_ext_adv_t));
  count = num;
}

/**
* @brief           This function is used by the Host to set the advertising parameters.
*
* @param[in]       instance : identifies the advertising set whose parameters are being configured.
* @param[in]       params   : advertising parameters
*
* @return            - true : success
*                    - false  : failed
*
*/
bool BLEMultiAdvertising::setAdvertisingParams(uint8_t instance, const esp_ble_gap_ext_adv_params_t *params) {
  if (params->type == ESP_BLE_GAP_SET_EXT_ADV_PROP_LEGACY_IND && params->primary_phy == ESP_BLE_GAP_PHY_2M) {
    return false;
  }
  esp_err_t rc;
  rc = esp_ble_gap_ext_adv_set_params(instance, params);

  return ESP_OK == rc;
}

/**
* @brief           This function is used to set the data used in advertising PDUs that have a data field
*
* @param[in]       instance : identifies the advertising set whose data are being configured
* @param[in]       length   : data length
* @param[in]       data     : data information
*
* @return            - true : success
*                    - false  : failed
*
*/
bool BLEMultiAdvertising::setAdvertisingData(uint8_t instance, uint16_t length, const uint8_t *data) {
  esp_err_t rc;
  rc = esp_ble_gap_config_ext_adv_data_raw(instance, length, data);
  if (rc) {
    log_e("set advertising data err: %d", rc);
  }

  return ESP_OK == rc;
}

bool BLEMultiAdvertising::setScanRspData(uint8_t instance, uint16_t length, const uint8_t *data) {
  esp_err_t rc;
  rc = esp_ble_gap_config_ext_scan_rsp_data_raw(instance, length, data);
  if (rc) {
    log_e("set scan resp data err: %d", rc);
  }

  return ESP_OK == rc;
}

/**
* @brief           This function is used to request the Controller to enable one or more
*                  advertising sets using the advertising sets identified by the instance parameter.
*
* @return            - true : success
*                    - false  : failed
*
*/
bool BLEMultiAdvertising::start() {
  return start(count, 0);
}

/**
* @brief           This function is used to request the Controller to enable one or more
*                  advertising sets using the advertising sets identified by the instance parameter.
*
* @param[in]       num : Number of advertising sets to enable or disable
* @param[in]       from : first sxt adv set to use
*
* @return            - true : success
*                    - false  : failed
*
*/
bool BLEMultiAdvertising::start(uint8_t num, uint8_t from) {
  if (num > count || from >= count) {
    return false;
  }

  esp_err_t rc;
  rc = esp_ble_gap_ext_adv_start(num, &ext_adv[from]);
  if (rc) {
    log_e("start extended advertising err: %d", rc);
  }

  return ESP_OK == rc;
}

/**
* @brief           This function is used to request the Controller to disable one or more
*                  advertising sets using the advertising sets identified by the instance parameter.
*
* @param[in]       num_adv : Number of advertising sets to enable or disable
* @param[in]       ext_adv_inst : ext adv instance
*
* @return            - ESP_OK : success
*                    - other  : failed
*
*/
bool BLEMultiAdvertising::stop(uint8_t num_adv, const uint8_t *ext_adv_inst) {
  esp_err_t rc;
  rc = esp_ble_gap_ext_adv_stop(num_adv, ext_adv_inst);
  if (rc) {
    log_e("stop extended advertising err: %d", rc);
  }

  return ESP_OK == rc;
}

/**
* @brief           This function is used to remove an advertising set from the Controller.
*
* @param[in]       instance : Used to identify an advertising set
*
* @return            - ESP_OK : success
*                    - other  : failed
*
*/
bool BLEMultiAdvertising::remove(uint8_t instance) {
  esp_err_t rc;
  rc = esp_ble_gap_ext_adv_set_remove(instance);
  if (rc) {
    log_e("remove extended advertising err: %d", rc);
  }

  return ESP_OK == rc;
}

/**
* @brief           This function is used to remove all existing advertising sets from the Controller.
*
*
* @return            - ESP_OK : success
*                    - other  : failed
*
*/
bool BLEMultiAdvertising::clear() {
  esp_err_t rc;
  rc = esp_ble_gap_ext_adv_set_clear();
  if (rc) {
    log_e("clear extended advertising err: %d", rc);
  }

  return ESP_OK == rc;
}

/**
* @brief           This function is used by the Host to set the random device address specified by the Random_Address parameter.
*
* @param[in]       instance  : Used to identify an advertising set
* @param[in]       addr_legacy : Random Device Address
*
* @return            - true : success
*                    - false  : failed
*
*/
bool BLEMultiAdvertising::setInstanceAddress(uint8_t instance, uint8_t *addr_legacy) {
  esp_err_t rc;
  rc = esp_ble_gap_ext_adv_set_rand_addr(instance, addr_legacy);
  if (rc) {
    log_e("set random address err: %d", rc);
  }

  return ESP_OK == rc;
}

/**
* @brief           This function is used by the Host to set the parameters for periodic advertising.
*
* @param[in]       instance : identifies the advertising set whose periodic advertising parameters are being configured.
* @param[in]       params : periodic adv parameters
*
* @return            - true : success
*                    - false  : failed
*
*/
bool BLEMultiAdvertising::setPeriodicAdvertisingParams(uint8_t instance, const esp_ble_gap_periodic_adv_params_t *params) {
  esp_err_t rc;
  rc = esp_ble_gap_periodic_adv_set_params(instance, params);
  if (rc) {
    log_e("set periodic advertising params err: %d", rc);
  }

  return ESP_OK == rc;
}

/**
* @brief           This function is used to set the data used in periodic advertising PDUs.
*
* @param[in]       instance : identifies the advertising set whose periodic advertising parameters are being configured.
* @param[in]       length : the length of periodic data
* @param[in]       data : periodic data information
*
* @return            - true : success
*                    - false  : failed
*
*/
bool BLEMultiAdvertising::setPeriodicAdvertisingData(uint8_t instance, uint16_t length, const uint8_t *data) {
  esp_err_t rc;
  rc = esp_ble_gap_config_periodic_adv_data_raw(instance, length, data);
  if (rc) {
    log_e("set periodic advertising raw data err: %d", rc);
  }

  return ESP_OK == rc;
}

/**
* @brief           This function is used to request the Controller to enable the periodic advertising for the advertising set specified
*
* @param[in]       instance : Used to identify an advertising set
*
* @return            - true : success
*                    - false  : failed
*
*/
bool BLEMultiAdvertising::startPeriodicAdvertising(uint8_t instance) {
  esp_err_t rc;
  rc = esp_ble_gap_periodic_adv_start(instance);
  if (rc) {
    log_e("start periodic advertising err: %d", rc);
  }

  return ESP_OK == rc;
}

void BLEMultiAdvertising::setDuration(uint8_t instance, int duration, int max_events) {
  ext_adv[instance] = {instance, duration, max_events};
}

#endif /* SOC_BLE_50_SUPPORTED */

#endif /* CONFIG_BLUEDROID_ENABLED */

/***************************************************************************
 *                           NimBLE functions                              *
 ***************************************************************************/

#if defined(CONFIG_NIMBLE_ENABLED)

void BLEAdvertising::reset() {
  if (BLEDevice::getInitialized() && isAdvertising()) {
    stop();
  }
  memset(&m_advData, 0, sizeof m_advData);
  memset(&m_scanData, 0, sizeof m_scanData);
  memset(&m_advParams, 0, sizeof m_advParams);
  memset(&m_slaveItvl, 0, sizeof m_slaveItvl);
  const char *name = ble_svc_gap_device_name();

  m_advData.name = (uint8_t *)name;
  m_advData.name_len = strlen(name);
  m_advData.name_is_complete = 1;
  m_advData.tx_pwr_lvl = BLEDevice::getPower();
  m_advData.flags = (BLE_HS_ADV_F_DISC_GEN | BLE_HS_ADV_F_BREDR_UNSUP);
  m_advParams.conn_mode = BLE_GAP_CONN_MODE_UND;
  m_advParams.disc_mode = BLE_GAP_DISC_MODE_GEN;
  m_customAdvData = false;
  m_customScanResponseData = false;
  m_scanResp = true;
  m_advDataSet = false;
  // Set this to non-zero to prevent auto start if host reset before started by app.
  m_duration = BLE_HS_FOREVER;
  m_advCompCB = nullptr;
}  // BLEAdvertising

void BLEAdvertising::setName(String name) {
  m_name = name;
  m_advData.name = (uint8_t *)m_name.c_str();
  m_advData.name_len = m_name.length();
  m_advDataSet = false;
}

/**
 * @brief Add the transmission power level to the advertisement packet.
 */
void BLEAdvertising::addTxPower() {
  m_advData.tx_pwr_lvl_is_present = 1;
  m_advDataSet = false;
}  // addTxPower

/**
 * @brief Handles the callback when advertising stops.
 */
void BLEAdvertising::advCompleteCB() {
  if (m_advCompCB != nullptr) {
    m_advCompCB(this);
  }
}  // advCompleteCB

/**
 * @brief Check if currently advertising.
 * @return true if advertising is active.
 */
bool BLEAdvertising::isAdvertising() {
  return ble_gap_adv_active();
}  // isAdvertising

/*
 * Host reset seems to clear advertising data,
 * we need clear the flag so it reloads it.
 */
void BLEAdvertising::onHostSync() {
  log_v("Host re-synced");

  m_advDataSet = false;
  // If we were advertising forever, restart it now
  if (m_duration == 0) {
    start(m_duration, m_advCompCB);
  } else {
    // Otherwise we should tell the app that advertising stopped.
    advCompleteCB();
  }
}  // onHostSync

/**
 * @brief Handler for gap events when not using peripheral role.
 * @param [in] event the event data.
 * @param [in] arg pointer to the advertising instance.
 */
int BLEAdvertising::handleGAPEvent(struct ble_gap_event *event, void *arg) {
  BLEAdvertising *pAdv = (BLEAdvertising *)arg;

  if (event->type == BLE_GAP_EVENT_ADV_COMPLETE) {
    switch (event->adv_complete.reason) {
      // Don't call the callback if host reset, we want to
      // preserve the active flag until re-sync to restart advertising.
      case BLE_HS_ETIMEOUT_HCI:
      case BLE_HS_EOS:
      case BLE_HS_ECONTROLLER:
      case BLE_HS_ENOTSYNCED:
        log_e("host reset, rc=%d", event->adv_complete.reason);
        BLEDevice::onReset(event->adv_complete.reason);
        return 0;
      default: break;
    }
    pAdv->advCompleteCB();
  }
  return 0;
}

/**
 * @brief Start advertising.
 * @param [in] duration The duration, in seconds, to advertise, 0 == advertise forever.
 * @param [in] advCompleteCB A pointer to a callback to be invoked when advertising ends.
 * @return True if advertising started successfully.
 */
bool BLEAdvertising::start(uint32_t duration, void (*advCompleteCB)(BLEAdvertising *pAdv)) {
  log_v(">> Advertising start: customAdvData: %d, customScanResponseData: %d", m_customAdvData, m_customScanResponseData);

  // If Host is not synced we cannot start advertising.
  if (!BLEDevice::m_synced) {
    log_e("Host reset, wait for sync.");
    return false;
  }

  // If already advertising just return
  if (ble_gap_adv_active()) {
    log_w("Advertising already active");
    return true;
  }

  BLEServer *pServer = BLEDevice::getServer();
  if (pServer != nullptr) {
    if (!pServer->m_gattsStarted) {
      pServer->start();
    } else if (pServer->getConnectedCount() >= CONFIG_BT_NIMBLE_MAX_CONNECTIONS) {
      log_e("Max connections reached - not advertising");
      return false;
    }
  }

  // Save the duration in case of host reset so we can restart with the same parameters
  m_duration = duration;

  if (duration == 0) {
    duration = BLE_HS_FOREVER;
  } else {
    duration = duration * 1000;  // convert duration to milliseconds
  }

  m_advCompCB = advCompleteCB;

  m_advParams.disc_mode = BLE_GAP_DISC_MODE_GEN;
  m_advData.flags = (BLE_HS_ADV_F_DISC_GEN | BLE_HS_ADV_F_BREDR_UNSUP);
  if (m_advParams.conn_mode == BLE_GAP_CONN_MODE_NON) {
    if (!m_scanResp) {
      m_advParams.disc_mode = BLE_GAP_DISC_MODE_NON;
      m_advData.flags = BLE_HS_ADV_F_BREDR_UNSUP;
    }
  }

  int rc = 0;

  if (!m_customAdvData && !m_advDataSet) {
    //start with 3 bytes for the flags data
    uint8_t payloadLen = (2 + 1);
    if (m_advData.mfg_data_len > 0) {
      payloadLen += (2 + m_advData.mfg_data_len);
    }

    if (m_advData.svc_data_uuid16_len > 0) {
      payloadLen += (2 + m_advData.svc_data_uuid16_len);
    }

    if (m_advData.svc_data_uuid32_len > 0) {
      payloadLen += (2 + m_advData.svc_data_uuid32_len);
    }

    if (m_advData.svc_data_uuid128_len > 0) {
      payloadLen += (2 + m_advData.svc_data_uuid128_len);
    }

    if (m_advData.uri_len > 0) {
      payloadLen += (2 + m_advData.uri_len);
    }

    if (m_advData.appearance_is_present) {
      payloadLen += (2 + BLE_HS_ADV_APPEARANCE_LEN);
    }

    if (m_advData.tx_pwr_lvl_is_present) {
      payloadLen += (2 + BLE_HS_ADV_TX_PWR_LVL_LEN);
    }

    if (m_advData.slave_itvl_range != nullptr) {
      payloadLen += (2 + BLE_HS_ADV_SLAVE_ITVL_RANGE_LEN);
    }

    for (auto &it : m_serviceUUIDs) {
      if (it.getNative()->u.type == BLE_UUID_TYPE_16) {
        int add = (m_advData.num_uuids16 > 0) ? 2 : 4;
        if ((payloadLen + add) > BLE_HS_ADV_MAX_SZ) {
          m_advData.uuids16_is_complete = 0;
          continue;
        }
        payloadLen += add;

        if (nullptr == (m_advData.uuids16 = (ble_uuid16_t *)realloc((void *)m_advData.uuids16, (m_advData.num_uuids16 + 1) * sizeof(ble_uuid16_t)))) {
          log_e("Error, no mem");
          abort();
        }
        memcpy((void *)&m_advData.uuids16[m_advData.num_uuids16], &it.getNative()->u16, sizeof(ble_uuid16_t));
        m_advData.uuids16_is_complete = 1;
        m_advData.num_uuids16++;
      }
      if (it.getNative()->u.type == BLE_UUID_TYPE_32) {
        int add = (m_advData.num_uuids32 > 0) ? 4 : 6;
        if ((payloadLen + add) > BLE_HS_ADV_MAX_SZ) {
          m_advData.uuids32_is_complete = 0;
          continue;
        }
        payloadLen += add;

        if (nullptr == (m_advData.uuids32 = (ble_uuid32_t *)realloc((void *)m_advData.uuids32, (m_advData.num_uuids32 + 1) * sizeof(ble_uuid32_t)))) {
          log_e("Error, no mem");
          abort();
        }
        memcpy((void *)&m_advData.uuids32[m_advData.num_uuids32], &it.getNative()->u32, sizeof(ble_uuid32_t));
        m_advData.uuids32_is_complete = 1;
        m_advData.num_uuids32++;
      }
      if (it.getNative()->u.type == BLE_UUID_TYPE_128) {
        int add = (m_advData.num_uuids128 > 0) ? 16 : 18;
        if ((payloadLen + add) > BLE_HS_ADV_MAX_SZ) {
          m_advData.uuids128_is_complete = 0;
          continue;
        }
        payloadLen += add;

        if (nullptr == (m_advData.uuids128 = (ble_uuid128_t *)realloc((void *)m_advData.uuids128, (m_advData.num_uuids128 + 1) * sizeof(ble_uuid128_t)))) {
          log_e("Error, no mem");
          abort();
        }
        memcpy((void *)&m_advData.uuids128[m_advData.num_uuids128], &it.getNative()->u128, sizeof(ble_uuid128_t));
        m_advData.uuids128_is_complete = 1;
        m_advData.num_uuids128++;
      }
    }

    // check if there is room for the name, if not put it in scan data
    if ((payloadLen + (2 + m_advData.name_len)) > BLE_HS_ADV_MAX_SZ) {
      if (m_scanResp && !m_customScanResponseData) {
        m_scanData.name = m_advData.name;
        m_scanData.name_len = m_advData.name_len;
        if (m_scanData.name_len > BLE_HS_ADV_MAX_SZ - 2) {
          m_scanData.name_len = BLE_HS_ADV_MAX_SZ - 2;
          m_scanData.name_is_complete = 0;
        } else {
          m_scanData.name_is_complete = 1;
        }
        m_advData.name = nullptr;
        m_advData.name_len = 0;
        m_advData.name_is_complete = 0;
      } else {
        if (m_advData.tx_pwr_lvl_is_present) {
          m_advData.tx_pwr_lvl_is_present = 0;
          payloadLen -= (2 + 1);
        }
        // if not using scan response just cut the name down
        // leaving 2 bytes for the data specifier.
        if (m_advData.name_len > (BLE_HS_ADV_MAX_SZ - payloadLen - 2)) {
          m_advData.name_len = (BLE_HS_ADV_MAX_SZ - payloadLen - 2);
          m_advData.name_is_complete = 0;
        }
      }
    }

    if (m_scanResp && !m_customScanResponseData) {
      rc = ble_gap_adv_rsp_set_fields(&m_scanData);
      switch (rc) {
        case 0: break;

        case BLE_HS_EBUSY: log_e("Already advertising"); break;

        case BLE_HS_EMSGSIZE: log_e("Scan data too long"); break;

        default: log_e("Error setting scan response data; rc=%d, %s", rc, BLEUtils::returnCodeToString(rc)); break;
      }
    }

    if (rc == 0) {
      rc = ble_gap_adv_set_fields(&m_advData);
      switch (rc) {
        case 0: break;

        case BLE_HS_EBUSY: log_e("Already advertising"); break;

        case BLE_HS_EMSGSIZE: log_e("Advertisement data too long"); break;

        default: log_e("Error setting advertisement data; rc=%d, %s", rc, BLEUtils::returnCodeToString(rc)); break;
      }
    }

    if (m_advData.num_uuids128 > 0) {
      free((void *)m_advData.uuids128);
      m_advData.uuids128 = nullptr;
      m_advData.num_uuids128 = 0;
    }

    if (m_advData.num_uuids32 > 0) {
      free((void *)m_advData.uuids32);
      m_advData.uuids32 = nullptr;
      m_advData.num_uuids32 = 0;
    }

    if (m_advData.num_uuids16 > 0) {
      free((void *)m_advData.uuids16);
      m_advData.uuids16 = nullptr;
      m_advData.num_uuids16 = 0;
    }

    if (rc != 0) {
      return false;
    }

    m_advDataSet = true;
  }

  rc = ble_gap_adv_start(
    BLEDevice::m_ownAddrType, NULL, duration, &m_advParams, (pServer != nullptr) ? BLEServer::handleGATTServerEvent : BLEAdvertising::handleGAPEvent,
    (pServer != nullptr) ? (void *)pServer : (void *)this
  );

  switch (rc) {
    case 0: break;

    case BLE_HS_EINVAL: log_e("Unable to advertise - Duration too long"); break;

    case BLE_HS_EPREEMPTED: log_e("Unable to advertise - busy"); break;

    case BLE_HS_ETIMEOUT_HCI:
    case BLE_HS_EOS:
    case BLE_HS_ECONTROLLER:
    case BLE_HS_ENOTSYNCED:   log_e("Unable to advertise - Host Reset"); break;

    default: log_e("Error enabling advertising; rc=%d, %s", rc, BLEUtils::returnCodeToString(rc)); break;
  }

  log_d("<< Advertising start");
  return (rc == 0);
}  // start

/**
 * @brief Stop advertising.
 */
bool BLEAdvertising::stop() {
  log_d(">> stop");

  int rc = ble_gap_adv_stop();
  if (rc != 0 && rc != BLE_HS_EALREADY) {
    log_e("ble_gap_adv_stop rc=%d %s", rc, BLEUtils::returnCodeToString(rc));
    return false;
  }

  log_d("<< stop");
  return true;
}  // stop

#endif /* CONFIG_NIMBLE_ENABLED */




/* BLEAdvistermentData class functions 
*
*
*/


bool BLEAdvertisementData::addData(const uint8_t* data, size_t length) {
   if (m_payload.size() + length > BLE_HS_ADV_MAX_SZ) {
       log_e( "Data length exceeded");
        return false;
    }

    m_payload.insert(m_payload.end(), data, data + length);
    return true;
} // addData

/**
 * @brief Add data to the payload to be advertised.
 * @param [in] data The data to be added to the payload.
 */
bool BLEAdvertisementData::addData(const std::vector<uint8_t>& data) {
    return addData(&data[0], data.size());
} // addData

/**
 * @brief Set the appearance.
 * @param [in] appearance The appearance code value.
 * @return True if successful.
 * @details If the appearance value is 0 then it will be removed from the advertisement if set previously.
 */
bool BLEAdvertisementData::setAppearance(uint16_t appearance) {
    if (appearance == 0) {
        return removeData(BLE_HS_ADV_TYPE_APPEARANCE);
    }

    uint8_t data[4];
    data[0] = 3;
    data[1] = BLE_HS_ADV_TYPE_APPEARANCE;
    data[2] = appearance;
    data[3] = (appearance >> 8) & 0xFF;
    return addData(data, 4);
} // setAppearance

/**
 * @brief Set the advertisement flags.
 * @param [in] flag The flags to be set in the advertisement.
 * * BLE_HS_ADV_F_DISC_LTD
 * * BLE_HS_ADV_F_DISC_GEN
 * * BLE_HS_ADV_F_BREDR_UNSUP - must always use with NimBLE
 * A flag value of 0 will remove the flags from the advertisement.
 */
bool BLEAdvertisementData::setFlags(uint8_t flag) {
    int dataLoc = getDataLocation(BLE_HS_ADV_TYPE_FLAGS);
    if (dataLoc != -1) {
        if (flag) {
            m_payload[dataLoc + 2] = flag | BLE_HS_ADV_F_BREDR_UNSUP;
            return true;
        } else {
            return removeData(BLE_HS_ADV_TYPE_FLAGS);
        }
    }

    uint8_t data[3];
    data[0] = 2;
    data[1] = BLE_HS_ADV_TYPE_FLAGS;
    data[2] = flag | BLE_HS_ADV_F_BREDR_UNSUP;
    return addData(data, 3);
} // setFlags

/**
 * @brief Adds Tx power level to the advertisement data.
 * @return True if successful.
 */
bool BLEAdvertisementData::addTxPower() {
    uint8_t data[3];
    data[0] = BLE_HS_ADV_TX_PWR_LVL_LEN + 1;
    data[1] = BLE_HS_ADV_TYPE_TX_PWR_LVL;
# ifndef CONFIG_IDF_TARGET_ESP32P4
    data[2] = BLEDevice::getPower();
# else
    data[2] = 0;
# endif
    return addData(data, 3);
} // addTxPower

/**
 * @brief Set the preferred min and max connection intervals to advertise.
 * @param [in] minInterval The minimum preferred connection interval.
 * @param [in] maxInterval The Maximum preferred connection interval.
 * @details  Range = 0x0006(7.5ms) to 0x0C80(4000ms), values not within the range will be limited to this range.
 * @return True if successful.
 */
bool BLEAdvertisementData::setPreferredParams(uint16_t minInterval, uint16_t maxInterval) {
    minInterval = std::max<uint16_t>(minInterval, 0x6);
    minInterval = std::min<uint16_t>(minInterval, 0xC80);
    maxInterval = std::max<uint16_t>(maxInterval, 0x6);
    maxInterval = std::min<uint16_t>(maxInterval, 0xC80);
    maxInterval = std::max<uint16_t>(maxInterval, minInterval); // Max must be greater than or equal to min.

    uint8_t data[6];
    data[0] = BLE_HS_ADV_SLAVE_ITVL_RANGE_LEN + 1;
    data[1] = BLE_HS_ADV_TYPE_SLAVE_ITVL_RANGE;
    data[2] = minInterval;
    data[3] = minInterval >> 8;
    data[4] = maxInterval;
    data[5] = maxInterval >> 8;
    return addData(data, 6);
} // setPreferredParams

/**
 * @brief Add a service uuid to exposed list of services.
 * @param [in] serviceUUID The UUID of the service to expose.
 */
bool BLEAdvertisementData::addServiceUUID(const BLEUUID& serviceUUID) {
    uint8_t bytes = serviceUUID.bitSize() / 8;
    int     type;
    switch (bytes) {
        case 2:
            type = BLE_HS_ADV_TYPE_COMP_UUIDS16;
            break;
        case 4:
            type = BLE_HS_ADV_TYPE_COMP_UUIDS32;
            break;
        case 16:
            type = BLE_HS_ADV_TYPE_COMP_UUIDS128;
            break;
        default:
            log_e("Cannot add UUID, invalid size!");
            return false;
    }

    int     dataLoc = getDataLocation(type);
    uint8_t length  = bytes;
    if (dataLoc == -1) {
        length += 2;
    }

    if (length + getPayload().size() > BLE_HS_ADV_MAX_SZ) {
        log_e( "Cannot add UUID, data length exceeded!");
        return false;
    }

    uint8_t        data[BLE_HS_ADV_MAX_SZ];
    const uint8_t* uuid = serviceUUID.getValue();
    if (dataLoc == -1) {
        data[0] = 1 + bytes;
        data[1] = type;
        memcpy(&data[2], uuid, bytes);
        return addData(data, length);
    }

    m_payload.insert(m_payload.begin() + dataLoc + m_payload[dataLoc] + 1, uuid, uuid + bytes);
    m_payload[dataLoc] += bytes;
    return true;
} // addServiceUUID

/**
 * @brief Add a service uuid to exposed list of services.
 * @param [in] serviceUUID The string representation of the service to expose.
 * @return True if successful.
 */
bool BLEAdvertisementData::addServiceUUID(const char* serviceUUID) {
    return addServiceUUID(BLEUUID(serviceUUID));
} // addServiceUUID

/**
 * @brief Remove a service UUID from the advertisement.
 * @param [in] serviceUUID The UUID of the service to remove.
 * @return True if successful or uuid not found, false if uuid error or data could not be reset.
 */
bool BLEAdvertisementData::removeServiceUUID(const BLEUUID& serviceUUID) {
    uint8_t bytes = serviceUUID.bitSize() / 8;
    int     type;
    switch (bytes) {
        case 2:
            type = BLE_HS_ADV_TYPE_COMP_UUIDS16;
            break;
        case 4:
            type = BLE_HS_ADV_TYPE_COMP_UUIDS32;
            break;
        case 16:
            type = BLE_HS_ADV_TYPE_COMP_UUIDS128;
            break;
        default:
            log_e("Cannot remove UUID, invalid size!");
            return false;
    }

    int dataLoc = getDataLocation(type);
    if (dataLoc == -1) {
        return true;
    }

    int uuidLoc = -1;
    for (size_t i = dataLoc + 2; i < m_payload.size(); i += bytes) {
        if (memcmp(&m_payload[i], serviceUUID.getValue(), bytes) == 0) {
            uuidLoc = i;
            break;
        }
    }

    if (uuidLoc == -1) {
        return true;
    }

    if (m_payload[dataLoc] - bytes == 1) {
        return removeData(type);
    }

    m_payload.erase(m_payload.begin() + uuidLoc, m_payload.begin() + uuidLoc + bytes);
    m_payload[dataLoc] -= bytes;
    return true;
} // removeServiceUUID

/**
 * @brief Remove a service UUID from the advertisement.
 * @param [in] serviceUUID The UUID of the service to remove.
 * @return True if successful or uuid not found, false if uuid error or data could not be reset.
 */
bool BLEAdvertisementData::removeServiceUUID(const char* serviceUUID) {
    return removeServiceUUID(BLEUUID(serviceUUID));
} // removeServiceUUID

/**
 * @brief Remove all service UUIDs from the advertisement.
 */
bool BLEAdvertisementData::removeServices() {
    return true;
} // removeServices

/**
 * @brief Set manufacturer specific data.
 * @param [in] data The manufacturer data to advertise.
 * @param [in] length The length of the data.
 * @return True if successful.
 */
bool BLEAdvertisementData::setManufacturerData(const uint8_t* data, size_t length) {
    if (length > BLE_HS_ADV_MAX_FIELD_SZ) {
        log_e( "MFG data too long");
        return false;
    }

    uint8_t mdata[BLE_HS_ADV_MAX_SZ];
    mdata[0] = length + 1;
    mdata[1] = BLE_HS_ADV_TYPE_MFG_DATA;
    memcpy(&mdata[2], data, length);
    return addData(mdata, length + 2);
} // setManufacturerData

/**
 * @brief Set manufacturer specific data.
 * @param [in] data The manufacturer data to advertise.
 * @return True if successful.
 */
bool BLEAdvertisementData::setManufacturerData(const std::string& data) {
    return setManufacturerData(reinterpret_cast<const uint8_t*>(data.data()), data.length());
} // setManufacturerData

/**
 * @brief Set manufacturer specific data.
 * @param [in] data The manufacturer data to advertise.
 * @return True if successful.
 */
bool BLEAdvertisementData::setManufacturerData(const std::vector<uint8_t>& data) {
    return setManufacturerData(&data[0], data.size());
} // setManufacturerData

/**
 * @brief Set the URI to advertise.
 * @param [in] uri The uri to advertise.
 * @return True if successful.
 */
bool BLEAdvertisementData::setURI(const std::string& uri) {
    if (uri.length() > BLE_HS_ADV_MAX_FIELD_SZ) {
        log_e( "URI too long");
        return false;
    }

    uint8_t data[BLE_HS_ADV_MAX_SZ];
    uint8_t length = 2 + uri.length();
    data[0]        = length - 1;
    data[1]        = BLE_HS_ADV_TYPE_URI;
    memcpy(&data[2], uri.c_str(), uri.length());
    return addData(data, length);
} // setURI

/**
 * @brief Set the complete name of this device.
 * @param [in] name The name to advertise.
 * @param [in] isComplete If true the name is complete, which will set the data type accordingly.
 * @details If the name is longer than 29 characters it will be truncated.
 * and the data type will be set to incomplete name.
 * @return True if successful.
 */
bool BLEAdvertisementData::setName(const std::string& name, bool isComplete) {
    if (name.length() > BLE_HS_ADV_MAX_FIELD_SZ) {
        log_e( "Name too long - truncating");
        isComplete = false;
    }

    uint8_t data[BLE_HS_ADV_MAX_SZ];
    uint8_t length = 2 + std::min<uint8_t>(name.length(), BLE_HS_ADV_MAX_FIELD_SZ);
    data[0]        = length - 1;
    data[1]        = isComplete ? BLE_HS_ADV_TYPE_COMP_NAME : BLE_HS_ADV_TYPE_INCOMP_NAME;
    memcpy(&data[2], name.c_str(), std::min<uint8_t>(name.length(), BLE_HS_ADV_MAX_FIELD_SZ));
    return addData(data, length);
} // setName

/**
 * @brief Set the short name.
 * @param [in] name The short name of the device.
 * @return True if successful.
 */
bool BLEAdvertisementData::setShortName(const std::string& name) {
    return setName(name, false);
} // setShortName

/**
 * @brief Set a single service to advertise as a complete list of services.
 * @param [in] uuid The service to advertise.
 * @return True if successful.
 */
bool BLEAdvertisementData::setCompleteServices(const BLEUUID& uuid) {
    return setServices(true, uuid.bitSize(), {uuid});
} // setCompleteServices

/**
 * @brief Set the complete list of 16 bit services to advertise.
 * @param [in] uuids A vector of 16 bit UUID's to advertise.
 * @return True if successful.
 */
bool BLEAdvertisementData::setCompleteServices16(const std::vector<BLEUUID>& uuids) {
    return setServices(true, 16, uuids);
} // setCompleteServices16

/**
 * @brief Set the complete list of 32 bit services to advertise.
 * @param [in] uuids A vector of 32 bit UUID's to advertise.
 * @return True if successful.
 */
bool BLEAdvertisementData::setCompleteServices32(const std::vector<BLEUUID>& uuids) {
    return setServices(true, 32, uuids);
} // setCompleteServices32

/**
 * @brief Set a single service to advertise as a partial list of services.
 * @param [in] uuid The service to advertise.
 * @return True if successful.
 */
bool BLEAdvertisementData::setPartialServices(const BLEUUID& uuid) {
    return setServices(false, uuid.bitSize(), {uuid});
} // setPartialServices

/**
 * @brief Set the partial list of services to advertise.
 * @param [in] uuids A vector of 16 bit UUID's to advertise.
 * @return True if successful.
 */
bool BLEAdvertisementData::setPartialServices16(const std::vector<BLEUUID>& uuids) {
    return setServices(false, 16, uuids);
} // setPartialServices16

/**
 * @brief Set the partial list of services to advertise.
 * @param [in] uuids A vector of 32 bit UUID's to advertise.
 * @return True if successful.
 */
bool BLEAdvertisementData::setPartialServices32(const std::vector<BLEUUID>& uuids) {
    return setServices(false, 32, uuids);
} // setPartialServices32

/**
 * @brief Utility function to create the list of service UUID's from a vector.
 * @param [in] complete If true the vector is the complete set of services.
 * @param [in] size The bit size of the UUID's in the vector. (16, 32, or 128).
 * @param [in] uuids The vector of service UUID's to advertise.
 * @return True if successful.
 * @details The number of services will be truncated if the total length exceeds 31 bytes.
 */
bool BLEAdvertisementData::setServices(bool complete, uint8_t size, const std::vector<BLEUUID>& uuids) {
    uint8_t bytes  = size / 8;
    uint8_t length = 2; // start with 2 for length + type bytes
    uint8_t data[BLE_HS_ADV_MAX_SZ];

    for (const auto& uuid : uuids) {
        if (uuid.bitSize() != size) {
            log_e( "Service UUID(%d) invalid", size);
            continue;
        } else {
            if (length + bytes >= BLE_HS_ADV_MAX_SZ) {
                log_w("Too many services - truncating");
                complete = false;
                break;
            }
            memcpy(&data[length], uuid.getValue(), bytes);
            length += bytes;
        }
    }

    data[0] = length - 1; // don't count the length byte as part of the AD length

    switch (size) {
        case 16:
            data[1] = (complete ? BLE_HS_ADV_TYPE_COMP_UUIDS16 : BLE_HS_ADV_TYPE_INCOMP_UUIDS16);
            break;
        case 32:
            data[1] = (complete ? BLE_HS_ADV_TYPE_COMP_UUIDS32 : BLE_HS_ADV_TYPE_INCOMP_UUIDS32);
            break;
        case 128:
            data[1] = (complete ? BLE_HS_ADV_TYPE_COMP_UUIDS128 : BLE_HS_ADV_TYPE_INCOMP_UUIDS128);
            break;
        default:
            log_e( "Cannot set services, invalid size!");
            return false;
    }

    return addData(data, length);
} // setServices

/**
 * @brief Set the service data advertised for the UUID.
 * @param [in] uuid The UUID the service data belongs to.
 * @param [in] data The data to advertise.
 * @param [in] length The length of the data.
 * @note If data length is 0 the service data will not be advertised.
 * @return True if successful, false if data length is too long or could not be set.
 */
bool BLEAdvertisementData::setServiceData(const BLEUUID& uuid, const uint8_t* data, size_t length) {
    uint8_t uuidBytes = uuid.bitSize() / 8;
    uint8_t sDataLen  = 2 + uuidBytes + length;
    if (sDataLen > BLE_HS_ADV_MAX_SZ) {
        log_e("Service Data too long");
        return false;
    }

    uint8_t type;
    switch (uuidBytes) {
        case 2:
            type = BLE_HS_ADV_TYPE_SVC_DATA_UUID16;
            break;
        case 4:
            type = BLE_HS_ADV_TYPE_SVC_DATA_UUID32;
            break;
        case 16:
            type = BLE_HS_ADV_TYPE_SVC_DATA_UUID128;
            break;
        default:
            log_e("Cannot set service data, invalid size!");
            return false;
    }

    if (length == 0) {
        removeData(type);
        return true;
    }

    uint8_t sData[BLE_HS_ADV_MAX_SZ];
    sData[0] = uuidBytes + length + 1;
    sData[1] = type;
    memcpy(&sData[2], uuid.getValue(), uuidBytes);
    memcpy(&sData[2 + uuidBytes], data, length);
    return addData(sData, sDataLen);
} // setServiceData

/**
 * @brief Set the service data (UUID + data)
 * @param [in] uuid The UUID to set with the service data.
 * @param [in] data The data to be associated with the service data advertised.
 * @return True if the service data was set successfully.
 * @note If data length is 0 the service data will not be advertised.
 */
bool BLEAdvertisementData::setServiceData(const BLEUUID& uuid, const std::string& data) {
    return setServiceData(uuid, reinterpret_cast<const uint8_t*>(data.data()), data.length());
} // setServiceData

/**
 * @brief Set the service data advertised for the UUID.
 * @param [in] uuid The UUID the service data belongs to.
 * @param [in] data The data to advertise.
 * @return True if the service data was set successfully.
 * @note If data length is 0 the service data will not be advertised.
 */
bool BLEAdvertisementData::setServiceData(const BLEUUID& uuid, const std::vector<uint8_t>& data) {
    return setServiceData(uuid, &data[0], data.size());
} // setServiceData

/**
 * @brief Get the location of the data in the payload.
 * @param [in] type The type of data to search for.
 * @return -1 if the data is not found, otherwise the index of the data in the payload.
 */
int BLEAdvertisementData::getDataLocation(uint8_t type) const {
    size_t index = 0;
    while (index < m_payload.size()) {
        if (m_payload[index + 1] == type) {
            return index;
        }
        index += m_payload[index] + 1;
    }
    return -1;
} // getDataLocation

/**
 * @brief Remove data from the advertisement data.
 * @param [in] type The type of data to remove.
 * @return True if successful, false if the data was not found.
 */
bool BLEAdvertisementData::removeData(uint8_t type) {
    int dataLoc = getDataLocation(type);
    if (dataLoc != -1) {
        std::vector<uint8_t> swap(m_payload.begin(), m_payload.begin() + dataLoc);
        int                  nextData = dataLoc + m_payload[dataLoc] + 1;
        swap.insert(swap.end(), m_payload.begin() + nextData, m_payload.end());
        swap.swap(m_payload);
        return true;
    }

    return false;
} // removeData

/**
 * @brief Retrieve the payload that is to be advertised.
 * @return The payload of the advertisement data.
 */
std::vector<uint8_t> BLEAdvertisementData::getPayload() const {
    return m_payload;
} // getPayload

/**
 * @brief Clear the advertisement data for reuse.
 */
void BLEAdvertisementData::clearData() {
    std::vector<uint8_t>().swap(m_payload);
} // clearData

/**
 * @brief Get the string representation of the advertisement data.
 * @return The string representation of the advertisement data.
 */
std::string BLEAdvertisementData::toString() const {
    std::string hexStr = GeneralUtils::dataToHexString(&m_payload[0], m_payload.size());
    std::string str;
    for (size_t i = 0; i < hexStr.length(); i += 2) {
        str += hexStr[i];
        str += hexStr[i + 1];
        if (i + 2 < hexStr.length()) {
            str += " ";
        }
    }

    return str;
} // toString



#endif /* CONFIG_BLUEDROID_ENABLED || CONFIG_NIMBLE_ENABLED */
#endif /* SOC_BLE_SUPPORTED */