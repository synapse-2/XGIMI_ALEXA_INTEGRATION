#pragma once
#ifndef BOARD_HAS_PSRAM
#define BOARD_HAS_PSRAM
#endif

#ifndef ARDUINO_RUNNING_CORE
#define ARDUINO_RUNNING_CORE 1
#endif

#ifndef ARDUINO_EVENT_RUNNING_CORE
#define ARDUINO_EVENT_RUNNING_CORE 1
#endif

/*
 * @brief Log level
    ESP_LOG_NONE    = 0,    //!< No log output 
    ESP_LOG_ERROR   = 1,    //!< Critical errors, software module can not recover on its own 
    ESP_LOG_WARN    = 2,    //!< Error conditions from which recovery measures have been taken 
    ESP_LOG_INFO    = 3,    //!< Information messages which describe normal flow of events 
    ESP_LOG_DEBUG   = 4,    //!< Extra information which is not necessary for normal use (values, pointers, sizes, etc). 
    ESP_LOG_VERBOSE = 5,    //!< Bigger chunks of debugging information, or frequent messages which can potentially flood the output. 
    ESP_LOG_MAX     = 6,    //!< Number of levels supported 
*/
// do in platform.ini for log_e to output -DCORE_DEBUG_LEVEL=YOURLEVEL

//#ifdef CONFIG_BT_NIMBLE_LOG_LEVEL
//#undef CONFIG_BT_NIMBLE_LOG_LEVEL
//#endif
// for the NewNimBle stack
/** @brief Un-comment to set the debug log messages level from the NimBLE host stack.\n
 *  Values: 0 = DEBUG, 1 = INFO, 2 = WARNING, 3 = ERROR, 4 = CRITICAL, 5+ = NONE\n
 *  Uses approx. 32kB of flash memory.
 */
#ifdef CONFIG_BT_NIMBLE_LOG_LEVEL
#undef CONFIG_BT_NIMBLE_LOG_LEVEL
#endif
#define CONFIG_BT_NIMBLE_LOG_LEVEL 4

 /** @brief Un-comment to set the debug log messages level from the NimBLE CPP Wrapper.\n
 *  Values: 0 = NONE, 1 = ERROR, 2 = WARNING, 3 = INFO, 4+ = DEBUG\n
 *  Uses approx. 32kB of flash memory.
 */
 #define CONFIG_NIMBLE_CPP_LOG_LEVEL 4

/** @brief Un-comment to see NimBLE host return codes as text debug log messages.
 *  Uses approx. 7kB of flash memory.
 */
#define CONFIG_NIMBLE_CPP_ENABLE_RETURN_CODE_TEXT 1

/** @brief Un-comment to see GAP event codes as text in debug log messages.
 *  Uses approx. 1kB of flash memory.
 */
#define CONFIG_NIMBLE_CPP_ENABLE_GAP_EVENT_CODE_TEXT 1


//#ifndef CONFIG_NIMBLE_ENABLED
//#define CONFIG_NIMBLE_ENABLED
//#endif


// esp32 Ble does not support having null cars in mauf data so we created our own NewBLE stack
//  the h2zero/NimBLE-Arduino@ ^2.3.3 derrived stack does not work with AruinoIoT  so  we have the updated stack that works with ArduinoIoT, and esp32 arduino 3.3.0 core called it NewNimBLE
#define USE_H2ZERO_NIMBLE_LIB // set for NewNimBLE and  h2zero/NimBLE-Arduino
//#undef USE_H2ZERO_NIMBLE_LIB // unset for BLE and NewBLE
//#define CONFIG_BT_NIMBLE_EXT_ADV 1 // set for BLE 5.0 extended advertisment


#define RGBCHIP WS2812B
#define LED_BUILTINIO GPIO_NUM_48
#define RGB_DATA_ORDER GRB
#define NUMPIXELS 1
//#define FASTLED_RMT_MAX_CHANNELS 1 
//#define FASTLED_ESP32_I2S 

#define I2C_SCLK GPIO_NUM_12
#define I2C_SDA GPIO_NUM_21
#define I2C_FREQ 100000     // 100 kHz
#define I2C_SLAVE_ADDR 0x3f // Slave address for I2C


//set to not start the wifi and aIoT connection
// #define XIGIMI_DEBUG_WIFI_OFF
#undef XIGIMI_DEBUG_WIFI_OFF
#define AIOT_POLL_TIME 3000 //milis for arduino poll any less(faster) and blutooth is not able to connecet


#define HOHSTNAME_Local "ALEXA_ESP32.local"
#define AP_CONNECT_TIMEOUT 120 // seconds or 2 mins

/* debugging
    typedef enum {
        WM_DEBUG_SILENT    = 0, // debug OFF but still compiled for runtime
        WM_DEBUG_ERROR     = 1, // error only
        WM_DEBUG_NOTIFY    = 2, // default stable,INFO
        WM_DEBUG_VERBOSE   = 3, // move verbose info
        WM_DEBUG_DEV       = 4, // development useful debugging info
        WM_DEBUG_MAX       = 5  // MAX extra dev auditing, var dumps etc (MAX+1 will print timing,mem and frag info)
    } wm_debuglevel_t;
 */

#define WIFIDEBUG WM_DEBUG_VERBOSE

/*
     The following function allows you to obtain more information
     related to the state of network and IoT Cloud connection and errors
     the higher number the more granular information you’ll get.
     The default is 0 (only errors).
     Maximum is 4
 */
#define ArduinoCloudDebugLevel 5

#define ResetButton GPIO_NUM_0 // the boot button on the ESP32

// Make your device IoT–ready
// To use this board you will need a Device ID and a Secret Key, please copy and save them or download the PDF.
// Also, keep in mind that this device authentication has a lower security level compared to other Arduino devices.
// Device ID
// dc175b6d-0190-4d72-9445-980a76e7f872
// Secret Key
// #e0num8EJjkWIVZCv5Avp0RmO